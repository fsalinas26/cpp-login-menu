const macros = require("../Macros");
module.exports = {
name: "extend",
adminOnly: true,
execute(db,body,out_obj,adminMode){
    return new Promise(resolve=>{
        db.serialize(function(){
            db.get("SELECT * FROM Users WHERE Username = ?",[body.username], async function(err,row)
            {
                if(row)
                {
                    var extendDate = await macros.extendTime(row.Expiry,parseInt(body.extendBy));
                    db.run("UPDATE Users SET Expiry = ? WHERE Username = ?",[extendDate,body.username],function(err){
                        if(err)
                            resolve(err.message);
                        else
                            resolve(`${body.extendBy} days have been added to ${body.username}. New Expiry ${extendDate}`);
                    })
                }
                else{
                    resolve("No user found");
                }
            })
        });
    })
}
}