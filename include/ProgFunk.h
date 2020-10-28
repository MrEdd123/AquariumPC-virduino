// Funktionen.h

#ifndef _ProgFunk_h
#define _ProgFunk_h

void PowerLEDplus()
{
	/*
	Serial.print("PowerLEDFade : ");
	Serial.println((DurchWait * 50));
	Serial.print("millis : ");
	Serial.println(millis());
	Serial.print("PowerLEDMillis : ");
	Serial.println(PowerLEDMillis);
	Serial.print("millis summe : ");
	Serial.println(millis() - PowerLEDMillis);
	Serial.print("PowerLEDWert : ");
	Serial.println(Powerledwert);
	*/

	if (millis() - PowerLEDMillis > (DurchWait * 50))
	{
		PowerLEDMillis = millis();
		if (Powerledwert < Powerledmax)
		{
			Powerledwert++;
			ledcWrite(PowerledKanal, Powerledwert);
		}
		if (Powerledwert == Powerledmax)
		{
			ledcWrite(PowerledKanal, Powerledwert);
			Durchlauf = 9;
		}
	}
}

void PowerLEDminus()
{
	/*
	Serial.print("PowerLEDFade : ");
	Serial.println((DurchWait * 50));
	Serial.print("millis : ");
	Serial.println(millis());
	Serial.print("PowerLEDMillis : ");
	Serial.println(PowerLEDMillis);
	Serial.print("millis summe : ");
	Serial.println(millis() - PowerLEDMillis);
	Serial.print("PowerLEDWert : ");
	Serial.println(Powerledwert);
	*/

	if (millis() - PowerLEDMillis > (DurchWait * 50))
	{
		PowerLEDMillis = millis();
		if (Powerledwert > 0)
		{
			Powerledwert--;
			ledcWrite(PowerledKanal, Powerledwert);
		}
		if (Powerledwert == 0)
		{
			ledcWrite(PowerledKanal, Powerledwert);
			Durchlauf = 2;
		}	
	}
}

int calculateStep(int prevValue, int endValue)
{
	int step = endValue - prevValue; // What's the overall gap?
	if (step)
	{							// If its non-zero,
		step = StepWert / step; //   divide by 1020
	}
	return step;
}

/* The next function is calculateVal. When the loop value, i,
*  reaches the step size appropriate for one of the
*  colors, it increases or decreases the value of that color by 1.
*  (R, G, and B are each calculated separately.)
*/

int calculateVal(int step, int val, int i)
{

	if ((step) && i % step == 0)
	{ // If step is non-zero and its time to change a value,
		if (step > 0)
		{ //   increment the value if step is positive...
			val += 2;
		}
		else if (step < 0)
		{ //   ...or decrement it if step is negative
			val -= 2;
		}
	}
	// Defensive driving: make sure val stays in the range 0-255
	if (val > 255)
	{
		val = 255;
	}
	else if (val < 0)
	{
		val = 0;
	}
	return val;
}

/* crossFade() converts the percentage colors to a
*  0-255 range, then loops 1020 times, checking to see if
*  the value needs to be updated each time, then writing
*  the color values to the correct pins.
*/

void crossFade(int color[3])
{

	int stepR = calculateStep(prevR, (color[0] * 255) / 100);
	int stepG = calculateStep(prevG, (color[1] * 255) / 100);
	int stepB = calculateStep(prevB, (color[2] * 255) / 100);

	
	if (millis() - CrossLEDMillis > (DurchWait*10))
	{
	
		CrossLEDMillis = millis();

		if (LEDStep <= StepWert)
		{

			LEDStep++;
			if (LEDStep >= (StepWert + 1))
			{
				LEDStep = 0;
				Durchlauf++;
			}

			redVal = calculateVal(stepR, redVal, LEDStep);
			grnVal = calculateVal(stepG, grnVal, LEDStep);
			bluVal = calculateVal(stepB, bluVal, LEDStep);
			
			for (int i = 0; i < NUMLEDS; i++)
			{
				strip1.SetPixelColor(i, RgbColor(redVal, grnVal, bluVal));
				/*
				Serial.print ("R:");
				Serial.println (redVal);
				Serial.print ("G:");
				Serial.println (grnVal);
				Serial.print ("B:");
				Serial.println (bluVal);
				*/
			}


			prevR = redVal; // Update current values for next loop
			prevG = grnVal;
			prevB = bluVal;

			delay(1);
			strip1.Show();

		}

	}

	
	
	
}

