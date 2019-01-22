#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <IRLibSendBase.h>
#include <IRLibDecodeBase.h>
#include <IRLib_HashRaw.h>  //Must be last protocol
#include <IRLibCombo.h>     // After all protocols, include this
// All of the above automatically creates a universal decoder
// class called "IRdecode" containing only the protocols you want.
// Now declare an instance of that decoder.
IRdecode myDecoder;

// Include a receiver either this or IRLibRecvPCI or IRLibRecvLoop
#include <IRLibRecv.h>
IRrecv myReceiver(11);

const int IR_DATA_LEN = 68;
IRsendRaw irLED;

unsigned long irDatIn = -1;


uint16_t TV_POWER[IR_DATA_LEN] = {4410, 4606, 474, 1762, 482, 1762, 478, 1766, 454, 662, 478, 650, 458, 658, 454, 670, 450, 670, 450, 1794, 478, 1762, 454, 1794, 474, 642, 458, 666, 450, 670, 458, 658, 454, 670, 458, 666, 450, 1794, 450, 662, 458, 674, 450, 662, 458, 666, 450, 670, 478, 650, 450, 1790, 474, 650, 470, 1766, 462, 1778, 482, 1762, 478, 1762, 466, 1782, 478, 1762, 478, 1000};
uint16_t TV_SOURCE[IR_DATA_LEN] = {4434, 4582, 478, 1766, 470, 1766, 458, 1790, 478, 638, 458, 666, 450, 670, 458, 666, 454, 662, 458, 1782, 458, 1790, 478, 1762, 454, 670, 466, 662, 470, 642, 458, 670, 474, 642, 478, 1766, 458, 662, 454, 670, 450, 670, 454, 670, 450, 670, 450, 670, 454, 670, 450, 674, 446, 1794, 450, 1794, 478, 1762, 482, 1762, 478, 1758, 486, 1766, 470, 1766, 478, 1000};
uint16_t TV_UP[IR_DATA_LEN] = {4442, 4574, 478, 1762, 478, 1766, 450, 1786, 482, 642, 454, 670, 478, 638, 454, 670, 450, 670, 458, 1778, 458, 1794, 450, 1786, 486, 638, 454, 670, 474, 650, 450, 666, 450, 670, 450, 670, 478, 642, 482, 642, 450, 674, 450, 674, 446, 1790, 454, 1794, 470, 650, 470, 1762, 458, 1786, 482, 1766, 450, 1794, 478, 1762, 454, 670, 478, 638, 458, 1782, 486, 1000};
uint16_t TV_DOWN[IR_DATA_LEN] = {4470, 4538, 514, 1734, 510, 1734, 506, 1734, 514, 602, 518, 610, 510, 610, 510, 610, 510, 610, 510, 1730, 510, 1738, 510, 1726, 514, 610, 510, 610, 518, 610, 510, 610, 510, 610, 510, 1730, 506, 614, 510, 614, 450, 674, 450, 666, 454, 1794, 454, 1782, 462, 658, 454, 670, 458, 1786, 478, 1762, 458, 1790, 474, 1762, 482, 638, 458, 670, 450, 1790, 478, 1000};
uint16_t TV_ENTER[IR_DATA_LEN] = {4438, 4578, 474, 1762, 486, 1762, 474, 1762, 486, 642, 450, 674, 450, 662, 458, 670, 450, 670, 454, 1790, 478, 1762, 478, 1766, 478, 642, 454, 670, 454, 662, 454, 670, 450, 670, 458, 666, 450, 670, 450, 670, 454, 1794, 450, 662, 458, 1786, 482, 1762, 478, 638, 458, 1790, 478, 1762, 478, 1766, 478, 642, 458, 1790, 470, 650, 450, 670, 450, 1794, 474, 1000};
uint16_t TV_EXIT[IR_DATA_LEN] = {4434, 4578, 478, 1766, 474, 1770, 474, 1762, 482, 638, 458, 666, 458, 658, 486, 642, 462, 654, 454, 1794, 474, 1762, 482, 1762, 478, 650, 470, 650, 450, 670, 450, 666, 454, 674, 450, 1786, 478, 650, 450, 1790, 474, 1770, 474, 638, 462, 1782, 482, 642, 478, 646, 450, 674, 450, 1790, 474, 650, 450, 670, 454, 1782, 482, 642, 454, 1782, 486, 1762, 478, 1000};
uint16_t TV_0[IR_DATA_LEN] = {4418, 4598, 478, 1762, 454, 1794, 450, 1790, 478, 642, 450, 674, 450, 666, 482, 642, 478, 650, 474, 1762, 454, 1742, 526, 1762, 458, 662, 458, 662, 454, 674, 478, 642, 474, 650, 450, 1790, 450, 670, 450, 670, 478, 650, 450, 1786, 454, 666, 482, 642, 458, 662, 450, 670, 458, 1790, 450, 1790, 478, 1762, 458, 662, 458, 1782, 458, 1790, 458, 1790, 478, 1000};
uint16_t TV_1[IR_DATA_LEN] = {4438, 4578, 474, 1762, 478, 1762, 478, 1766, 458, 662, 454, 674, 474, 650, 450, 670, 474, 642, 454, 1782, 482, 1762, 458, 1786, 454, 674, 450, 670, 478, 638, 458, 670, 450, 666, 458, 670, 450, 666, 450, 1794, 478, 642, 450, 674, 474, 642, 458, 670, 450, 666, 478, 1762, 458, 1790, 474, 642, 458, 1782, 478, 1774, 470, 1762, 458, 1790, 462, 1782, 478, 1000};
uint16_t TV_2[IR_DATA_LEN] = {4438, 4574, 478, 1762, 478, 1762, 482, 1766, 478, 646, 450, 670, 454, 670, 474, 642, 482, 638, 458, 1790, 454, 1794, 478, 1766, 470, 646, 454, 662, 458, 670, 466, 650, 482, 642, 482, 1762, 454, 670, 450, 1790, 478, 646, 450, 674, 450, 674, 450, 670, 450, 670, 474, 642, 482, 1762, 458, 666, 454, 1794, 470, 1766, 470, 1770, 454, 1794, 478, 1762, 454, 1000};
uint16_t TV_3[IR_DATA_LEN] = {4414, 4598, 482, 1766, 450, 1786, 458, 1790, 478, 642, 454, 670, 450, 670, 454, 670, 478, 642, 474, 1766, 454, 1786, 482, 1762, 478, 650, 450, 670, 450, 670, 478, 646, 446, 674, 450, 666, 454, 1794, 454, 1790, 478, 638, 458, 666, 450, 670, 458, 666, 486, 634, 478, 1766, 450, 674, 450, 670, 454, 1790, 474, 1758, 478, 1774, 478, 1766, 450, 1786, 482, 1000};
uint16_t TV_4[IR_DATA_LEN] = {4442, 4570, 486, 1758, 482, 1766, 450, 1790, 478, 642, 454, 670, 450, 670, 454, 670, 450, 670, 478, 1766, 478, 1766, 450, 1790, 478, 638, 458, 670, 450, 666, 454, 666, 486, 638, 478, 642, 458, 662, 458, 662, 458, 1790, 478, 638, 458, 666, 450, 670, 458, 666, 450, 1794, 478, 1766, 450, 1802, 466, 638, 482, 1762, 458, 1790, 478, 1762, 458, 1782, 458, 1000};
uint16_t TV_5[IR_DATA_LEN] = {4438, 4574, 478, 1762, 454, 1798, 470, 1766, 458, 670, 450, 666, 454, 666, 478, 650, 470, 650, 450, 1794, 450, 1786, 482, 1762, 458, 666, 450, 670, 458, 666, 450, 670, 450, 670, 454, 1794, 450, 662, 486, 642, 478, 1762, 454, 670, 478, 638, 486, 638, 482, 642, 454, 670, 450, 1786, 482, 1766, 478, 642, 454, 1794, 474, 1762, 478, 1762, 458, 1794, 474, 1000};
uint16_t TV_6[IR_DATA_LEN] = {4414, 4598, 482, 1762, 458, 1786, 478, 1762, 458, 670, 450, 670, 450, 670, 478, 638, 482, 642, 482, 1766, 450, 1786, 482, 1762, 458, 670, 450, 666, 458, 670, 474, 642, 482, 638, 458, 662, 458, 1794, 458, 662, 478, 1762, 454, 670, 450, 670, 454, 670, 478, 638, 478, 1766, 478, 646, 454, 1786, 478, 650, 450, 1790, 450, 1794, 474, 1762, 470, 1774, 458, 1000};
uint16_t TV_7[IR_DATA_LEN] = {4422, 4590, 478, 1766, 478, 1766, 450, 1790, 478, 642, 482, 638, 486, 638, 454, 670, 450, 670, 458, 1782, 478, 1766, 478, 1762, 482, 642, 454, 670, 450, 670, 454, 670, 450, 666, 482, 642, 454, 670, 478, 1762, 458, 1790, 478, 638, 466, 662, 450, 670, 450, 670, 478, 1762, 454, 1782, 458, 670, 454, 670, 478, 1762, 454, 1794, 450, 1786, 482, 1766, 450, 1000};
uint16_t TV_8[IR_DATA_LEN] = {4414, 4598, 482, 1766, 474, 1762, 462, 1778, 462, 670, 450, 670, 462, 662, 450, 666, 454, 674, 474, 1762, 478, 1762, 486, 1758, 486, 638, 454, 670, 454, 662, 454, 674, 450, 666, 482, 1762, 466, 658, 450, 1794, 450, 1790, 478, 642, 454, 670, 454, 662, 454, 674, 474, 642, 458, 1790, 450, 670, 450, 670, 454, 1794, 470, 1766, 458, 1782, 458, 1790, 450, 1000};
uint16_t TV_9[IR_DATA_LEN] = {4410, 4602, 482, 1762, 454, 1782, 482, 1762, 486, 638, 454, 670, 450, 674, 450, 666, 482, 642, 478, 1762, 458, 1790, 462, 1782, 478, 638, 458, 662, 454, 674, 450, 666, 482, 642, 454, 666, 450, 1794, 478, 1762, 482, 1762, 454, 666, 454, 674, 474, 642, 478, 642, 458, 1790, 474, 650, 470, 650, 478, 638, 454, 1782, 458, 1794, 478, 1762, 458, 1790, 474, 1000};
uint16_t TV_VU[IR_DATA_LEN] = {4418, 4598, 454, 1782, 486, 1766, 478, 1762, 482, 638, 478, 638, 486, 638, 454, 670, 458, 662, 458, 1782, 486, 1762, 454, 1790, 478, 642, 482, 638, 478, 650, 450, 666, 454, 674, 450, 1786, 482, 1762, 458, 1790, 478, 638, 458, 666, 450, 670, 462, 666, 450, 662, 458, 670, 478, 646, 470, 650, 450, 1794, 450, 1790, 478, 1762, 454, 1786, 458, 1794, 474, 1000};
uint16_t TV_VD[IR_DATA_LEN] = {4414, 4594, 490, 1758, 486, 1754, 458, 1786, 482, 642, 454, 666, 450, 670, 458, 666, 450, 670, 478, 1762, 458, 1782, 458, 1794, 474, 650, 450, 670, 450, 666, 450, 670, 458, 670, 450, 1790, 478, 1766, 450, 674, 478, 1762, 478, 638, 454, 670, 458, 666, 462, 658, 478, 642, 454, 674, 450, 1786, 474, 650, 478, 1762, 478, 1762, 458, 1790, 482, 1758, 454, 1000};
uint16_t TV_CHU[IR_DATA_LEN] = {4442, 4570, 482, 1766, 478, 1762, 478, 1762, 454, 670, 454, 670, 478, 638, 478, 646, 454, 674, 446, 1794, 474, 1762, 482, 1762, 454, 670, 454, 670, 478, 638, 478, 646, 454, 666, 454, 674, 450, 1794, 474, 642, 478, 650, 450, 1790, 450, 670, 450, 674, 470, 646, 478, 1766, 454, 662, 462, 1782, 482, 1762, 458, 670, 450, 1790, 478, 1766, 478, 1762, 454, 1000};
uint16_t TV_CHD[IR_DATA_LEN] = {4442, 4574, 478, 1762, 482, 1762, 454, 1786, 458, 662, 486, 638, 478, 650, 450, 666, 450, 670, 458, 1790, 474, 1766, 454, 1786, 458, 670, 474, 638, 470, 658, 454, 670, 450, 670, 450, 670, 478, 650, 470, 646, 454, 662, 458, 1794, 458, 658, 478, 638, 486, 638, 454, 1794, 478, 1758, 486, 1766, 450, 1786, 466, 658, 478, 1762, 466, 1782, 450, 1794, 478, 1000};

