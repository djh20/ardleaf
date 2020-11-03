const SerialPort = require('serialport');
var ports = [];
var connected = false;


async function getPorts() {
  if (connected) return;
  const ports = await SerialPort.list();
  ports.forEach((port) => {
    console.log(port.path);
  });
  //console.log(ports);
}

setInterval(() => {
  getPorts();
}, 3000);

getPorts();