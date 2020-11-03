# ArdLeaf
An arduino library for reading information from a Nissan Leaf canbus.
Includes an application that displays information like speed, power consumption and current gear.

This is a personal project, so there isn't much documentation.

ArdLeaf uses files from the MCP_CAN library by coryjfowler https://github.com/coryjfowler/MCP_CAN_lib 
(These files come with this library).

This README will be updated with more information when I have time.

## Installation
1. Download the ZIP from https://github.com/danmrdj/ardleaf/archive/master.zip
2. Extract the ZIP file
3. Copy the "ArdLeaf_Library" to your arudino libraries folder

## Application
The application is located at ArdLeaf_Application\out\ardleaf-win32-x64\ardleaf.exe (64-bit, Windows)
When started, it will search through COM ports for an arduino (Make sure any other arduinos are disconnected).

Image of application:
![Image of application](Images/application.jpg)