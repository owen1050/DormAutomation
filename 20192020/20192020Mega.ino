//this will be the code for mega dorm automation 20192020

String gVars[] = {"blindsMoveAllUp", "0", "blindMoveAllDown", "0", "blindMovePUp", "0", "blindMovePDown", "0", "mainLightOn", "0", "mainLightOff", "0", "hallLightOn", "0", "hallLightOff", "0", "projectorOn", "0", "projectorOff", "0", "tvOn", "0", "tvOff", "0", "tvInComp", "0", "tvInChrome", "0", "projInComp", "0", "projInChrome", "0", "speakerMute", "0", "speakerInProj", "0", "speeakerInTV", "0", "speakerInPhono", "0", "speakerInChrome", "0", "speakerVolumeUp", "0", "speakerVolumeDown", "0"}


void setup()
{
}

void main()
{
}


void updateGvars(nVars)
{	
	for(int i = 0; i < sizeof(gVars); i = i + 2)
	{
		lowI = nVars.indexof(gVars[i])
		sVI = nVars.indexof("=", lowI)
		eVI = nVars.indexof("!", sVI)
		gVars[i+1] = nVars.substring(sVI, eVI)
	}
}

