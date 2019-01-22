#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
RF24 radio(7,8); // CE, CSN
char text[32];

Servo hallServo;
Servo mainServo;

boolean hallPos = false;
boolean mainPos = false;
int hallPosOff = 50;
int hallPosOn = 60;
int mainPosOff = 128;
int mainPosOn = 0;
unsigned long prevT = 0;

boolean resetMain = false;
boolean resetHall = false;

int hallPin = 5;
int mainPin = 3;

int hallDelt = 0;
int mainDelt = 0;

int setHallTime = 0;
int moveHallTime = 0;

int setMainTime = 0;
int moveMainTime = 0;
String prev ="";
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  //radio.setPALevel(RF24_PA_MIN);
  
  radio.startListening();
}

void loop() {
  
  if (radio.available()) {
    radio.read(&text, radio.getPayloadSize());
    Serial.println(text);
    
    String dataIn = String(text);
    /**
    Serial.println("/////");
    Serial.println(prev);
    Serial.println(dataIn);
    Serial.println(prevT);
    Serial.println(abs(millis() - prevT));
    Serial.println(prev == dataIn);
    Serial.println(abs(millis() - prevT) <5000);
    */
    if(prev == dataIn && abs(millis() - prevT) <5000)
    {
      dataIn = "";
      prevT = millis();
    }
    else
    {
      prev = dataIn;
      prevT = millis();
      Serial.println(dataIn);
    }
    
   hallDelt = 0;
    mainDelt = 0;
    //all the ways in which to move jsut hall off
    if(dataIn == "F0_" || dataIn == "F00" || ((dataIn == "0_" || dataIn == "00") && hallPos == true))
    {
      hallPos = false;
      hallDelt = 1;
      setHallTime = millis();
    }
    if(dataIn == "F1_" || dataIn == "F11" || ((dataIn == "1_" || dataIn == "11") && hallPos == false))
    {
      hallPos = true;
      hallDelt = 1;
      setHallTime = millis();
    }
    if(dataIn == "F_0" || dataIn == "F00" || ((dataIn == "_0" || dataIn == "00") && mainPos == true))
    {
      mainPos = false;
      mainDelt = 1;
      setMainTime = millis();
    }
    if(dataIn == "F_1" || dataIn == "F11" || ((dataIn == "_1" || dataIn == "11") && mainPos == false))
    {
      mainPos = true;
      mainDelt = 1;
      setMainTime = millis();
    }
    if(mainPos && mainDelt == 1&&abs(moveMainTime - setMainTime)>2750)
    {
      mainServo.attach(5);
      Serial.println("turning on main");
      mainServo.write(92);
      resetMain = true;
      
    }
    if(!mainPos && mainDelt == 1&&abs(moveMainTime - setMainTime)>2750)
    {
      mainServo.attach(5);
      Serial.println("turning off main");
      mainServo.write(22);
      resetMain = true;
      
    }
    if(hallPos&& hallDelt == 1&&abs(moveHallTime - setHallTime)>2750)
    {
      hallServo.attach(4);
      Serial.println("turning on hall");
      hallServo.write(55);
      resetHall = true;
      
      //setHallServoHallOn
    }
    if(!hallPos&& hallDelt == 1&&abs(moveHallTime - setHallTime)>2750)
    {
      hallServo.attach(4);
      Serial.println("turning off hall");
      
      hallServo.write(115);
      resetHall = true;
      
      //setHallServoHallOff
    }
    if(resetHall || resetMain)
    {
      delay(1500);
    }
    if(resetHall)
    {
      hallServo.write(85);
      
    }
    if(resetMain)
    {
      mainServo.write(55);
      
    }
    if(resetHall || resetMain)
    {
      delay(1000);
    }
    if(resetHall)
    {
      hallServo.detach();
      moveHallTime = millis();
      resetHall = false;
    }
    if(resetMain)
    {
      mainServo.detach();
      moveMainTime = millis();
      resetMain = false;
    }
    
  }

}

