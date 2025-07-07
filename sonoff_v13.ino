//{check value
/* deze software is generalistisch voor 4 types:
 voor de sonoff met sensors: define SENSORS
 voor de ESP01 #define ESP01
 voor de ESP8285 #define ESP8285
 voor de Sonoff S20 #define BUILDIN
 voor de Sonoff mini #define MINI
 Geen Wifimanager maar eigen wifi configportal

 we use events to send a message to the client than he needs to udate the webpage
 we do this when the switch state changes
 when the sensor readings are done so that new data can be displayed
 other occasions?
*/
// this .bin cannot be uploaded by a sonoff, it is too large, over 470 ?
// what changed is switchHL into bool array and timerActive but that requires some extra programming in SPIFFS 
// now changed switchHL to byte and timerActive also 
// still too large, removed serial info now 467 it fits

#define SENSORS
//#define ESP01_FET
//#define ESP01_TRIAC
//#define ESP8285
//#define S20  // dit is de S20 met tweekleurige led
//#define MINI
//#define SONOFF // normal sonoff w/o sensors
//#define WEMOS  // for testing


//AsyncWebSocket ws("/ws");
#include <ESP.h>
#include <time.h>
#include <sunMoon.h>

#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal

#include <ArduinoJson.h>
#include <Arduino.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"

AsyncWebServer server(80);
AsyncEventSource events("/events"); 

#ifdef SENSORS
//#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266HTTPClient.h>
#include <MAX44009.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#endif

#include <PubSubClient.h>
#include "HTML.h"
#include "AAA_PORTAL_HTML.h"
#include "TIMERS_HTML.h"
WiFiClient My_WifiClient ;
PubSubClient MQTT_Client ( My_WifiClient ) ;
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "nl.pool.ntp.org");
const byte DNS_PORT = 53;
DNSServer dnsServer;

#define TIMERCOUNT  4


// variabelen die met de klok en timers te maken hebben
//  WiFiUDP Udp; 
  time_t dagbegintijd = 0;
  time_t uitschakeltijd[4] = {0};
  time_t inschakeltijd[4] = {0};
  time_t switchOnmoment = 0;
  time_t repeateruitschakeltijd = 0;  
  
  byte mDay = 0;
  String  maan = "";
  int dst;

 // variabelen wificonfig
  char pswd[11] = "0000";
  int securityLevel = 6;
  float longi = 5.123;
  float lati = 51.123;
  char gmtOffset[5] = "+120";  //+5.30 GMT
  char ssid[33] = ""; // was 33 
  char pass[40] = ""; // was 40
  bool tryConnectFlag = false;

  bool dts = false;
  bool diagNose = false;
 
 
 //variabelen timerconfig 
  //char timerActive[5] = "0000"; // this contains only 1 and 0, we make this a bool
  byte timerActive = 0b00000000; 
  char relToSunOn[5] = ""; 
  char relToSunOff[5] = ""; 
  char switchOn[21] = "";
  char switchOff[21] = "";
  char weekDag[29] = "";
  char switchcdwn[6] = "";
//  char zetAan[3] = "00";
  //char switchHL[5] = "0000"; // bool voor hoger of lager therm hyg licht dig
  byte switchHL = 0b00000000;
  float switchTemp = 0.1; // was 4
  int switchMoist = 11; // de vochtigheidsgraad
  float switchLicht = 20; // ambient light in lux 
  uint8_t Bds = 0;  // boodschap
  char tempCal[5] = "+0.0"; //calibation
  //char sensor[2] = ""; // what sensor is configured
  int meetRes = 180; // hebben we nodig om de mqtt verbinding te bewaken
  //char timer[2] = "1"; //what automation is configured
  uint8_t autoMate = 1; //(timers)
  uint8_t senSor = 1; //(timers)


  //universelevariabelen basis config 
  char dvName[21] = "";
  char userPasswd[11] = "1111";
  char aso[6] = "06:00";

  char    mqttBroker[30]=    "192.168.0.100";
  //char  mqttIntopic[ =   "esp/in";
  char    mqttOuttopic[26] =  "domoticz/in";
  int     mqttPort =     1883;
  bool    mqttEnabled =  false;
  int     idxSwitch = 0;
  int     idxSensor = 0;  
  char    requestUrl[12] = {""}; // to remember where we came from  

  bool mustSwitch[4] = {false, false, false, false }; // = 0;
  bool hasSwitched[4] = {false, false, false, false }; // = 0;  
  bool mustCalc[4] = {false, false, false, false }; // = 0;
  int actionFlag = 0;
