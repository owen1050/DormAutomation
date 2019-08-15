//this will be the code for mega dorm automation 20192020

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
char url[] = {"www.auto.owenbusler.com"};
int serverPort = 23654;

String gVars[] = {"blindsMoveAllUp", "0", "blindMoveAllDown", "0", "blindMovePUp", "0", "blindMovePDown", "0", "mainLightOn", "0", "mainLightOff", "0", "hallLightOn", "0", "hallLightOff", "0", "projectorOn", "0", "projectorOff", "0", "tvOn", "0", "tvOff", "0", "tvInComp", "0", "tvInChrome", "0", "projInComp", "0", "projInChrome", "0", "speakerMute", "0", "speakerInProj", "0", "speeakerInTV", "0", "speakerInPhono", "0", "speakerInChrome", "0", "speakerVolumeUp", "0", "speakerVolumeDown", "0"};


void setup()
{
	Serial.begin(9600);
}

void loop()
{
}

void updateGvars(String nVars){	
  
	for(int i = 0; i < sizeof(gVars); i = i + 2)
	{
		int lowI = nVars.indexOf(gVars[i]);
		int sVI = nVars.indexOf("=", lowI);
		int eVI = nVars.indexOf("!", sVI);
		gVars[i+1] = nVars.substring(sVI, eVI);
	}
}
