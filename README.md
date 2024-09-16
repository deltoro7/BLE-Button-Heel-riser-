# BLE-Button-Heel-riser-
Senior Design project final code and schematics. This code serves to be a demonstration of the final product and to provide an open source Bluetooth low energy (BLE) guide to arduino Nano BLE. 

This project uses 2 Arduino BLE sense rev2, to communicate over the Arduino BLE library and protocol, note other BLE peripherals and centrals will conflict with the given library. I.E: if you plan to use an ESP32 microcontroller, you will need two to communciate. A single Arduino and ESP32 cannot communicate with this library. 

The basis of this project was to create a easier method to lifting an Back Country skiing heel riser. Current methods require the skier to bend down and flip the riser manually, or use their pole. This final iteration uses a linear actuator and DC motor to move the heel riser up or down. 

![alt text](solidworks_part.JPG)



### Wiring Schmeatics 

![alt text](heel_wire.JPG) 

![alt text](pole_wire.JPG)
