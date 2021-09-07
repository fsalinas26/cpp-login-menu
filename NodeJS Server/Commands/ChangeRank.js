module.exports = {
name: "rank",
adminOnly: true,
execute(db,body){
    return new Promise(resolve=>{
        db.serialize(function(){
            db.get("SELECT * FROM Users WHERE Username = ?",[body.username], async function(err,row)
            {
                if(row)
                {
                    db.run("UPDATE Users SET Rank = ? WHERE Username = ?",[body.newRank,body.username],function(err){
                        if(err)
                            resolve(err.message);
                        else
                            resolve(`${body.username} rank has been changed from ${row.Rank} to ${body.newRank}`);
                    })
                }
                else{
                    resolve("No user found");
                }
            })
        });
    });
}
}