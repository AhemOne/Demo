# XCIS

## Project

XCIS aims at producing low cost customised monitoring solutions for farmers. Working closely with one of Australias premier wagyu farms, our team produced a network of sensors using the LoRa standard for data tranmission. The solution produced three working components to form a basic network; 
- **Gateways** which moderated a interface between the outside world and the XCIS network. These devices were WiFi enabled and reply to HTTP requests in order to return data to an external connection. These use an ESP8266EX Core and are linked by SPI to a prebuilt LoRa Modules. 
- **Nodes** which act as a data transceiver and a solar charged power source for sensors. These units have an ATMega328P core and linked by SPI to prebuilt LoRa Modules. These boards then interface to a loop of sensors, providing power and data connections to them. These boards have lithium battery chargers installed and connect to 18650 cells which are in turn charged from a 5-10 watt solar panel.
- **Devices** which are the sensor units. These units use an ATMega328P core and may link to a sensor by any unused pins. The device boards are designed to provide an Arduino-link environment for rapid prototyping and deployment of sensor solutions. A series of breakout points and prototyping space is provided to accomodate sensors which may be interfaced to the board by analog and digital I/O, SPI, I2C or UART. The data is then transmitted to the attached node.

Nodes and Devices were designed to be used in conjunction, whereby a single node could be daisy chained to up to three sensors. This modular approach allows nodes to be pole mounted to ensure the solar panels and battery was clear of any livestock and maintained proper sunlight collection, while the sensor devices could be placed at the point of reading. To install a farmer would install a node, then connect the required devices (i.e sensors) for the parameters being monitored. A typical installation was at watering troughs. A pole mounted node was then wired to an ultrasonic water level sensor and temperature sensor at the trough.

## Software

### Usage agreement

Permission has been granted by XCIS to reproduce this code for the purpose of demonstrating my (Jesse Falzon) coding technique. It may not be reproduced in any other form and remains property of XCIS.

### Introduction

This code forms an early iteration for message transmission within the XCIS LoRa environment

### Files

- Packet.([hpp](Packet.hpp)/[cpp](Packet.cpp)): A type to contain an incoming or outgoing data packet.
- SimpleBuffer.[hpp](SimpleBuffer.hpp): A buffer type for the packet to use, can be static or dynamic.
- Helper.([hpp](Helper.hpp)/[cpp](Helper.cpp)): Some type definitions to get things going.
- Debug.[hpp](Debug.hpp): Some debugging facilities.

## Hardware

### Boards

Boards were designed using KiCAD and manufactured externally. Prototypes were produced one side at time using a screen printed solder mask, SMD components were then hand placed and that side was baked in an infra-red soldering oven. Finally, through hole components then soldered by hand.

![4 XCIS boards](XCIS%20Boards.jpg)

These are some example boards produced while working for XCIS. Clockwise from upper left:
1)  Populated "Node" board, topside.
2)  Populated "Gateway board", topside. The blue rework wire was due to a fault in design. The ESP8266EX programming pin is shared with an SPI channel. While this functionally works, if processor is reset while an SPI device is holding this pin low the processor is unable to exit its programming mode, requiring a full power cycle to ensure the SPI device is reset. To address this the LoRa module was placed on a second available SPI bus.
3)  Unpopulated board, underside
4)  Populated "Node" board, underside.
