const express = require('express'),
      app = express(),
      router = express.Router();
const config = require('./config.json');    
const port = 80; 

const bodyParser = require('body-parser');

const sqlite3 = require("sqlite3");
const database_filepath = "./users.db";
const db = new sqlite3.Database(database_filepath);

const crypto = require('./Encryption/crypto');
const key = Buffer.from("oUHqJ9IOlyjA4edqmyFdkeNi8J/x+dte2AWlGRd2uTM=",'base64');



let Commands = new Map();
const fs = require("fs");
function ImportCommands() {
    const commandFiles = fs.readdirSync('./Commands').filter(file => file.endsWith('.js'));
    for (const file of commandFiles) {
        const command = require(`./Commands/${file}`);
        Commands.set(command.name, command);
        console.log(`Imported ${file}...`)
    }
}
ImportCommands();


router.post("/post",async(req,res)=>{
    req.body = await crypto.decryptBody(req.body,key,req.body.iv); //decrypt the JSON Object request
    req.body.ip = req.headers['x-forwarded-for'] || req.socket.remoteAddress;
    const command = Commands.get(req.body.command);
    let resObj = {};
    if(!command.adminOnly)
        resObj.res = await command.execute(db,req.body,resObj);
    else 
        resObj.res = "You do not have access to this command";
    resObj = await crypto.encryptResponse(resObj,key,req.body.iv); //encrypt the JSON Object response
    res.send(resObj);
});

router.post("/admin",async(req,res)=>{
    if(config.auth === req.headers.authorization)
    {
        const Database = Commands.get(req.body.command);
        var response = await Database.execute(db,req.body,res,true);
        res.send(response);
    }else{
        res.send("Access Denied");
    }
});

app.get("/initialize",async(req,res)=>{
    let iv = crypto.generateIV();
    res.send(iv.toString('base64'));
})

app.use(bodyParser.json()); // support json encoded bodies
app.use(bodyParser.urlencoded({ extended: true })); // support encoded bodies 
app.use('/',router);
app.listen(port, ()=>{console.log(`Listening on port ${port}`)});