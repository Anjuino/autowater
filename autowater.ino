#include <avr/wdt.h>
#include <microDS18B20.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SimpleKeypad.h>

#define OLED_RESET 4
#define soilWet 500   // Определяет максимальное значение, при котором, мы решили, что почва 'влажная'
#define soilDry 750   // Определяет минимальное значение, при котором, мы решили, что почва 'сухая'
0
#define sensorPin A0
#define RELAY_IN1 7
#define RELAY_IN2 3

MicroDS18B20<8> sensor;
Adafruit_SSD1306 display(OLED_RESET);

unsigned long timer1;
unsigned long timer2;

const byte nb_rows = 4;                         // four rows
const byte nb_cols = 4;                         // four columns
char key_chars[nb_rows][nb_cols] = {            // The symbols of the keys
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[nb_rows] = {13, 12, 11, 10};           // The pins where the rows are connected
byte colPins[nb_cols] = {6, 5, 4, 3};           // The pins where the columns are connected

SimpleKeypad kp1((char*)key_chars, rowPins, colPins, nb_rows, nb_cols);   // New keypad called kp1
void setup() 
{
pinMode(sensorPower, OUTPUT); 
pinMode(RELAY_IN1, OUTPUT);  
pinMode(RELAY_IN2, OUTPUT); 
display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
digitalWrite(sensorPower, LOW);
timer1 = millis(); 
timer2 = millis();
Serial.begin(9600);
int ground = readSensor();
  display.display();
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  if (ground < soilWet) 
  {

    display.setCursor(0,10);
    display.print("Hum : "); 
    display.print(ground);
    display.setCursor(0,0);    
    display.print("Status: wet");       
  }
  else if (ground >= soilWet && ground < soilDry) 
  {
    display.setCursor(0,10);
    display.print("Hum : ");  
    display.print(ground);
    display.setCursor(0,0);    
    display.print("Status: perfect");      
  }
  else 
  {
    display.setCursor(0,10);
    display.print("Hum : ");  
    display.print(ground);
    display.setCursor(0,0);    
    display.print("Status: Time to water");               
  }
   sensor.requestTemp(); // запрос температуры
   int t = sensor.readTemp() ;
   Serial.println(sensor.getTemp());      
   display.setTextColor(WHITE);
   display.setTextSize(1);
   display.setCursor(0,20);
   display.print("Temp : ");         
   display.print(sensor.getTemp());  
   sensor.requestTemp();
}
void loop() 
{  
  digitalWrite(RELAY_IN1, HIGH);
  digitalWrite(RELAY_IN2, HIGH);

if (millis() - timer1 > 1200000) {
  int ground = readSensor();
  display.display();
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  if (ground < soilWet) 
  {

    display.setCursor(0,10);
    display.print("Hum : "); 
    display.print(ground);
    display.setCursor(0,0);    
    display.print("Status: wet");    
              
  }
  else if (ground >= soilWet && ground < soilDry) 
  {
    display.setCursor(0,10);
    display.print("Hum : ");  
    display.print(ground);
    display.setCursor(0,0);    
    display.print("Status: perfect"); 
         
  }
  else 
  { 
    digitalWrite(RELAY_IN1, LOW);
    delay(7000);
    display.setCursor(0,10);
    display.print("Hum : ");  
    display.print(ground);
    display.setCursor(0,0);    
    display.print("Status: Time to water"); 
                      
  }
  timer1 = millis();
}
if (millis() - timer2 > 30000) {
   sensor.requestTemp(); // запрос температуры
   int t = sensor.readTemp() ;
   Serial.println(sensor.getTemp());      
   display.setTextColor(WHITE);
   display.setTextSize(1);
   display.setCursor(0,20);
   display.print("Temp : ");         
   display.print(sensor.getTemp());  
   sensor.requestTemp();
   timer2 = millis();
   }   
  char key = kp1.getKey();                      // The getKey function scans the keypad every 10 ms and returns a key only one time, when you start pressing it
  if (key == '7') {  
      pinMode(RELAY_IN2, OUTPUT); 
      digitalWrite(RELAY_IN2, LOW); 
      delay(4000)  ;
      digitalWrite(RELAY_IN2, HIGH);                             // If getKey returned any key
    Serial.println(key);                        // it is printed on the serial monitor
  } 
  digitalWrite(RELAY_IN1, HIGH);                      // The getKey function scans the keypad every 10 ms and returns a key only one time, when you start pressing it
  if (key == '4') {  
      pinMode(RELAY_IN1, OUTPUT); 
      digitalWrite(RELAY_IN1, LOW); 
      delay(4000)  ;
      digitalWrite(RELAY_IN1, HIGH);                             // If getKey returned any key
    Serial.println(key);                        // it is printed on the serial monitor
  } 
  if (key == '1') {  
      int ground = readSensor();
      display.display();
      display.clearDisplay();
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(0,10);
      display.print("Hum : "); 
      display.print(ground);  
      sensor.requestTemp(); // запрос температуры
   int t = sensor.readTemp() ;
   Serial.println(sensor.getTemp());      
   display.setCursor(0,20);
   display.print("Temp : ");         
   display.print(sensor.getTemp());  
   sensor.requestTemp();                     // it is printed on the serial monitor
  } 
  if (key == '*') {  
     display.display();
     display.clearDisplay();
     display.setTextColor(WHITE);
     display.setTextSize(1);
     display.setCursor(0,0);
     display.print("1 - Status ");
     display.setCursor(0,10);
     display.print("4 - Water ");
     display.setCursor(0,20);
     display.print("7 - Power ");                              // If getKey returned any key
    Serial.println(key);                        // it is printed on the serial monitor
  } 
}

int readSensor() 
{
  digitalWrite(sensorPower, HIGH);  // Включить датчик
  delay(10);                        // Дать время питанию установиться
  int val = analogRead(sensorPin);  // Прочитать аналоговое значение от датчика
  digitalWrite(sensorPower, LOW);   // Выключить датчик
  return val;                       // Вернуть аналоговое значение влажности
  
}