uint16_t SPEAKER_VU[IR_DATA_LEN] = {8910, 4542, 506, 1726, 506, 622, 506, 1730, 502, 626, 506, 622, 506, 626, 506, 622, 502, 1734, 498, 1734, 506, 622, 506, 1730, 502, 1730, 482, 646, 506, 1730, 502, 1730, 482, 1750, 494, 1738, 506, 626, 502, 626, 506, 626, 482, 646, 506, 1726, 506, 1730, 502, 626, 506, 622, 506, 1726, 506, 1730, 502, 1730, 506, 1726, 506, 626, 502, 626, 506, 1726, 506, 1000};
uint16_t SPEAKER_VD[IR_DATA_LEN] = {8890, 4562, 506, 1726, 482, 646, 506, 1730, 502, 626, 506, 626, 502, 626, 482, 646, 490, 1746, 502, 1730, 482, 646, 506, 1730, 478, 1754, 482, 646, 482, 1754, 478, 1754, 490, 1742, 506, 626, 478, 650, 482, 646, 506, 626, 490, 638, 482, 1750, 482, 1754, 478, 650, 506, 1726, 506, 1730, 478, 1754, 482, 1750, 482, 1750, 482, 650, 502, 626, 506, 1726, 486, 1000};
uint16_t SPEAKER_AUXONE[IR_DATA_LEN] = {8886, 4562, 506, 1730, 502, 626, 506, 1726, 494, 638, 502, 626, 506, 622, 506, 626, 502, 1730, 506, 1730, 502, 626, 502, 1730, 506, 1726, 506, 626, 494, 1738, 506, 1726, 506, 1726, 506, 626, 506, 1726, 506, 1726, 506, 1730, 502, 626, 506, 1726, 486, 1746, 486, 646, 506, 1726, 506, 626, 502, 626, 506, 622, 506, 1730, 506, 622, 506, 626, 502, 1730, 506, 1000};
uint16_t SPEAKER_AUXTWO[IR_DATA_LEN] = {8890, 4562, 538, 1694, 534, 598, 482, 1750, 506, 622, 486, 646, 502, 626, 486, 646, 502, 1730, 506, 1726, 482, 650, 478, 1754, 502, 1730, 498, 634, 498, 1734, 482, 1750, 506, 1730, 478, 1754, 502, 1730, 482, 1750, 506, 1726, 506, 626, 502, 1730, 482, 1750, 506, 626, 506, 622, 506, 626, 502, 626, 506, 622, 506, 1730, 502, 626, 506, 626, 502, 1730, 494, 1000};
uint16_t SPEAKER_MUTE[IR_DATA_LEN] = {8890, 4562, 506, 1726, 506, 626, 502, 1730, 506, 622, 506, 626, 506, 622, 506, 626, 502, 1730, 502, 1730, 506, 622, 506, 1730, 502, 1730, 506, 622, 506, 1730, 502, 1734, 502, 1726, 506, 626, 502, 1730, 494, 634, 506, 1730, 502, 626, 506, 1726, 506, 1730, 490, 638, 506, 1726, 486, 646, 502, 1730, 482, 646, 506, 1730, 498, 630, 506, 622, 506, 1726, 506, 1000};
uint16_t SPEAKER_POWERON[IR_DATA_LEN] = {8898, 4546, 522, 1694, 510, 630, 502, 1734, 494, 630, 526, 606, 494, 630, 526, 606, 494, 1734, 526, 1714, 490, 630, 530, 1706, 518, 1710, 526, 606, 494, 1738, 522, 1710, 494, 1734, 518, 614, 470, 1758, 494, 658, 474, 630, 498, 634, 494, 634, 498, 630, 494, 642, 514, 1710, 518, 634, 498, 1710, 494, 1734, 494, 1738, 498, 1734, 498, 1734, 522, 1706, 498, 1000};
uint16_t SPEAKER_POWEROFF[IR_DATA_LEN] = {4442, 4574, 478, 1762, 478, 1766, 450, 1786, 482, 642, 454, 670, 478, 638, 454, 670, 450, 670, 458, 1778, 458, 1794, 450, 1786, 486, 638, 454, 670, 474, 650, 450, 666, 450, 670, 450, 670, 478, 642, 482, 642, 450, 674, 450, 674, 446, 1790, 454, 1794, 470, 650, 470, 1762, 458, 1786, 482, 1766, 450, 1794, 478, 1762, 454, 670, 478, 638, 458, 1782, 486, 1000};

