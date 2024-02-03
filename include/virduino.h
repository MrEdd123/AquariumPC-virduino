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
		SollTemp = V[1];
	}

	if (V[2] != LuefTemp)
	{
		LuefTemp = V[2];
		preferences.putUInt("LueT", LuefTemp);
		Heizung();
		LuefTemp = V[2];
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
		V3SonneAuf = V[3];
		if(debug) 
		{
			Serial.println("Sonne Auf");
			Serial.println(SoAuStd);
			Serial.println(SoAuMin);
		}
		//V3Zeit = V3Zeit - SoAuMin*(60*1000);
    	//uint16_t s = x / 1000;
    	//x = x - s*1000;
		SunTimer();
	
	}

	if (V[4] != V4MittagAN)
	{
		V4MittagAN = V[4];
		preferences.putFloat("V4Zeit", V4MittagAN);

		SoMiAnStd = V4MittagAN / (60*60*1000);
		V4MittagAN = V4MittagAN - SoMiAnStd*(60*60*1000);
		SoMiAnMin = V4MittagAN / (60*1000);
	

		preferences.putUInt("MiAnLS", SoMiAnStd);
		preferences.putUInt("MiAnLM", SoMiAnMin);
		V4MittagAN = V[4];
		if(debug) 
		{
			Serial.println("Mittag Timer AN");
			Serial.println(SoMiAnStd);
			Serial.println(SoMiAnMin);
		}
		SunTimer();
	}

	if (V[5] != V5MittagAUS)
	{	
		V5MittagAUS = V[5];
		preferences.putFloat("V5Zeit", V5MittagAUS);

		SoMiAusStd = V5MittagAUS / (60*60*1000);
		V5MittagAUS = V5MittagAUS - SoMiAusStd*(60*60*1000);
		SoMiAusMin = V5MittagAUS / (60*1000);

		preferences.putUInt("MiAuLS", SoMiAusStd);
		preferences.putUInt("MiAuLM", SoMiAusMin);
		V5MittagAUS = V[5];
		if (debug) 
		{
			Serial.println("Mittag Timer AUS");
			Serial.println(SoMiAusStd);
			Serial.println(SoMiAusMin);
		}
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
		V6SonneUnt = V[6];
		if(debug) 
		{
			Serial.println("Sonne Unt");
			Serial.println(SoUnStd);
			Serial.println(SoUnMin);
		}
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
		V7SonneNac = V[7];
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
		V8CO2AN = V[8];
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
		V9CO2AUS = V[9];
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
		V10Futter = V[10];
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
		if (debug)
		{
		Serial.print("Mittag Hell: ");
		Serial.println(mittagHell);
		}
		mittagHell = V[13];
		preferences.putUInt("MitH", mittagHell);
	} 

	if (V[14] != Powerledmax)
	{
		Powerledmax = V[14];
		preferences.putUInt ("PowH", Powerledmax);
		Powerledwert = Powerledmax;
		
	}

	if (V[15] != BacklightwertTag)
	{
		
		ledcWrite(BacklightKanalTFT, BacklightwertTag);
		BacklightwertTag = V[15];
		preferences.putUInt("BackLT", BacklightwertTag);
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
		Futterdauerhelp = V[18];
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

	if (V[22] == 1)		//Powerled Helligkeit setzten
	{
		Powerledwert = V[21];

		ledcWrite(PowerledKanal, Powerledwert);	
		Powerledmax = Powerledwert;
		
	}

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

	if (V[27] == 1)		//Sonnen Mittag AN
	{
		Serial.println("Mittag AN Manu");
		SonneIndex = 3;
		V[27] = 0;
	}

	if (V[28] == 1)		//Sonnen Mittag AUS
	{
		Serial.println("Mittag AUS Manu");
		SonneIndex = 4;
		V[28] = 0;
	}

	if (V[29] == 1)		//Futterautomat
	{
		FutterIndex = 1;
		V[29] = 0;
	}

}
#endif