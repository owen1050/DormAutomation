#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
char text[32];

boolean hallPos = false;
boolean mainPos = true;
int hallPosOff = 0;
int hallPosOn = 128;
int mainPosOff = 128;
int mainPosOn = 0;

int hallPin = 5;
int mainPin = 3;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, 0x00000001);
  radio.setChannel(124);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&text, radio.getPayloadSize());
    Serial.println(text);
    String dataIn = String(text);

    //all the ways in which to move jsut hall off
    if(dataIn == "F0_" || dataIn == "F00" || ((dataIn == "0_" || dataIn == "00") && hallPos = true))
    {
      Serial.println("close blinds");
    }

    
    
  }

}