const int CUST_REDPOWER = 0x202B24D;
const int CUST_GREENMUTE = 0x202C23D;
const int CUST_UP = 0x202D02F;
const int CUST_DOWN = 0x202D02F;
const int CUST_LEFT = 0x20208F7;
const int CUST_RIGHT = 0x2028877;
const int CUST_OK = 0x202B04F;
const int CUST_VOLUP = 0x202A857;
const int CUST_VOLDOWN = 0x20238C7;
const int CUST_GREYPOWER = 0x2028F70;

int action = -1;

int tvPos = -1;
int tvSetpoint = -1;
const int TV_INPUT_TV = 0;
const int TV_INPUT_360 = 1;
const int TV_INPUT_CHROME = 2;
const int TV_INPUT_ONE = 3;
unsigned long tvTimeDelay = 250;
unsigned long tvLastUpdated = -1;
int ups = 0;
int downs = 0;
boolean tvChanging = false;
boolean tvFirst = false;
boolean hideTvChange = false;

int speakerVolume = 0;
int speakerSetpoint = 0;
boolean speakerVolumeChanging = false;
unsigned long speakerTimeDelay = 200;
unsigned long speakerLastUpdated = -1;
unsigned long currentTime = 0;
RF24 radio(3, 5); // CE, CSN
char text[32];

