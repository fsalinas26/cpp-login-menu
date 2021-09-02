const { response } = require("express");

const uuid4 = require("uuid4");

module.exports = {
    name: "generate",
    adminOnly: true,
    execute(db,body,res){
        var returnres = "";
        let id = uuid4();
        new Promise(resolve=>{
            db.serialize(async function(){
                for(var i =0; i < parseInt(body.quantity);i++)
                {
                    await new Promise(resolve=>{
                        db.run(`INSERT INTO Users(License, Expiry, Rank) VALUES (?,?,?)`,[uuid4(),body.length,body.rank],async function(err)
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
                                
                    }).then(resf=>{
                        returnres+= resf;
                    })
                }
            })
        resolve(returnres);
        }).then(response=>{
            res.send(response);
        })
    }
};