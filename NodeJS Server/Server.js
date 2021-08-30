const express = require('express'),
      app = express(),
      router = express.Router();
const config = require('./config.json');    
const port = 80; 
const fs = require("fs");
const bodyParser = require('body-parser');
const sqlite3 = require("sqlite3");
const database_filepath = "./users.db";
const db = new sqlite3.Database(database_filepath);

let Commands = new Map();

function ImportCommands() {
    const commandFiles = fs.readdirSync('./Commands').filter(file => file.endsWith('.js'));
    for (const file of commandFiles) {
        const command = require(`./Commands/${file}`);
        Commands.set(command.name, command);
        console.log(`Imported ${command.name}...`)
    }
}
ImportCommands();

//app.use(express.json());
app.use(bodyParser.json()); // support json encoded bodies
app.use(bodyParser.urlencoded({ extended: true })); // support encoded bodies 

router.post("/post",async(req,res)=>{
    console.log(req.body);
    req.body.ip = req.headers['x-forwarded-for'] || req.socket.remoteAddress;
    const command = Commands.get(req.body.command);
    if(!command.adminOnly)
        command.execute(db,req.body,res);
    else
        res.send("You do not have access to this Command");
});

router.post("/admin",async(req,res)=>{
    if(config.auth === req.headers.authorization)
    {
        const Database = Commands.get(req.body.command);
        Database.execute(db,req.body,res);
    }else{
        res.send("Access Denied");
    }
});

app.use('/',router);
app.listen(port, ()=>{console.log(`Listening on port ${port}`)});