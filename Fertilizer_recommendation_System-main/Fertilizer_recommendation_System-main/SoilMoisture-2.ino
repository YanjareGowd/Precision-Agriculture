#include <ESP8266WiFi.h>
#include <WiFiClient.h>;
#include <ThingSpeak.h>;

#define soilMoi1 A0  //V2.0 Soil Moisture PIN To A0


const char* ssid = "Vi";   // Your Network SSID
const char* password = "29289190";       // Your Network Password
int soilVal1;


WiFiClient client;


unsigned long myChannelNumber = 1706723; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "Y35JM1W9L0U4QRQ8"; //Your Write API Key

int soilMoisture();
float thinkspeak(int moisture1);


void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to wifi");
  
  ThingSpeak.begin(client);
 
}

void loop()
{
 
  soilMoisture(&soilVal1);
  Serial.print("Moisture1 level: ");// Print on Serial Monitor
  Serial.println(soilVal1);   
  thinkspeak(soilVal1);    //updates in thingspeak
}

int soilMoisture(int *moi1)
{
   *moi1=analogRead(soilMoi1);
   
   delay(100);
   return 0;
}

float thinkspeak(int moisture1)
{
  ThingSpeak.writeField(myChannelNumber, 1,moisture1, myWriteAPIKey); //Update in ThingSpeak
  ThingSpeak.setField(1,(int)moisture1);
  return 0;
}
