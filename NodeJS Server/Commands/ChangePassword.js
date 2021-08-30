module.exports = {
    name:"resetpw",
    adminOnly: false,
    execute(db,body,res)
    {
        return new Promise(resolve=>{
            db.serialize(function(){
                db.get("SELECT * FROM Users WHERE License = ? AND Username = ?",[body.license,body.username],async function(err,row){
                    if(row)
                    {
                        db.run("UPDATE Users SET Password = ? WHERE License = ?",[body.newPassword, body.license],function(err){},
                        (err,result)=>{
                            if(err)
                                resolve(err.message)
                            else
                                resolve("Password Changed Successfully");
                        })
                    }else{
                        resolve("No user found");
                    }
                })
            });
        
        }).then(response=>{
            res.send(response);
        })
    }
}