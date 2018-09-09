# Arduino

## Autonomous Vehicle

Functional code for Navigation, Sweeping and Weighing function of an autonomous vehicle. This vehicle is coded to run in a test environment(sandbox) where it will communicate with a satellite using RF sensors to navigate through the start position. It will then use an ultrasonic sensor to detect obstacles and then navigate through boulders to the final position where it will use its mechanical scoop to lift a cylindrical object off the sand. As an added bonus it will then weigh the object and then transmit the data to the satellite.

## Data Logger

This code is supported by Ubidots' external libraries and servers. The data logger can be uploaded to any Arduino/ESP8266 duo which can then log analog data from a sensor(Ultrasonic, Temperature, MQ-X's, etc) to the Ubidots' server where I can view it or push it to a mobile application using their API. 

## QR Clock

This project will run on an LCD screen where it will print the current time in a QR code format which can be verified using any QR scanner. 

## Slave-Master Communication

This is a useful library which can make two arduino's connected in Master-Slave combination can communicate with each other using the i2c channel. 
