//includem libraria specifica LCD
#include <LiquidCrystal.h>
// #define aref_voltage 3.3
//se initializeaza libraria cu numarul pinilor digitali pe care i-am folosit
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//definim constantele corespunzatoare pinilor analogici folositi pentru senzori
//definim constanta corespunzatoare pinului digital folosit pentru pompa de apa
const int TsensorPin=A0; //senzor temperatura
const int UsensorPin=A3; //senzor umiditate
const int LsensorPin=A2; //senzor luminozitate
const int pompaPin=6; //pompa de apa

//initializam corespondentii in procente a valorilor primite de la senzori
int procentUm;
int procentLum;


//definim pragurile in functie de care planta va fi udata sau nu
int pragU=45;
int pragL=25;
int pragT=25;

//initializam variabilele de calibrare a valorii minime si maxime
int sensorLow = 1024;
int sensorHigh = 0;

void setup() {

// se initializeaza pinul pompei ca output
  pinMode(pompaPin,OUTPUT);
// setam numarul de coloane si linii pe care le are LCD-ul folosit
  lcd.begin(20, 4);

 //se realizeaza calibrarea fotocelulei
 while (millis() < 5000) {
    // inregistreaza valoarea de maxim
     LsensorVal = analogRead(LsensorPin);
    if (LsensorVal > sensorHigh) {
      sensorHigh = LsensorVal;
    }
    // inregistreaza valoarea de minim
    if (LsensorVal < sensorLow) {
      sensorLow = LsensorVal;
    }
  }

    // era folosit pentru definirea lui aref si configureaza tensiunea de referinta pentru intrarea analog
    // analogReference(0);

}

void loop() {  
  //se citesc valoarile de pe A0,A3,A2 
  //si se stocheaza in mai multe variabile 
   
  int TsensorVal = analogRead(TsensorPin);      
  int UsensorVal = analogRead(UsensorPin);
  int LsensorVal = analogRead(LsensorPin);

//Temperatura
  
  //se realizeaza conversia dintre valoarea citita de pe ADC in Volti
  
  float voltage = (TsensorVal / 1024.0) * 5.0;

  //Valabil in cazul in care folosim aref_voltage la 3.3V
  //float voltage = (TsensorVal/1024.0)*aref_voltage;
  
  
  //face conversia din volti in grade Celsius
  //sensorul se schimba cu 10mv per grad 
  //in datasheet se precizeaza un offset de 500mV
  // ((voltage - 500mV) times 100)

  float temp = (voltage - .5) * 100;
  lcd.print("Temp=");
  lcd.print(temp);   
  lcd.print(" C");
  
//Umiditate
  //maparea senzorului de umiditate 
  procentUm=map(UsensorVal,509,211,0,100);
 
  lcd.setCursor(0,1); //cursorul LCD-ului se pozitioneaza pe coloana 0, linia 1
  lcd.print("Umiditate:"); 
  if (procentUm < 100) lcd.print(" ");
  if (procentUm < 10) lcd.print(" ");
  lcd.print(procentUm);
  //lcd.print(UsensorVal);
  lcd.print("%");
  
//Luminozitate
    //maparea senzorului de luminozitate
  procentLum=map(LsensorVal,0,950,0,100);

  lcd.setCursor(0,2); //cursorul LCD-ului se pozitioneaza pe coloana 0, linia 2
  lcd.print("Luminozitate:");
  if (procentLum < 100) lcd.print(" ");
  if (procentLum < 10) lcd.print(" ");
  lcd.print(procentLum);
  //lcd.print(LsensorVal);
  lcd.print("%");

//Pornire/Oprire pompa in functie de pragul de udare stabilit
  if(procentUm<pragU && procentLum < pragL && temp<pragT )
  {
    lcd.setCursor(0,3); //cursorul LCD-ului se pozitioneaza pe coloana 0, linia 3
    lcd.print("Planta trebuie udata");
    digitalWrite(pompaPin,HIGH);
}
else{ 
  lcd.setCursor(0,3); //cursorul LCD-ului se pozitioneaza pe coloana 0, linia 3
  lcd.write("Parametrii normali");
  digitalWrite(pompaPin,LOW);
}
  delay(1000);            
  lcd.clear();
}

