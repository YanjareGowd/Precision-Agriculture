#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;
#define motorStart D0
#define motorStop D1
#define solValve1 D2
#define solValve2 D3
#define rainDrop D8
#define control D5
#define mValve1 D6
#define mValve2 D7


const char* ssid = "Vi";   // SSID
const char* password = "29289190";       //  Password

int val;
int AirValue1=642;
int WaterValue1=307;
int AirValue2=642;
int WaterValue2=307;
int flag=0;
int flag1=0;
int abt=0;
int rn=0;

WiFiClient client;

unsigned long counterChannelNumber1 = 1693435; // Channel Number 
const char * myCounterReadAPIKey1 = "94X5ERLHJ53ZA09P"; //Write API 
unsigned long counterChannelNumber2 = 1706723; // Channel Number 
const char * myCounterReadAPIKey2 = "N03KHEUW4ICAGJ0N"; //Write API 
unsigned long counterChannelNumber3 = 1727683; // Channel Number 
const char * myCounterReadAPIKey3 = "H4EPBEFD9OR9OLYX"; //Write API


unsigned long myChannelNumber = 1727179; //Your Channel Number (Without Brackets) //FOR RAIN DATA
const char * myWriteAPIKey = "MRPVPEXO5I8IZA0K"; //Your Write API Key

int thinkspeak();

void setup()
{
  Serial.begin(9600);
  delay(10);                          
  WiFi.begin(ssid, password);
   while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to wifi");
  ThingSpeak.begin(client);  
  pinMode(motorStop, OUTPUT);//starter control pins Configured as OUTPUT
  pinMode(motorStart, OUTPUT);
  pinMode(solValve1, OUTPUT);//solanoid valves configured as output
  pinMode(solValve2, OUTPUT);
  pinMode(rainDrop, INPUT);//Raindrop sensor as input
  pinMode(control, INPUT);//AUTO_manual SW
  
  digitalWrite(motorStop, HIGH); //Default motor to off
  digitalWrite(motorStart, HIGH);
  digitalWrite(solValve1, LOW); //Default solenoid valve to open
  digitalWrite(solValve2, LOW);
}

void loop()
{
  int SoilPercente1;
  int SoilPercente2;
  go:int tempSW=digitalRead(control);
  rain(); 
  if(rn==1) 
  goto go;
    
  if(abt==1)              //to maintain stop switch 2sec.
    {
     delay(2000);
     digitalWrite(motorStop, HIGH);
    }
  switch ((tempSW)&&(rn==0))
  {
    case 1: automatic();break;
    case 0: manual();break;
  }
  
}

//END OF MAIN LOOP//

void rain()
{
  int rain=digitalRead(rainDrop);
  if((rain==1)&&(rn==0))                                   //Rain data write //Abort Auto call
    {
       thinkspeakWrite(HIGH);
       Serial.println("RAINING");
       Abort();
       rn=1;
    }
  else
  {
  thinkspeakWrite(LOW);
  rn=0;
  }
}
void automatic()
{
    int SoilPercente1, SoilPercente2,AutoManual,motorSTR,motorSTP,solVAL1,solVAL2;
  
    thinkspeakRead(&SoilPercente1, &SoilPercente2);                                           //Read Soil data
    thingspeakReadMnual(&AutoManual,&motorSTR,&motorSTP,&solVAL1,&solVAL2);                   //Read Manual-Field Data
    
    switch(AutoManual)
    {
      case 1:{
        Serial.println("AUTOMATIC MODE ON");
        Serial.println("Moisture Level: ");
        Serial.println(SoilPercente1);
        Serial.println(SoilPercente2);
  
              if ((SoilPercente1<=30)&&(SoilPercente2<=30)&&(flag==0))
                 {
                   if(flag1==0)
                      {
                        solenoidON();
                        starterON();
                        delay(20000); //presure maintainance
                        flag1=1; 
                        abt=0;
                      }
                   digitalWrite(solValve2,HIGH);
                   flag=1;
                  }
                  
              else if(((SoilPercente1)>=85)&&(SoilPercente2<=30)&&(flag=1))
                   {
                      digitalWrite(solValve2,LOW);
                      delay(15000);
                      digitalWrite(solValve1,HIGH);
                   }
                   
              else if((SoilPercente1>85)&&(SoilPercente2>85)&&(abt==0))
              {
                    Abort();
                   
              }
              }                                                                         //end of case 1
              break;
      case 0:   { 
                  Serial.println("Mobile Control Tunred ON");//////////Mobile control
                  if((AutoManual==0)&&(motorSTR==1))
                    {
                      starterON();
                    }
                  if(solVAL1==1){
                  digitalWrite(solValve1,LOW);
                  Serial.println("SOLVAL1 ON");
                  }
                      if(solVAL1==0)
                      {
                      digitalWrite(solValve1,HIGH);
                      Serial.println("SOLVAL1 OFF");
                      }
                      
                  if(solVAL2==1){
                      digitalWrite(solValve2,LOW);
                      Serial.println("SOLVAL2 ON");}
                      
                  if(solVAL2==0){
                      digitalWrite(solValve2,HIGH);
                      Serial.println("SOLVAL2 OFF");}
                      
                  if(motorSTP==1){
                    Abort();}
              }
              break;
   }
}

