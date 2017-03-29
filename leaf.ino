//------------------------------------------------------------------------------------
// Libraries Needed For This Project
//------------------------------------------------------------------------------------
//  #include <EEPROM.h>         // To Be Able To Store & Read Data Between Power Off
  #include <ESP8266WiFi.h>    // The Basic Function Of The ESP NOD MCU
  #include "DHT.h"
//------------------------------------------------------------------------------------
// Defining I/O Pins
//------------------------------------------------------------------------------------
  #define       LED0      2         // WIFI Module LED
  #define       LED1      D0        // Indicate Connectivity With Client #1
  #define       LED2      D2        // Indicate Connectivity With Client #2
//------------------------------------------------------------------------------------
// LED Delay Variables
//------------------------------------------------------------------------------------
  int           LEDState          = LOW;
  unsigned long CurrMillis        = 0;
  unsigned long PrevMillis        = 0;
  unsigned long Interval          = 1000;
//------------------------------------------------------------------------------------
// WIFI Authentication Variables
//------------------------------------------------------------------------------------
/* This Client Is Going To Connect To A WIFI Server Access Point
 * So You Have To Specify Server WIFI SSID & Password In The Code Not Here
 * Please See The Function Below Name (WiFi.Begin)
 * If WIFI dont need Any password Then WiFi.begin("SSIDNAME")
 * If WIFI needs a Password Then WiFi.begin("SSIDNAME", "PASSWORD")
 */
  char*         TKDssid;            // Wifi Name
  char*         TKDpassword;        // Wifi Password
  #define DHTPIN          5                   // DHT sensor signal pin.
  #define DHTTYPE         DHT11               // DHT sensor type (DHT11, DHT22,
                                            // or DHT21).  The AM2302 is a DHT22
                                            // and the AM2301 is a DHT21.
//------------------------------------------------------------------------------------
// WIFI Module Role & Port
//------------------------------------------------------------------------------------
/* This WIFI Module Code Works As A Client
 * That Will Connect To A Server WIFI Modul With (IP ADDress 192.168.4.1)
 */
  int             TKDServerPort  = 5050;
  IPAddress       TKDServer(192,168,0,50);
  WiFiClient      TKDClient;
//====================================================================================
  DHT dht(DHTPIN, DHTTYPE);
  float temperature, humidity;
  int c;
  
  void setup() 
  {
    // Setting The Serial Port ----------------------------------------------
    Serial.begin(115200);           // Computer Communication
    
    // Setting The Mode Of Pins ---------------------------------------------
    pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
    digitalWrite(LED0, !LOW);       // Turn WiFi LED Off
    
    // Print Message Of I/O Setting Progress --------------------------------
    Serial.println("\nI/O Pins Modes Set .... Done");

    
    
    // Conecting The Device As A Client -------------------------------------
    //TKDRequest();
  }

//====================================================================================
  
  void loop()
  {

    // Starting To Connect --------------------------------------------------
    WiFi.mode(WIFI_STA);            // To Avoid Broadcasting An SSID
    WiFi.begin("NODE2","sau100008");          // The SSID That We Want To Connect To

    // Printing Message For User That Connetion Is On Process ---------------
    Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");

    // WiFi Connectivity ----------------------------------------------------
    CheckWiFiConnectivity();        // Checking For Connection

    // Stop Blinking To Indicate Connected ----------------------------------
    digitalWrite(LED0, !HIGH);
    Serial.println("!-- Client Device Connected --!");

    // Printing IP Address --------------------------------------------------
    Serial.println("Connected To      : " + String(WiFi.SSID()));
    Serial.println("Signal Strenght   : " + String(WiFi.RSSI()) + " dBm");
    Serial.print  ("Server IP Address : ");
    Serial.println(TKDServer);
    Serial.print  ("Device IP Address : ");
    Serial.println(WiFi.localIP());
    
    ReadButton();
    TKDRequest();
    digitalWrite(LED1, HIGH);
    delay(1000);
    digitalWrite(LED1, LOW);
    delay(7000);
  }

//====================================================================================

  void ReadButton()
  {
          
        
          humidity = dht.readHumidity();
          temperature = dht.readTemperature();
          delay(2000); 
        
          char tempF[6]; 
          char humF[6];
          dtostrf(temperature, 5, 1, tempF);
          dtostrf(humidity, 2, 0, humF);
          Serial.print("Temperature : ");
          Serial.print(temperature);
          Serial.print(" Humidity: ");
          Serial.println(humidity);
          
  }

//====================================================================================

  void CheckWiFiConnectivity()
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      for(int i=0; i < 10; i++)
      {
        digitalWrite(LED0, !HIGH);
        delay(250);
        digitalWrite(LED0, !LOW);
        delay(250);
        Serial.print(".");
      }
      Serial.println("");
    }
  }


  void TKDRequest()
  {
    // First Make Sure You Got Disconnected
    TKDClient.stop();

    // If Sucessfully Connected Send Connection Message
    if(TKDClient.connect(TKDServer, TKDServerPort))
    {
          c++;
          Serial.println    ("CONNECTED 2 and DATA SEND");
          TKDClient.print("NODEMCU BOARD 1: ----- Humidity : ");
          TKDClient.print(humidity);
          TKDClient.print(" Temperature : ");
          TKDClient.println(temperature);
          TKDClient.print(" PACKET NUMBER : ");
          TKDClient.println(c);
          TKDClient.flush();      
    }
  }
