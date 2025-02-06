const { SerialPort } = require("serialport");
const { ReadlineParser } = require("@serialport/parser-readline");
const { Server } = require("socket.io");
const http = require("http");

const express = require("express");
const { error } = require("console");

const app = express();
const server = http.createServer(app);
const io = new Server(server);

app.use(express.json())

app.get("/", (req, res) => {
    res.sendFile(__dirname + "/views/index.html")
})


io.on("connection", (socket) => {
    console.log("konek...")
    socket.on("diskonek", () => {
        console.log("diskonek...")
    })
})

app.listen(3000, () => {
    console.log("serper idup")
})


const port = new SerialPort({
    path: "COM4",
    baudrate: 9600
})


const parser = port.pipe(new ReadlineParser({ delimiter: "r\n" }));

 parser.on("data", (result) => {
    console.log("data dari adruino >>", result)
    io.emit("data", { data:result });
 })

 app.post("/aselole", (req, res)=>{
    const data = req.body.data
    port.write(data, (err) => {
        if(err){
            console.log('err: ', err)
            res.status(500).json({error: "data error"})
        }
        console.log("data sukses terkirim >>", data)
        res.end();
    })



 })