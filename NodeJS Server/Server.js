const express = require('express'),
      app = express(),
      router = express.Router();
const config = require('./config.json');
const Users = require('./users');      
const port = 3000; 

app.use(express.json()); 


app.get('/',(req,res)=>{
    res.send("Test");
})

router.post("/post",async(req,res)=>{
    switch(req.body.a)
    {
        case "login":
            res.send("logging in");
            break
        case "register":
            res.send("registering");
            break
        case "database":
            Users.create_user_db();
            break
        default:
            res.send("unknown");
            break
    }
    console.log(req.body);
});


app.use('/',router);
app.listen(port, ()=>{console.log(`Listening on port ${port}`)});