int inData[11];
int tempAction = -1;
boolean blindIsDown = true;
int percentUp = 100;
const int fullTimeUp = 10000;
boolean goingDown = false;
int motorPin = 6;
int stopPin = 22;
unsigned long blindStartTime = 0;
boolean closeBlinds = false;
boolean openBlinds = false;
int blindsSetpoint = 0;
int finalT = 0;
unsigned long deltaT = 0;

void setup() {
  Wire.begin(0x05);                // join i2c bus with address #0x05
  Wire.onReceive(receiveEvent); // register event
  radio.begin();
  radio.setChannel(124);
  radio.openWritingPipe(0x00000001);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
  myReceiver.enableIRIn();
  pinMode(motorPin, OUTPUT);
  pinMode(stopPin, INPUT_PULLUP);
}

void loop() {

  checkGoodI2CData();
  checkIRData();
  updateOutputs();

}

void checkGoodI2CData()
{
  if (tempAction != -1)
  {
    action = tempAction;
    tempAction = -1;
  }
}

void updateOutputs()
{
  updateTV(action);
  updateSpeakers(action);
  updateBlinds(action);
  updateLights(action);
  FupdateLights(action);
  action = -1;
}
void shiftData()
{
  for (int i = 9; i > -1; i--)
  {
    inData[i + 1] = inData[i];
  }
}

