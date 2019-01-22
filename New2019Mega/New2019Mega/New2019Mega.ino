#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#define W5100_CS  10
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8);
char text[32];
const byte address[6] = "00001";
char auth[] = "9904859d0fe5408eba3eb69e7fbc955e";

int motorPin = 5;
int switchPin = 22;
unsigned long blindT = 0;

int blindState = -1; //0 is closed 1 is half 2 is open
BLYNK_WRITE(V9)
{
  blindState = 0;//close shades
}

BLYNK_WRITE(V11)
{
  blindState = 2;//full shades
}

//forcing lights
int FORCEhallLightState = -1;//0 is off 1 is on
BLYNK_WRITE(V18)
{
  FORCEhallLightState = 1;//hall luights on
}
BLYNK_WRITE(V19)
{
  FORCEhallLightState = 0;//hall luights off
}

int FORCEmainLightState = -1;//0 is off 1 is on
BLYNK_WRITE(V20)
{
  FORCEmainLightState = 1;//hall luights on
}
BLYNK_WRITE(V21)
{
  FORCEmainLightState = 0;//hall luights off
}
int FbothLightState = -1;
BLYNK_WRITE(V22)
{
  FbothLightState = 1;//hall luights on
}
BLYNK_WRITE(V23)
{
  FbothLightState = 0;//hall luights off

}


void setup()
{
 // Serial.begin(9600);
  Blynk.begin(auth);
  radio.begin();
  radio.openWritingPipe(address);
 // radio.setPALevel(RF24_PA_HIGH);
 // radio.setPayloadSize(16);
  radio.stopListening();
  pinMode(5, OUTPUT);
  pinMode(22, INPUT);
}

void loop()
{
  Blynk.run();
  //Serial.println(blindState);
  if (blindState == 0)
  {
    //close blinds
    if (!digitalRead(switchPin))
    {
      analogWrite(motorPin, 75);
      blindT = millis();
      blindState = 4;
    }
    else
    {
      blindState = -1;
      analogWrite(motorPin, 0);
    }
  }
  if (blindState == 4)
  {
    //close blinds
    if (!digitalRead(switchPin) && millis() - blindT < 10000)
    {
      analogWrite(motorPin, 75);
      //Serial.println("closing");
    }
    else
    {
      blindState = -1;
      analogWrite(motorPin, 0);
    }
  }
  if (blindState == 2)
  {

    if (digitalRead(switchPin))
    {
      analogWrite(motorPin, 170);
      blindT = millis();
      blindState = 3;
    }
    else
    {
      blindState = -1;
      analogWrite(motorPin, 0);
    }
  }
  if (blindState == 3)
  {

    if (abs(millis() - blindT) < 10000)
    {
      analogWrite(motorPin, 170);
      //Serial.println("opening");
    }
    else
    {
      blindState = -1;
      analogWrite(motorPin, 0);
    }
  }
  if (blindState == -1)
  {
    blindState = -1;
    analogWrite(motorPin, 0);
  }

if (FORCEhallLightState == 0)
{
  setMsg("F0_");
  sendRFMsgConfirm();
  FORCEhallLightState = -1;
}
if (FORCEhallLightState == 1)
{
  setMsg("F1_");
  sendRFMsgConfirm();
  FORCEhallLightState = -1;
}
if (FORCEmainLightState == 1)
{
  setMsg("F_1");
  sendRFMsgConfirm();
  FORCEmainLightState = -1;
}
if (FORCEmainLightState == 0)
{
  setMsg("F_0");
  sendRFMsgConfirm();
  FORCEmainLightState = -1;
}
if (FbothLightState == 0)
{
  setMsg("F00");
  sendRFMsgConfirm();

  FbothLightState = -1;
}
if (FbothLightState == 1)
{
 // Serial.println("on");
  setMsg("F11");
  sendRFMsgConfirm();
  FbothLightState = -1;
}
}

void setMsg(String msg)
{
  msg.toCharArray(text, 32);
}

void sendRFMsgConfirm()
{
  int time0 = millis();
  for (int i = 0; i < 30; i++)
  {
    radio.write(&text, sizeof(text));
  }
 // Serial.println("done");
}



