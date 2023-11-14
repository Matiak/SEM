# SEM project 1
### author: Martin Babaca
### mail: xbabac02@stud.fit.vutbr.cz

# Data format
Data were collected into csv files with following structure:
1st column: timestamp
2nd column: DHT11 temperature
3rd column: DS18B20 temperature
4th column: LM35DZ temperature
5th column: Thermistor temperature
6th column: DHT11 relative humidity

# How to run project
Project is made in platformio extension of Visual Studio Code for ARDUINO UNO.
It should be build and upload via VSCode with platformio extension.

## Collecting data
For collecting of data were used 2 commands:
for setup:

stty -F /dev/ttyACM0 raw 9600

for write itself:

cat /dev/ttyACM0 | ts >data.txt

to install ts utility use:

sudo apt install moreutils
