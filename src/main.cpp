

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "bitmaps.h"
#include <TimeLib.h>
#include <NeoPixelBusLg.h>
#include <OneWire.h>
#include <Preferences.h>
#include <SimpleTimer.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <DallasTemperature.h>


boolean debug = false;              			// HIER AUF "TRUE" SETZTEN WENN SERIAL DEBUG AUSGABE SEIN SOLL.

SimpleTimer timer;

/******************* SETTINGS ********************/

const char* ssid = "Andre+Janina";            	// WIFI network SSID
const char* password = "sommer12";            	// WIFI network PASSWORD
const char* hostname = "AquariumPC";
WiFiServer server(8000);               		    // Server port
IPAddress ip(192, 168, 178, 32);         		// where 150 is the desired IP Address.
IPAddress gateway(192, 168, 178, 1);         	// set gateway to match your network. Replace with your router IP


/*********** VirtuinoCM  Library settings *********/

#include "VirtuinoCM.h"
VirtuinoCM virtuino; 
           
#define V_memory_count 32          				// the size of V memory. You can change it to a number <=255)
float V[V_memory_count];           				// This array is synchronized with Virtuino V memory. You can change the type to int, long etc.


/*********** EEPROM Speichern ********************/

Preferences preferences; 


/***********  NeoPixel Einstellungen   ***********/

#define PIN_STRIPE			13
#define NUMLEDS			    5

xSemaphoreHandle semaphore = NULL;
TaskHandle_t commit_task;

NeoPixelBus<NeoRgbFeature, NeoEsp32Rmt0Ws2812xMethod> strip1(NUMLEDS, PIN_STRIPE); 
//NeoPixelBusLg<NeoRgbFeature, NeoEsp32I2s1X8Ws2812xMethod> strip1(NUMLEDS, PIN_STRIPE);

/************ TFT Einstellungen ******************/

TFT_eSPI tft = TFT_eSPI();
//#define MAX_GEN_COUNT 500
//#define USE_DMA_TO_TFT

/************* One Wire (Tempfühler) **************/

#define ONE_WIRE_BUS	 26				// Anschlusspin für OneWire			

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature Tempfueh(&oneWire);
//#define REQUIRESNEW true
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
uint8_t Futterdauerhelp = 2;

uint8_t PowerledPin = 16;
uint16_t Powerledfreq = 2000;
uint8_t PowerledKanal = 1;
uint8_t PowerledBit = 8;
uint8_t Powerledwert = 0;
uint8_t PowerledwertManu = 0;
uint8_t Powerledmax = 190;
uint8_t Powerledmin = 0;

uint8_t LichtZustand;
uint8_t LichtZustandStatus = 1;

float V3SonneAuf;
float V4MittagAN;
float V5MittagAUS;
float V6SonneUnt;
float V7SonneNac;
float V8CO2AN;
float V9CO2AUS;
float V10Futter;

unsigned long PowerLEDMillis = 0;
unsigned long FutterMillis = 0;
/***************** Millis für WLAN Reconnect *****/
unsigned long previousMillis = 0;
unsigned long interval = 30000;		//30 sec dann Abfrage ob WLAN ja/nein

/**************** Pin Belegung *******************/

#define heizung				12      //Pin Relais Heizung
#define luefter				25      //Pin Lüfter
#define co2					27		//Pin Relais CO2 Abschaltung

/**************** NeoPixel Init ******************/
// Sonnenaufgang Color Array
//				{   R,  G,   B };
int SonAu1[3] = { 50, 	 0,   0 };
int SonAu2[3] = { 100,	 9,  10 };
int SonAu3[3] = { 150, 	12,  30 };
int SonAu4[3] = { 180, 	20,  50 };
int SonAu5[3] = { 200, 	25,  70 };
int SonAu6[3] = { 230, 	35,  90 };
int SonAu7[3] = { 250, 	35, 240 };

// Sonnenuntergang Color Array
//				{   R,  G,   B };
int SonUn1[3] = { 250, 60,  80 };
int SonUn2[3] = { 240, 50,  60 };
int SonUn3[3] = { 230, 40,  30 };
int SonUn4[3] = { 220, 20,  15 };
int SonUn5[3] = { 200, 10,   8 };
int SonUn6[3] = { 100, 5,   30 };
int SonUn7[3] = {  0,   0,  80 };

//Nachtlicht AUS Color Array
//					 { R, G, B };
int Nachtlicht1[3] = { 0, 0, 0 };

// Set initial color
uint8_t redVal = 0;
uint8_t grnVal = 0;
uint8_t bluVal = 0;

uint16_t DurchWait = 180;
unsigned long CrossLEDMillis = 0;

