#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <IRLibSendBase.h>    //We need the base code
#include <IRLib_HashRaw.h>    //Only use raw sender
IRsendRaw mySender;

#define RAW_DATA_LEN 68
uint16_t projPower[RAW_DATA_LEN]={8998, 4502, 562, 566, 562, 566, 562, 566, 562, 562, 562, 566, 562, 566, 534, 594, 562, 566, 562, 1682, 562, 1686, 562, 1682, 562, 1682, 562, 1686, 562, 566, 562, 1682, 562, 1686, 562, 562, 562, 566, 562, 1686, 562, 1682, 562, 1686, 562, 562, 534, 594, 562, 566, 562, 1682, 562, 1686, 562, 566, 562, 562, 566, 562, 534, 1714, 534, 1694, 582, 1682, 562, 1000};
uint16_t tvPower[RAW_DATA_LEN]={9002, 4510, 474, 654, 534, 1698, 558, 570, 474, 654, 534, 570, 558, 570, 558, 570, 558, 574, 530, 1698, 498, 630, 534, 1698, 558, 1702, 534, 1698, 470, 1762, 558, 1698, 534, 594, 530, 574, 558, 1698, 534, 1698, 554, 574, 558, 570, 558, 546, 558, 1698, 534, 566, 590, 1670, 450, 678, 534, 570, 558, 1698, 558, 1674, 558, 1698, 534, 574, 554, 1702, 530, 1000};
uint16_t volumeUp[RAW_DATA_LEN]={8958, 4518, 530, 1702, 530, 582, 550, 1702, 530, 598, 530, 602, 530, 598, 530, 602, 530, 1702, 530, 1702, 530, 602, 530, 1706, 526, 1706, 530, 598, 530, 1706, 530, 1702, 530, 1702, 530, 1706, 530, 598, 530, 602, 530, 598, 530, 602, 530, 1702, 530, 1706, 526, 602, 530, 598, 530, 1706, 530, 1702, 530, 1702, 530, 1706, 530, 598, 530, 602, 530, 1702, 530, 1000};
uint16_t volumeDown[RAW_DATA_LEN]={8986, 4486, 562, 1674, 530, 602, 554, 1678, 558, 570, 558, 574, 558, 570, 558, 574, 558, 1674, 558, 1674, 562, 570, 558, 1674, 562, 1674, 558, 570, 558, 1674, 558, 1678, 558, 1674, 558, 574, 530, 598, 558, 574, 558, 570, 558, 574, 558, 1674, 558, 1674, 558, 574, 558, 1674, 562, 1670, 562, 1674, 558, 1674, 562, 1674, 558, 570, 530, 602, 558, 1658, 546, 1000};
uint16_t auxOne[RAW_DATA_LEN]={8990, 4462, 582, 1674, 558, 574, 558, 1654, 550, 598, 558, 574, 558, 570, 530, 602, 558, 1674, 558, 1674, 562, 570, 558, 1674, 558, 1678, 558, 570, 558, 1678, 558, 1674, 558, 1678, 554, 574, 558, 1674, 558, 1678, 558, 1674, 530, 602, 558, 1674, 558, 1674, 558, 574, 558, 1674, 558, 574, 558, 570, 558, 570, 534, 1702, 558, 570, 562, 570, 558, 1674, 558, 1000};
uint16_t auxTwo[RAW_DATA_LEN]={8986, 4486, 558, 1674, 530, 602, 558, 1674, 558, 570, 562, 570, 558, 574, 558, 570, 558, 1674, 562, 1674, 558, 570, 558, 1678, 558, 1674, 558, 574, 530, 1702, 530, 1702, 530, 1706, 530, 1702, 530, 1686, 550, 1702, 558, 1674, 558, 574, 530, 1682, 550, 1706, 526, 602, 530, 602, 558, 570, 530, 602, 530, 598, 558, 1678, 526, 602, 530, 598, 530, 1706, 530, 1000};
uint16_t mute[RAW_DATA_LEN]={8986, 4486, 582, 1650, 586, 546, 586, 1646, 586, 546, 558, 570, 558, 574, 582, 546, 586, 1650, 558, 1674, 558, 570, 586, 1650, 558, 1674, 558, 574, 558, 1674, 558, 1678, 582, 1650, 582, 546, 586, 1650, 582, 546, 558, 1678, 566, 562, 558, 1674, 558, 1678, 582, 546, 586, 1650, 582, 546, 558, 1674, 562, 570, 558, 1674, 582, 550, 558, 570, 586, 1650, 558, 1000};
uint16_t cd[RAW_DATA_LEN]={8986, 4486, 558, 1678, 554, 574, 558, 1674, 558, 574, 526, 602, 558, 574, 526, 602, 558, 1678, 558, 1674, 558, 570, 562, 1674, 554, 1678, 558, 574, 554, 1678, 558, 1674, 558, 1678, 558, 570, 558, 1678, 558, 1674, 558, 574, 554, 1678, 558, 1678, 554, 1658, 578, 570, 558, 1658, 578, 570, 558, 574, 554, 1678, 558, 574, 554, 574, 558, 570, 558, 1678, 554, 1000};
uint16_t phono[RAW_DATA_LEN]={8958, 4514, 530, 1706, 530, 598, 530, 1706, 526, 602, 530, 598, 530, 602, 530, 598, 530, 1706, 530, 1702, 530, 582, 550, 1702, 530, 1702, 530, 602, 530, 1702, 530, 1706, 526, 1706, 530, 602, 530, 1702, 530, 598, 534, 598, 530, 1702, 530, 1706, 530, 1702, 530, 602, 530, 1702, 530, 602, 526, 1706, 530, 1702, 530, 582, 550, 598, 530, 602, 530, 1702, 530, 1000};

