const config = require("../config.json");
const macros = require("../Macros");
module.exports = {
name: "resethwid",
adminOnly: false,
execute(db,body,obj_out,adminMode){
    console.log(adminMode);
    return new Promise(resolve=>{
        db.serialize(function(){
            db.get("SELECT * FROM Users WHERE Username = ?"+(adminMode?"":" AND Password = ?"),adminMode?[body.username]:[body.username,body.password], async function(err,row)
            {
                if(row)
                {
                    var DaysFromLastReset = await macros.dayDifference(new Date(row.LastReset));
                    if(DaysFromLastReset > config.HWID_RESET_FREQUENCY_DAYS || adminMode)
                    {
                        db.run("UPDATE Users SET HWID = '0', LastReset = ? WHERE Username = ?",[macros.getTimestamp(new Date()),body.username],function(err){
                            if(err)
                                resolve(err.message);
                            else
                                resolve(`HWID Successfully Updated for ${body.username}`);
                        })
                    }else{
                        resolve(`You cannot reset your HWID for another ${(config.HWID_RESET_FREQUENCY_DAYS-DaysFromLastReset).toFixed(2)} days`);
                    }
                }
                else{
                    resolve("No user found");
                }
            })
        });
    })
}
}