module.exports = {
    name:"resetpw",
    adminOnly: false,
    execute(db,body,out_obj,adminMode)
    {
        return new Promise(resolve=>{
            db.serialize(function(){
                db.get("SELECT * FROM Users WHERE Username = ?"+(adminMode?"":" AND License = ?"),adminMode?[body.username]:[body.username,body.license],async function(err,row){
                    if(row)
                    {
                        db.run("UPDATE Users SET Password = ? WHERE Username = ?",[body.newPassword, body.username],function(err){},
                        (err,result)=>{
                            if(err)
                                resolve(err.message)
                            else
                                resolve("Password Changed Successfully");
                        })
                    }else if(err){
                        resolve(er.message);
                    }else{
                        resolve("No user found");
                    }
                })
            });
        
        })
    }
}