const sqlite3 = require("sqlite3");

module.exports = {
    name: "create",
    adminOnly: true,
    execute(){
        return new Promise(resolve=>{
            let db = new sqlite3.Database("./users.db", (err)=>{
            if(err){
            resolve(err.message); 
            }
            resolve("Succesfully created Database");
        }); 
        
        db.serialize(function() {
            db.run("CREATE TABLE Users (License UNIQUE, Username UNIQUE, Password TEXT, HWID TEXT, LastLogin TEXT, LastIP TEXT, Expiry TEXT, UserVar TEXT, Rank INTEGER, LastHWIDReset TEXT)");
            
            db.each("SELECT * FROM Users", function(err, row) {
                console.log(row);
            });
        });
        
        db.close();
        });
    }
}