boolean realDataIn0()
{
  boolean allGood = true;
  for (int i = 1; i < 11; i++)
  {
    if (inData[i] != 10 - i)
    {
      allGood = false;
    }
  }
  return allGood;
}

void updateTV(int action)
{
  if (action == 16)
  {
    //Serial.println("set the tv input to tv");
    tvSetpoint = TV_INPUT_TV;
    if (tvPos > tvSetpoint)
    {
      ups = tvPos - tvSetpoint;
      tvChanging = true;
      tvFirst = true;
    }
    if (tvPos < tvSetpoint)
    {
      downs = tvSetpoint - tvPos;
      tvChanging = true;
      tvFirst = true;
    }
  }
  if (action == 17)
  {
    //Serial.println("set the tv input to chrome");
    tvSetpoint = TV_INPUT_CHROME;
    if (tvPos > tvSetpoint)
    {
      ups = tvPos - tvSetpoint;
      tvChanging = true;
      tvFirst = true;
    }
    if (tvPos < tvSetpoint)
    {
      downs = tvSetpoint - tvPos;
      tvChanging = true;
      tvFirst = true;
    }
  }
  if (action == 18)
  {
    //Serial.println("set the tv input to xbox one");
    tvSetpoint = TV_INPUT_ONE;
    if (tvPos > tvSetpoint)
    {
      ups = tvPos - tvSetpoint;
      tvChanging = true;
      tvFirst = true;
    }
    if (tvPos < tvSetpoint)
    {
      downs = tvSetpoint - tvPos;
      tvChanging = true;
      tvFirst = true;
    }
  }
  if (action == 19)
  {
    //Serial.println("set the tv input to xbox 360 compnent");
    tvSetpoint = TV_INPUT_360;
    if (tvPos > tvSetpoint)
    {
      ups = tvPos - tvSetpoint;
      tvChanging = true;
      tvFirst = true;
    }
    if (tvPos < tvSetpoint)
    {
      downs = tvSetpoint - tvPos;
      tvChanging = true;
      tvFirst = true;
    }
  }
  if (action == 20)
  {
    //Serial.println("set the tv input to on");
    irLED.send(TV_POWER, IR_DATA_LEN, 36);
  }
  if (action == 21)
  {
    //Serial.println("set the tv input to off");
    irLED.send(TV_POWER, IR_DATA_LEN, 36);
  }
  if (tvChanging && abs(millis() - tvLastUpdated) > tvTimeDelay)
  {
    tvLastUpdated = millis();
    if (tvFirst)
    {
      irLED.send(TV_SOURCE, IR_DATA_LEN, 36);
      tvFirst = false;

    }
    else
    {
      if (downs == 0 && ups == 0)
      {
        irLED.send(TV_ENTER, IR_DATA_LEN, 36);
        tvChanging = false;
        if (!hideTvChange)
        {
          tvPos = tvSetpoint;

        }
        else
        {
          hideTvChange = false;
          tvSetpoint = tvPos;
        }


      }
      else
      {
        if (downs > 0)
        {
          irLED.send(TV_DOWN, IR_DATA_LEN, 36);
          downs--;
        }
        else
        {
          if (ups > 0)
          {
            irLED.send(TV_UP, IR_DATA_LEN, 36);
            ups--;
          }
        }
      }
    }
  }

}