void manual()
{
  Serial.println("Manul ON, Control Using Starter");
    int mtemp1;
    int mtemp2;
    mtemp1=digitalRead(mValve1);
    mtemp2=digitalRead(mValve2);
    if(mtemp1==1)
    digitalWrite(solValve1,LOW);
    else
    digitalWrite(solValve1,HIGH);
    if(mtemp2==1)
    digitalWrite(solValve2,LOW);
    else
    digitalWrite(solValve2,HIGH);
    
}


int thinkspeakRead(int *SoilPercente1,int *SoilPercente2)
{
  long temp1 = ThingSpeak.readLongField(counterChannelNumber1, 1, myCounterReadAPIKey1);
  long temp2 = ThingSpeak.readLongField(counterChannelNumber2, 1, myCounterReadAPIKey2);
   *SoilPercente1=map(temp1, AirValue1, WaterValue1, 0, 100);//soil Moisture value to percentage conversion
   *SoilPercente2=map(temp2, AirValue2, WaterValue2, 0, 100);
return 0; 
}

int thingspeakReadMnual(int *AutoManual, int *motorSTR, int *motorSTP, int *solVAL1, int *solVAL2)
{
  *AutoManual=ThingSpeak.readLongField(counterChannelNumber3, 5, myCounterReadAPIKey3);
  delay(100);
  *motorSTR=ThingSpeak.readLongField(counterChannelNumber3, 1, myCounterReadAPIKey3);
  delay(100);
  *motorSTP=ThingSpeak.readLongField(counterChannelNumber3, 2, myCounterReadAPIKey3);
  delay(100);
  *solVAL1=ThingSpeak.readLongField(counterChannelNumber3, 3, myCounterReadAPIKey3);
  delay(100);
  *solVAL2=ThingSpeak.readLongField(counterChannelNumber3, 4, myCounterReadAPIKey3);
  return 0;
}


int thinkspeakWrite(int raindrop)
{
  ThingSpeak.writeField(myChannelNumber, 1,raindrop, myWriteAPIKey); //Update rain data in ThingSpeak
  ThingSpeak.setField(1,(int)raindrop);
}



void starterON()
{
  digitalWrite(motorStart,LOW);
  delay(15000);
  digitalWrite(motorStart,HIGH);
}

void Abort()
{
  digitalWrite(motorStop, LOW);
  Serial.println("STARTER OFF");
  solenoidOFF();
  flag1=0;
  abt=1;
  flag=0;
  //ESP.reset();
}
void solenoidOFF()
{
  digitalWrite(solValve1,HIGH);
  digitalWrite(solValve2,HIGH);
}

void solenoidON()
{
  digitalWrite(solValve1,LOW);
  digitalWrite(solValve2,LOW);
}
