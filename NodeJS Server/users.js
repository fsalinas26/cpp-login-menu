const sqlite3 = require("sqlite3");
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
    db.run("CREATE TABLE Users (License UNIQUE, Username UNIQUE, Password TEXT, HWID TEXT, LastLogin TEXT, LastIP TEXT, Expiry TEXT, Rank TEXT)");
    
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


module.exports = {
    Create_User_Database:Create_User_Database,
    Generate_License:Generate_License,
    Show_All_Entries:Show_All_Entries
};