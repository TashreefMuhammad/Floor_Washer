/*
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
   Arduino 34 -> Digital  -> RS of LCD                  -> int RS
   Arduino 35 -> Digital  -> E for LCD                  -> int E
   Arduino 36 -> Digital  -> d4 for LCD                 -> int d4
   Arduino 37 -> Digital  -> d5 for LCD                 -> int d5
   Arduino 38 -> Digital  -> d6 for LCD                 -> int d6
   Arduino 39 -> Digital  -> d7 for LCD                 -> int d7
*/

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <L298N.h>

//assigning arduino pins to variables
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
int RS   = 34;
int E    = 35;
int d4   = 36;
int d5   = 37;
int d6   = 38;
int d7   = 39;


SoftwareSerial terminal(10, 11);                       //associative variable for virtual terminal

L298N motWheel1(enLeft, lIn1, lIn2);                   //variable declaration for left wheel
L298N motWheel2(enRight, rIn1, rIn2);                  //variable declaration for right wheel
L298N brushWheel(enBrush, bIn1, bIn2);                 //variable declaration for brush

LiquidCrystal lcd(RS, E, d4, d5, d6, d7);             //variable declaration for lcd display

Servo servo;                                          //variable declaration for servo

//additional variables declaration code logic
int servoVal = 90, motSpeed = 0;                     
char in, wheelChar[] = {'|', '-', '/', '-', '|', '/', '-'};
int flag = 0;
int wheelStatus, brushStatus;
int wheelLeftAr = 0, wheelRightAr = 0, brushAr = 0;

void setup() {
 
  Serial.begin(9600);                               
  terminal.begin(9600);                             

  lcd.begin(20,4);
  servo.attach(servoPin);
  delay(20);


  //declaring pinmode
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

  //setting maximum speed for  motors
  motWheel1.setSpeed(250);                                      
  motWheel2.setSpeed(250);                                      
  brushWheel.setSpeed(250);                                     

  //initating all motors switch offed           
  motWheel1.stop();                                             
  motWheel2.stop();                                             
  brushWheel.stop();

  terminal.println("Setup complete");                          

  lcd.setCursor(0, 0);                                        
  lcd.print("Wheel:");                                         
  lcd.setCursor(0, 1);                                         
  lcd.print("Brush:");                                      
  lcd.setCursor(0, 2);                                         
  lcd.print("Pump:");                                         
  lcd.setCursor(10, 2);                                       
  lcd.print("Buzz:");                                          
  lcd.setCursor(0, 3);                                      
  lcd.print("Servo:");                                        


  //function declaration for initial lcd display print
  lcdprintPump("OFF");
  lcdprintServo("OFF");
  wheelStop();
}

void loop() {
  
  //checking for serial input
  if (Serial.available()) {
    in = Serial.read();
    terminal.println(in);

    if (in == 'F') {
      goForward();
      wheelStatus = 1;
    }
    else if (in == 'B') {
      goBackward();
      wheelStatus = 2;
    }
    else if (in == 'L') {
      goLeft();
      wheelStatus = 3;
    }
    else if (in == 'R') {
      goRight();
      wheelStatus = 4;
    }
    else if (in == 'S') {
      wheelStop();
      wheelStatus = 0;
    }
    else if (in == '2') {
      terminal.println(" Brush motor start");
      brushWheel.forward();
      brushStatus = 1;
    }

    else if (in == '3') {
      terminal.println(" Brush motor Stop");
      brushWheel.stop();
      brushStatus = 0;
    }

    else if (in == '0') {
      flag = 1; //brush up
      lcdprintServo("Anti-ClockWise");
    }
    else if (in == '1') {
      flag = 2; //brush down
      lcdprintServo("ClockWise");
    }
    else if (in == 'O') {
      flag = 0; //brush stop
      lcdprintServo("OFF");
    }
    else if (in == '4') {
      digitalWrite(relayPin, HIGH);
      terminal.println(" Pump On");
      lcdprintPump("ON");
    }
    else if (in == '5') {
      digitalWrite(relayPin, LOW);
      terminal.println(" Pump Off");
      lcdprintPump("OFF");
    }
  }

  wheelAnimation();
  brushAnimation();

  long duration, cm;

  digitalWrite(sTRI, LOW);
  delayMicroseconds(2);

  digitalWrite(sTRI, HIGH);
  delayMicroseconds(10);

  digitalWrite(sTRI, LOW);


  duration = pulseIn(sECH, HIGH);                 // using pulsin function to determine total time
  cm = duration / 29 / 2;                         // calling method


  if (cm <= 10) {
    tone(buzz, 100);
    terminal.println("!Buzzing!");
    lcd.setCursor(15, 2);
    lcd.print(")))");
  }
  else {
    noTone(buzz);
    lcd.setCursor(15, 2);
    lcd.print("   ");
  }

  if (flag == 1) {
    servo.write(servoVal++);
    if (servoVal > 180)
      flag = 0;
  }
  else if (flag == 2) {
    servo.write(servoVal--);
    if (servoVal <= 0)
      flag = 0;
  }
  delay(100);
}

