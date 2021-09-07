const sqlite3 = require("sqlite3");
module.exports = {
    name: "delete",
    adminOnly: true,
    execute(db,body){
        return new Promise(resolve=>{
            db.serialize(function(){
                db.get("SELECT * FROM Users Where Username = ?",[body.username],async function(err,row){
                    if(row)
                    {
                        db.run("DELETE FROM Users WHERE Username = ?",[body.username],(err)=>{
                        },(err,fin)=>{
                            resolve(`Deleted ${body.username} from database`);
                        });
                    }else{
                        resolve("No user found");
                    }
                });
            });
        });
    }
}