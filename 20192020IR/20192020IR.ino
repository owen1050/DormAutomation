#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8); // CE, CSN
char text[32];
long prevT = 0;
String prev ="";
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  
  radio.startListening();
}

void loop() {
  
  if (radio.available()) {
    radio.read(&text, radio.getPayloadSize());
    //Serial.println(text);
    String dataIn = String(text);
    
    
    if(prev == dataIn && abs(millis() - prevT) <5000)
    {
      dataIn = "";
      prevT = millis();
    }
    else
    {
      prev = dataIn;
      prevT = millis();
     //Serial.println(dataIn);
    }
    Serial.print(dataIn);
    //if(dataIn.toInt() == 
  }
}
