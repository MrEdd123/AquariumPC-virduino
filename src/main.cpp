

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <NeoPixelAnimator.h>
#include "bitmaps.h"
#include <ESPmDNS.h>
#include <Time.h>
#include <SPI.h>
#include <NeoPixelBrightnessBus.h>
#include <OneWire.h>
#include <DS18B20.h>
#include <Preferences.h>
#include <Ticker.h>
#include <WiFi.h>



//--- SETTINGS ------------------------------------------------
const char* ssid = "Andre+Janina";            // WIFI network SSID
const char* password = "sommer12";            // WIFI network PASSWORD
WiFiServer server(8000);                      // Server port
IPAddress ip(192, 168, 178, 150);            // where 150 is the desired IP Address. The first three numbers must be the same as the router IP
IPAddress gateway(192, 168, 178, 1);         // set gateway to match your network. Replace with your router IP
//---

//---VirtuinoCM  Library settings --------------
#include "VirtuinoCM.h"
VirtuinoCM virtuino;               
#define V_memory_count 32          // the size of V memory. You can change it to a number <=255)
float V[V_memory_count];           // This array is synchronized with Virtuino V memory. You can change the type to int, long etc.
//---


boolean debug = true;              // set this variable to false on the finale code to decrease the request time.

//-------------------------------------------------------------



/*********** EEPROM Speichern ********************/

Preferences preferences; 


/***********  NeoPixel Einstellungen   ***********/

#define PIN_STRIPE1			13
#define NUMLEDS				166

NeoPixelBrightnessBus<NeoGrbwFeature, NeoEsp32Rmt0800KbpsMethod> strip1(NUMLEDS, PIN_STRIPE1);

/************ TFT Einstellungen ***************/

TFT_eSPI tft = TFT_eSPI();
#define MAX_GEN_COUNT 500

/************* One Wire (Tempfühler) **********/

#define ONE_WIRE_BUS			26				// Anschlusspin für OneWire			
OneWire oneWire(ONE_WIRE_BUS);
DS18B20 Tempfueh(&oneWire);

/******* Variablen *******************************/

uint8_t TFTRotation = 3;

uint8_t SoAuStd = 10;
uint8_t SoAuMin = 00;
uint8_t SoAuZei;
uint8_t SoUnStd = 20;
uint8_t SoUnMin = 00;
uint8_t SoUnZei;
uint8_t SoNaStd = 22;
uint8_t SoNaMin = 00;

uint8_t SoMiAnStd = 13;
uint8_t SoMiAnMin = 00;
uint8_t SoMiAusStd = 15;
uint8_t SoMiAusMin = 00;

uint8_t FutterStd = 00;
uint8_t FutterMin = 00;

uint8_t maxHell = 250;
uint8_t minHell = 0;
uint8_t mittagHell = 5;
uint8_t aktHell = 200;

float_t SollTemp = 25.0;
float_t IstTemp;
float_t LuefTemp = 30.0;
float_t Hysterese = 0.30;

uint8_t CO2AnStd = 11;
uint8_t CO2AnMin = 00;
uint8_t CO2AusStd = 18;
uint8_t CO2AusMin = 00;

uint16_t Zeit;

uint8_t AblaufX = 0;
uint8_t AblaufY = 159;
uint8_t AblaufI = 0;

uint8_t BacklightPin = 22;
uint16_t BacklightFrequenz = 500;
uint8_t BacklightKanalTFT = 2;
uint8_t BacklightBit = 8;
uint8_t BacklightwertTag = 100;
uint8_t BacklightwertNacht = 100;

uint8_t FutterPin = 14;
uint16_t Futterfreq = 250;
uint8_t FutterKanal = 0;
uint8_t FutterRes = 8;
uint16_t Futtergesch = 180;
uint16_t Futterdauer = 2000;

uint8_t PowerledPin = 16;
uint16_t Powerledfreq = 2000;
uint8_t PowerledKanal = 1;
uint8_t PowerledBit = 8;
uint8_t Powerledwert = 0;
uint8_t Powerledwert_virtuell = 0;
uint8_t Powerledmax = 250;
uint8_t Powerledmin = 0;

