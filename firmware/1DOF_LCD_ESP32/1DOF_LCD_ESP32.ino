#include <LiquidCrystal.h>
#include <ESP32Servo.h>

int rs = 13;
int en = 12;
int d4 = 14;
int d5 = 27;
int d6 = 26;
int d7 = 25;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo motorServo;
int servoPin = 32;

void setup() {

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  
  motorServo.setPeriodHertz(50);
  motorServo.attach(servoPin, 500, 2400);
  
  lcd.begin(16, 2);
  Serial.begin(115200);
}

String temp = "";
char c;
int position=90;

void loop() {
  if (Serial.available()) {
    c = (char)Serial.read();
    if (c == ';') {
      position=map(temp.toInt(), 0, 1023, 20, 173);
      motorServo.write(position);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(temp);
      temp = "";
      sleep(0.01);
    } else {
      temp += c;
    }
  }
}