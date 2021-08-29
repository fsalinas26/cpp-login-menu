const sqlite3 = require("sqlite3");
const uuid4 = require("uuid4");

function create_user_db(){
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
    create_user_db:create_user_db
};