uint8_t LEDRot;
uint8_t LEDGruen;
uint8_t LEDBlau;
uint8_t LEDWeiss;
float V3SonneAuf;
float V4MittagAN;
float V5MittagAUS;
float V6SonneUnt;
float V7SonneNac;
float V8CO2AN;
float V9CO2AUS;
float V10Futter;
uint8_t V11Durchwait;
uint8_t V12MaxHell;
uint8_t V13MiHell;
uint8_t V14PowerHell;
uint8_t V15BackLT;
uint8_t V16BackLN;
uint8_t V17TFTRot;
uint8_t V18FuDau;
uint8_t V19FuGes;


unsigned long PowerLEDMillis = 0;
unsigned long FutterMillis = 0;

/**************** Pin Belegung *******************/

#define heizung				12      //Pin Relais Heizung
#define luefter				25      //Pin Lüfter
#define co2					27		//Pin Relais CO2 Abschaltung

/**************** NeoPixel Init ******************/
// Sonnenaufgang Color Array
//				{ R, G , B, W }
int SonAu1[4] = { 30,0,0,0 };
int SonAu2[4] = { 150,5,0,0 };
int SonAu3[4] = { 157,13,0,0 };
int SonAu4[4] = { 163,21,1,0 };
int SonAu5[4] = { 200,30,60,0 };
int SonAu6[4] = { 240,50,100,80 };
int SonAu7[4] = { 230,60,240,100 };

// Sonnenuntergang Color Array
//				{ R, G, B, W }
int SonUn1[4] = { 250,60,100,40 };
int SonUn2[4] = { 240,50,60,0 };
int SonUn3[4] = { 200,30,40,0 };
int SonUn4[4] = { 150,20,0,0 };
int SonUn5[4] = { 50, 14,1, 0 };
int SonUn6[4] = { 0, 0, 6, 0 };
int SonUn7[4] = { 0, 0, 4, 0 };

//Nachtlicht AUS Color Array
//					 { R, G, B, W }
int Nachtlicht1[4] = { 0, 0, 0, 0 };

// Set initial color
uint8_t redVal = 0;
uint8_t grnVal = 0;
uint8_t bluVal = 0;
uint8_t whiteVal = 0;

uint16_t DurchWait = 180;
unsigned long CrossLEDMillis = 0;

uint8_t prevR = redVal;
uint8_t prevG = grnVal;
uint8_t prevB = bluVal;
uint8_t prevW = whiteVal;
uint16_t LEDStep = 0;
uint8_t Durchlauf = 1;
uint8_t SonneIndex = 0;
uint8_t FutterIndex = 0;

uint16_t StepWert = 256;


/***** NTP Server abrufen für Local Time ********/

static const char ntpServerName[] = "de.pool.ntp.org";
const int timeZone = 1;

const uint32_t syncIntervalMax = 300; // could be the same value like the syncInterval in time.cpp | 300
const uint32_t syncIntervalAfterFail = 30; // if sync with NTP fails, retry earlier | 60
const uint32_t ntpWaitMax = 1500; // maximum time in ms to wait for an answer of NTP Server, most used value 1500 I prefere below one second: 900
uint32_t ntpWaitActual = ntpWaitMax; // optimized/reduced wait time, start with maximum.

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

const int NTP_PACKET_SIZE = 48;		// NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

/************** Funktionen *********************/

#include <ProgFunk.h>
#include <tft.h>
#include <timer.h>

/************************** NTP code ********************************************/

