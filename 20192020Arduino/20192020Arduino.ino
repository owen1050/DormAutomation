#include <SPI.h>
#include <Ethernet.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(35,36);
char text[32];
const byte address[6] = "00001";

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char serverName[] = "100.35.205.75";
int serverPort = 23654;
EthernetClient client;
boolean inDataGood = true;
int blindsState = 0;
long blindsUpTime = 0;
int lsPin = 10;
int mcPin = 5;
boolean lsState = false;
String gVars[] = {"blindsMoveAllUp", "0", "blindMoveAllDown", "0", "blindMovePUp", "0", "blindMovePDown", "0", "mainLightOn", "0", "mainLightOff", "0", "hallLightOn", "0", "hallLightOff", "0", "projectorOn", "0", "projectorOff", "0", "tvOn", "0", "tvOff", "0", "tvInComp", "0", "tvInChrome", "0", "projInComp", "0", "projInChrome", "0", "speakerMute", "0", "speakerInProj", "0", "speeakerInTV", "0", "speakerInPhono", "0", "speakerInChrome", "0", "speakerVolumeUp", "0", "speakerVolumeDown", "0"};
//                 0                         2                         4                       6                   8                         10                12                    14              16                      18                20         22            24                  26                28                  30                  32                  34                    36                      38                    40                      42                    44
void setup() {
  Serial.begin(9600);
  pinMode(lsPin, INPUT_PULLUP);
  pinMode(mcPin, OUTPUT);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  Serial.print(F("Starting ethernet..."));
  if (!Ethernet.begin(mac)) Serial.println(F("failed"));
  else Serial.println(Ethernet.localIP());
  delay(2000);
  Serial.println(F("Ready"));
}

void loop() {
  maintainServer();
  lightsUpdate();
  blindsUpdate();
  irUpdate();
  delay(30);
}

boolean irUpdate()
{
  if (gVars[17].toInt() == 1)
  {
    Serial.println("proj on");
    setMsg("0");
    sendRFMsgConfirm();
    postPage("set:projectorOn=0;");
  }
  if (gVars[19].toInt() == 1)
  {
    Serial.println("proj off");
    setMsg("1");
    sendRFMsgConfirm();
    postPage("set:projectorOff=0;");
  }
  if (gVars[21].toInt() == 1)
  {
    Serial.println("tv on");
    setMsg("2");
    sendRFMsgConfirm();
    postPage("set:tvOn=0;");
  }
  if (gVars[23].toInt() == 1)
  {
    Serial.println("tv off");
    setMsg("3");
    sendRFMsgConfirm();
    postPage("set:tvOff=0;");
  }
  if (gVars[33].toInt() == 1)
  {
    Serial.println("sp mute");
    setMsg("4");
    sendRFMsgConfirm();
    postPage("set:speakerMute=0;");
  }
  if (gVars[35].toInt() == 1)
  {
    Serial.println("sp proj");
    setMsg("5");
    sendRFMsgConfirm();
    postPage("set:speakerInProj=0;");
  }
  if (gVars[37].toInt() == 1)
  {
    Serial.println("sp tv");
    setMsg("6");
    sendRFMsgConfirm();
    postPage("set:speeakerInTV=0;");
  }
  if (gVars[39].toInt() == 1)
  {
    Serial.println("sp phono");
    setMsg("7");
    sendRFMsgConfirm();
    postPage("set:speakerInPhono=0;");
  }
  if (gVars[41].toInt() == 1)
  {
    Serial.println("sp chrome");
    setMsg("8");
    sendRFMsgConfirm();
    postPage("set:speakerInChrome=0;");
  }
  if (gVars[43].toInt() > 0)
  {
    Serial.println("vu");
    setMsg("U"+gVars[43]);
    sendRFMsgConfirm();
    postPage("set:speakerVolumeUp=0;");
  }
  if (gVars[45].toInt() > 0)
  {
    Serial.println("vd");
    setMsg("U"+gVars[45]);
    sendRFMsgConfirm();
    postPage("set:speakerVolumeDown=0;");
  }
}

