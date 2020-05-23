// tft.h

#ifndef _TFT_h
#define _TFT_h


void TFT_Layout() 
{

	/****** Display initalisieren mit Layout*****/

	/******** Leiste Oben ***********/
	tft.init();
	tft.setRotation(TFTRotation);
	tft.fillScreen(TFT_BLACK);
	tft.drawLine(0, 20, 160, 20, TFT_GREEN);
	tft.setTextColor(TFT_WHITE);
	tft.drawBitmap(140, 0, wlan, 20, 20, TFT_RED);

	tft.setTextColor(TFT_RED);
	tft.drawString("0", 3, 2, 2);
	tft.drawString("0", 12, 2, 2);
	tft.drawString(":", 21, 2, 2);
	tft.drawString("0", 25, 2, 2);
	tft.drawString("0", 34, 2, 2);
	tft.drawString(":", 43, 2, 2);
	tft.drawString("0", 48, 2, 2);
	tft.drawString("0", 57, 2, 2);

	/******* Temperatur Fenster ***/
	tft.drawRect(20, 25, 60, 20, TFT_WHITE);
	tft.drawRect(80, 25, 80, 40, TFT_WHITE);
	tft.drawRect(20, 45, 60, 20, TFT_WHITE);

	tft.drawBitmap(0, 25, temp, 21, 40, TFT_WHITE);
	tft.setTextColor(TFT_WHITE);
	tft.drawString("Soll", 37, 31, 1);
	tft.setCursor(63, 50);
	tft.print(char(247));
	tft.print("C");
	tft.setCursor(143, 30);
	tft.print(char(247));
	tft.print("C");

	/******** CO2 Fenster ***********/
	tft.drawBitmap(0, 67, co2s, 21, 22, TFT_WHITE);
	tft.drawRect(20, 67, 60, 22, TFT_WHITE);


	/******** Sonne Fenster *********/
	tft.drawBitmap(0, 90, sonne, 21, 22, TFT_WHITE);
	tft.drawRect(20, 90, 60, 22, TFT_WHITE);

	/******** Futterautomat Fenster *********/
	tft.drawBitmap(80, 67, futter, 21, 22, TFT_WHITE);
	tft.drawRect(100, 67, 60, 22, TFT_WHITE);

	/******** Ablauf Fenster ********/

	tft.drawRect(0, 115, 160, 13, TFT_WHITE);

	tft.drawBitmap(140, 0, wlan, 20, 20, TFT_RED);

}

void digitalClockDisplay() 
{

	uint32_t local_t = nowLocal();

	tft.setTextColor(TFT_GREEN, TFT_BLACK);
	if (hour(local_t) < 10) {
		tft.drawString("0", 3, 2, 2);
		tft.drawNumber(hour(local_t), 12, 2, 2);
	}
	else
		tft.drawNumber(hour(local_t), 3, 2, 2);

	tft.drawString(":", 21, 2, 2);

	if (minute(local_t) < 10) {
		tft.drawString("0", 25, 2, 2);
		tft.drawNumber(minute(local_t), 34, 2, 2);
	}
	else
		tft.drawNumber(minute(local_t), 25, 2, 2);

	tft.drawString(":", 43, 2, 2);

	if (second(local_t) < 10) {
		tft.drawString("0", 48, 2, 2);
		tft.drawNumber(second(), 57, 2, 2);
	}
	else
		tft.drawNumber(second(local_t), 48, 2, 2);

}

void SunTimer() 
{
	tft.setTextSize(1);
	tft.setCursor(24, 92);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.print("Ein");
	tft.setCursor(48, 92);
	if (SoAuStd < 10)
		tft.print("0");
	tft.print(SoAuStd);
	tft.print(":");
	if (SoAuMin < 10)
		tft.print("0");
	tft.print(SoAuMin);

	tft.setCursor(24, 102);
	tft.print("Aus");
	tft.setCursor(48, 102);
	if (SoUnStd < 10)
		tft.print("0");
	tft.print(SoUnStd);
	tft.print(":");
	if (SoUnMin < 10)
		tft.print("0");
	tft.print(SoUnMin);
}

void CO2Timer() 
{
	tft.setTextSize(1);
	tft.setCursor(24, 70);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.print("Ein");
	tft.setCursor(48, 70);
	if (CO2AnStd < 10)
		tft.print("0");
	tft.print(CO2AnStd);
	tft.print(":");
	if (CO2AnMin < 10)
		tft.print("0");
	tft.print(CO2AnMin);

	tft.setCursor(24, 80);
	tft.print("Aus");
	tft.setCursor(48, 80);
	if (CO2AusStd < 10)
		tft.print("0");
	tft.print(CO2AusStd);
	tft.print(":");
	if (CO2AusMin < 10)
		tft.print("0");
	tft.print(CO2AusMin);
}

void FutterTimer() 
{

	tft.setTextSize(1);
	tft.setTextColor(TFT_WHITE, TFT_BLACK);
	tft.setCursor(120, 74);
	if (FutterStd < 10)
		tft.print("0");
	tft.print(FutterStd);
	tft.print(":");
	if (FutterMin < 10)
		tft.print("0");
	tft.print(FutterMin);
}


void WIFI_TFT() 
{
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		tft.drawBitmap(140, 0, wlan, 20, 20, TFT_RED);
	}
	tft.drawBitmap(140, 0, wlan, 20, 20, TFT_GREEN);

	while (WiFi.status() == WL_CONNECTION_LOST) {

		tft.drawBitmap(140, 0, wlan, 20, 20, TFT_RED);
		WiFi.reconnect();
	}
}


#endif