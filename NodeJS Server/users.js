const sqlite3 = require("sqlite3");
const uuid4 = require("uuid4");

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

function Create_License_Keys(){
    let db = new sqlite3.Database("./users.db", (err)=>{
    if(err){
      return console.error(err.message); 
    }
    console.log("Succesfully created Database");
  }); 
  
  db.serialize(function() {
    db.run("CREATE TABLE Users (License UNIQUE, Username UNIQUE, Password TEXT, HWID TEXT, LastLogin TEXT, LastIP TEXT, Expiry TEXT)");
    
    db.each("SELECT * FROM Users", function(err, row) {
        console.log(row);
    });
  });

db.close();
}


module.exports = {
    Create_User_Database:Create_User_Database,
    Create_License_Keys:Create_License_Keys
};