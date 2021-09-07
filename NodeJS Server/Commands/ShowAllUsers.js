module.exports = {
    name: "show",
    adminOnly: true,
    execute(db,body,out_obj)
    {
        var obj = [];
        return new Promise(resolve=>{
            db.serialize(function(){
                db.each("SELECT * FROM Users",async function(err,row){
                    if(row)
                    {
                        obj.push(row);
                    }else{
                        resolve("No user found");
                    }
                },
                (err,finished)=>
                {
                    if(err)
                        resolve(err.message);
                    else
                        resolve(obj);
                })
            });
        })
    }
}