/*       Project: Abrasion Tester Controller Code          __________________________________
 *       Written by:Chris Marella                         | MS1 | MS2 | MS2 |   Resolution   |
 *       Date: June 22, 2017                              |_____|_____|_____|________________|
 *       Version: 2.0                                     | Low | Low | Low |   Full Step    |
 *                                                        |_____|_____|_____|________________|
 *                                                        |High | Low | Low |   Half Step    |
 *       Motor SPECS                                      |_____|_____|_____|________________|
 *       Steps/rot: 200                                   | Low |High | Low |  Quarter Step  |
 *       Max Current: 2.8A                                |_____|_____|_____|________________|
 *       Type: BLDC                                       |High |High | Low |  Eighth Step   |
 *       Frame: NEMA 23                                   |_____|_____|_____|________________|
 *       Class: B                                         |High |High |High | Sixteenth Step |
 *                                                        -----------------------------------
 */                                                     
// include the library code:
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

// Connect via i2c, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);

//Driver Pin Definitions
const int stepPin = 3;
const int dirPin = 4;
const int MS1 = 5;
const int MS2 = 6;
const int MS3 = 7;

//Global Variables
const int stepTime =181;
int steps = 1120;
int offset = 500;
int count = 0;
long startTime;
long elapsedTime;
long elapsedTime1;
long lap;


//LCD i2c Pin Definitions

//Limit Switch Pin Definitions
//const int intPin = 2;    //Define back limit switch as an interrupt

void setup() {
  
  Serial.begin(9600);      //Set baud rate=9600b/s

  //LCD Configuration
  lcd.begin(20,3);
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("GA-ASI EE-Int");
  lcd.setCursor(0, 1);
  lcd.print("Abrasion Tester");
  delay(2000);
  
  
  //Driver Pin Configuration            
  pinMode(stepPin, OUTPUT);             
  pinMode(dirPin, OUTPUT);              
  pinMode(MS1, OUTPUT);                 
  pinMode(MS2, OUTPUT);                 
  pinMode(MS3, OUTPUT);

  //Limit Switch Configuration
  pinMode(8,INPUT_PULLUP);
  //Interrupt  Pin Configuration
  //attachInterrupt(digitalPinToInterrupt(intPin), calibrate, LOW);

  //Step Resolution
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);

  //Calibrate
  Calibrate();
  startTime=millis();  //Time for start of test 
}                                       

//ISR

void loop() {
 
  digitalWrite(dirPin, LOW);   // Set Direction

  //Perform one back-and-forth cycle = 2 strokes
  Cycle();
  upLCD();                    

}

//Function to cycle one time
void Cycle() {
  for(int i=0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepTime);   
  }
  strokeCount();
  delay(50);
  digitalWrite(dirPin, HIGH);
  for(int i=0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepTime);   
  }
  delay(50);
  strokeCount();
}

//Fuction to Calibrate to Home Position
void Calibrate(){

  lcd.setCursor(0, 0);
  lcd.print("Calibrating");
  
  //This finds the Limit Switch or Home Postion
  digitalWrite(dirPin, HIGH);
  while(digitalRead(8)==HIGH){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(200);
  }

  //This zeros the motor at an offset from the Home Position
  digitalWrite(dirPin, LOW);
  delay(1000);
  for(int i=0; i < offset; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepTime);
  }
  delay(2000);
}

//Count and Print Counts and Elapsed Time to Serial Monitor and LCD
void strokeCount(){
  count++;
  elapsedTime = (millis()-startTime)/1000;
  elapsedTime1 = millis();
//  Serial.print("Count:");
//  Serial.println(count);
//  Serial.print("Time:");
//  Serial.println(elapsedTime);
}

void upLCD(){
  lcd.setCursor(0, 0);
  lcd.print("Strokes:");
  lcd.setCursor(6, 0);
  lcd.print(count);
  lcd.setCursor(0, 1);
  lcd.print("Time:");
  lcd.setCursor(5, 1);
  lcd.print(elapsedTime);
  lap = millis()-elapsedTime1;
  lcd.setCursor(0,2);
  lcd.print(lap);  
}

