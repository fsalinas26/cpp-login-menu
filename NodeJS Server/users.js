const sqlite3 = require("sqlite3");
const { finished } = require("stream");
const uuid4 = require("uuid4");
const database_filepath = "./users.db";
const db = new sqlite3.Database(database_filepath);

function Create_User_Database(){
    let db = new sqlite3.Database("./users.db", (err)=>{
    if(err){
      return console.error(err.message); 
    }
    console.log("Succesfully created Database");
  }); 
  
  db.serialize(function() {
    db.run("CREATE TABLE Users (License UNIQUE, Username UNIQUE, Password TEXT, HWID TEXT, LastLogin TEXT, LastIP TEXT, Expiry TEXT, 'User Variable' TEXT, Rank TEXT)");
    
    db.each("SELECT * FROM Users", function(err, row) {
        console.log(row);
    });
  });

db.close();
}

async function Generate_License(length, rank){
    let id = uuid4();
    return new Promise(resolve=>{
        db.serialize(function(){
            db.run(`INSERT INTO Users(License, Expiry, Rank) VALUES (?,?,?)`,[id,length,rank],function(err)
            {
                if(err)
                    console.log(err.message);
            },
            (err,res)=>{
                if(err)
                    resolve("FAILED: " + err.message);
                else
                    resolve(`Appended ${length} Day License: ${id} to database with rank ${rank}`)
                }
            );               
        });
    })
}

async function Show_All_Entries(length, rank){
    var object;
    return new Promise(resolve=>{
        db.serialize(function(){
            db.each("SELECT * FROM Users",function(err,row){
                console.log(row);
            })
        });
    })
}

function getTimestamp(today)
{
    var date = today.getFullYear()+'-'+(today.getMonth())+'-'+today.getDate();
    var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
    var dateTime = date+' '+time;
    return dateTime;
}

async function getExpiry(days)
{
  return new Promise(resolve =>{
    var today = new Date();
    today.setDate(today.getDate() + days);
    resolve(getTimestamp(today));
  })
}

async function Redeem_License(username, password, license, hwid,ip)
{
  return new Promise(resolve=>{
    db.serialize(function(){
        db.get(`SELECT * FROM Users WHERE License = ? AND Username IS NULL`,[license],async function(err,row){
          if(row)
          {
            var expiry = await getExpiry(parseInt(row.Expiry));
            db.run(`UPDATE Users SET Username = ?, Password = ?, Expiry = ?, HWID = ?, LastIP = ? WHERE License = ?`,[username,password,expiry,hwid, ip, license],function(err,row){

            },
            (err,finished)=>{
                if(err)
                    resolve(err.message);
                else
                    resolve("License Redeemed");
            });
          }else{
            resolve("License not found or already redeemed");
          }
      })              
  
    });
    
  })
}

async function Login(username, password,hwid)
{
    return new Promise(resolve=>{
        db.serialize(function(){
            db.get("SELECT * FROM Users WHERE Username = ?",[username],async function(err,row)
            {
                if(row)
                {
                    console.log(row);
                    if(row.Password === password)
                    {
                        if(new Date(row.Expiry) > new Date())
                        {
                            if(row.HWID === hwid)
                            {
                                db.run("UPDATE Users SET LastLogin = ? WHERE Username = ?",[getTimestamp(new Date()),username],async function(err,row){});
                                resolve("Login Success");
                            }
                            else if(row.HWID === "0"){
                                db.run("UPDATE Users SET HWID = ? WHERE Username = ?",[hwid,username],async function(err,row){
                                    resolve("HWID UPDATED");
                                });
                            }else{
                                resolve("Invalid HWID");
                            }
                        }else{
                            resolve("License Expired");
                        }
                    }else{
                        resolve("Invalid Password");
                    }
                }
                else
                {
                    resolve("Invalid Credentials");
                }
            })
        });
    
    })
}

async function ChangePassword(username,license,newPassword)
{
    return new Promise(resolve=>{
        db.serialize(function(){
            db.get("SELECT * FROM Users WHERE License = ? AND Username = ?",[license,username],async function(err,row){
                if(row)
                {
                    db.run("UPDATE Users SET Password = ? WHERE License = ?",[newPassword, license],function(err){},
                    (err,result)=>{
                        if(err)
                            resolve(err.message)
                        else
                            resolve("Password Changed Successfully");
                    })
                }else{
                    resolve("No user found");
                }
            })
        });
    
    })
}

async function LookupUser(searchFor)
{
    return new Promise(resolve=>{
        db.serialize(function(){
            db.get("SELECT * FROM Users WHERE Username = ? OR License = ?",[searchFor,searchFor], async function(err,row){
                if(err)
                    resolve(err.message);
                else
                    resolve(row);
            });
        });
    })
}


module.exports = {
    Create_User_Database:Create_User_Database,
    Generate_License:Generate_License,
    Show_All_Entries:Show_All_Entries,
    Redeem_License:Redeem_License,
    Login:Login,
    ChangePassword:ChangePassword,
    LookupUser:LookupUser
};