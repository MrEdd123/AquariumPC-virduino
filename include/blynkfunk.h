// blynkfunk.h

#ifndef _BLYNKFUNK_h
#define _BLYNKFUNK_h

/******* LED Timer Sonne Auf/Untergang *******/

BLYNK_WRITE(V0) 
{
	TimeInputParam t(param);

	if (t.hasStartTime())
	{
		uint8_t SoAuStdEE;
		uint8_t SoAuMinEE;
		SoAuStdEE = t.getStartHour();
		preferences.putUInt("StartS", SoAuStdEE);
		SoAuMinEE = t.getStartMinute();
		preferences.putUInt("StartM", SoAuMinEE);
		SoAuStd = preferences.getUInt("StartS", 0);
		SoAuMin = preferences.getUInt("StartM", 0);
	}

	if (t.hasStopTime())
	{
		uint8_t SoUnStdEE;
		uint8_t SoUnMinEE;
		SoUnStdEE = t.getStopHour();
		preferences.putUInt("StopS", SoUnStdEE);
		SoUnMinEE = t.getStopMinute();
		preferences.putUInt("StopM", SoUnMinEE);
		SoUnStd = preferences.getUInt("StopS", 0);
		SoUnMin = preferences.getUInt("StopM", 0);
	}

	SunTimer();
}

/*********** LED Timer Mittagssonne ************/

BLYNK_WRITE(V6) {
	TimeInputParam t(param);

	if (t.hasStartTime())
	{
		SoMiAnStd = t.getStartHour();
		SoMiAnMin = t.getStartMinute();
		preferences.putUInt("MiAnLS", SoMiAnStd);
		preferences.putUInt("MiAnLM", SoMiAnMin);
		SoMiAnStd = preferences.getUInt("MiAnLS", 0);
		SoMiAnMin = preferences.getUInt("MIAnLM", 0);

	}

	if (t.hasStopTime())
	{
		SoMiAusStd = t.getStopHour();
		SoMiAusMin = t.getStopMinute();
		preferences.putUInt("MiAuLS", SoMiAusStd);
		preferences.putUInt("MiAuLM", SoMiAusMin);
		SoMiAusStd = preferences.getUInt("MiAuLS", 0);
		SoMiAusMin = preferences.getUInt("MIAuLM", 0);

	}
}

/************ LED Timer Nachtlicht *****************/

BLYNK_WRITE(V11) {

	TimeInputParam t(param);

	if (t.hasStartTime())
	{
		SoNaStd = t.getStartHour();
		SoNaMin = t.getStartMinute();
		preferences.putUInt("NaLS", SoNaStd);
		preferences.putUInt("NaLM", SoNaMin);
		SoNaStd = preferences.getUInt("NaLS", 0);
		SoNaMin = preferences.getUInt("NaLM", 0);
	}
}

/**************** CO2 Timer ******************/

BLYNK_WRITE(V4) {
	TimeInputParam t(param);

	if (t.hasStartTime())
	{
		CO2AnStd = t.getStartHour();
		CO2AnMin = t.getStartMinute();
		preferences.putUInt("COSS", CO2AnStd);
		preferences.putUInt("COSM", CO2AnMin);
		CO2AnStd = preferences.getUInt("COSS", 0);
		CO2AnMin = preferences.getUInt("COSM", 0);
	}

	if (t.hasStopTime())
	{
		CO2AusStd = t.getStopHour();
		CO2AusMin = t.getStopMinute();
		preferences.putUInt("COAS", CO2AusStd);
		preferences.putUInt("COAM", CO2AusMin);
		CO2AusStd = preferences.getUInt("COAS", 0);
		CO2AusMin = preferences.getUInt("COAM", 0);
	}

	CO2Timer();
}

/*************** Soll Temperatur ***********/

BLYNK_WRITE(V2) {

	Blynk.virtualWrite(V1, param.asFloat());
	SollTemp = param.asFloat();
	preferences.putUInt("SollT", SollTemp);
	SollTemp = preferences.getUInt("SollT", 0);
}

/******** Luefter **************************/

BLYNK_WRITE(V20) {

	Blynk.virtualWrite(V20, param.asFloat());
	LuefTemp = param.asFloat();
	preferences.putUInt("LueT", LuefTemp);
	LuefTemp = preferences.getUInt("LueT", 0);
}

/************* Durchlauf Zeit *************/

BLYNK_WRITE(V5) {

	Blynk.virtualWrite(V5, param.asFloat());
	DurchWait = param.asFloat();
	preferences.putUInt ("Wait", DurchWait);
	DurchWait = preferences.getUInt("Wait", 0);
}

/******* Futterautomat ********************/

BLYNK_WRITE(V7) {

	TimeInputParam t(param);

	if (t.hasStartTime())
	{
		FutterStd = t.getStartHour();
		FutterMin = t.getStartMinute();
		preferences.putUInt("FuttS", FutterStd);
		preferences.putUInt("FuttM", FutterMin);
		FutterStd = preferences.getUInt("FuttS", 0);
		FutterMin = preferences.getUInt("FuttM", 0);
	}

	FutterTimer();
}

BLYNK_WRITE(V34) {

	Blynk.virtualWrite(V34, param.asFloat());
	Futterdauer = param.asFloat() * 1000;
	//Futterdauer = Futterdauer * 1000;
	preferences.putUInt("FutD", Futterdauer);
	Futterdauer = preferences.getUInt("FutD", 0);
}