void updateSpeakers(int action)
{
  if (action == 32)
  {
    //Serial.println("set the speaker chrome/aux2");
    irLED.send(SPEAKER_AUXTWO, IR_DATA_LEN, 36);
  }
  if (action == 33)
  {
    //Serial.println("set the speaker tv");
    irLED.send(SPEAKER_AUXONE, IR_DATA_LEN, 36);
  }
  if (action == 34)
  {
    //Serial.println("set the speaker record player");
  }
  if (action == 35)
  {
    //Serial.println("set the speaker mute or unmute");
    irLED.send(SPEAKER_MUTE, IR_DATA_LEN, 36);
  }
  if (action == 36)
  {
    //Serial.println("speaker power on");
    irLED.send(SPEAKER_POWERON, IR_DATA_LEN, 36);
  }
  if (action == 37)
  {
    //Serial.println("speaker power off");
    irLED.send(SPEAKER_POWEROFF, IR_DATA_LEN, 36);
  }
  if (action > 127 && action < 192 )
  {
    //Serial.println("set the speaker volume to " + String(action - 128));
    speakerSetpoint = action - 128;
    speakerVolumeChanging = true;
  }
  currentTime = millis();
  if (speakerVolumeChanging && abs(currentTime - speakerLastUpdated) > speakerTimeDelay)
  {
    speakerLastUpdated = currentTime;
    if (speakerSetpoint == speakerVolume) {
      speakerVolumeChanging = false;
      //Serial.println("volume at setpoint");
    }
    if (speakerSetpoint > speakerVolume)
    {
      speakerVolume++;
      irLED.send(SPEAKER_VU, IR_DATA_LEN, 36);
      //Serial.println(speakerVolume);

    }
    if (speakerSetpoint < speakerVolume)
    {
      speakerVolume--;
      irLED.send(SPEAKER_VD, IR_DATA_LEN, 36);
      //Serial.println(speakerVolume);
    }
  }
}