time_t getNtpTime()
{
	IPAddress ntpServerIP; // NTP server's ip address

	while (Udp.parsePacket() > 0); // discard any previously received packets
	Serial.println("Transmit NTP Request");
	// get a random server from the pool
	WiFi.hostByName(ntpServerName, ntpServerIP);
	Serial.print(ntpServerName);
	Serial.print(": ");
	Serial.println(ntpServerIP);
	sendNTPpacket(ntpServerIP);
	uint32_t beginWait = millis();
	while (millis() - beginWait < 1500) {
		int size = Udp.parsePacket();
		if (size >= NTP_PACKET_SIZE) {
			Serial.println("Receive NTP Response");
			Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
			unsigned long secsSince1900;
			// convert four bytes starting at location 40 to a long integer
			secsSince1900 = (unsigned long)packetBuffer[40] << 24;
			secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
			secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
			secsSince1900 |= (unsigned long)packetBuffer[43];
			setSyncInterval(syncIntervalMax);
			return secsSince1900 - 2208988800UL; // Important: Systemtime = UTC!!!

		}
	}
	Serial.println("No NTP Response :-(");
	tft.setTextColor(TFT_BLACK);
	tft.drawString("           ", 3, 2, 2);
	tft.setTextColor(TFT_RED);
	tft.drawString("8", 3, 2, 2);
	tft.drawString("8", 12, 2, 2);
	tft.drawString(":", 21, 2, 2);
	tft.drawString("8", 25, 2, 2);
	tft.drawString("8", 34, 2, 2);
	tft.drawString(":", 43, 2, 2);
	tft.drawString("8", 48, 2, 2);
	tft.drawString("8", 57, 2, 2);

	if (ntpWaitActual + 200 < ntpWaitMax) ntpWaitActual += 200; // expand wait time if necessary - I'm pretty aware of that this value will hardly reach ntpWaitMax with this simple condition.
	setSyncInterval(syncIntervalAfterFail); // in both cases: no NTP response or no DNS lookup

	return 0; // return 0 if unable to get the time
}

/************ Timer setzen fuer Funktion *********/

Ticker tickerPro(ProgrammTimer, 1000);
Ticker tickerHei(Heizung, 5000);

/**************************************************/


//============================================================== onCommandReceived
//==============================================================
/* This function is called every time Virtuino app sends a request to server to change a Pin value
 * The 'variableType' can be a character like V, T, O  V=Virtual pin  T=Text Pin    O=PWM Pin 
 * The 'variableIndex' is the pin number index of Virtuino app
 * The 'valueAsText' is the value that has sent from the app   */
 void onReceived(char variableType, uint8_t variableIndex, String valueAsText)
 {     
    if (variableType=='V'){
        float value = valueAsText.toFloat();        // convert the value to float. The valueAsText have to be numerical
        if (variableIndex<V_memory_count) V[variableIndex]=value;              // copy the received value to arduino V memory array
    }
}

//==============================================================
/* This function is called every time Virtuino app requests to read a pin value*/
String onRequested(char variableType, uint8_t variableIndex)
{     
    if (variableType=='V') {
    if (variableIndex<V_memory_count) return  String(V[variableIndex]);   // return the value of the arduino V memory array
  }
  return "";
}


 //==============================================================
  void virtuinoRun()
  {
   WiFiClient client = server.available();
   if (!client) return;
   if (debug) Serial.println("Connected");
   unsigned long timeout = millis() + 3000;
   while (!client.available() && millis() < timeout) delay(1);
   if (millis() > timeout) {
    Serial.println("timeout");
    client.flush();
    client.stop();
    return;
  }
    virtuino.readBuffer="";    // clear Virtuino input buffer. The inputBuffer stores the incoming characters
      while (client.available()>0) {        
        char c = client.read();         // read the incoming data
        virtuino.readBuffer+=c;         // add the incoming character to Virtuino input buffer
        if (debug) Serial.write(c);
      }
     client.flush();
     if (debug) Serial.println("\nReceived data: "+virtuino.readBuffer);
     String* response= virtuino.getResponse();    // get the text that has to be sent to Virtuino as reply. The library will check the inptuBuffer and it will create the response text
     if (debug) Serial.println("Response : "+*response);
     client.print(*response);
     client.flush();
     delay(10);
     client.stop(); 
    if (debug) Serial.println("Disconnected");
}


 //============================================================== vDelay
  void vDelay(int delayInMillis){long t=millis()+delayInMillis;while (millis()<t) virtuinoRun();}



