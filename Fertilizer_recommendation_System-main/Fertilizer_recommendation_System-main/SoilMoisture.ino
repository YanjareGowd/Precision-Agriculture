const int AirValue = 620;   
const int WaterValue = 310;  
int soilMoistureValue = 0;
int soilmoisturepercent=0;
#define analog A0
void setup() {
  Serial.begin(9600); 
}
void loop() {
soilMoistureValue = analogRead(analog); 
Serial.println(soilMoistureValue);
digitalWrite(0,HIGH);
soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
if(soilmoisturepercent >= 100)
{
  Serial.println("100 %");
}
else if(soilmoisturepercent <=0)
{
  Serial.println("0 %");
}
else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
{
  Serial.print(soilmoisturepercent);
  Serial.println("%");
  
}
  delay(250);
}
