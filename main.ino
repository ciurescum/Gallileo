//include LCD Library
#include <LiquidCrystal.h>
// #define aref_voltage 3.3
//Initialise GPIO pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Constants definition

const int TsensorPin=A0; //temperature sensor
const int UsensorPin=A3; //humidity sensor
const int LsensorPin=A2; //luminosity sensor
const int pompaPin=6; //water pump


int procentUm;
int procentLum;


//define limits when the plant should be watered
int pragU=45;
int pragL=25;
int pragT=25;

//calibration values
int sensorLow = 1024;
int sensorHigh = 0;

void setup() {

// initialise water pump pin as an output
  pinMode(pompaPin,OUTPUT);
  lcd.begin(20, 4);

 //calibration of the photocell for luminosity
 while (millis() < 5000) {
    // max value
     LsensorVal = analogRead(LsensorPin);
    if (LsensorVal > sensorHigh) {
      sensorHigh = LsensorVal;
    }
    // min value
    if (LsensorVal < sensorLow) {
      sensorLow = LsensorVal;
    }
  }

    // reference analog voltage 
    // analogReference(0);

}

void loop() {  
  //read analog sensors
 
   
  int TsensorVal = analogRead(TsensorPin);      
  int UsensorVal = analogRead(UsensorPin);
  int LsensorVal = analogRead(LsensorPin);

//Temp
  
  //conversion from adc values to v 
  
  float voltage = (TsensorVal / 1024.0) * 5.0;

  //if aref_voltage is 3.3V
  //float voltage = (TsensorVal/1024.0)*aref_voltage;
  
  

  // ((voltage - 500mV) times 100)

  float temp = (voltage - .5) * 100;
  lcd.print("Temp=");
  lcd.print(temp);   
  lcd.print(" C");
  
//humidity
  //mapping the humidity sensor
  procentUm=map(UsensorVal,509,211,0,100);
 
  lcd.setCursor(0,1); 
  lcd.print("Humidity:"); 
  if (procentUm < 100) lcd.print(" ");
  if (procentUm < 10) lcd.print(" ");
  lcd.print(procentUm);
  //lcd.print(UsensorVal);
  lcd.print("%");
  
//Luminosity
    //mapping the luminosity sensor
  procentLum=map(LsensorVal,0,950,0,100);

  lcd.setCursor(0,2); 
  lcd.print("Luminosity:");
  if (procentLum < 100) lcd.print(" ");
  if (procentLum < 10) lcd.print(" ");
  lcd.print(procentLum);
  //lcd.print(LsensorVal);
  lcd.print("%");

//start/stop water pump
  if(procentUm<pragU && procentLum < pragL && temp<pragT )
  {
    lcd.setCursor(0,3); 
    lcd.print("Plant needs water");
    digitalWrite(pompaPin,HIGH);
}
else{ 
  lcd.setCursor(0,3); 
  lcd.write("Normal Param");
  digitalWrite(pompaPin,LOW);
}
  delay(1000);            
  lcd.clear();
}