void updateBlinds(int action)
{
  blindIsDown = !digitalRead(stopPin);
  if (action == 48)
  {
    //Serial.println("close bliunds");
    closeBlinds = true;
    blindStartTime = millis();
  }
  if (action == 49)
  {
    //Serial.println("half blinds");
    if (blindIsDown)
    {
      openBlinds = true;
      blindStartTime = millis();
      blindsSetpoint = 50;
    }
  }
  if (action == 50)
  {
    //Serial.println("full blinds");
    if (blindIsDown)
    {
      openBlinds = true;
      blindStartTime = millis();
      blindsSetpoint = 100;
    }
  }
  if (action > 191 && action < 243)
  {
    //Serial.println("set the blinds to " + String(2 * (action - 192)) + "percent");
    if (blindIsDown)
    {
      openBlinds = true;
      blindStartTime = millis();
      blindsSetpoint = 2 * (action - 192);
    }
  }

  if (closeBlinds)
  {
    if (!blindIsDown && abs(millis() - blindStartTime) < (double)fullTimeUp * 1.5)
    {
      analogWrite(motorPin, 75);//close

    }
    else
    {
      digitalWrite(motorPin, 0);
      closeBlinds = false;
    }
  }
  if (openBlinds)
  {
    deltaT = abs(millis() - blindStartTime);
    finalT = (int)(((double)blindsSetpoint / (double)100) * (double)fullTimeUp);
    //Serial.println(deltaT );
    //Serial.println(finalT);
    if (deltaT < finalT)
    {
      //Serial.println("high");
      analogWrite(motorPin, 170);
    }
    else
    {
      //Serial.println("low");
      digitalWrite(motorPin, 0);
      openBlinds = false;
      percentUp = blindsSetpoint;
    }
  }

}

