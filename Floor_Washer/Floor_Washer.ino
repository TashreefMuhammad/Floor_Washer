/**
   Setting up pin variables accordingly:
   Arduino  5 -> PWM      -> Relay for Water Pump       -> int relayPin
   Arduino  6 -> PWM      -> Value for Servo Motor      -> int servoPin
   Arduino  7 -> PWM      -> Speed of Left Side Wheel   -> int enLeft
   Arduino  8 -> PWM      -> Speed of Right Side Wheel  -> int enRight
   Arduino  9 -> PWM      -> Speed of Brush motors      -> int enBrush
   Arduino 22 -> Digital  -> Right Wheel In1            -> int rIn1
   Arduino 23 -> Digital  -> Right Wheel In2            -> int rIn2
   Arduino 24 -> Digital  -> Left Wheel In1             -> int lIn1
   Arduino 25 -> Digital  -> Left Wheel In2             -> int lIn2
   Arduino 26 -> Digital  -> Brush Motor In1            -> int bIn1
   Arduino 27 -> Digital  -> Brush Motor In2            -> int bIn2
   Arduino 29 -> Digital  -> Sonar Trigger Pin          -> int sTRI
   Arduino 30 -> Digital  -> Sonar Echo Pin             -> int sECH
   Arduino 12 -> Digital  -> Buzzer Pin                 -> int buzz
*/
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Servo.h>

int wheelstatus ;
int brushstatus ;
int buzzerstatus;
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


LiquidCrystal lcd(34, 35, 36, 37, 38, 39);
SoftwareSerial terminal(10, 11);
Servo servo;
int servoVal = 0, motSpeed = 0;
char in;
int flag = 0;
void setup() {
  // put your setup code here, to run once:
  lcd.begin(20, 4);
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
  pinMode(buzz, OUTPUT);


  terminal.println("Setup complete");

  lcd.setCursor(0, 0);
  lcd.print("Wheel:");
  lcd.setCursor(0, 1);
  lcd.print("Brush:");
  lcd.setCursor(0, 2);
  lcd.print("Pump:");
  lcd.setCursor(0, 3);
  lcd.print("Servo:");
  lcd.setCursor(10, 1);
  lcd.println("Speed:");

}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()) {
    in = Serial.read();
    terminal.println(in);

    if (in == 'F') {
      goForward();
      wheelstatus = 1;
    }
    else if (in == 'B') {
      goBackward();
      wheelstatus = 2;
    }
    else if (in == 'L') {
      goLeft();
      wheelstatus = 3;
    }
    else if (in == 'R') {
      goRight();
      wheelstatus = 4;
    }
    else if (in == 'S') {
      wheelStop();
      wheelstatus = 0;
    }
    else if (in == '2') {
      brushStart();
      brushstatus = 1;
    }

    else if (in == '3') {
      analogWrite(enBrush, 0);
      brushstatus = 0;
    }

    else if (in == '0')
      flag = 1; //brush up

    else if (in == '1')
      flag = 2; //brush down

    else if (in == 'O')
      flag = 0; //brush stop
    // servo.write(servoVal--);
    else if (in == '4')
      digitalWrite(relayPin, HIGH);
    else if (in == '5')
      digitalWrite(relayPin, LOW);


  }

  long duration, cm;

  digitalWrite(sTRI, LOW);
  delayMicroseconds(2);

  digitalWrite(sTRI, HIGH);
  delayMicroseconds(10);

  digitalWrite(sTRI, LOW);


  duration = pulseIn(sECH, HIGH); // using pulsin function to determine total time
  cm = microsecondsToCentimeters(duration); // calling method


  if (cm <= 10) {

    tone(buzz, 100);
    terminal.println("!Buzzing!");
    buzzerstatus = 1;
  }
  else {
    noTone(buzz);
    buzzerstatus = 0;
  }

  if (flag == 1) {
    servo.write(servoVal++);
    if (servoVal > 180) {
      flag = 0;
    }
  }
  else if (flag == 2) {
    servo.write(servoVal--);
    if (servoVal <= 0) {
      flag = 0;
    }
  }
  delay(100);


}

void goForward() {
  terminal.println(" Go Forward");
  analogWrite(enRight, 250);
  analogWrite(enLeft, 250);
  digitalWrite(rIn1, LOW);
  digitalWrite(rIn2, HIGH);
  digitalWrite(lIn1, HIGH);
  digitalWrite(lIn2, LOW);
}

void goBackward() {
  terminal.println(" Go Backward");
  analogWrite(enRight, 250);
  analogWrite(enLeft, 250);
  digitalWrite(rIn1, HIGH);
  digitalWrite(rIn2, LOW);
  digitalWrite(lIn1, LOW);
  digitalWrite(lIn2, HIGH);
}

void goRight() {
  terminal.println(" Go Right");
  analogWrite(enRight, 0);
  analogWrite(enLeft, 250);
  digitalWrite(lIn1, HIGH);
  digitalWrite(lIn2, LOW);

}

void goLeft() {
  terminal.println(" Go Left");
  analogWrite(enRight, 250);
  analogWrite(enLeft, 0);
  digitalWrite(rIn1, LOW);
  digitalWrite(rIn2, HIGH);

}

void wheelStop() {
  // Stop All Wheel
  terminal.println(" Brush motor stop");
  terminal.println(" Stop All Wheel");
  analogWrite(enRight, 0);
  analogWrite(enLeft, 0);
}

void brushStart() {
  // Spin Brush Motors
  terminal.println(" Brush motor start");
  digitalWrite(bIn1, HIGH);
  digitalWrite(bIn2, LOW);

  analogWrite(enBrush, 100);
}


long microsecondsToCentimeters(long microseconds) // method to covert microsec to centimeters
{
  return microseconds / 29 / 2;
}
