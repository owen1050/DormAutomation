#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(3, 5); // CE, CSN
char text[32];

int inData[11];

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(124);
  radio.openWritingPipe(0x00000001);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setPayloadSize(128);
  radio.stopListening();
}

void loop() {
  checkGoodData();
}

void checkGoodData()
{
  if (Serial.available())
  {
    shiftData();
    inData[0] = Serial.read();

    if (realDataIn0())
    {
      Serial.print(String(inData[0]) + " ");
      int action = inData[0];
      updateTV(action);
      updateSpeakers(action);
      updateBlinds(action);
      updateLights(action);
      FupdateLights(action);
      action = -1;
    }
  }
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
    Serial.println("set the tv input to tv");
  }
  if (action == 17)
  {
    Serial.println("set the tv input to chrome");
  }
  if (action == 18)
  {
    Serial.println("set the tv input to xbox one");
  }
  if (action == 19)
  {
    Serial.println("set the tv input to xbox 360 compnent");
  }
  if (action == 20)
  {
    Serial.println("set the tv input to on");
  }
  if (action == 21)
  {
    Serial.println("set the tv input to off");
  }
}

void updateSpeakers(int action)
{
  if (action == 32)
  {
    Serial.println("set the speaker chrome");
  }
  if (action == 33)
  {
    Serial.println("set the speaker tv");
  }
  if (action == 34)
  {
    Serial.println("set the speaker record player");
  }
  if (action == 35)
  {
    Serial.println("set the speaker mute or unmute");
  }
  if (action > 127 && action < 192 )
  {
    Serial.println("set the speaker volume to " + String(action - 128));
  }
}

void updateBlinds(int action)
{
  if (action == 48)
  {
    Serial.println("close bliunds");
  }
  if (action == 49)
  {
    Serial.println("half blinds");
  }
  if (action == 50)
  {
    Serial.println("full blinds");
  }
  if (action > 191 && action < 243)
  {
    Serial.println("set the blinds to " + String(2 * (action - 192)) + "percent");
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
  Serial.println( millis() - time0);
  
  

}

