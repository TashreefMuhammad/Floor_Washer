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
 * Arduino 29 -> Digital  -> Sonar Trigger Pin          -> int sTRI
 * Arduino 30 -> Digital  -> Sonar Echo Pin             -> int sECH
 * Arduino 12 -> Digital  -> Buzzer Pin                 -> int buzz
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
int sTRI = 29;
int sECH = 30;
int buzz = 12;
SoftwareSerial terminal(10, 11);
Servo servo;
int servoVal=0, motSpeed = 0;
char in;
int flag=0;
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
  pinMode(sTRI, OUTPUT);
  pinMode(sECH, INPUT);
  pinMode(buzz,OUTPUT);


  terminal.println("Setup complete");
}

void loop() {
  // put your main code here, to run repeatedly:

  if(Serial.available()){
    in = Serial.read();
    terminal.println(in);
    
    if(in == 'F')
      goForward();
    else if(in == 'B')
      goBackward();
    else if(in == 'L')
      goLeft();
    else if(in == 'R')
      goRight();
    else if(in == 'S')
      wheelStop();
    else if(in == 'W')
      brushStart();
    else if(in == 'w')
      analogWrite(enBrush, 0);
    else if(in == 'U')
      flag =1;
      //servo.write(servoVal++);
    else if(in == 'u')
      flag =2;
     // servo.write(servoVal--);
    else if(in == 'O')
      flag =0;
     // servo.write(servoVal--);
    else if(in == 'V')
      digitalWrite(relayPin, HIGH);
    else if(in == 'v')
      digitalWrite(relayPin, LOW);
    else if(in >= '1' && in <= '9')
      motSpeed = 28 * (in-'0');
    delay(40);
  }
  
  long duration, inches, cm;
  
  digitalWrite(sTRI, LOW);
  delayMicroseconds(2);
  
  digitalWrite(sTRI, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(sTRI, LOW);

  
  duration = pulseIn(sECH, HIGH); // using pulsin function to determine total time
  cm = microsecondsToCentimeters(duration); // calling method


  if(cm <=10){

      tone(buzz,100);
      terminal.println("!Buzzing!");
  }
  else{
     noTone(buzz);
    }
  terminal.println(servoVal);
  if(flag==1){
   servo.write(servoVal++);
   if(servoVal>180){
    flag =0;
   }
  }
  else if(flag==2){
     servo.write(servoVal--);
     if(servoVal<=0){
    flag =0;
   }
  }
  delay(100);
  
  /*
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
  */
}

void goForward(){
  terminal.println(" Go Forward");
  analogWrite(enRight, 250);
  analogWrite(enLeft, 250); 
  digitalWrite(rIn1, LOW);
  digitalWrite(rIn2, HIGH);
  digitalWrite(lIn1, HIGH);
  digitalWrite(lIn2, LOW);
}

void goBackward(){
  terminal.println(" Go Backward");
  analogWrite(enRight, 250);
  analogWrite(enLeft, 250); 
  digitalWrite(rIn1, HIGH);
  digitalWrite(rIn2, LOW);
  digitalWrite(lIn1, LOW);
  digitalWrite(lIn2, HIGH);
}

void goRight(){
  terminal.println(" Go Right");
  analogWrite(enRight, 250);
  analogWrite(enLeft, 250); 
  digitalWrite(lIn1, HIGH);
  digitalWrite(lIn2, LOW);
  digitalWrite(rIn1, HIGH);
  digitalWrite(rIn2, LOW);
}

void goLeft(){
  terminal.println(" Go Left");
  analogWrite(enRight, 250);
  analogWrite(enLeft, 250); 
  digitalWrite(rIn1, LOW);
  digitalWrite(rIn2, HIGH);
  digitalWrite(lIn1, LOW);
  digitalWrite(lIn2, HIGH);
}

void wheelStop(){
  // Stop All Wheel
  terminal.println(" Stop All Wheel"); 
  analogWrite(enRight, 0);
  analogWrite(enLeft, 0);
}

void brushStart(){
  // Spin Brush Motors
  digitalWrite(bIn1, HIGH);
  digitalWrite(bIn2, LOW);

  analogWrite(enBrush, 100);
}

long microsecondsToInches(long microseconds) // method to covert microsec to inches 
{
 return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) // method to covert microsec to centimeters
{
   return microseconds / 29 / 2;
}
