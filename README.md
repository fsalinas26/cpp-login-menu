# cpp-login-menu
A simple and ready-to-use ImGui login menu with a fully functioning NodeJS Server for securely storing user info using SQLite.
<p align="center">
<kbd><img height="400" width = "650" src="https://i.gyazo.com/67c1d77797cb4ea84751cbd337ca99e4.gif"/></kbd>
</p>


* [Database Structure](https://github.com/fsalinas26/cpp-login-menu#sqlite-database-structure)  
* [Database Commands](https://github.com/fsalinas26/cpp-login-menu#database-commands)  
* [HTTP Request](https://github.com/fsalinas26/cpp-login-menu#http-request-c++)
* [Encryption](https://github.com/fsalinas26/cpp-login-menu#Encryption)  

## SQLite Database Structure  
<img src="https://i.gyazo.com/15d1064b2e246d6facc2d7e8bed6f9e1.png">

| Column  | Type | Details |
| ------------- |---| ------------- |
| License |UNIQUE| For storing uuid4 serials.   |
| Username |UNIQUE|For storing users usernames.  |
| Password  |TEXT|SHA256 Password for hash authentication.  |
| HWID  |TEXT|SHA256 Hardware Identifier to lock a license to a single PC.  |
| LastLogin  |TEXT| Time of last users login.  |
| LastIP  |TEXT|Most recent IP the user logged in with.  |
| Expiry  |TEXT|Expiry date of the license.  |
| UserVar  |TEXT|User variable that can be set to anything.  |
| Rank  |INTEGER|Rank of license.  |
| LastReset  |TEXT| Stores the last HWID reset time for cooldown period.  |

## Database Commands  
These are a list of [functions](https://github.com/fsalinas26/cpp-login-menu/tree/master/NodeJS%20Server/Commands) that can be performed on the database to insert, modify, or delete table entries.
*You can test most of these functions in Server.js or also use a REST Client to test the HTTP Request (use /admin route to bypass encryption). I plan on adding some kind of Admin Panel built into the menu to have control of all these callbacks*  

Each command will take the form:  
```javascript
module.exports = {
    name:"login", //the command name to be reference in the request ex. {"command": "login"}
    adminOnly: false, //declare if the function can be only accessed by admin clients
    execute(db,body,out_obj,adminMode){//see below
```
**db** is the working sqlite database.  
**body** is the incoming request body (JSON Object).  
**out_obj** is the outgoing response JSON object (Use this to pass any data from the database to your client)  
**adminMode** When true, certain commands will not have cooldown/argument restrictions (resethwid, resetpw)   

### ChangePassword.js  
**Name:** resetpw  
**body**: username, license, newPassword  
Changes the password of the table entry where both username and license are found.  

### ChangeRank.js (Admin Only)  
**Name:** rank  
**body:** username,newRank  
Changes the rank of the table entry where username is found.

### CreateDB.js (Admin Only) **  
**Name:** create  
This function should only be called once if you want to create a new database.  

### ExtendLicense.js (Admin Only)   
**Name:** extend  
**body:** username, extendBy  
Extends the expiry date of the table entry where username is found. **extendBy** takes days.  

### GenerateKey.js (Admin Only)  
**Name:** generate  
**body:** length, rank  
Inserts a new license key into the table with length (in days) and rank.      

### Login.js 
**Name:** login  
**body:** username, password, HWID  
Returns rank, user variable, expiry date, and login success if an entry is found in the table.  

### LookupUser.js (Admin Only)  
**Name:** find  
**body:** entry  
Returns all key/value data of a user where either username or license are found in the table.  

### Redeem.js 
**Name:** redeem  
**body:** username, password, hwid, license  
Populates an entry in the table where license is found.  

### ResetHWID.js (AdminOnly)
**Name:** resethwid  
**body:** username, password (not required if calling from admin route)  
Resets the HWID of an entry in the table where username is found. The HWID reset cooldown can be set in config.json.  

### ShowAllUsers.js (AdminOnly)
**Name:** show  
**body:** n/a  
Returns an array of objects of all entries in the table.  

## HTTP Request C++
Each request to the server will look similar. The default content-type is **application/x-www-form-urlencoded**,  
however you can change this by declaring it in the post headers.  

*Example of login request*
```js  
request.add_field("command", c_crypto::encrypt("login", g_crypto.key, g_crypto.iv).c_str());   //Name of command to process on server.
request.add_field("username", c_crypto::encrypt(username, g_crypto.key, g_crypto.iv).c_str()); //Required arguments for command
request.add_field("password", c_crypto::b64url_safe(c_crypto::SHA256_HASH(password)).c_str()); //Required arguments for command
request.add_field("hwid", c_crypto::b64url_safe(c_crypto::SHA256_HASH(HWID)).c_str());         //Required arguments for command
request.add_field("iv", (g_crypto.iv).c_str());                                                //Public Session IV

string tempRes;
vector<wstring> headers = { PUBLIC_TOKEN };
send.post(L"http://localhost/post", tempRes, request, headers);
```
**Server Receives...**  
```js
{
  command: 'Kys_GLAN0jd4JNi1E_fu3g',
  username: 'H87CL5e0UygddHC5pbGOtw',
  password: 'WZRHGrsBESr8wYFZ9sx0tPURuZgG2lmzyvWpwXPKz8U',
  hwid: 'R0A-0vP6e0kqQMYIGQVSneQwCmBcwGwn0L-6cNUqpS0',
  iv: 'oU-GRa5OijdrumD0zDcOsg'
}
```
**After Decryption...**
```js
{
  command: 'login',
  username: 'fsalinas26',
  password: 'WZRHGrsBESr8wYFZ9sx0tPURuZgG2lmzyvWpwXPKz8U',
  hwid: 'R0A-0vP6e0kqQMYIGQVSneQwCmBcwGwn0L-6cNUqpS0',
  iv: 'oU-GRa5OijdrumD0zDcOsg'
}
```

## Encryption
The body of each HTTP request and response is encrypted using AES-256-CBC with a randomly generated session IV that is initialized from the server. Both client and server will have a shared secret key. Each generated IV is stored in memory on the server and will become invalid after 30 seconds or destroyed after single use.  
I made this visual to show how the client communicates with the server using symmetric encryption. *I'm still learning more about crypto so this is subject to change.*
![](https://i.gyazo.com/79d3e56cf9dd33d50355d041a7c8845f.jpg)
