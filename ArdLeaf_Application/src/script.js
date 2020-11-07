const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline')
var info = {};
var ports = [];
var connectedPort;

const SCREEN_1 = document.getElementById("screen-1");
const SCREEN_2 = document.getElementById("screen-2");
const SPEED_LABEL = document.getElementById("speed-label");
const GEAR_LABEL = document.getElementById("gear-label");
const POWER_LABEL = document.getElementById("power-label");
const POWER_BAR_POSITIVE = document.getElementById("power-bar-positive");
const POWER_BAR_NEGATIVE = document.getElementById("power-bar-negative");
const ECO_INDICATOR = document.getElementById("eco-indicator");
const CAR_STATUS_LABEL = document.getElementById("car-status");
const CHARGE_LABEL = document.getElementById("car-charge");
const AMBIENT_TEMP_LABEL = document.getElementById("ambient-temp");
const BATTERY_TEMP_LABEL = document.getElementById("battery-temp");
const AC_SPEED_LABEL = document.getElementById("ac-speed");

ECO_INDICATOR.style.display = "none";

function setScreen(screen) {
  if (screen == 1) {
    SCREEN_1.style.display = "block";
    SCREEN_2.style.display = "none";
  } else {
    SCREEN_1.style.display = "none";
    SCREEN_2.style.display = "flex";
  }
}
function update(type, value) {
  if (type == "spd") {
    value = parseInt(value);
    SPEED_LABEL.innerText = value;

  } else if (type == "gear") {
    if (value == 0) value = "P";
    else if (value == 1) value = "P";
    else if (value == 2) value = "R";
    else if (value == 3) value = "N";
    else if (value == 4) value = "D";
    GEAR_LABEL.innerText = value;

  } else if (type == "tmp_a") { // Ambient temperature
    value = parseFloat(value);
    AMBIENT_TEMP_LABEL.innerText = `${value}°C`;

  } else if (type == "tmp_b") { // Ambient temperature
    value = parseFloat(value);
    BATTERY_TEMP_LABEL.innerText = `${value}°C`;

  } else if (type == "ac") { // A/C speed
    AC_SPEED_LABEL.innerText = value;

  } else if (type == "soc") {
    value = parseFloat(value);
    CHARGE_LABEL.innerText = `${value}%`;
    
  } else if (type == "kw") {
    value = parseFloat(value);
    var positiveValue = (value/90)*100;
    if (positiveValue < 0) positiveValue = 0;

    var negativeValue = (-value/20)*100;
    if (negativeValue < 0) negativeValue = 0;

    POWER_LABEL.innerText = `${value}kW`;
    POWER_BAR_POSITIVE.style.width = `${positiveValue}%`;
    POWER_BAR_NEGATIVE.style.width = `${negativeValue}%`;

  } else if (type == "eco") {
    value = (value == 1 ? true : false);

    if (value) {
      ECO_INDICATOR.style.display = "";
    } else {
      ECO_INDICATOR.style.display = "none";
    }

  } else if (type == "on") {
    value = (value == 1 ? true : false);

    if (value) {
      CAR_STATUS_LABEL.innerText = "ON";
    } else {
      CAR_STATUS_LABEL.innerText = "OFF";
    }
  }

  info[type] = value;
}
function connect(path) {
  return new Promise((resolve, reject) => {
    let port = new SerialPort(path, {baudRate: 115200}, function (err) {
      if (err) {
        reject();
        return console.log('Error: ', err.message);
      }
    });
    connectedPort = port;
    setScreen(2);
    
    port.on('close', function() {
      setScreen(1);
      connectedPort = false;
    });

    const parser = port.pipe(new Readline({ delimiter: '\n' }))
    parser.on('data', function(data) {
      if (data == "") return;
      let split = data.split(" ");
      let type = split[0];
      let value = split[1];

      update(type, value);
      console.log(info);
    });

    resolve(`Connected to ${path}`);
  });
}
async function getPorts() {
  if (connectedPort) return;
  const ports = await SerialPort.list();
  for (i in ports) {
    let port = ports[i];
    console.log(port);
    if (port.manufacturer == "Arduino LLC (www.arduino.cc)" || port.manufacturer == "wch.cn") {
      let result = await connect(port.path);
      console.log(result);
    }
  }
}

setInterval(getPorts, 2000);
getPorts();

/*
function darkMode() {
  let elements = [].slice.call( document.body.getElementsByTagName("*") );
  
  elements.forEach((element) => {
    element.classList.add("dark-mode");
  });

  document.body.classList.add("dark-mode");
  
}
*/