void SonneAuf(void)
{
	LichtZustand = 1;
	preferences.putUInt("LichtZu", LichtZustand);
	ledcWrite(BacklightKanalTFT, BacklightwertTag);
	
	switch (Durchlauf)
	{
	case 1:
		//Serial.println("Case1");
		crossFade(SonAu1);
		break;
	case 2:
		//Serial.println("Case2");
		crossFade(SonAu2);
		break;
	case 3:
		//Serial.println("Case3");
		crossFade(SonAu3);
		break;
	case 4:
		//Serial.println("Case4");
		crossFade(SonAu4);
		break;
	case 5:
		//Serial.println("Case5");
		crossFade(SonAu5);
		break;
	case 6:
		//Serial.println("Case6");
		crossFade(SonAu6);
		break;
	case 7:
		//Serial.println("Case7");
		crossFade(SonAu7);
		break;
	case 8:
		//Serial.println("Case8 PowerLED");
		PowerLEDplus();
		break;

	case 9:
		//Serial.println("Case9 Ende");
		Durchlauf = 1;
		SonneIndex = 0;
		//AblaufX = 1;
		
		break;
	}
}

void SonneUn(void)
{
	LichtZustand = 2;
	preferences.putUInt("LichtZu", LichtZustand);
	ledcWrite(BacklightKanalTFT, BacklightwertNacht);

	switch (Durchlauf)
	{

	case 1:
		PowerLEDminus();
		break;
	case 2:
	//Serial.println("SonUn1");
		crossFade(SonUn1);
		break;
	case 3:
	//Serial.println("SonUn2");
		crossFade(SonUn2);
		break;
	case 4:
	//Serial.println("SonUn3");
		crossFade(SonUn3);
		break;
	case 5:
	//Serial.println("SonUn4");
		crossFade(SonUn4);
		break;
	case 6:
	//Serial.println("SonUn5");
		crossFade(SonUn5);
		break;
	case 7:
	//Serial.println("SonUn6");
		crossFade(SonUn6);
		break;
	case 8:
	//Serial.println("SonUn7");
		crossFade(SonUn7);
		break;
	case 9:
		
		for (int i = 0; i < NUMLEDS; i++)
		{
			strip1.SetPixelColor(i, RgbColor(0, 0, 0));	
			strip1.Show();
		}
	
		strip1.SetPixelColor(15, RgbColor(0, 0, 200));
		strip1.SetPixelColor(16, RgbColor(0, 0, 200));

		strip1.Show();
		Durchlauf++;
		break;

	case 10:
		Durchlauf = 1;
		SonneIndex = 0;
		//AblaufY = 159;
		
		break;
	}
}

void SonneMitAn(void)
{
	LichtZustand = 3;
	preferences.putUInt("LichtZu", LichtZustand);
	strip1.SetBrightness(mittagHell);
	delay(1);
	strip1.Show();
	aktHell = mittagHell;
	Powerledwert = mittagHell;
	ledcWrite(PowerledKanal, Powerledwert);
	SonneIndex = 0;
	
}

void SonneMitAus(void)
{
	LichtZustand = 4;
	preferences.putUInt("LichtZu", LichtZustand);
	strip1.SetBrightness(maxHell);
	delay(1);
	strip1.Show();
	aktHell = maxHell;
	Powerledwert = Powerledmax;
	ledcWrite(PowerledKanal, Powerledwert);
	SonneIndex = 0;
	
}

