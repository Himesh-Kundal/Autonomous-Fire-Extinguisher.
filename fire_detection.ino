
#include "LiquidCrystal_I2C/LiquidCrystal_I2C.h"
#include "LiquidCrystal_I2C/LiquidCrystal_I2C.cpp"
#include "MQ135/MQ135.h"
#include "MQ135/MQ135.cpp"
#include "DHT11/src/DHT11.h"
#include "DHT11/src/DHT11.cpp"
#include <Servo.h>
#include "Pitches/pitches.h"

Servo myservo;  

int Buzz = 13;
int IR_Left = 12;
DHT11 dht11(11);
int FlameLeftGreen = 10;
int Servo = 9;
int FlameLeftRed = 3;
int IR_Right = 8;
int FlameRightRed = 7;
int FlameRightGreen = 6;
int Pump = 5;
int IR_Mid = 4;

#define FlameMidGreen A0
#define FlameMidRed A1

#define PIN_MQ135 A0
MQ135 mq135_sensor(PIN_MQ135);

int FlameLeft=0;
int FlameRight=0;
int FlameMid=0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(Buzz,OUTPUT);
  pinMode(Pump,OUTPUT);
  pinMode(IR_Left,INPUT);
  pinMode(IR_Right,INPUT);
  pinMode(IR_Mid,INPUT);
  pinMode(FlameRightRed,OUTPUT);
  pinMode(FlameRightGreen,OUTPUT);
  pinMode(FlameMidGreen,OUTPUT);
  pinMode(FlameMidRed,OUTPUT);
  pinMode(FlameLeftRed,OUTPUT);
  pinMode(FlameLeftGreen,OUTPUT);
  myservo.attach(9);
  dht11.setDelay(500);
  PEACE();

}

void loop() {

  int temperature = 0;
  int humidity = 0;
  int result = dht11.readTemperatureHumidity(temperature, humidity);

  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

  int FlameLeft=(digitalRead(IR_Left));
  FlameLeft=!FlameLeft; //Flame is inversed.

  int FlameRight=(digitalRead(IR_Right));
  FlameRight=!FlameRight; //Flame is inversed.

  int FlameMid=(digitalRead(IR_Mid));
  FlameMid=!FlameMid; //Flame is inversed.

  if (result == 0) {

    Serial.print("\tTemperature: ");
    Serial.print(temperature);
    Serial.print(" °C\tHumidity: ");
    Serial.print(humidity);
    Serial.print(" %");

    Serial.print("\tFlameLeft = ");
    Serial.print(FlameLeft);
    Serial.print("\tFlameRight = ");
    Serial.print(FlameRight);
    Serial.print("\tMQ135 RZero: ");
    Serial.print(rzero);
    Serial.print("\tCorrected RZero: ");
    Serial.print(correctedRZero);
    Serial.print("\tResistance: ");
    Serial.print(resistance);
    Serial.print(" PPM: ");
    Serial.print(ppm);
    Serial.print("\tCorrected PPM: ");
    Serial.print(correctedPPM);
    Serial.println("ppm");
    Serial.println(".");

    lcd.setCursor(0,1);
    lcd.print("Temp ");
    lcd.print(temperature);
    lcd.print("C Hum ");
    lcd.print(humidity);
    lcd.print("%");
  }


if(FlameLeft || FlameRight|| FlameMid){
  if(FlameLeft) AAGLeft();
  if(FlameMid) AAGMid();
  if(FlameRight) AAGRight();

  // direction(FlameLeft, FlameMid, FlameRight);
  }
else PEACE();
}


void direction(int l, int m, int r){
  if (l || m || r) myservo.write(90);
  else if (l || m) myservo.write(68);
  else if (r || m) myservo.write(113);
  else if (l) myservo.write(45);
  else if (m) myservo.write(90);
  else if (r) myservo.write(135);

}

void AAGLeft(){
  myservo.write(45);
  digitalWrite(FlameLeftGreen,0);
  digitalWrite(FlameLeftRed,1);
  digitalWrite(Pump,1);
  digitalWrite(Buzz,1);
  lcd.setCursor(3,0);
  lcd.print("  Fire !     ");
}

void AAGMid(){
  myservo.write(90);
  analogWrite(FlameMidGreen,0);
  analogWrite(FlameMidRed,1020);
  digitalWrite(Pump,1);
  digitalWrite(Buzz,1);
  lcd.setCursor(3,0);
  lcd.print("  Fire !     ");
}

void AAGRight(){
  myservo.write(135);
  digitalWrite(FlameRightGreen,0);
  digitalWrite(FlameRightRed,1);
  digitalWrite(Pump,1);
  digitalWrite(Buzz,1);
  lcd.setCursor(3,0);
  lcd.print("  Fire !     ");
}

void PEACE(){
  myservo.write(90);
  digitalWrite(FlameLeftGreen,1);
  digitalWrite(FlameLeftRed,0);
  digitalWrite(FlameRightGreen,1);
  digitalWrite(FlameRightRed,0);
  analogWrite(FlameMidGreen,1020);
  analogWrite(FlameMidRed,0);
  digitalWrite(Pump,0);
  digitalWrite(Buzz,0);
  lcd.setCursor(3,0);
  lcd.print("Peace Hai !");
}



