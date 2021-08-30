module.exports = {
    name: "find",
    adminOnly: true,
    execute(db,body,res)
    {
        return new Promise(resolve=>{
            db.serialize(function(){
                db.get("SELECT * FROM Users WHERE Username = ? OR License = ?",[body.entry,body.entry], async function(err,row){
                    if(err)
                        resolve(err.message);
                    else
                        resolve(row);
                });
            });
        }).then(response=>{
            res.send(response);
        })
    }
}