// Funktionen.h

#ifndef _ProgFunk_h
#define _ProgFunk_h

void PowerLEDplus()
{
	uint16_t PowerLEDFade;
	PowerLEDFade = DurchWait * 70;

	if (millis() - PowerLEDMillis > PowerLEDFade)
	{
		PowerLEDMillis = millis();
		if (Powerledwert < Powerledmax)
		{
			Powerledwert++;
			ledcWrite(PowerledKanal, Powerledwert);
		}
		if (Powerledwert == Powerledmax)
		{
			Durchlauf = 9;
		}

		/*Serial.print("PowerLED++ ");
			Serial.print(Powerledwert);
			Serial.print("  ");
			Serial.println(Powerledmax);	
			Serial.print(DurchWait);
			Serial.print("  ");
			Serial.println(PowerLEDFade);*/
	}
}

void PowerLEDminus()
{
	uint16_t PowerLEDFade;
	PowerLEDFade = DurchWait * 70;
	if (millis() - PowerLEDMillis > PowerLEDFade)
	{
		PowerLEDMillis = millis();
		if (Powerledwert > 0)
		{
			Powerledwert--;
			ledcWrite(PowerledKanal, Powerledwert);
		}
		if (Powerledwert == 0)
		{
			Durchlauf = 2;
		}
		/*Serial.print("PowerLED-- ");
		Serial.println(Powerledwert);	*/
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

void crossFade(int color[4])
{

	int stepR = calculateStep(prevR, (color[0] * 255) / 100);
	int stepG = calculateStep(prevG, (color[1] * 255) / 100);
	int stepB = calculateStep(prevB, (color[2] * 255) / 100);
	int stepW = calculateStep(prevW, (color[3] * 255) / 100);

	
	if (millis() - CrossLEDMillis > DurchWait)
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
			whiteVal = calculateVal(stepW, whiteVal, LEDStep);

			/*for (int i = 0; i < NUMLEDS; i++)
			{
				strip1.SetPixelColor(i, RgbwColor(redVal, grnVal, bluVal, whiteVal));
			}*/

			for (int i = 0; i < NUMLEDS; i++)
			{
				strip1.SetPixelColor(i, RgbwColor(redVal, 0, bluVal, whiteVal));
			}


			prevR = redVal; // Update current values for next loop
			prevG = grnVal;
			prevB = bluVal;
			prevW = whiteVal;

			strip1.Show();

		}

	}

	
	
	
}

void SonneAuf(void)
{
	ledcWrite(BacklightKanalTFT, BacklightwertTag);
	AblaufI++;

	if (AblaufI >= 100)
	{
		AblaufX++;
		AblaufI = 0;
		tft.drawFastVLine(AblaufX, 116, 11, TFT_WHITE);
	}

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
		Serial.println("Case9 Ende");
		Durchlauf = 1;
		SonneIndex = 0;
		AblaufX = 1;
		LichtZustand = 1;
		preferences.putUInt("LichtZu", LichtZustand);

		break;
	}
}

void SonneUn(void)
{
	ledcWrite(BacklightKanalTFT, BacklightwertNacht);
	AblaufI++;

	if (AblaufI >= 102)
	{
		AblaufY--;
		AblaufI = 0;
		tft.drawFastVLine(AblaufY, 116, 11, TFT_BLACK);
	}

	switch (Durchlauf)
	{

	case 1:
		PowerLEDminus();
		break;
	case 2:
		crossFade(SonUn1);
		break;
	case 3:
		crossFade(SonUn2);
		break;
	case 4:
		crossFade(SonUn3);
		break;
	case 5:
		crossFade(SonUn4);
		break;
	case 6:
		crossFade(SonUn5);
		break;
	case 7:
		crossFade(SonUn6);
		break;
	case 8:
		crossFade(SonUn7);
		break;
	case 9:
		for (int i = 0; i < 10; i++)
		{

			strip1.SetPixelColor(i, RgbwColor(0, 0, 0, 0));
			delay(10);
			strip1.Show();
		}

		for (int i = 166; i > 100; i--)
		{

			strip1.SetPixelColor(i, RgbwColor(0, 0, 0, 0));
			delay(10);
			strip1.Show();
		}

		for (int i = 10; i < 100; i++)
		{

			strip1.SetPixelColor(i, RgbwColor(0, 0, 200, 0));
			delay(10);
			strip1.Show();
		}
		Durchlauf++;
		break;

	case 10:
		Durchlauf = 1;
		SonneIndex = 0;
		AblaufY = 159;
		LichtZustand = 2;
		preferences.putUInt("LichtZu", LichtZustand);
		break;
	}
}

void SonneMitAn(void)
{

	strip1.SetBrightness(mittagHell);
	strip1.Show();
	aktHell = mittagHell;
	//PowerLEDminus();

	/*for (int i = Powerledmin; Powerledwert > i; Powerledwert--)
	{
		ledcWrite(PowerledKanal, Powerledwert);
	}*/
	Powerledwert = Powerledmin;
	ledcWrite(PowerledKanal, Powerledwert);
	SonneIndex = 0;
	LichtZustand = 3;
	preferences.putUInt("LichtZu", LichtZustand);
}

void SonneMitAus(void)
{

	strip1.SetBrightness(maxHell);
	strip1.Show();
	aktHell = maxHell;
	//PowerLEDplus();
	/*for (int i = Powerledmax; Powerledwert < i; Powerledwert++) 
	{
		ledcWrite(PowerledKanal, Powerledwert);
	}*/
	Powerledwert = Powerledmax;
	ledcWrite(PowerledKanal, Powerledwert);
	SonneIndex = 0;
	LichtZustand = 4;
	preferences.putUInt("LichtZu", LichtZustand);
}

void SonneNaAus(void)
{
	switch (Durchlauf)
	{
	case 1:
		crossFade(Nachtlicht1);
		break;

	case 2:
		Durchlauf = 1;
		SonneIndex = 0;
		LichtZustand = 5;
		preferences.putUInt("LichtZu", LichtZustand);
		break;
	}
}

void Heizung(void)
{
	Tempfueh.requestTemperatures();
	IstTemp = Tempfueh.getTempC();
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