void goForward() {
  //wheels go forward
  terminal.println(" Go Forward");
  lcd.setCursor(11, 0);
  lcd.print("F");
  motWheel1.forward();
  motWheel2.backward();
}

void goBackward() {
  //wheels go backward
  terminal.println(" Go Backward");
  lcd.setCursor(11, 0);
  lcd.print("B");
  motWheel1.backward();
  motWheel2.forward();
}

void goRight() {
  //wheel goes right
  terminal.println(" Go Right");
  lcd.setCursor(11, 0);
  lcd.print("R");
  motWheel1.forward();
  motWheel2.stop();
}

void goLeft() {
  //wheel goes left
  terminal.println(" Go Left");
  lcd.setCursor(11, 0);
  lcd.print("L");
  motWheel2.backward();
  motWheel1.stop();
}

void wheelStop() {
  // Stop All Wheels
  terminal.println(" Stop All Wheel");
  lcd.setCursor(11, 0);
  lcd.print("S");
  motWheel1.stop();
  motWheel2.stop();
}

//wheel animation shown in lcd display
void wheelAnimation(){
  if(wheelStatus == 1){
    lcd.setCursor(7, 0);
    lcd.print(wheelChar[wheelLeftAr]);
    lcd.setCursor(9, 0);
    lcd.print(wheelChar[wheelRightAr]);

    wheelLeftAr++;
    wheelRightAr++;

    wheelLeftAr %= 7;
    wheelRightAr %= 7;
  }
  else if(wheelStatus == 2){
    lcd.setCursor(7, 0);
    lcd.print(wheelChar[wheelLeftAr]);
    lcd.setCursor(9, 0);
    lcd.print(wheelChar[wheelRightAr]);

    wheelLeftAr--;
    wheelRightAr--;

    if(wheelLeftAr == -1)
      wheelLeftAr = 6;
    if(wheelRightAr == -1)
      wheelRightAr = 6;
  }
  else if(wheelStatus == 3){
    lcd.setCursor(9, 0);
    lcd.print(wheelChar[wheelRightAr]);

    wheelRightAr++;

    wheelRightAr %= 7;
  }
  else if(wheelStatus == 4){
    lcd.setCursor(7, 0);
    lcd.print(wheelChar[wheelLeftAr]);

    wheelLeftAr++;

    wheelLeftAr %= 7;
  }
}

//brush animation shown in lcd display
void brushAnimation(){
  if(brushStatus == 1){
    lcd.setCursor(7, 1);
    lcd.print(wheelChar[brushAr]);
    lcd.setCursor(9, 1);
    lcd.print(wheelChar[brushAr]);

    brushAr++;
    brushAr %= 7;
  }
}

//Servo status shown in lcd display
void lcdprintServo(String str){
  lcd.setCursor(6, 3);
  lcd.print("              ");
  lcd.setCursor(6, 3);
  lcd.print(str);
}

//Pump status shown in lcd display
void lcdprintPump(String str){
  lcd.setCursor(5, 2);
  lcd.print("   ");
  lcd.setCursor(5, 2);
  lcd.print(str);
}
