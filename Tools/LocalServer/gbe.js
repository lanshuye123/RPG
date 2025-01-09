const express = require("express");
const App = express();
App.use(express.json());
var Obj = {};
App.post("/query",(res,req)=>{
    if (!res.body) {
        return req.status(400).json({error: 'Request body is missing.'}).end();
    }
    console.log(res.body);
    if(!Obj[res.body['query']]){
        Obj[res.body['query']] = 114514;
    }
    var back = {}
    back[res.body['query']] = Obj[res.body['query']];
    req.json(back).end();
    console.log(JSON.stringify(Obj));
});
App.post('/upload',(res,req)=>{
    if (!res.body) {
        return req.status(400).json({error: 'Request body is missing.'}).end();
    }
    console.log(res.body);
    Obj = Object.assign({}, Obj, res.body);
    req.json({msg:"OK"}).end()
})

var save = {};

App.post('/game',(res,req)=>{
    if (!res.body) {
        return req.status(400).json({error: 'Request body is missing.'}).end();
    }
    console.log(res.body);
    if(res.body['type'] == "save"){
        save[res.body['userid']] = res.body['save'];
    }
    if(res.body['type'] == "load"){
        if(save[res.body['userid']]){
            req.write(Buffer.from(save[res.body['userid']],"base64"))
        }else{
            req.write(Buffer.from(0));
        }
    }
    req.end();
})
var Server = App.listen(9999);
console.log("Listen on ",Server.address());