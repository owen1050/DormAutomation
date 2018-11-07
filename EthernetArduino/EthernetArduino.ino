#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <Wire.h>
char auth[] = "9904859d0fe5408eba3eb69e7fbc955e";

#define W5100_CS  10

int tvInput = -1;//0 is tv, 1 is chromecase 2 is xbox/ computer 3 is xbox 360
//tv iunpouts 4 and 5 turn on adn off

BLYNK_WRITE(V0)
{
  tvInput = 0;//set to tv
}
BLYNK_WRITE(V1)
{
  tvInput = 1;//set to chromecase
}
BLYNK_WRITE(V2)
{
  tvInput = 2;//set to xbox one
}
BLYNK_WRITE(V3)
{
  tvInput = 3;//set to 360
}

BLYNK_WRITE(V4)
{
  tvInput = 4;//turn tv on
}
BLYNK_WRITE(V5)
{
  tvInput = 5;//turn tv off
}

int speakerInput = -1;//speaker inputs 0 is chrome 1 is tv 2 is record
BLYNK_WRITE(V6)
{
  speakerInput = 0;//set to chrome
}
BLYNK_WRITE(V7)
{
  speakerInput = 1;//set to tv
}
BLYNK_WRITE(V8)
{
  speakerInput = 2;//set to record
}
BLYNK_WRITE(V26)
{
  speakerInput = 3;//set to mute
}
BLYNK_WRITE(V27)
{
  speakerInput = 4;//set to vuone
}
BLYNK_WRITE(V28)
{
  speakerInput = 5;//set to vdownone
}

int blindState = -1; //0 is closed 1 is half 2 is open
BLYNK_WRITE(V9)
{
  blindState = 0;//close shades
}
BLYNK_WRITE(V10)
{
  blindState = 1;//half shades
}
BLYNK_WRITE(V11)
{
  blindState = 2;//full shades
}

int hallLightState = -1;//0 is off 1 is on
BLYNK_WRITE(V12)
{
  hallLightState = 1;//hall luights on
}
BLYNK_WRITE(V13)
{
  hallLightState = 0;//hall luights off
}

int mainLightState = -1;//0 is off 1 is on
BLYNK_WRITE(V14)
{
  mainLightState = 1;//hall luights on
}
BLYNK_WRITE(V15)
{
  mainLightState = 0;//hall luights off
}
int bothLightState = -1;
BLYNK_WRITE(V16)
{
  bothLightState = 1;//hall luights on

}
BLYNK_WRITE(V17)
{
  bothLightState = 0;//hall luights off

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

int steroVolume = -1;
BLYNK_WRITE(V24)
{
  steroVolume = param.asInt();
}

int blindPercent = -1;
BLYNK_WRITE(V25)
{
  blindPercent = param.asInt();
}

void setup()
{
  Wire.begin();
  Blynk.begin(auth);
}

void loop()
{
  Blynk.run();
  if (tvInput != -1)
  {
    sendWithAuth(16 + tvInput);
    tvInput = -1;
  }
  if (speakerInput != -1)
  {
    sendWithAuth(32 + speakerInput);
    speakerInput = -1;
  }
  if (blindState != -1)
  {
    sendWithAuth(48 + blindState);
    blindState = -1;
  }
  if (hallLightState != -1)
  {
    sendWithAuth(65 - hallLightState);
    hallLightState = -1;
  }
  if (mainLightState != -1)
  {
    sendWithAuth(67 - mainLightState);
    mainLightState = -1;
  }
  if (bothLightState != -1)
  {
    sendWithAuth(69 - bothLightState);
    bothLightState = -1;
  }
  if (FORCEhallLightState != -1)
  {
    sendWithAuth(71 - FORCEhallLightState);
    FORCEhallLightState = -1;
  }
  if (FORCEmainLightState != -1)
  {
    sendWithAuth(73 - FORCEmainLightState);
    FORCEmainLightState = -1;
  }
  if (FbothLightState != -1)
  {
    sendWithAuth(75 - FbothLightState);
    FbothLightState = -1;
  }
  if (steroVolume != -1)
  {
    sendWithAuth(128 + steroVolume);
    steroVolume = -1;
  }
  if (blindPercent != -1)
  {
    sendWithAuth(192 + (int)(blindPercent / 2));
    blindPercent = -1;
  }
}

boolean sendWithAuth(int value)
{
  Wire.beginTransmission(0x05);
  Wire.write(value);
  Wire.endTransmission();
 
  return true;
}