RF24 radio(7,8); // CE, CSN
char text[32];
long prevT = 0;
String prev ="";
const byte address[6] = "00001";
int volCount = 0;
long irCount = 0;
long prevIR = -1;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);  
  radio.startListening();
  Serial.println("running");
}

void loop() {
  
  if (radio.available()) {
    radio.read(&text, radio.getPayloadSize());
    //Serial.println(text);
    String dataIn = String(text);
    irCount = dataIn.substring(0,dataIn.indexOf(":")).toInt();
    if(prevIR == irCount)
    {
      dataIn = "";
    }
    else
    {
      dataIn = dataIn.substring(dataIn.indexOf(":") +1);
    }
    prevIR = irCount;
    if(dataIn != "")
    {
      Serial.println(dataIn);
      
      
      //at this point inData only comes up once per data
      if(dataIn.equals("0"))
      {
        mySender.send(projPower,RAW_DATA_LEN,36);
      }
      if(dataIn.equals("1"))
      {
        mySender.send(projPower,RAW_DATA_LEN,36);
        delay(1000);
        mySender.send(projPower,RAW_DATA_LEN,36);
        delay(1000);
        mySender.send(projPower,RAW_DATA_LEN,36);
      }
      if(dataIn.equals("2"))
      {
        mySender.send(tvPower,RAW_DATA_LEN,36);
      }
      if(dataIn.equals("3"))
      {
        mySender.send(tvPower,RAW_DATA_LEN,36);
      }
      if(dataIn.equals("4"))
      {
        mySender.send(mute,RAW_DATA_LEN,36);
      }
      if(dataIn.equals("5"))
      {
        mySender.send(auxTwo,RAW_DATA_LEN,36);
      }
      if(dataIn.equals("6"))
      {
        mySender.send(auxOne,RAW_DATA_LEN,36);
      }
      if(dataIn.equals("7"))
      {
        mySender.send(phono,RAW_DATA_LEN,36);
      }
      if(dataIn.equals("8"))
      {
        mySender.send(cd,RAW_DATA_LEN,36);
      }
      if(dataIn.indexOf("VD") > -1)
      {
        volCount = dataIn.substring(dataIn.indexOf("VD")+2).toInt();
        while(volCount > 0)
        {
          mySender.send(volumeDown,RAW_DATA_LEN,36);
          volCount = volCount - 1;
          Serial.println("vd");
          delay(250);
        }
      }
      if(dataIn.indexOf("VU") > -1)
      {
        volCount = dataIn.substring(dataIn.indexOf("VU")+2).toInt();
        while(volCount > 0)
        {
          mySender.send(volumeUp,RAW_DATA_LEN,36);
          volCount = volCount - 1;
          Serial.println("vu");
          delay(250);
        }
      }
    }
    
  }
}
