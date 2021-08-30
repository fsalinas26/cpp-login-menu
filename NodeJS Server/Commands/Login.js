const config = require("../config.json");
const macros = require("../Macros");

module.exports = {
    name:"login",
    adminOnly: false,
    //fields are {username, password, hwid}
    execute(db,body,res,adminMode)
    {
        new Promise(resolve=>{
            db.serialize(function(){
                db.get("SELECT * FROM Users WHERE Username = ?",[body.username],async function(err,row)
                {
                    if(row)
                    {
                        if(row.Password === body.password)
                        {
                            if(new Date(row.Expiry) > new Date())
                            {
                                if(row.HWID === body.hwid || (!config.HWID_LOCKED))
                                {
                                    db.run("UPDATE Users SET LastLogin = ? WHERE Username = ?",[macros.getTimestamp(new Date()),body.username],async function(err,row){});
                                    resolve("Login Success");
                                }
                                else if(row.HWID === "0")
                                {
                                    db.run("UPDATE Users SET HWID = ? WHERE Username = ?",[body.hwid,body.username],async function(err,row){
                                        resolve("HWID Updated");
                                    });
                                }
                                else
                                {
                                    resolve("Invalid HWID");
                                }
                            }
                            else
                            {
                                resolve("License Expired");
                            }
                        }
                        else
                        {
                            resolve("Invalid Password");
                        }
                    }
                    else
                    {
                        resolve("Invalid Credentials");
                    }
                })
            });
        
        }).then(response=>{
            res.send(response);
        })
}
}