void setup() 
{

	  if (debug) 
	  {
    	Serial.begin(115200);
    	while (!Serial) continue;
	  }	

	/********** Virtuino settings *****************/
 	virtuino.begin(onReceived,onRequested,512);  //Start Virtuino. Set the buffer to 512. With this buffer Virtuino can control about 50 pins (1 command >= 9bytes) The T(text) commands with 20 characters need 20+6 bytes
  	virtuino.key="4593";                       //This is the Virtuino password. Only requests the start with this key are accepted from the library

     Serial.println("Connecting to "+String(ssid));
   // If you don't want to config IP manually disable the next two lines
   IPAddress subnet(255, 255, 255, 0);        // set subnet mask to match your network
 //WiFi.config(ip, gateway, subnet);          // If you don't want to config IP manually disable this line
 // WiFi.mode(WIFI_STA);                       // Config module as station only.
  WiFi.begin(ssid, password);
  delay(500);
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
    }
   Serial.println("");
   Serial.println("WiFi connected");
   Serial.println(WiFi.localIP());

  server.begin();

	/************ TFT Layout setzen ***************/  

	TFT_Layout();

	/************** EEPROM auslesen ***************/

	preferences.begin("aq", false);                         //Ordner az anlegen/verwenden

  	//preferences.clear();                                  //Alle "Dateien" unter dem Ordner aq löschen

 	//preferences.remove("aq");                          	// EEPROM Datei "StartS" löschen
	SoAuStd = preferences.getUInt("StartS", 0); 			
	SoAuMin = preferences.getUInt("StartM", 0);
	V3SonneAuf = preferences.getFloat("V3Zeit");
	SoMiAnStd = preferences.getUInt("MiAnLS", 0);
	SoMiAnMin = preferences.getUInt("MIAnLM", 0);
	V4MittagAN = preferences.getFloat("V4Zeit");
	SoMiAusStd = preferences.getUInt("MiAuLS", 0);
	SoMiAusMin = preferences.getUInt("MIAuLM", 0);
	V5MittagAUS = preferences.getFloat("V5Zeit");
	SoUnStd = preferences.getUInt("StopS", 0);
	SoUnMin = preferences.getUInt("StopM", 0);
	V6SonneUnt = preferences.getFloat("V6Zeit");
	SoNaStd = preferences.getUInt("NaLS", 0);
	SoNaMin = preferences.getUInt("NaLM", 0);
	V7SonneNac = preferences.getFloat("V7Zeit");
	CO2AnStd = preferences.getUInt("COSS", 0);
	CO2AnMin = preferences.getUInt("COSM", 0);
	V8CO2AN = preferences.getFloat("V8CO2AN");
	CO2AusStd = preferences.getUInt("COAS", 0);
	CO2AusMin = preferences.getUInt("COAM", 0);
	V9CO2AUS = preferences.getFloat("V9CO2AUS");
	FutterStd = preferences.getUInt("FuttS", 0);
	FutterMin = preferences.getUInt("FuttM", 0);
	V10Futter = preferences.getFloat("V10Futt");
	DurchWait = preferences.getUInt("Wait", 0);
	V11Durchwait = preferences.getUInt("V11Wait");
	maxHell = preferences.getUInt("MaxH", 0);
	V12MaxHell = preferences.getUInt("V12MaxHell", 100);
	mittagHell = preferences.getUInt("MitH", 0);
	V13MiHell = preferences.getUInt("V13MitH", 100);
	Powerledmax = preferences.getUInt("PowH", 0);
	V14PowerHell = preferences.getUInt("V14PoHell", 100);
	BacklightwertTag = preferences.getUInt("BackLT", 0);
	V15BackLT = preferences.getUInt("V15BackLT", 100);
	BacklightwertNacht = preferences.getUInt("BackLN", 100);
	V16BackLN = preferences.getUInt("V16BackLN", 5);
	TFTRotation = preferences.getInt("TFTR", 0);
	V17TFTRot = preferences.getUInt("V17TFTR", 1);
	Futterdauer = preferences.getUInt("FutD", 0);
	V18FuDau = preferences.getUInt("V18FuDau", 2);
	Futtergesch = preferences.getUInt("FutG", 0);
	V19FuGes = preferences.getUInt("V19FuGes", 150);
	SollTemp = preferences.getFloat("SollT");
	LuefTemp = preferences.getUInt("LueT", 0);
	Hysterese = preferences.getFloat("Hyst", 0);
	
	
	CO2Timer();
	SunTimer();
	FutterTimer();

	/**************** Virduino Werte zuordnen *******/

	V[1] = SollTemp;
	V[2] = LuefTemp;
	V[3] = V3SonneAuf;
	V[4] = V4MittagAN;
	V[5] = V5MittagAUS;
	V[6] = V6SonneUnt;
	V[7] = V7SonneNac;
	V[8] = V8CO2AN;
	V[9] = V9CO2AUS;
	V[10] = V10Futter;
	V[11] = DurchWait;
	V[12] =	maxHell;
	V[13] = minHell;
	V[14] = Powerledmax;
	V[15] = BacklightwertTag;
	V[16] = BacklightwertNacht;
	V[17] = TFTRotation;
	V[18] = Futterdauer;
	V[19] = Futtergesch;
	V[20] = Hysterese;
	
	
	/*********** GPIO´s definieren ****************/

	pinMode(heizung, OUTPUT);
	pinMode(luefter, OUTPUT);
	pinMode(co2, OUTPUT);

	/**** Alarm Timer starten für Funktionen ******/ 
	
	tickerPro.start();
	tickerHei.start();

	/*********** Neopixel Starten ***************/

	strip1.Begin();
	strip1.ClearTo(0);
	strip1.SetPixelColor(10, RgbwColor(0, 0, 200, 0));
	strip1.SetBrightness(250);
	aktHell = maxHell;
	strip1.Show();

	/************ Tempfuehler *******************/

	Tempfueh.begin();
	Tempfueh.setResolution(10);			// 0.25 Grad Genauigkeit
	Heizung();

	/******** TFT Backlight *******************/

	ledcSetup(BacklightKanalTFT, BacklightFrequenz, BacklightBit);	//ledcSetup(Kanal, Frequenz, Bit);
	ledcAttachPin(BacklightPin, BacklightKanalTFT);					//ledcAttachPin(Pin, Kanal);
	ledcWrite(BacklightKanalTFT, BacklightwertTag);

	/******** Futterautomat *******************/

	ledcSetup(FutterKanal, Futterfreq, FutterRes);
	ledcAttachPin(FutterPin, FutterKanal);

	/******* PowerLED ************************/

	ledcSetup(PowerledKanal, Powerledfreq, PowerledBit);
	ledcAttachPin(PowerledPin, PowerledKanal);

	/******** Time Server Satr und setzen *****/

	Udp.begin(localPort);
	setSyncProvider(getNtpTime);
	setSyncInterval(300);
	digitalClockDisplay();

}

