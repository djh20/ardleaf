# ArdLeaf
An arduino library for reading information from a Nissan Leaf ev-canbus, supports MCP2515 controllers.

This is a personal project, so there isn't much documentation.

This repository contains files from the MCP_CAN library by coryjfowler https://github.com/coryjfowler/MCP_CAN_lib

## Installation
1. Download this repository.
2. Open Arduino, do Sketch > Include Library > Add .ZIP Library and select the zip file.

## Examples
This library contains a few example sketches. They can be found at **File > Examples > ArdLeaf.**
All of the examples print data to the serial monitor.
The "ArdLeaf_Bluetooth" example uses a HC-05 module to send data through bluetooth.

### Important
This library is being developed on a 2011 Nissan Leaf ZE0, it most likely won't work on later leaf models.