//  int verklikker = 0; // heeft domoticz iets verstuurd
  int cdwnuur = 0;
  int cdwnminuut = 0;
  int asouur = 0; // beveiliging
  int asominuut = 0;
  time_t sunset = 0;
  time_t sunrise = 0;
  bool timeRetrieved = false;
  static unsigned long laatsteMeting = 0; //wordt ook bij OTA gebruikt en bij wifiportal
  static unsigned long lastCheck = 0;
  int tKeuze=0;
  int networksFound =0;
  int aantal=0;

  int weerteller = 1;
  int verzonden = 0;

  Adafruit_BME280 bme; // I2C
  //float h, t, p, pin, dp;
  float p, pin, dp;
  int bar_for = 0;
  float luchtdruktabel[11];
  String ldString="←→";

  MAX44009 licht;

#define ONE_WIRE_BUS  3  // pin3 ont the sonoff
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//#define BME280_ADDRESS                (0x76)
#define DHTPIN  3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE, 22); // 11 works fine for ESP8266
float humidity, temp_f, temp_c;  // Values read from sensor
float switch_temp = 0;

int datum = 0; //
#define knop              0  //

#if defined SENSORS || defined S20  || defined MINI   || defined SONOFF // alle sonoffs
   #define RELAY_PIN        12  // esp-12 en sonoff
   #define led_onb         13  // esp8266
   //#define led_onb           2  // sinc! esp8285
   #define LED_AAN  LOW     // 8266 sonoff
   #define LED_UIT  HIGH    // 8266 sonoff
   #define SWITCH_AAN  HIGH // 8266 sonoff
   #define SWITCH_UIT  LOW  // 8266 sonoff
   #define TXpin       1
#endif

#ifdef ESP8285
   #define RELAY_PIN     12  // stuurt een transistor met FET dus hoog  == uit 
   #define led_onb        2   //  onboard led  sinc ! cannot be low @ boot
   //#define touch_pin    4   // kunnen we gewoon aan pin 0 hangen
   #define LED_AAN  LOW
   #define LED_UIT  HIGH
   #define SWITCH_AAN  LOW
   #define SWITCH_UIT  HIGH
#endif

String toSend = ""; 
int value = 0;
int event = 0;
//  bool tryConnectFlag = false;

// ////////////////////////////////////////////////////////
// // variables To record and display last events on the home page.
// struct logEvent {
//   String    Log_date ;
//   String    Log_kind ;
//   String    Log_message;
// };

// static const int Log_MaxEvents = 18;    // Please avoid to make a too big value here, the past events will be stored in memory and displayed in the html home
// bool Log_MaxReached = false;
// byte Log_CurrentEvent = 0;
// logEvent Log_EventList[Log_MaxEvents];  // To stock in memory the last x events (20 by default)

#include <Ticker.h>
Ticker resetTicker;

// *****************************************************************************
// *                              SETUP
// *****************************************************************************

