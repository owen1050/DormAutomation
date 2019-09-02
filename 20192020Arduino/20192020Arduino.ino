#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char serverName[] = "100.35.205.75";
int serverPort = 23654;
EthernetClient client;
boolean good = true;
String gVars[] = {"blindsMoveAllUp", "0", "blindMoveAllDown", "0", "blindMovePUp", "0", "blindMovePDown", "0", "mainLightOn", "0", "mainLightOff", "0", "hallLightOn", "0", "hallLightOff", "0", "projectorOn", "0", "projectorOff", "0", "tvOn", "0", "tvOff", "0", "tvInComp", "0", "tvInChrome", "0", "projInComp", "0", "projInChrome", "0", "speakerMute", "0", "speakerInProj", "0", "speeakerInTV", "0", "speakerInPhono", "0", "speakerInChrome", "0", "speakerVolumeUp", "0", "speakerVolumeDown", "0"};
 //                 0                         2                         4                       6                   8                         10                12                    14              16                      18                20    
void setup() {
  Serial.begin(9600);
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  Serial.print(F("Starting ethernet..."));
  if(!Ethernet.begin(mac)) Serial.println(F("failed"));
  else Serial.println(Ethernet.localIP());
  delay(2000);
  Serial.println(F("Ready"));
}

void loop() {
  Ethernet.maintain();
  //Serial.println();
  String str = postPage("return_if_changed");
  int check = str.indexOf("no_change");
  good = false;
  if(check < 0)
  {
    good = true;
    for(int i = 0; i <= 22 && good; i++)
    {
      int ti = str.indexOf(gVars[2*i]);
      if(ti<0)
      {
        good = false;
      }      
    }    
  }
  if(good)
  {
    updateGvars(str);
  }
  if(gVars[9].toInt()==1)
  {
    Serial.println("TURN MAIN LIGHT ON");
    postPage("set:mainLightOn=0;");
  }
  if(gVars[11].toInt()==1)
  {
    Serial.println("TURN MAIN LIGHT OFF");
    postPage("set:mainLightOff=0;");
  }
  
  delay(20);
}

String postPage(char* thisData)
{
  int inChar;
  char outBuf[64]; 

  if(client.connect(serverName,serverPort) == 1)
  {
    
    sprintf(outBuf,"POST %s HTTP/1.1","a");
    client.println(outBuf);
    
    sprintf(outBuf,"Content-Length: %u\r\n",strlen(thisData));
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
  while(client.connected())
  {
    while(client.available())
    {
      inChar = client.read();
      //Serial.write(inChar);
     retStr.concat(char(inChar));
      connectLoop = 0;
    }

    delay(1);
    connectLoop++;
    if(connectLoop > 2000)
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



void updateGvars(String nVars){  
  Serial.println("vars updated");
  for(int i = 0; i < 46; i = i + 2)
  {
    int lowI = nVars.indexOf(gVars[i]);
    int sVI = nVars.indexOf("=", lowI);
    int eVI = nVars.indexOf("!", sVI);
    gVars[i+1] = nVars.substring(sVI+1, eVI);
    //Serial.print("var=");
    //Serial.print(gVars[i]);
    //Serial.print(":");
    //Serial.println(gVars[i+1]);
  }
}
