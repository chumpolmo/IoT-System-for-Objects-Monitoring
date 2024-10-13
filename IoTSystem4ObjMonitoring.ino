// Thank you :
//  https://www.analogread.com/article/155/how to use the arduino board with an active buzzer module with sample code.
//  https://www.analogread.com/article/151/how to use the arduino board with a mini mp3 player module with sample code.

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

byte volume = 30; //กำหนดระดับความดัง 0 - 30

SoftwareSerial mySoftwareSerial(3, 2); // กำหนดพินสำหรับติดต่อกับ Mini MP3 Player Module (TX, RX)
DFRobotDFPlayerMini myDFPlayer;

#define BUZZER_PIN 2

#define echoPin 7
#define trigPin 6
#define insDistance 90 

// คำนวณเวลาตรวจสอบการพบวัตถุ
unsigned long old_time;
unsigned long new_time;
unsigned long period = 1000;

void setup() {
  //Serial.begin(9600);
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(volume);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println("Hi, IoT monitoring...");
}

void loop() {
  //Serial.println(Ultrasonic());
  int cm = Ultrasonic();
  Serial.print("CM -> ");
  Serial.println(cm);
  Serial.println("Inspecting Distances -> " + insDistance);

  // คำนวณเวลาเพื่อตรวจสอบการพบวัตถุ
  new_time = millis();
  Serial.println("Time -> " + new_time - old_time);
  old_time = new_time + period;
  delay(period);

  if (cm < distanceB && cm != 0) {   //เมื่อวัตถุเข้าใกลเสียงเตือนดังถี่ขึ้น
    digitalWrite(BUZZER_PIN, HIGH);  //ปิดเสียงเตือน
    delay(cm * 16);
    digitalWrite(BUZZER_PIN, LOW);   //เปิดเสียงเตือน
    delay(cm * 16);
    Serial.println("Found object.");

    if(old_time >= 15000) {           // กรณีที่เวลาพบวัตุมากกว่า  15 วินาที
      // ในกรณีที่พบวัตถุมากกว่า 15 วินาที ให้แจ้งเตือนด้วยเสียงระดับที่สอง
      Serial.println("Play 002.mp3 --> Sound 2nd level.");
      myDFPlayer.play(2);               //Play 002.mp3
      delay(5000);
    }else{
      // ในกรณีที่พบวัตถุให้แจ้งเตือนด้วยเสียงเริ่มต้น (เสียงระดับที่ 1)
      Serial.println("Play 001.mp3 --> Sound 1st level.");
      myDFPlayer.play(1);               //Play 001.mp3
      delay(5000);
    }

  } else {                              //เมื่อห่างมากกว่า 60 cm จะไม่มีเสียง
    digitalWrite(BUZZER_PIN, HIGH);   //ปิดเสียงเตือน
    delay(500);
    Serial.println("No object.");
  }
}

int Ultrasonic() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  Serial.print("Ultrasonic Distance -> ");
  Serial.println(distance);
  return distance;
}