void SonneNaAus(void)
{
	LichtZustand = 5;
	preferences.putUInt("LichtZu", LichtZustand);

	switch (Durchlauf)
	{
	case 1:
		crossFade(Nachtlicht1);
		break;

	case 2:
		Durchlauf = 1;
		SonneIndex = 0;
	
		break;
	}
}

void Heizung(void)
{
	Tempfueh.setWaitForConversion(false); 
	Tempfueh.requestTemperatures();
	Tempfueh.setWaitForConversion(true); 
	IstTemp = Tempfueh.getTempCByIndex(0);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.drawFloat(SollTemp, 1, 27, 47, 2); //SollWert in TFT einblenden
	V[0] = IstTemp;
	
	if (IstTemp > -127)
	{

		if (IstTemp < SollTemp - Hysterese)
		{
			digitalWrite(heizung, HIGH);
			tft.setTextColor(TFT_RED, TFT_BLACK);
			tft.drawFloat(IstTemp, 1, 90, 33, 4);
			V[21] = 1;
			
		}

		if (IstTemp > SollTemp + Hysterese)
		{
			digitalWrite(heizung, LOW);
			tft.setTextColor(TFT_GREEN, TFT_BLACK);
			tft.drawFloat(IstTemp, 1, 90, 33, 4);
			V[21] = 0;
		
		}

		/******************* Luefter Temperatur *******************/
		if (IstTemp >= LuefTemp)
		{
			digitalWrite(luefter, HIGH);
			V[22] = 1;
			
		}
		else
		{
			digitalWrite(luefter, LOW);
			V[22] = 0;
		
		}
	}

	else
		{
			uint8_t err=99;
			tft.setTextColor(TFT_RED, TFT_BLACK);
			tft.drawFloat(err, 1, 90, 33, 4);
		}
}

void Futterautomat(void)
{
	int i = millis() - FutterMillis;
	if ((millis() - FutterMillis) > Futterdauer)
	{
		FutterMillis = millis();

		ledcWrite(FutterKanal, Futtergesch);
		Serial.println(i);
		//if (i > Futterdauer)
		if (i > Futterdauer && i < (Futterdauer + 5 ))
		{
			Serial.print("Stop");
			Serial.println(i);
			ledcWrite(FutterKanal, 0);
			FutterIndex = 0;
		}
	}

}

/********************************************/

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011; // LI, Version, Mode
	packetBuffer[1] = 0;		  // Stratum, or type of clock
	packetBuffer[2] = 6;		  // Polling Interval
	packetBuffer[3] = 0xEC;		  // Peer Clock Precision
								  // 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	Udp.beginPacket(address, 123); //NTP requests are to port 123
	Udp.write(packetBuffer, NTP_PACKET_SIZE);
	Udp.endPacket();
}

// returns the current date/time as UNIX timestamp, incl. timezone, including daylightsaving



// calculates the daylight saving time for middle Europe. Input: Unixtime in UTC (!)
boolean isDayLightSaving(uint32_t local_t)
{
if (month(local_t) < 3 || month(local_t) > 10) return false; // no DSL in Jan, Feb, Nov, Dez
if (month(local_t) > 3 && month(local_t) < 10) return true; // DSL in Apr, May, Jun, Jul, Aug, Sep
// if (month == 3 && (hour + 24 * day) >= (1 + tzHours + 24 * (31 - (5 * year / 4 + 4) % 7)) || month == 10 && (hour + 24 * day) < (1 + tzHours + 24 * (31 - (5 * year / 4 + 1) % 7)));
if (month(local_t) == 3 & (hour(local_t) + 24 * day(local_t)) >= (1 + 24 * (31 - (5 * year(local_t) / 4 + 4) % 7)) || month(local_t) == 10 & (hour(local_t) + 24 * day(local_t)) < (1 + 24 * (31 - (5 * year(local_t) / 4 + 1) % 7)))
return true;
else
return false;
}
uint32_t nowLocal()
{
	uint32_t local_t = now();
	if (isDayLightSaving(local_t))
		local_t += 3600 + timeZone * SECS_PER_HOUR;
	else
		local_t += timeZone * SECS_PER_HOUR;
	return local_t;
}

#endif