const { response } = require("express");

const uuid4 = require("uuid4");

module.exports = {
    name: "generate",
    adminOnly: true,
    execute(db,body,res){
        let id = uuid4();
        return new Promise(resolve=>{
            db.serialize(function(){
                db.run(`INSERT INTO Users(License, Expiry, Rank) VALUES (?,?,?)`,[id,body.length,body.rank],function(err)
                {
                    if(err)
                        console.log(err.message);
                },
                (err,res)=>{
                    if(err)
                        resolve("FAILED: " + err.message);
                    else
                        resolve(`Appended ${body.length} Day License: ${id} to database with rank ${body.rank}`)
                    }
                );               
            });
        }).then(response=>{
            res.send(response);
        })
    }
};