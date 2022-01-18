/*
 *        *****  *   *  *      *****  *   *
 *        *      *   *  *      *   *   * *
 *        *****  *   *  *      *****    *
 *            *  *   *  *      *   *    *
 *        *****  *****  *****  *   *    *
 *                     PEDALS
 *                     
 *                   PHASERCODE
 *                   
 *        
 * Autor: Eric & Heinz         
 * Version: 0.1 für Arduino
 * Datum: 27.12.2021
 * 
 * 
 * 
 *        
 */
 
#include <Bounce2.h>

const int buttonPin = 2;    //momentary switch
const int ledPin1 = 12;
const int ledPin2 = 10;

int ledState1 = LOW;
int ledState2;

//Speedpot
int potPin = A0;  //wiper of analogPot
int potValue;
int lastPotValue;

//Footswitch Stuff
const int longPressTime = 2000;
unsigned long currentDuration = millis();
byte mode = 0;
bool lastState = HIGH;

//Fading Stuff
const int fadingInterval = 1;
unsigned long fadingStart = millis();

Bounce b = Bounce();        //Instantiate a Bounce object

/*unsigned long currentDuration() {
  if (b.read() == LOW && lastState == HIGH) {
    unsigned long startCount = millis();
    lastState = LOW;
  }
}*/

void setup() {
  pinMode(ledPin1, OUTPUT);
  digitalWrite(ledPin1, ledState1);
  pinMode(ledPin2, OUTPUT);
  potValue = analogRead(potPin);
  lastPotValue = potValue;
  ledState2 = (map(potValue, 0, 1023, 0, 255));
  analogWrite(ledPin2, ledState2); 

  b.attach(buttonPin,INPUT_PULLUP);
  b.interval(30);

  Serial.begin(9600);
}

void loop() {
  b.update();

  potValue = analogRead(potPin);
   
  if (potValue <= lastPotValue - 10 || potValue >= lastPotValue + 10) {
    lastPotValue = potValue;
    ledState2 = (map(potValue, 0, 1023, 0, 255));
    analogWrite(ledPin2, ledState2);
  }
  
  switch(mode) {
    case 0:
      if (b.read() == LOW) {
        ledState1 = HIGH;
        digitalWrite(ledPin1, ledState1);
        mode = 1;
      }
      break;
    case 1:
      if (b.read() == LOW && lastState == HIGH) {
        currentDuration = millis();
        lastState = LOW;
      }
      if (b.read() == LOW && millis() - currentDuration >= longPressTime) {
        //ledState2 = HIGH;
        //digitalWrite(ledPin2, ledState2);
        fadingStart = millis();
        mode = 4;
      }
      if (b.read() == HIGH) {
        lastState = HIGH;
        mode = 2;
      }
      break;
    case 2:
      if (b.read() == LOW) {
        mode = 3;
      }
      break;
    case 3:
      if (b.read() == LOW && lastState == HIGH) {
        currentDuration = millis();
        lastState = LOW;
      } 
      if (b.read() == LOW && millis() - currentDuration >= longPressTime) {
        //ledState2 = HIGH;
        //digitalWrite(ledPin2, ledState2);
        fadingStart = millis();
        mode = 4;
      }
      if (b.read() == HIGH) {
        lastState = HIGH;
        ledState1 = LOW;
        digitalWrite(ledPin1, ledState1);
        mode = 0;
      }
      break;
    case 4:
      if (b.read() == LOW && millis() - fadingStart >= fadingInterval && ledState2 != 255) {
        ledState2++;
        analogWrite(ledPin2, ledState2);
        fadingStart = millis();
      }
      if (b.read() == HIGH && millis() - fadingStart >= fadingInterval && ledState2 != (map(potValue, 0, 1023, 0, 255))) {
        lastState = HIGH;
        ledState2--;
        analogWrite(ledPin2, ledState2);
        fadingStart = millis();
      }
      if (b.read() == HIGH && ledState2 == (map(potValue, 0, 1023, 0, 255))) {
        mode = 2; 
      }
      break;
  }
  Serial.print(mode );
  Serial.print('|');
  Serial.println( ledState2);
}
