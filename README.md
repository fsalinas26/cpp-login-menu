# cpp-login-menu
A simple and ready-to-use ImGui login menu with a fully functioning NodeJS Server for securely storing user info using SQLite.
<p align="center">
<kbd><img height="400" width = "650" src="https://i.gyazo.com/67c1d77797cb4ea84751cbd337ca99e4.gif"/></kbd>
</p>


* [Database Structure](https://github.com/fsalinas26/cpp-login-menu#sqlite-database-structure)  
* [Database Commands](https://github.com/fsalinas26/cpp-login-menu#database-commands)  
* [Server Auth](https://github.com/fsalinas26/cpp-login-menu#authentication)  

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
Changes the password of the database entry where both username and license are found.  

### ChangeRank.js (Admin Only)  
**Name:** rank  
**body:** username,newRank  
Changes the rank of the database entry where username is found.

### CreateDB.js (Admin Only) **  
**Name:** create  
This function should only be called once if you want to create a new database.  

### ExtendLicense.js (Admin Only)   
**Name:** extend  
**body:** username, extendBy  
Extends the expiry date of the database entry where username is found. **extendBy** takes days.  

### GenerateKey.js (Admin Only)  
**Name:** generate  
**body:** length, rank  
Inserts a new license key into the database with length (in days) and rank.      

### Login.js 
**Name:** login  
**body:** username, password, HWID  
Returns rank, user variable, expiry date, and login success if an entry is found in the database.  

### LookupUser.js (Admin Only)  
**Name:** find  
**body:** entry  
Returns all key/value data of a user in the database where either username or license are found.  

### Redeem.js 
**Name:** redeem  
**body:** username, password, hwid, license  
Populates an entry in the database where license is found.  

### ResetHWID.js (AdminOnly)
**Name:** resethwid  
**body:** username, password (not required if calling from admin route)  
Resets the HWID of an entry in the database where username is found. The HWID reset cooldown can be set in config.json.  

### ShowAllUsers.js (AdminOnly)
**Name:** show  
**body:** n/a  
Returns an array of objects of all entries in the database.  


## Authentication
