const express = require('express'),
      app = express(),
      router = express.Router();
const config = require('./config.json');    
const port = 3000; 
const fs = require("fs");
const sqlite3 = require("sqlite3");
const database_filepath = "./users.db";
const db = new sqlite3.Database(database_filepath);

let Database = new Map();

function ImportCommands() {
    const commandFiles = fs.readdirSync('./Commands').filter(file => file.endsWith('.js'));
    for (const file of commandFiles) {
        const command = require(`./Commands/${file}`);
        Database.set(command.name, command);
    }
}
ImportCommands();

app.use(express.json()); 
app.get('/',(req,res)=>{
    res.sendStatus(200);
})

router.post("/post",async(req,res)=>{
    req.body.ip = req.headers['x-forwarded-for'] || req.socket.remoteAddress;
    const command = Database.get(req.body.a);
    if(!command.adminOnly)
        command.execute(db,req.body,res);
    else
        res.send("You do not have access to this Command");
});

router.post("/admin",async(req,res)=>{
    if(config.auth === req.headers.authorization)
    {
        const command = Database.get(req.body.a);
        command.execute(db,req.body,res);
    }else{
        res.send("Access Denied");
    }
});


app.use('/',router);
app.listen(port, ()=>{console.log(`Listening on port ${port}`)});