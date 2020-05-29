// Funktionen.h

#ifndef _virduino_h
#define _virduino_h

/*********** Virtuino Werte lesen *****************/

void virduino()
{
	if (V[1] != SollTemp)
	{
		SollTemp = V[1];
		preferences.putFloat("SollT", SollTemp);
		Heizung();

	}

	if (V[2] != LuefTemp)
	{
		LuefTemp = V[2];
		preferences.putUInt("LueT", LuefTemp);
		Heizung();
	}


	if (V[3] != V3SonneAuf)
	{

		V3SonneAuf = V[3];
		preferences.putFloat("V3Zeit", V3SonneAuf);

		SoAuStd = V3SonneAuf  / (60*60*1000);
   		V3SonneAuf  = V3SonneAuf  - SoAuStd*(60*60*1000);
    	SoAuMin = V3SonneAuf  / (60*1000);

		preferences.putUInt("StartS", SoAuStd);
		preferences.putUInt("StartM", SoAuMin);
	
    	//V3Zeit = V3Zeit - SoAuMin*(60*1000);
    	//uint16_t s = x / 1000;
    	//x = x - s*1000;
		SunTimer();
	
	}

	if (V[4] != V4MittagAN)
	{
		V4MittagAN = V[4];
		preferences.putFloat("V4Zeit", V4MittagAN);

		SoMiAnStd = V4MittagAN / (60*60+100);
		V4MittagAN = V4MittagAN - SoMiAnStd*(60*6*1000);
		SoMiAnMin = V4MittagAN / (60*1000);

		preferences.putInt("MiAnLS", SoMiAnStd);
		preferences.putInt("MiAnLM", SoMiAnMin);

		SunTimer();
	}

	if (V[5] != V5MittagAUS)
	{
		V5MittagAUS = V[5];
		preferences.putFloat("V5Zeit", V5MittagAUS);

		SoMiAusStd = V5MittagAUS / (60*60+100);
		V5MittagAUS = V5MittagAUS - SoMiAusStd*(60*6*1000);
		SoMiAusMin = V5MittagAUS / (60*1000);

		preferences.putInt("MiAuLS", SoMiAusStd);
		preferences.putInt("MiAuLM", SoMiAusMin);

		SunTimer();
	}



	if (V[6] != V6SonneUnt)
	{

		V6SonneUnt = V[6];
		preferences.putFloat("V6Zeit", V6SonneUnt);

		SoUnStd = V6SonneUnt / (60*60*1000);
    	V6SonneUnt = V6SonneUnt - SoUnStd*(60*60*1000);
    	SoUnMin = V6SonneUnt / (60*1000);

		preferences.putUInt("StopS", SoUnStd);
		preferences.putUInt("StopM", SoUnMin);
	
    	//V3Zeit = V3Zeit - SoAuMin*(60*1000);
   	 	//uint16_t s = x / 1000;
   	 	//x = x - s*1000;
		SunTimer();
	
	}

	if (V[7] != V7SonneNac)
	{

		V7SonneNac = V[7];
		preferences.putFloat("V7Zeit", V7SonneNac);

		SoNaStd = V7SonneNac / (60*60*1000);
    	V7SonneNac = V7SonneNac - SoNaStd*(60*60*1000);
    	SoNaMin = V7SonneNac / (60*1000);

		preferences.putUInt("NaLS", SoNaStd);
		preferences.putUInt("NaLM", SoNaMin);
	
    	//V3Zeit = V3Zeit - SoAuMin*(60*1000);
    	//uint16_t s = x / 1000;
    	//x = x - s*1000;
		SunTimer();
	
	}
	
	
	if (V[8] != V8CO2AN)
	{
		V8CO2AN = V[8];
		preferences.putFloat("V8CO2AN", V8CO2AN);

		CO2AnStd = V8CO2AN / (60*60*1000);
		V8CO2AN = V8CO2AN - CO2AnStd*(60*60*1000);
		CO2AnMin = V8CO2AN / (60*1000);

		preferences.putUInt("COSS", CO2AnStd);
		preferences.putUInt("COSM", CO2AnMin);

		CO2Timer();
	}

	if (V[9] != V9CO2AUS)
	{
		V9CO2AUS = V[9];
		preferences.putFloat("V9CO2AUS", V9CO2AUS);

		CO2AusStd = V9CO2AUS / (60*60*1000);
		V9CO2AUS = V9CO2AUS - CO2AusStd*(60*60*1000);
		CO2AusMin = V9CO2AUS / (60*1000);

		preferences.putUInt("COAS", CO2AusStd);
		preferences.putUInt("COAM", CO2AusMin);

		CO2Timer();
	}

	if (V[10] != V10Futter)
	{
		V10Futter = V[10];
		preferences.putFloat("V10Futt", V10Futter);

		FutterStd = V10Futter / (60*60*1000);
		V10Futter = V10Futter - FutterStd*(60*60*1000);
		FutterMin = V10Futter / (60*1000);

		preferences.putUInt("FuttS", FutterStd);
		preferences.putUInt("FuttM", FutterMin);

		FutterTimer();
	}

	if (V[11] != DurchWait)
	{
		DurchWait = V[11];
		preferences.putUInt ("Wait", DurchWait);
	}

	if  (V[12] != maxHell)
	{
		maxHell = V[12];
		preferences.putUInt("MaxH", maxHell);
	} 

	if  (V[13] != mittagHell)
	{
		mittagHell = V[13];
		preferences.putUInt("MitH", mittagHell);
	} 

	if (V[14] != Powerledmax)
	{
		Powerledmax = V[14];
		preferences.putUInt ("PowH", Powerledmax);
	}

	if (V[15] != BacklightwertTag)
	{
		BacklightwertTag = V[15];
		preferences.putUInt("BackLT", BacklightwertTag);
		ledcWrite(BacklightKanalTFT, BacklightwertTag);
	}

	if (V[16] != BacklightwertNacht)
	{
		BacklightwertNacht = V[16];
		preferences.putUInt("BackLN", BacklightwertNacht);
	}

	/*if (V[17] == 1)
	{
		TFTRotation = 3;
		delay(250);
		preferences.putInt("TFTR", TFTRotation);	

		TFT_Layout();
		WIFI_TFT();
		CO2Timer();
		SunTimer();
		FutterTimer();
	
	}
	
	if (V[17] == 0)
	{
		TFTRotation = 1;

		preferences.putInt("TFTR", TFTRotation);	
		TFT_Layout();
		WIFI_TFT();
		CO2Timer();
		SunTimer();
		FutterTimer();
	}
	*/

	if (V[18] != Futterdauerhelp)
	{
		Futterdauerhelp = V[18];
		Futterdauer = (Futterdauerhelp * 500);
		preferences.putUInt("FutD", Futterdauer);
		preferences.putUInt("FutDHelp", Futterdauerhelp);
	}

	if (V[19] != Futtergesch)
	{
		Futtergesch = V[19];
		preferences.putUInt("FutG", Futtergesch);
	}

	if (V[20] != Hysterese)
	{
		Hysterese = V[20];
		preferences.putFloat("Hyst", Hysterese);
	}

	/************** Manuelle Funktionen *****************/

	if (V[30] == 1)		//Sonnenaufgang
	{
		Serial.println("Sonnenauf Manu");
		SonneIndex = 1;
		V[30] = 0;
	}

	if (V[31] == 1)		//Sonnenuntergang
	{
		Serial.println("Sonnenunter Manu");
		SonneIndex = 2;
		V[31] = 0;
	}

}

#endif