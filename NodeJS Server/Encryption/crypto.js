const crypto = require('crypto');

function encrypt(text,key,iv64) {
 let iv = Buffer.from(iv64, 'base64');
 let cipher = crypto.createCipheriv('aes-256-cfb', Buffer.from(key), iv);
 let encrypted = cipher.update(text);
 encrypted = Buffer.concat([encrypted, cipher.final()]);
 return encrypted.toString('base64');
}

function decrypt(text,key,iv64) {
 let iv = Buffer.from(iv64, 'base64');
 let encryptedText = Buffer.from(text, 'base64');
 let decipher = crypto.createDecipheriv('aes-256-cfb', Buffer.from(key), iv);
 let decrypted = decipher.update(encryptedText);
 decrypted = Buffer.concat([decrypted, decipher.final()]);
 return decrypted.toString();
}

const nonencrypted = ['hwid','iv','password','newPassword'];
async function decryptBody(obj,keyf,iv64)
{
    let obj_out = {};
    return new Promise(resolve=>{
        for(var [key,value] of Object.entries(obj)){
            if(!nonencrypted.find(e=>e==key))
                value = decrypt(value,keyf,iv64);
            obj_out[key.toString()] = value;
        }
        resolve(obj_out);
    })  
}
function generateIV()
{
    return(crypto.randomBytes(16));
}
async function encryptResponse(obj,keyf,iv64)
{
    let obj_out = {};
    return new Promise(resolve=>{
        for(var [key,value] of Object.entries(obj)){
            if(!(value))value="null";
            obj_out[key.toString()] = encrypt(value,keyf,iv64);
        }
        resolve(obj_out);
    })  
}

module.exports = {
    decryptBody:decryptBody,
    encryptResponse:encryptResponse,
    generateIV:generateIV
}