void setup() {
// normally we use tx for serial. If we use a sensor we use RX and pin 14 
// we start with normal serial, to have some debug at start
  Serial.begin(115200); 
  delay(100);
  //Serial.println("sonof boot sequence");
  
  pinMode(knop, INPUT_PULLUP); // de knop
  pinMode(RELAY_PIN, OUTPUT); // de lamp
  digitalWrite(RELAY_PIN, SWITCH_UIT); // initieel uit
  pinMode(led_onb, OUTPUT); // onboard led
  
  SPIFFS_read();
  start_wifi(); // met de gelezen gegevens een verbinding maken
  getTijd(); // get the time, when successful the timers are armed for calculation

  ledblink(1, 800);

  //Serial.println("done with SPIFFS start_wifi and getTijd");
  delay(200);
// if not diagnose we re-define the pins to function for the sensors i2c onwire etc
if (diagNose == false) {
   Serial.println(F("changed pinmodes, no uart0 anymore, bye"));
   delay(100);
   pinMode(1, FUNCTION_3);  // tx als gpio pin
   pinMode(3, FUNCTION_3); //rx als gpio pin
   Serial.flush();
   Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
}

if(diagNose == false){
// we change these pins only when not debugging
if (senSor == 4 || senSor==5 || senSor==7 ){  // voor de body, touch, digital sensor
    pinMode(3, INPUT);
    digitalWrite(3, LOW);
  }
  if (senSor == 3 || senSor == 6){  // voor de bme of de lichtsensor
    //Wire.begin(3, TXpin);  // sda 3  scl 1  rx = pin3 tx=pin1
    Wire.begin(TXpin, 3);  // sda 3  scl 1  rx = pin3 tx=pin1
   }
   if(senSor == 3) {
          if (!bme.begin(0x76)) {
             ledblink(10, 100);
          }
     }
    if(senSor == 6) { 

          if (!licht.begin()) {
           consoleOut("no max44009 connected, check wiring!");
          }
     }
   sensors.begin();
  
}
  delay(1000);  //anders krijgen we een foutmeting 


//  ****************** mqtt init *********************
   MQTT_Client.setServer(mqttBroker, mqttPort);
   MQTT_Client.setCallback ( MQTT_Receive_Callback ) ;
  

  //Update_Log("system","boot up");
  healthCheck(); //starts mqtt and checks time
  
 // initWebSocket(); // for the console
  
  if(!diagNose) meetENschakel(); //

  ledblink(3, 500);
  
  laatsteMeting = 0; //
  Serial.println(F("setup done"));
  // if in debug mode, this interrupt is triggerd bij measuring the bme80
  // how do we prevent it
  // disable the autopolling and manual poll when diagNose true oke
  attachInterrupt(digitalPinToInterrupt(knop), isr, FALLING);
  eventSend();
}