void updateLights(int action)
{
  if (action == 64)
  {
    setMsg("1_");
    sendRFMsgConfirm();
  }
  if (action == 65)
  {
    setMsg("0_");
    sendRFMsgConfirm();
  }
  if (action == 66)
  {
    setMsg("_1");
    sendRFMsgConfirm();
  }
  if (action == 67)
  {
    setMsg("_0");
    sendRFMsgConfirm();
  }
  if (action == 68)
  {
    setMsg("11");
    sendRFMsgConfirm();
  }
  if (action == 69)
  {
    setMsg("00");
    sendRFMsgConfirm();
  }
}
void FupdateLights(int action)
{
  if (action == 70)
  {
    setMsg("F1_");
    sendRFMsgConfirm();
  }
  if (action == 71)
  {
    setMsg("F0_");
    sendRFMsgConfirm();
  }
  if (action == 72)
  {
    setMsg("F_1");
    sendRFMsgConfirm();
  }
  if (action == 73)
  {
    setMsg("F_0");
    sendRFMsgConfirm();
  }
  if (action == 74)
  {
    setMsg("F11");
    sendRFMsgConfirm();
  }
  if (action == 75)
  {
    setMsg("F00");
    sendRFMsgConfirm();
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
}

void checkIRData()
{
  if (myReceiver.getResults()) {
    myDecoder.decode();
    irDatIn = myDecoder.value;

  }

  if (irDatIn == 3859929889 || irDatIn == 2012435229 )
  {
    irLED.send(TV_POWER, IR_DATA_LEN, 36);
    
  }
  if (irDatIn == 3712536422)
  {
    irLED.send(TV_SOURCE, IR_DATA_LEN, 36);
    
  }

  if (irDatIn == 4039450310)
  {
    irLED.send(TV_UP, IR_DATA_LEN, 36);
    

  }
  if (irDatIn == 1772640354)
  {
    irLED.send(TV_DOWN, IR_DATA_LEN, 36);
    
  }
  if (irDatIn == 2291974051)
  {
    irLED.send(TV_CHU, IR_DATA_LEN, 36);
    
  }
  if (irDatIn == 879088869)
  {
    irLED.send(TV_CHD, IR_DATA_LEN, 36);
    
  }

  if (irDatIn == 3262729347)
  {
    irLED.send(SPEAKER_MUTE, IR_DATA_LEN, 36);
    
  }
  if (irDatIn == 1991468771)
  {
    openBlinds = true;
    blindStartTime = millis();
    blindsSetpoint = 100;

  }
  if (irDatIn == 1860168033)
  {
    closeBlinds = true;
    blindStartTime = millis();
  }
  if (irDatIn == 2453607691)
  {
    digitalWrite(motorPin, 0);
    openBlinds = false;
    closeBlinds = false;
    irLED.send(TV_ENTER, IR_DATA_LEN, 36);
    
  }
  if (irDatIn == 1162079395)
  {
    speakerVolume++;
    irLED.send(SPEAKER_VU, IR_DATA_LEN, 36);
    
  }
  if (irDatIn == 2052588673)
  {
    speakerVolume--;
    irLED.send(SPEAKER_VD, IR_DATA_LEN, 36);
    
  }
  if (irDatIn == 1150302533 || irDatIn == 1136104327)
  {
    irLED.send(SPEAKER_AUXTWO, IR_DATA_LEN, 36);
    
  }
  if (irDatIn == 2224235747 || irDatIn == 2477111649)
  {
    irLED.send(SPEAKER_AUXONE, IR_DATA_LEN, 36);
    
  }
  if (irDatIn == 1523480583 )
  {
    setMsg("F11");
    sendRFMsgConfirm();
  }
  if (irDatIn == 42510820553 || irDatIn == 4251082055)
  {
    setMsg("F00");
    sendRFMsgConfirm();
  }

  if (irDatIn == 1394745315 )
  {
    setMsg("F_1");
    sendRFMsgConfirm();
  }
  if (irDatIn == 3467979613)
  {
    setMsg("F_0");
    sendRFMsgConfirm();
  }

  if (irDatIn == 1212113475 ) //hall
  {
    setMsg("F1_");
    sendRFMsgConfirm();
  }
  if (irDatIn == 673965407)
  {
    setMsg("F0_");
    sendRFMsgConfirm();
  }
  if (irDatIn == 934531617)
  {
    tvSetpoint = 0;
    tvSet();
  }
  if (irDatIn == 3541082599)
  {
    tvSetpoint = 2;
    tvSet();
  }
  if (irDatIn == 2294479809)
  {
    tvSetpoint = 3;
    tvSet();
  }
  if (irDatIn == 952318659)
  {
    tvSetpoint = 1;
    tvSet();
  }

  if (irDatIn == 540941759)//all on
  {
    blindIsDown = !digitalRead(stopPin);
    if (blindIsDown)
    {
      openBlinds = true;
      blindStartTime = millis();
      blindsSetpoint = 100;
    }
    setMsg("F11");
    sendRFMsgConfirm();
  }

  if (irDatIn == 2050993606)//all off
  {
    closeBlinds = true;
    blindStartTime = millis();
    setMsg("F00");
    sendRFMsgConfirm();
  }


  if (irDatIn == 134254273)
  {
    irLED.send(TV_0, IR_DATA_LEN, 36);
  }
  if (irDatIn == 741238371)
  {
    irLED.send(TV_1, IR_DATA_LEN, 36);
  }
  if (irDatIn == 95080641)
  {
    irLED.send(TV_2, IR_DATA_LEN, 36);
  }
  if (irDatIn == 2759906759)
  {
    irLED.send(TV_3, IR_DATA_LEN, 36);
  }
  if (irDatIn == 421994529)
  {
    irLED.send(TV_4, IR_DATA_LEN, 36);
  }
  if (irDatIn == 1292669927)
  {
    irLED.send(TV_5, IR_DATA_LEN, 36);
  }
  if (irDatIn == 3543462021)
  {
    irLED.send(TV_6, IR_DATA_LEN, 36);
  }
  if (irDatIn == 2448539651)
  {
    irLED.send(TV_7, IR_DATA_LEN, 36);
  }
  if (irDatIn == 80439233)
  {
    irLED.send(TV_8, IR_DATA_LEN, 36);
  }
  if (irDatIn == 2269822023)
  {
    irLED.send(TV_9, IR_DATA_LEN, 36);
  }

  irDatIn = -1;
  myReceiver.enableIRIn();

}

void tvSet()
{
  if (tvPos > tvSetpoint)
  {
    ups = tvPos - tvSetpoint;
    tvChanging = true;
    tvFirst = true;
  }
  if (tvPos < tvSetpoint)
  {
    downs = tvSetpoint - tvPos;
    tvChanging = true;
    tvFirst = true;
  }
}

void receiveEvent(int howMany) {
  tempAction = Wire.read(); // receive byte as a character

}