void loop() 
{
	/*********** Timer updaten *************************/

	tickerPro.update();
	tickerHei.update();

	/*********** virtuino starten *********************/

	virtuinoRun();        // Necessary function to communicate with Virtuino. Client handler

	/*********** Virtuino Werte lesen *****************/

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

	if (V[18] != Futterdauer)
	{
		Futterdauer = V[18] * 1000;
		preferences.putUInt("FutD", Futterdauer);
	}

	if (V[19] != Futtergesch)
	{
		Futtergesch = V[18];
		preferences.putUInt("FutG", Futtergesch);
	}

	if (V[20] != Hysterese)
	{
		Hysterese = V[20];
		preferences.putFloat("Hyst", Hysterese);
	}

	/************** Stripe helligkeit ändern ************/

	strip1.SetBrightness(aktHell);
	strip1.Show();

	/************* Uhr im Display aktualisieren ********/

	if (second(nowLocal()) == 00)
	{
		digitalClockDisplay();
	}
	
	
	/******** Schalter für Beleuchtung ********/

	switch (SonneIndex) {

	case 1:
		SonneAuf();
		break;
	case 2:
		SonneUn();
		break;
	case 3:
		SonneMitAn();
		break;
	case 4:
		SonneMitAus();
		break;
	case 5:
		SonneNaAus();
		break;
	}

	switch (FutterIndex)
	{

	case 1:
		Futterautomat();
		break;
	
	}

}