//****************************************************************************
//*                         LOOP
//*****************************************************************************
void loop() {

  // ******************* foutcontroles ***************************

  if(mqttEnabled)MQTT_Client.loop();

  if ( !timeRetrieved && aantal < 3) {
    getTijd(); // // if successfull the mustCalc's are set
    aantal += 1;
  } //

// healtCheck
unsigned long nu = millis();  // 
 if ( nu - lastCheck >= 1000UL * 600 ) {
    lastCheck += 1000UL * 30 ; // each time 6 minutes more;
    healthCheck();
} 

#ifdef SENSORS
 nu = millis();  // 
 if ( nu - laatsteMeting >= 1000UL * meetRes ) {
    laatsteMeting += 1000UL * meetRes ; // each time (meetRes * miliseconden) more;
    if (!diagNose) meetENschakel(); // if we do this while debugging the interrupt is triggerd.
}
#endif      
  
// ****************** check the actionflags *************************
 if(actionFlag != 0) checkActionflags();
 
  // if(Serial.available() > 7) {
  //   //Serial.println(F("Serial incoming bytes"));
  //      handle_Serial();
  //  }  

// *********************** dayly synchronize and arm the timers for recalculation  
  // if there is a new date we synchronize at 4 o'clock
   if (day() != datum && hour() > 4) { // if date overflew and later than 4
   getTijd(); // synchronize and arm the timers for recalculation
   }

  
// ****************************************************************************
//                switch by the timers
// ****************************************************************************  
   schakelen();
  
   //  ****** countdown switch  **********
   // automatic switch off by cdwn in case motion sensor triggered
   // 
    if (event == 18 ) // switched on by motion sensor
    {   
           if (now() > switchOnmoment + ((cdwnuur * 3600 + cdwnminuut) * 60)) 
           { // now = current time in seconds
               switchOffNow(true, false, "countdown"); //send mqtt, no timercheck
               event = 0;
           }
     }


// ****************** forgotten lamp security *********************************************
// if the lamp is switched manual it would be on forever
// sutomatic switch off after the set time
  if ( value == 1 || value == 2 || value == 17) { // switched on by button or web or mosquitto
     if ( now() > switchOnmoment + ((asouur * 60 + asominuut) * 60)) { // 6 uur == 21600 voor test 3 min. == 180sec
             switchOffNow(true, false, "security"); //meteen de lamp uit, mqtt, geen timercheck
             consoleOut("switched off by security"); 
     }
}

 
// ***************************** motion sensor ****************************************
// behaviour can be set in timers
// only switch on when lamp is off
// mqtt message is always send, also if the lamp is on
// the lamp is switched off by countdwn timer when on by the motionsensor
if ( senSor == 4 ) { // works also when timers are active
   if ( digitalRead(3) == 1 ) // seen motion?
   { 
      //must the lamp be switched on? only when off
      //bool switched = false;
      if ( bwswitch() && value == 0 ) // we have to switch and the lamp = off
      { 
         value = 18;
         switchOnNow(true, false, "motion sensor"); // lamp on at once
         //Update_Log("system", "motion sensor triggered");
         event = 18;
         consoleOut("bodysensor triggered");
      }
      if ( boodschap() ) 
      {
          // first check if a message is sent, if not do this and make verzonden true  
          if ( verzonden == 0 ) 
          {
             verzonden = 1;
             sendMqttsensor(); //
             switchOnmoment = now(); // remember the moment that motion was seen
          }
      }      
   } 
   else 
   {
   //  sensor is low
       if ( verzonden == 1 ) // an on message is sent
       { 
        //we send an off message after 30 sec
            if (now() > switchOnmoment + 30 ) // now = current time in seconds thus after 30 sec
            { 
                verzonden = 2;
                sendMqttsensor(); //
                verzonden = 0; // 
           }
       }    
   }
}

 // delay will cause a crash in the async webserver and isr
 // so actions that require delay or take too long
 // are performed in the loop bij a value of actionflag

// ***************************** external button sensors ****************************************
if( senSor == 5 ) { // this works together with timers dus timer[0] can be 1
  if (digitalRead(3) == 1) {
      consoleOut("button detected, value = " + String(value));

      unsigned long starttijd = millis(); // save the time in currentMillis
      unsigned long eindtijd = millis();
        while (eindtijd - starttijd  <= 50) { //2000 millis = 2 sec // was 100
        eindtijd = millis();
        }
 // check if the button is still pressed after a while
int val = digitalRead(3);
     if (val == 0) { // button released
       
      } else { //button is still pressed
              digitalWrite(led_onb, LED_AAN);
               delay(200); // even wachten
               digitalWrite(led_onb, LED_UIT); // put the led off
               value = value + 1;
     
                if (value > 1) { // value was 1 so lamp was on
                    actionFlag = 21; // make it switch off
                    delay(1000); //to prevent repetition
                } else {
                     actionFlag = 20;
                     delay(1000); //
                }
            }
        }
    }



}
//****************End Loop   *****************************




// switch by the sensors
// we always check the state first (value)  
  
// *********** automatic switching thermostate *************************************************

void thermostaat() {
  if (temp_c == 1000 || temp_c == -127) {
    return;
  }
  //float temperatuur = atof(switchTemp);
  // temp_c = a float and switchTemp also
  bool b bitRead(switchHL,0); 
      if (temp_c > switchTemp) { // if the polled temperature is heigher
      //if (temp_c > temperatuur) { // if the polled temperature is heigher
      //if (temp_c > decideValue) { // if the polled temperature is heigher  
           if ( b == false && value == 0) { // switch on when 'heigher than' is selected and off
                  value = 12;
                  switchOnNow(true, false, "thermostate"); //lamp on
                  //DebugPrintln("ingeschakeld door thermostaat");
                  //event = 12;
                  
                  return;
           }
           if ( b == true && value > 0 ) { //switch off when 'lower than' is selected
                  //DebugPrintln("uitgeschakeld door thermostaat");
                  switchOffNow(true, false, "thermostate"); 
                  //event = 32;
                  return;
                 }
           
      } else { // if polled temperature lower
           if ( b == true && value == 0) { // switch on when 'lower than' is selected and off
                  switchOnNow(true, false, "thermostate"); 
                  value = 12;
                  return;
           }
           if ( b == false && value > 0 ) { //switch off when 'heigher' is geselected and is on
                  //DebugPrintln("uitgeschakeld door thermostaat");
                  switchOffNow(true, false, "thermostate"); 
                  return;
                 }
           }  
} 
  // *********** automatic switching hygrostate *************************************************
