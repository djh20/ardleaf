const fs = require('fs');
const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

var connected = false;
var connectedPort;
var id = 1;

var fileName = 'test.csv';
var initialContent = 'ID,STATE,SPEED,AC SPEED\n';

fs.writeFile(fileName, initialContent, (err) => {
  if (err) throw err;
});

async function scan() {
  if (connected) return;

  console.log("searching...");
  let ports = await SerialPort.list();
  ports.forEach(async (port) => {
    console.log(`found port (${port.path} ${port.manufacturer})`);
    if (port.manufacturer == 'wch.cn') {
      let result = await connect(port.path);
      console.log(result);
    }
  });
}

function connect(path) {
  console.log(`connecting to ${path}`);
  return new Promise((resolve) => {
    let port = new SerialPort(path, {baudRate: 115200}, (err) => {
      if (err) return resolve(err);

      connectedPort = port;
      connected = true;

      port.on('close', () => {
        console.log('lost connection');
        connected = false;
      });

      let parser = port.pipe(new Readline({delimiter: '\n'}));
      parser.on('data', onData);
      
      resolve(`connected to ${path}`);
    });
  });
}

function onData(data) {
  console.log(`> ${data}`);

  data = `${id},${data}`;
  id++;

  fs.appendFile(fileName, data, (err) => {
    if (err) throw err;
  });
}

setInterval(scan, 2000);
scan();