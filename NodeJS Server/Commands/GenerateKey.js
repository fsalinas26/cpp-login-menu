const { response } = require("express");

const uuid4 = require("uuid4");

module.exports = {
    name: "generate",
    adminOnly: true,
    async execute(db,body,out_obj){
        var id = await new Promise(resolve=>{var ID = uuid4();resolve(ID)});
        return new Promise(resolve=>{
            db.serialize(async function(){
                db.run(`INSERT INTO Users(License, Expiry, Rank) VALUES (?,?,?)`,[id,body.length,body.rank],async function(err)
                    {
                        if(err)
                            console.log(err.message);
                    },
                    (err,res)=>{
                        if(err)
                            resolve("FAILED: " + err.message);
                        else
                            resolve(`Appended ${body.length} Day License: ${id} to database with rank ${body.rank}\n`);
                        }
                    ); 
            })
        });
    }
};