boolean blindsUpdate()
{
  //0 = not moving, 1 = moving up, -1 = moivng down

  if (gVars[1].toInt() == 1)
  {
    blindsState = 1;
    blindsUpTime = millis();
    Serial.println("move blinds up");
    postPage("set:blindsMoveAllUp=0;");
  }

  if (gVars[3].toInt() == 1)
  {
    blindsUpTime = millis();
    blindsState = -1;
    Serial.println("move blinds down");
    postPage("set:blindMoveAllDown=0;");
  }

  lsState = !digitalRead(lsPin);
  if (blindsState == 1 and abs(millis() - blindsUpTime) < 10000)
  {
    
    analogWrite(mcPin, 170);//go up
  }

  if ((blindsState == 1)  and abs(millis() - blindsUpTime) > 10000)
  {
    digitalWrite(mcPin, 0);//stop
    blindsState = 0;
  }
  if ( blindsState == -1)  and abs(millis() - blindsUpTime) > 12000)
  {
    digitalWrite(mcPin, 0);//stop
    blindsState = 0;
  }

  if (blindsState == 0)
  {
    digitalWrite(mcPin, 0);//stop
    blindsState = 0;
  }

  if (blindsState == -1 and lsState == false)
  {
    analogWrite(mcPin, 75);//down
  }
  if (blindsState == -1 and lsState == true)
  {
    analogWrite(mcPin, 170);//up
    while(digitalRead(lsPin) == false){}
    digitalWrite(mcPin, 0);//stop
    blindsState = 0;
  }
}

boolean lightsUpdate()
{
  boolean bothMove = false;
  if (gVars[9].toInt() == 1 and gVars[13].toInt() == 1 )
  {
    bothMove = true;
    Serial.println("TURN both LIGHT ON");
    setMsg("F00");
    Serial.println("set");
    sendRFMsgConfirm();
    Serial.println("sent");
    postPage("set:mainLightOn=0;");
    postPage("set:hallLightOn=0;");
  }
  if (gVars[11].toInt() == 1 and gVars[15].toInt() == 1)
  {
    bothMove = true;
    Serial.println("TURN both LIGHT OFF");
    setMsg("F11");
    sendRFMsgConfirm();
    postPage("set:mainLightOff=0;");
    postPage("set:hallLightOff=0;");
  }

  if (gVars[9].toInt() == 1 && bothMove == false )
  {
    Serial.println("TURN MAIN LIGHT ON");
    setMsg("F_0");
    sendRFMsgConfirm();
    postPage("set:mainLightOn=0;");
  }
  if (gVars[11].toInt() == 1 && bothMove == false)
  {
    Serial.println("TURN MAIN LIGHT OFF");
    setMsg("F_1");
    sendRFMsgConfirm();
    postPage("set:mainLightOff=0;");
  }
  if (gVars[13].toInt() == 1 && bothMove == false)
  {
    Serial.println("TURN hall LIGHT ON");
    setMsg("F0_");
    sendRFMsgConfirm();
    postPage("set:hallLightOn=0;");
  }
  if (gVars[15].toInt() == 1 && bothMove == false)
  {
    Serial.println("TURN hall LIGHT OFF");
    setMsg("F1_");
    sendRFMsgConfirm();
    postPage("set:hallLightOff=0;");
  }

}
boolean maintainServer()
{
  Ethernet.maintain();
  //Serial.println();
  String str = postPage("return_if_changed");
  int check = str.indexOf("no_change");
  inDataGood = false;
  if (check < 0)
  {
    inDataGood = true;
    for (int i = 0; i <= 22 && inDataGood; i++)
    {
      int ti = str.indexOf(gVars[2 * i]);
      if (ti < 0)
      {
        inDataGood = false;
      }
    }
  }
  if (inDataGood)
  {
    updateGvars(str);
  }
}

String postPage(char* thisData)
{
  int inChar;
  char outBuf[64];

  if (client.connect(serverName, serverPort) == 1)
  {

    sprintf(outBuf, "POST %s HTTP/1.1", "a");
    client.println(outBuf);

    sprintf(outBuf, "Content-Length: %u\r\n", strlen(thisData));
    client.println(outBuf);

    // send the body (variables)
    client.print(thisData);
  }
  else
  {
    return "ERROR";
  }

  int connectLoop = 0;
  String retStr = "";
  while (client.connected())
  {
    while (client.available())
    {
      inChar = client.read();
      //Serial.write(inChar);
      retStr.concat(char(inChar));
      connectLoop = 0;
    }

    delay(1);
    connectLoop++;
    if (connectLoop > 2000)
    {
      Serial.println();
      Serial.println(F("Timeout"));
      client.stop();
    }
  }
  //delay(50);
  //Serial.println(millis());
  client.stop();
  return retStr;
}



void updateGvars(String nVars) {
  Serial.println("vars updated");
  for (int i = 0; i < 46; i = i + 2)
  {
    int lowI = nVars.indexOf(gVars[i]);
    int sVI = nVars.indexOf("=", lowI);
    int eVI = nVars.indexOf("!", sVI);
    gVars[i + 1] = nVars.substring(sVI + 1, eVI);
    //Serial.print("var=");
    //Serial.print(gVars[i]);
    //Serial.print(":");
    //Serial.println(gVars[i+1]);
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
    Serial.println("sending");
  }
  // Serial.println("done");
}