BLYNK_WRITE(V35) {

	Blynk.virtualWrite(V35, param.asFloat());
	Futtergesch = param.asFloat();
	preferences.putUInt("FutG", Futtergesch);
	Futtergesch = preferences.getUInt("FutG", 0);
}

/********** Temp Hysterese *****************/

BLYNK_WRITE(V8) {

	Blynk.virtualWrite(V8, param.asFloat());
	Hysterese = param.asFloat();
	preferences.putFloat("Hyst", Hysterese);
	Hysterese = preferences.getFloat("Hyst", 0);
}

/****** Maximale Helligkeit *****************/

BLYNK_WRITE(V9) {

	Blynk.virtualWrite(V9, param.asFloat());
	maxHell = param.asFloat();
	preferences.putUInt("MaxH", maxHell);
	maxHell = preferences.getUInt("MaxH", 0);
}

/******* Mittags Helligkeit *****************/

BLYNK_WRITE(V25) {

	Blynk.virtualWrite(V25, param.asFloat());
	mittagHell = param.asFloat();
	preferences.putUInt("MitH", mittagHell);
	mittagHell = preferences.getUInt("MitH", 0);
}

/******* PowerLED Max Hellighkeit *********/

BLYNK_WRITE(V36) 
{

	Blynk.virtualWrite(V36, param.asFloat());
	Powerledmax = param.asFloat();
	preferences.putUInt ("PowH", Powerledmax);
	Powerledmax = preferences.getUInt("PowH", 0);
}

/******* Aktuelle Helligkeit *****************/

BLYNK_WRITE(V28) 
{

	Blynk.virtualWrite(V28, param.asFloat());
	aktHell = param.asFloat();
}

/******** TFT Helligkeit ********************/

BLYNK_WRITE(V10) 
{
	
	Blynk.virtualWrite(V10, param.asFloat());
	BacklightwertTag = param.asFloat();
	preferences.putUInt("BackLT", BacklightwertTag);
	ledcWrite(BacklightKanalTFT, BacklightwertTag);
}

BLYNK_WRITE(V33) 
{

	Blynk.virtualWrite(V33, param.asFloat());
	BacklightwertNacht = param.asFloat();
	preferences.putUInt("BackLN", BacklightwertNacht);
	//ledcWrite(BacklightKanalTFT, BacklightwertNacht);
}

/****** TFT Rotation ************************/

BLYNK_WRITE(V19) 
{

	int i = param.asInt();
	if (i == 1) {
		TFTRotation = 3;
		delay(250);
	}
	else
		TFTRotation = 1;

	preferences.putInt("TFTR", TFTRotation);	

	TFT_Layout();
	
	WIFI_TFT();
	
	CO2Timer();
	
	SunTimer();
	
	FutterTimer();
	
}

/************ Manuelle Funktionen ************/

BLYNK_WRITE(V12) 
{ /*Sonne An*/

	int i = param.asInt();
	if (i == 1) {
		SonneIndex = 1;
		delay(250);
	}
}

BLYNK_WRITE(V14) 
{ /*Sonne Aus*/

	int i = param.asInt();
	if (i == 1) {
		SonneIndex = 2;
		delay(250);
	}
}

BLYNK_WRITE(V13) 
{ /*Sonne Mittag AN*/

	int i = param.asInt();
	if (i == 1) {
		SonneMitAn();
		delay(250);
		
	}
}

BLYNK_WRITE(V21) 
{ /*Sonne Mittag Aus*/

	int i = param.asInt();
	if (i == 1) {
		SonneMitAus();
		delay(250);
		
	}
}

BLYNK_WRITE(V15) 
{ /*Nachlicht Aus*/

	int i = param.asInt();
	if (i == 1) {
		SonneIndex = 5;
		delay(250);
		SonneIndex = 0;
	}
}

BLYNK_WRITE(V16) 
{ /*Futterautomat*/


	int i = param.asInt();
	if (i == 1) 
	{
		FutterIndex = 1;
	}	

}

BLYNK_WRITE(V17) 
{ /*Luefter*/

	int i = param.asInt();
	if (i == 1) {
		digitalWrite(luefter, HIGH);
		ledluefter.on();
		delay(250);
	}
}


/*********** System Neustart ************/

BLYNK_WRITE(V18) 
{

	int i = param.asInt();
	if (i == 1) {
		ESP.restart();
		delay(250);
	}
}

/************* LED Farbe Manuell *******/

BLYNK_WRITE(V29) 
{
	Blynk.virtualWrite(V29, param.asFloat());
	LEDRot = param.asFloat();
		
}

BLYNK_WRITE(V30) 
{
	Blynk.virtualWrite(V30, param.asFloat());
	LEDBlau = param.asFloat();	
}

BLYNK_WRITE(V31) 
{
	Blynk.virtualWrite(V31, param.asFloat());
	LEDGruen = param.asFloat();	
}

BLYNK_WRITE(V32) 
{
	Blynk.virtualWrite(V32, param.asFloat());
	LEDWeiss = param.asFloat();
}

BLYNK_WRITE(V37 ) 
{	
	Blynk.virtualWrite(V37, param.asFloat());
	Powerledwert_virtuell = param.asFloat();	
}

BLYNK_WRITE(V38) 
{

	int i = param.asInt();
	if (i == 1) {
		for (int i = 0; i < NUMLEDS; i++) {
			strip1.SetPixelColor(i, RgbwColor(LEDRot, LEDGruen, LEDBlau, LEDWeiss));
			//Powerledwert = Powerledwert_virtuell;
			ledcWrite(PowerledKanal, Powerledwert_virtuell);
		}	
	}
}



#endif