uint8_t prevR = redVal;
uint8_t prevG = grnVal;
uint8_t prevB = bluVal;
uint16_t LEDStep = 0;
uint8_t Durchlauf = 1;
uint8_t SonneIndex = 0;
uint8_t FutterIndex = 0;

uint16_t StepWert = 256;


/***** NTP Server abrufen für Local Time ********/

static const char ntpServerName[] = "de.pool.ntp.org";
const int timeZone = 1;

const uint32_t syncIntervalMax = 300; // could be the same value like the syncInterval in time.cpp | 300
const uint32_t syncIntervalAfterFail = 60; // if sync with NTP fails, retry earlier | 60
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
#include <virduino.h>

/************************** NTP code ********************************************/

time_t getNtpTime()
{
	IPAddress ntpServerIP; // NTP server's ip address

	while (Udp.parsePacket() > 0); // discard any previously received packets
	Serial.println("Transmit NTP Request");
	// get a random server from the pool
	WiFi.hostByName(ntpServerName, ntpServerIP);
	if (debug)
	{
	Serial.print(ntpServerName);
	Serial.print(": ");
	Serial.println(ntpServerIP);
	}
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
	if (debug) Serial.println("No NTP Response :-(");
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
     if (debug) Serial.println("timeout");
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


		Serial.begin(115200);
	  if (debug) 
	  {
    	Serial.begin(115200);
    	while (!Serial) continue;
	  }	
	/************ TFT Layout setzen ***************/  
	tft.initDMA(); 
	TFT_Layout();

	/********** Virtuino settings *****************/
 	virtuino.begin(onReceived,onRequested,512);  //Start Virtuino. Set the buffer to 512. With this buffer Virtuino can control about 50 pins (1 command >= 9bytes) The T(text) commands with 20 characters need 20+6 bytes
  	virtuino.key="4593";                       //This is the Virtuino password. Only requests the start with this key are accepted from the library

    //if (debug) Serial.println("Connecting to "+String(ssid));
   	// If you don't want to config IP manually disable the next two lines
   	//IPAddress subnet(255, 255, 255, 0);        // set subnet mask to match your network
 	//WiFi.config(ip, gateway, subnet);          // If you don't want to config IP manually disable this line
	//WiFi.mode(WIFI_STA);                       // Config module as station only.
  	//WiFi.setHostname(hostname);

	WiFi.begin(ssid,password);
  	//delay(1000);

		while ( WiFi.status() != WL_CONNECTED ) 
  		{
		tft.drawBitmap(140, 0, wlan, 20, 20, TFT_WHITE);	
    	delay (500);
		tft.drawBitmap(140, 0, wlan, 20, 20, TFT_VIOLET);
		delay (500);
    	Serial.print ( "." );
  		}

  	tft.drawBitmap(140, 0, wlan, 20, 20, TFT_GREEN);
  
  server.begin();

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
	maxHell = preferences.getUInt("MaxH", 0);
	mittagHell = preferences.getUInt("MitH", 0);
	Powerledmax = preferences.getUInt("PowH", 0);
	BacklightwertTag = preferences.getUInt("BackLT", 0);
	BacklightwertNacht = preferences.getUInt("BackLN", 100);
	TFTRotation = preferences.getInt("TFTR", 0);
	Futterdauer = preferences.getUInt("FutD", 0);
	Futtergesch = preferences.getUInt("FutG", 0);
	Futterdauerhelp = preferences.getUInt("FutDHelp", 0);
	SollTemp = preferences.getFloat("SollT");
	LuefTemp = preferences.getUInt("LueT", 0);
	Hysterese = preferences.getFloat("Hyst", 0);
	LichtZustand = preferences.getUInt("LichtZu", 0);

	
	
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
	V[13] = mittagHell;
	V[14] = Powerledmax;
	V[15] = BacklightwertTag;
	V[16] = BacklightwertNacht;
	V[17] = TFTRotation;
	V[18] = Futterdauerhelp;
	V[19] = Futtergesch;
	V[20] = Hysterese;
	V[21] = PowerledwertManu;
	
	
	/*********** GPIO´s definieren ****************/

	pinMode(heizung, OUTPUT);
	pinMode(luefter, OUTPUT);
	pinMode(co2, OUTPUT);

	/**** Alarm Timer starten für Funktionen ******/ 
	
	timer.setInterval(1000, ProgrammTimer);
	timer.setInterval(2000, Heizung);
	timer.setInterval(500, virduino);
	//timer.setInterval(60000, digitalClockDisplay);

	/*********** Neopixel Starten ***************/

	strip1.Begin();
	strip1.SetPixelColor(3, RgbColor(0, 0, 200));
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
	setSyncInterval(3600);
	digitalClockDisplay();


	/******************** OTA Update ********************/
	 // Port defaults to 3232
  //ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname("AquariumPC");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

	 ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

/****************** Lichtzustand bei Neustart *****************/

  	if (LichtZustand == 1) 	//Sonnenaufgang

  		{
	  	Powerledwert = Powerledmax;
	  	ledcWrite(PowerledKanal, Powerledwert);

  		strip1.SetPixelColor(0, 	RgbColor(0, 0, 240));
		strip1.SetPixelColor(1, 	RgbColor(250, 0, 0));
		strip1.SetPixelColor(2, 	RgbColor(0, 0, 240));
		strip1.SetPixelColor(3, 	RgbColor(250, 0, 0));
		strip1.SetPixelColor(4, 	RgbColor(0, 0, 240));
		delayMicroseconds(20);
		strip1.Show();	
  		}

   if (LichtZustand == 2)	//Sonnenauntergang

  		{
	  	Powerledwert = 0;
	  	ledcWrite(BacklightKanalTFT, BacklightwertNacht);
	  	ledcWrite(PowerledKanal, Powerledwert);

		strip1.SetPixelColor(0, 	RgbColor(0, 0, 0));
		strip1.SetPixelColor(1, 	RgbColor(0, 0, 100));
		strip1.SetPixelColor(2, 	RgbColor(0, 0, 200));
		strip1.SetPixelColor(3, 	RgbColor(0, 0, 100));
		strip1.SetPixelColor(4, 	RgbColor(0, 0, 0));
		delayMicroseconds(20);
		strip1.Show();
  		}

	if (LichtZustand == 3)	//Sonnen Mittag AN

  		{
	  	Powerledwert = mittagHell;
	  	ledcWrite(PowerledKanal, Powerledwert);

	  	strip1.SetPixelColor(0, 	RgbColor(0, 0, 240));
		strip1.SetPixelColor(1, 	RgbColor(250, 0, 0));
		strip1.SetPixelColor(2, 	RgbColor(0, 0, 240));
		strip1.SetPixelColor(3, 	RgbColor(250, 0, 0));
		strip1.SetPixelColor(4, 	RgbColor(0, 0, 240));
		delayMicroseconds(29);
		strip1.Show();	
  		}

  	if (LichtZustand == 4)	//Sonnen Mittag AUS

  		{
		Powerledwert = Powerledmax;
	  	ledcWrite(PowerledKanal, Powerledwert);

		strip1.SetPixelColor(0, 	RgbColor(0, 0, 240));
		strip1.SetPixelColor(1, 	RgbColor(250, 0, 0));
		strip1.SetPixelColor(2, 	RgbColor(0, 0, 240));
		strip1.SetPixelColor(3, 	RgbColor(250, 0, 0));
		strip1.SetPixelColor(4, 	RgbColor(0, 0, 240));
		delayMicroseconds(20);
		strip1.Show();		 		
 		}


   if (LichtZustand == 5)	//Nachtlicht AUS
  		
		{
	  	ledcWrite(BacklightKanalTFT, BacklightwertNacht);
	  	ledcWrite(PowerledKanal, 0);

		strip1.SetPixelColor(0, 	RgbColor(0, 0, 0));
		strip1.SetPixelColor(1, 	RgbColor(0, 0, 0));
		strip1.SetPixelColor(2, 	RgbColor(0, 0, 0));
		strip1.SetPixelColor(3, 	RgbColor(0, 0, 0));
		strip1.SetPixelColor(4, 	RgbColor(0, 0, 0));
		delayMicroseconds(20);
		strip1.Show();
  		}

}

void loop() 
{	
	/************** WLAN Reconnect **********************/

	unsigned long currentMillis = millis();
  	// if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  		if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval))
		{
    		if (debug) Serial.print(millis());
    		if (debug) Serial.println("Reconnecting to WiFi...");
				tft.drawBitmap(140, 0, wlan, 20, 20, TFT_RED);
				delay (500);
				tft.drawBitmap(140, 0, wlan, 20, 20, TFT_GREEN);
				delay (500);
    		if (debug) Serial.print ( "." );
    		WiFi.disconnect();
    		WiFi.reconnect();
   			previousMillis = currentMillis;
  		}


	/************** OTA ********************************/

	ArduinoOTA.handle();

	/*********** Timer updaten *************************/

	timer.run();

	/*********** virtuino starten *********************/

	virtuinoRun();        // Necessary function to communicate with Virtuino. Client handler

	/************** Stripe helligkeit ändern ************/

    // delay(10);
	//ledcWrite(PowerledKanal, PowerledwertManu);

	/************* Uhr im Display aktualisieren ********/

	if (second(nowLocal()) == 00)
	{
		digitalClockDisplay();
	}
	
	
	/******** Schalter für Beleuchtung ********/

	switch (SonneIndex) 
	{

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


