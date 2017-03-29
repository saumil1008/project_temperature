# project_temperature
Temperature monitoring using esp8266 using wifi
the project contains 3 files. one for leaf nodes, one for middle nodes, and one for sink node.
I have used ESP8266 12E as micro-controller, DHT11 temperature sensor and Raspberry PI as Base station.
I have used ARDUINO IDE and follwing libraries.
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiAP.h>
#include <DHT.h>
First the last node trasmites the data packet to middle node and after that middle node will forward packet to sink node and then finally packet arrived at base station.
