# cpp-login-menu
A simple and ready-to-use ImGui login menu with a fully functioning NodeJS Server for securely storing user info using SQL.
<p align="center">
<kbd><img height="400" width = "650" src="https://i.gyazo.com/e771cc0fda11d06892d73f6781cea939.gif"/></kbd>
</p>

[C++ ImGui Menu](#real-cool-heading)
[NodeJS Server](#real-cool-heading)
[Database Structure](#Database Structure)

## SQL Database Structure  
<img src="https://i.gyazo.com/15d1064b2e246d6facc2d7e8bed6f9e1.png">

| Column  | Type |
| ------------- | ------------- |
| License (UNIQUE)   | For storing UUID4 generated serials.   |
| Username (UNIQUE)  | For storing users usernames.  |
| Password  | SHA256 for hash authentication.  |
| HWID  | SHA256 Hardware Identifier to lock a license to a single PC.  |
| LastLogin  | Time of last users login.  |
| LastIP  | Most recent IP the user logged in with.  |
| Expiry  | Expiry date of the license.  |
| UserVar  | UserVariable that can be set to anything.  |
| LastHWIDReset  | Stores the last HWID Reset time for HWID_RESET_FREQUENCY.  |
 
