const config = require("../config.json");
const macros = require("../Macros");

module.exports = {
    name:"login",
    adminOnly: false,
    //fields are {username, password, hwid}
    execute(db,body,out_obj,adminMode)
    {
        return new Promise(resolve=>{
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
                                    out_obj["Rank"] = (row.Rank).toString();
                                    out_obj["UserVar"] = row.UserVar;
                                    out_obj["Expiry"] = row.Expiry;
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
                                    out_obj["status"] = "401";
                                    resolve("Invalid HWID");
                                }
                            }
                            else
                            {
                                out_obj["status"] = "401";
                                resolve("License Expired");
                            }
                        }
                        else
                        {
                            out_obj["status"] = "401";
                            resolve("Invalid Password");
                        }
                    }
                    else
                    {
                        out_obj["status"] = "401";
                        resolve("Invalid Credentials");
                    }
                })
            });
        
        })
}
}