void hygrostaat() {
  if (temp_c == 1000 || temp_c == -127) {
    return;
  }
  //int hum = atof(switchMoist);
  bool b = bitRead(switchHL, 1); 

      //if (humidity > hum) { // if polled humidity is heigher
      if (humidity > switchMoist) { // if polled humidity is heigher        
           if ( b == false && value == 0) { // switch on when 'heigher than' is selected and off
                  value = 13;
                  switchOnNow(true, false, "humidity sensor"); 
                  return;
           }
           if ( b == true && value > 0 ) { //switch off when 'lower than' is selected
                  switchOffNow(true, false, "humidity sensor"); 
                  return;
                 }
           
      } else { // if polled humidity is lower

           if ( b == true && value == 0) { // switch on when 'heigher than' is selected and off
                  value = 13;
                  switchOnNow(true, false, "humidity sensor"); 
                  return;
           }
           if ( b == false && value > 0 ) { //switch off when 'lower than' is selected
                  switchOffNow(true, false, "humidity sensor"); 
                  return;
                 }
        }
  }

// *********** automatic switching lightsensor *************************************************
void lichtSensor() {
  //float licht = atof(switchLicht);
  //DebugPrint("switchLicht = ");
  //DebugPrintln(licht);
  bool b = bitRead(switchHL,2); 
      //if (p > licht) { // if the polled light is higher
      if (p > switchLicht) { // if the polled light is higher 
           if (b == false && value == 0) { // switch on when 'higher than' is selected and off
                  value = 14;
                  switchOnNow(true, false, "lightsensor"); //meteen de lamp aan
                  return;
           }
          if (b == true && value >0) { //switch off when 'lower than' is selected and on
                  switchOffNow(true, false, "lightsensor"); //meteen de lamp uit en mqtt message
                  return;
                 }
           
      } else { //so the polled light is lower
           if (b == true && value == 0) { // //switch on when 'lower than' is selected and off
                  value = 14;
                  switchOnNow(true, false, "lightsensor"); 
                  return;
                  }
          if (b == false && value > 0) { //switch off when 'higer than' is selected and on
                  switchOffNow(true, false, "lightsensor"); 
                  return;
                 }
        }
}
// *********** automatic switching by generic digital sensor *************************************************
void digitalSensor() { //called by meten()

// if the sensor is high
  bool b = bitRead(switchHL,3);
      if (digitalRead(3) == HIGH) { // if the sensor is high
           if (b && value == 0) { // //switch on when 'high' is selected
                  value = 15;                  
                  switchOnNow(true, false, "digital sensor"); //meteen de lamp aan
                  return;
           }
          if (!b && value > 0) { //switch off when 'low' is selected
                   switchOffNow(true, false, "digital sensor");
                   return;
                 }
           
      } else { // sensor is low
           if (!b  && value == 0) { //switch on when 'high' is selected
                  value = 15;                 
                  switchOnNow(true, false, "digital sensor"); 
                  return;
           }
          if (b && value > 0) { //switch off when 'low' is selected
                  switchOffNow(true, false, "digital sensor"); 
                  return; 
                 }
       }
 }



bool donker() {
// als senSor == 5 then it is always dark because it has to work day and night
//if (senSor == 5 ) {return 1;}
if ( sunrise < now() && now() < sunset ) {
return 0;
}else {
return 1;   
}
}
bool boodschap() { //boodschap bij 0(cond), 1(cond), 2, 3 en 4 altijd
  if ( Bds == 0 || Bds == 1 ) {
     if ( donker() ) { return 1; } else { return 0; }
    } else { // 2 3 4 altijd boodschap
       return 1;
    } // always boodschap
}
bool bwswitch() { // lamp moet aan bij 0(cond), 2(cond) en 3(altijd)
  if ( Bds == 0 || Bds == 2 ) { 
      if ( donker() ) { return 1; }  
  } else {
    if ( Bds == 3 ) {return 1;} // altijd lamp
  }
  // if we are here the conditions above not true so don't switch
  return 0; // opties 0 of 2 en licht,  and options 1 adn 4
}
