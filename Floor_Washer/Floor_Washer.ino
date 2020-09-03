/**
 * Setting up pin variables accordingly:
 * Arduino  5 -> PWM      -> Relay for Water Pump       -> int relayPin
 * Arduino  6 -> PWM      -> Value for Servo Motor      -> int servoPin
 * Arduino  7 -> PWM      -> Speed of Left Side Wheel   -> int enLeft
 * Arduino  8 -> PWM      -> Speed of Right Side Wheel  -> int enRight
 * Arduino  9 -> PWM      -> Speed of Brush motors      -> int enBrush
 * Arduino 22 -> Digital  -> Right Wheel In1            -> int rIn1
 * Arduino 23 -> Digital  -> Right Wheel In2            -> int rIn2
 * Arduino 24 -> Digital  -> Left Wheel In1             -> int lIn1
 * Arduino 25 -> Digital  -> Left Wheel In2             -> int lIn2
 * Arduino 26 -> Digital  -> Brush Motor In1            -> int bIn1
 * Arduino 27 -> Digital  -> Brush Motor In2            -> int bIn2
 */
#include <SoftwareSerial.h>
#include <Servo.h>

int relayPin = 5;
int servoPin = 6;
int enLeft = 7;
int enRight = 8;
int enBrush = 9;
int rIn1 = 22;
int rIn2 = 23;
int lIn1 = 24;
int lIn2 = 25;
int bIn1 = 26;
int bIn2 = 27;

SoftwareSerial terminal(10, 11);
Servo servo;
int servoVal;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  terminal.begin(9600);
  
  servo.attach(servoPin);
  delay(20);

  pinMode(relayPin, OUTPUT);
  pinMode(enLeft, OUTPUT);
  pinMode(enRight, OUTPUT);
  pinMode(enBrush, OUTPUT);
  pinMode(rIn1, OUTPUT);
  pinMode(rIn2, OUTPUT);
  pinMode(lIn1, OUTPUT);
  pinMode(lIn2, OUTPUT);
  pinMode(bIn1, OUTPUT);
  pinMode(bIn2, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  terminal.println("Loop Starts");

  terminal.println("Relay Part");
  digitalWrite(relayPin, HIGH);
  delay(1000);
  digitalWrite(relayPin, LOW);

  terminal.println("Servo Part");
  for(servoVal = 0; servoVal <= 200; ++servoVal){
    servo.write(servoVal);
    delay(10);
  }
  while(servoVal--){
    servo.write(servoVal);
    delay(5);
  }

  terminal.println("Wheel Part");
  analogWrite(enRight, 250);
  analogWrite(enLeft, 250);
  
  // Go Forward
  terminal.println(" Go Forward"); 
  digitalWrite(rIn1, HIGH);
  digitalWrite(rIn2, LOW);
  digitalWrite(lIn1, HIGH);
  digitalWrite(lIn2, LOW);

  delay(2000);

  // Go Backward
  terminal.println(" Go Backward"); 
  digitalWrite(rIn1, LOW);
  digitalWrite(rIn2, HIGH);
  digitalWrite(lIn1, LOW);
  digitalWrite(lIn2, HIGH);

  delay(2000);

  // Go Right
  terminal.println(" Go Right"); 
  digitalWrite(rIn1, HIGH);
  digitalWrite(rIn2, LOW);

  delay(2000);

  // Go Left
  terminal.println(" Go Left"); 
  digitalWrite(lIn1, LOW);
  digitalWrite(lIn2, HIGH);

  delay(2000);

  // Stop All Wheel
  terminal.println(" Stop All Wheel"); 
  analogWrite(enRight, 0);
  analogWrite(enLeft, 0);

  terminal.println("Brush Part");
  // Spin Brush Motors
  digitalWrite(bIn1, HIGH);
  digitalWrite(bIn2, LOW);

  analogWrite(enBrush, 100);
  delay(2000);

  // Stop Brush Motors
  analogWrite(enBrush, 0);
  delay(2000);
}
