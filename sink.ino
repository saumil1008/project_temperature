// Raspberry Pi Talking To WiFi Things Part 1
// Use an ESP8266 connected to a DHT temperature & humidity sensor
// to log its sensor readings to a Raspberry Pi over WiFi.
// Based on the WiFiClientBasic example in the ESP8266 Arduino repository.
// Author: Tony DiCola
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiAP.h>

#include "DHT.h"
#define     LED0      2           // WIFI Module LED
#define     LED1      D0          // Connectivity With Client #1

// Configuration:
#define WIFI_NAME       "TP-LINk"      // Your WiFi AP.
#define WIFI_PASSWORD   "sau100008"  // Your WiFi AP password.
#define PI_IP_ADDRESS   "192.168.0.30"      // IP address of the listening Pi.
#define NAME            "NODEMCU BOARD 3: ----- "       // Name of the device, written as the
                                            // first CSV value.
#define PI_PORT         5000                // Port for the listening Pi.
#define DHTPIN          5                   // DHT sensor signal pin.
#define DHTTYPE         DHT11               // DHT sensor type (DHT11, DHT22,
                                            // or DHT21).  The AM2302 is a DHT22
                                            // and the AM2301 is a DHT21.

WiFiServer server(5040);
DHT dht(DHTPIN, DHTTYPE);
ESP8266WiFiMulti WiFiMulti;
ESP8266WiFiAPClass WiFiAP;
String request;
boolean f=false;

char*       TKDssid;              // SERVER WIFI NAME
char*       TKDpassword;          // SERVER PASSWORD
const uint16_t port = PI_PORT;
const char * host = PI_IP_ADDRESS; // ip or dns
void setup() {
    Serial.begin(115200);
    delay(10);

    dht.begin();
    // Setting The Mode Of Pins
    pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
    pinMode(LED1, OUTPUT);          // Indicator For Client #1 Connectivity
    

}

void loop() {

    rec();
    sen();
    
    
}

void sen(){
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
        Serial.println("WIFI Mode : Station");
        // We start by connecting to a WiFi network
        WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
    
        Serial.println();
        Serial.println();
        Serial.print("Wait for WiFi... ");
    
        while(WiFiMulti.run() != WL_CONNECTED) {
            Serial.print(".");
            digitalWrite(LED0, LOW);
            delay(250);
            digitalWrite(LED0, HIGH);
            delay(250);
        }
        digitalWrite(LED0, HIGH);
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
    }

      float temperature, humidity;
    
      humidity = dht.readHumidity();
      temperature = dht.readTemperature();
      delay(2000); 
    
     char tempF[6]; 
     char humF[6];
     dtostrf(temperature, 5, 1, tempF);
     dtostrf(humidity, 2, 0, humF);
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humdity: ");
    Serial.println(humidity);
    // Send the sensor data to the server as a comma separated list.
    client.print(NAME);
    client.print("  Humidity : ");
    client.print(humidity, 2);
    client.print("  Temperature : ");
    client.println(temperature, 2);
    client.println("Data from connected node: ");
    client.println(request);
    
    digitalWrite(LED1, HIGH);
    delay(1000);
    digitalWrite(LED1, LOW);
    delay(1000);
    Serial.println("closing connection");
    client.stop();

    Serial.println("wait 5 sec...");
    delay(5000);
}


void rec(){
        WiFi.disconnect();
        // Setting The Wifi Mode
        WiFi.mode(WIFI_AP);
        Serial.println("WIFI Mode : AccessPoint");
        
        // Setting The AccessPoint Name & Password
        TKDssid      = "NODE3";
        TKDpassword  = "sau100008";
        


        // Starting The Access Point
        WiFi.softAP(TKDssid, TKDpassword);
        Serial.println("WIFI < " + String(TKDssid) + " > ... Started");
        
        // Wait For Few Seconds
        delay(1000);
        
        // Start the server
        server.begin();
        Serial.println("Server started");
            
        // Print the IP address
        Serial.println("ip address..");
        IPAddress ip(192,168,0,40);
        IPAddress gateway(192,168,0,1);
        Serial.println(F("selecting static ip to: "));
        IPAddress subnet(255,255,255,0);
        WiFiAP.softAPConfig(ip,gateway,subnet);
        Serial.println(WiFiAP.softAPIP());
    
        delay(500);


        
        // Check if a client has connected
        WiFiClient client = server.available();
        while(!client) {
          client = server.available();
        }
        //else {
              int c=0;
              // Wait until the client sends some data
              Serial.println("new client");
              while(!client.available()){
                if(c==3000){
                  break;
                }
                delay(1);
                c++;
              }
             
              // Read the first line of the request
              request = client.readStringUntil('\0');
              Serial.println(request);
              client.flush();
        //}
        delay(5000);
}

