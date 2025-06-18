
const char SENSORCONFIG_START[] PROGMEM = R"=====(
<body>
<style>
a:link, a:visited  {width: 64px;}
</style>

<div id='msect'>
  <div id='help'>
  <span class='close' onclick='sl();'>&times;</span><h3>SENSOR SETTINGS HELP</h3>
  <b>sensor:</b><br>Enter here which sensor you have connected.
  <br><br><b>measureinterval:</b><br>Every so many seconds is measured.  
  <br><br><b>thermostate etc:</b><br>You can set the behavior of the thermostate, etc.
  <br>Don't forget to activate your automation in the settings menu. 
  <br><br><b>motion sensor</b><br>You don't need to set up the motion sensor under active automation.<br>
  It always works when set, even together with on/off timers.
  </div>
</div>
<div id='msect'>
  <div id='menu'>
  <a href="#" class='close' onclick='cl();'>&times;</a>
  <a href='#' onclick='helpFunction()'>&quest;</a>
  <a href='/SENSOR?welke=16' class="extra" >sensor</a>
  <a href='#' id="sub" onclick='submitFunction()'>save</a>

  </div>
</div>
<div id='msect'>
<kop>  SONOFF SENSOR CONFIG </kop>
</div>

<div id='msect'>
  <div class='divstijl'>
  <form id='formulier' method='get' action='submitForm' oninput='showSubmit()'>
  <input name='sensorID' value="sensorID" type='hidden'>
  <center><irame name>
  </form></div>
</div>
<br>
<div id='msect'>
  <div id='menu'>
  <a href='#' style='width:20px;'></a>
  <a href='/SENSOR?welke=11' >thermost</a>
  <a href='/SENSOR?welke=12' >hygrost</a>
  <a href='/SENSOR?welke=13' >motion</a>
  <a href='/SENSOR?welke=14' >light</a>
  <a href='/SENSOR?welke=15' >digital</a>

  </div>
</div>
</body></html>
 )=====";

// welke sensor en calibratie
const char SENSOR_WS[] PROGMEM = R"=====(
<h4>SENSOR, CALIBRATION AND MEASURE-INTERVAL</h4>
<table><tr><td style='width:140px;'>used sensor<td><select class='sb1' name='sensor'> 
<option value='0' sensor0>no sensor</option> 
<option value='1' sensor1>DS18B20 sensor</option>
<option value='2' sensor2>DHT22 sensor</option>
<option value='3' sensor3>BME280 sensor</option>
<option value='4' sensor4>motionsensor</option>
<option value='5' sensor5>push/touchbutton</option>
<option value='6' sensor6>MAX44009 lightsensor</option>
<option value='7' sensor7>generic digital sensor</option>
</select></table>

<table><tr><td style='width:140px;'>temp. calibration<td><input onchange="subMit()" class='inp2' name='tcal' value='+00' size='4'  required pattern='^([+-]{1}[0-9]{1}[.][0-9])$' title=' i.g. +02.4 or -1.2'><td>&nbsp&nbsp temperature correction.
</td></tr>
<tr><td style='width:140px;'>measure interval<td><input class='inp2' type='number' min='10' max='3600' name='mf' value='300' size='4' ><td>&nbsp&nbsp seconds.
</td></tr></table>
<p>Attention: Enter always a + of - for de temperaturecalibration, i.g. +09.3
<br></p>
 )=====";

const char SENSOR_TS[] PROGMEM = R"=====(
<h4>THERMOSTATE</h4>
value last measurement : n/a &#8451;<br><br>
Switch on when the temperature is:<br><br>
<select name='tempHL' class='sb1'>
<option value='1' tempHL1>under</option>
<option value='0' tempHL0>over</option></select>
&nbsp:&nbsp&nbsp <input class='inp2' name='switchTemp' value='+00' size='5'  required pattern='^([+-]{1}[0-9]{1,2}[.][0-9])$' title=' bijv. +09.4'>&nbsp&deg;C
<p>Attention: Enter always a + of - in for the temperature, i.g. +09.3
<br><br>Enter domoticz sensor idx if necessary under the mosquitto menu.</p>
 )====="; 

 const char SENSOR_HS[] PROGMEM = R"=====( 
<h4>HYGROSTATE</h4>
value last measurement : n/a %<br><br>
Switch on when the humidity is:<br><br>
<select name='hygHL' class='sb1'>
<option value='0' hygHL0>over</option>
<option value='1' hygHL1>under</option></select>
&nbsp:&nbsp&nbsp<input class='inp1' name='switchMoist' value='000A' size='2' maxlength='2' required pattern='^([1-9]{0,1})([0-9]{1})?$' title='tussen 1 en 99'>&nbsp%
<br><p>Enter domoticz sensor idx if necessary under the mosquitto menu.</p></p>
 )====="; 

 const char SENSOR_MS[] PROGMEM = R"=====(
<h4>MOTION SENSOR:</h4>
Set the behaviour of the sensor.<br>When are mqtt messages sent<br>and when is switched on.<br><br>
<table><tr><td style=width:110px>behaviour:<td><select name='behBS' class='sb1'>
<option value='0' bew0>night: lamp and message</option>
<option value='1' bew1>night: only message</option>
<option value='2' bew2>night: lamp, always message</option>
<option value='3' bew3>always: lamp and message</option>
<option value='4' bew4>always: only message</option>
</select></tr>
<tr><td>on time<td><input class='inp2' name='cdwn' value='{cdwn}' pattern='(?:[01]|2(?![4-9])){1}[0-9]{1}:[0-5]{1}[0-9]{1}'
title='hh:mm'></table>
<p>NB: Night is betweeen sunset and sunrise.<br>
the on time determines how long the lamp stays on after motion detection
</p></div>
 )====="; 

 const char SENSOR_LS[] PROGMEM = R"=====(
<h4>LIGHT/DARK SWITCH</h4>
value last measurement : n/a Lux<br><br>
Turn on when the ambient light is<br><br>
<select name='lichtHL' class='sb1'>
<option value='0' lichtHL0>over</option>
<option value='1' lichtHL1>under</option></select>
&nbsp:&nbsp&nbsp<input class='inp2' name='switchLicht' value='{00000}' size='5' maxlength='5' required pattern='^(0|[1-9][0-9]{0,4})$' title='tussen 1 en 99999'>&nbspLux
<br><p>Tip: 100.000 lux is direct sunlight, 0 is absolute darkness.
<br>Enter domoticz sensor idx if necessary under the mosquitto menu.</p></div>
)====="; 

 const char SENSOR_DS[] PROGMEM = R"=====(
<h4>GENERIC DIGITAL SENSOR</h4>The digital sensor is high or low when triggered. <br>
Turn on when the sensor state is:<br><br>
<select name='digitalHL' class='sb1'>
<option value='1' digitalHL1>high</option>
<option value='0' digitalHL0>low</option></select>
<br><p>Tip: Use a pull-up resistance if necessary
<br>Enter domoticz sensor idx if necessary under the mosquitto menu.</p></div>
 )====="; 

void zendPageSensors() {
DebugPrintln("sendPageSensors");
String webPage = FPSTR(HTML_HEAD);
webPage += FPSTR(SENSORCONFIG_START);

    //toSend.replace("tieTel", dvName);
    //toSend.replace("HANSIART" , String(dvName)); 
// kunnen we standaard de sensor pagina openen?
   tKeuze=16;
   zendPageRelevantSensors();
//   toSend.replace("<irame name>" , FPSTR(SENSOR_WS));
//   replace_sensordata(); // is voor het goedzetten van de timer select

//request->send(200, "text/html", toSend); //send the html code to the client
 
}

// we gaan nu een pagina tonen met de voor te timer relevante inputs
void zendPageRelevantSensors() {
  
toSend = FPSTR(HTML_HEAD);
toSend += FPSTR(SENSORCONFIG_START);
    //toSend.replace("tieTel", dvName);
    //toSend.replace("HANSIART" , String(dvName)); 

    DebugPrintln("zendPageRelevantSensors");
    DebugPrint("tKeuze = "); DebugPrintln(String(tKeuze));
    // we gaan nu de gegevens in de getoonde pagina goed ztten, afhankelijk van tKeuze  
    place_sensorpage();
  
//    server.send(200, "text/html", toSend); //send the html code to the client 
//    delay(500); //wait half a second after sending the data 
}



// deze functie wordt opgeroepen na timer instellingen
//*******************************************************************************************
//              voorbereiden voor opslaan van de gegevens
// *****************************************************************************************

//void handleSensorconfig(AsyncWebServerRequest *request) { // form action = handletimerConfig
//// we krijgen alleen severargumenten van de betreffende timer bijv zonattaan0 dus
////lees de serverargumenten en plaats deze in de betreffende variabelen
//
//String temp = "";
//switch (tKeuze) {
//case 11:    
//// thermostaat
//   if (request->arg("tempHL") == "1") {
//    switchHL[0] = '1'; } else {switchHL[0]='0';}
//    strcpy(switchTemp, request->arg("switchTemp").c_str());
//   break;   
// case 12:    
//// hygrostaat
//   if (request->arg("hygHL") == "1") {
//       switchHL[1] = '1'; } else {switchHL[1]='0';}
//   strcpy(switchMoist, request->arg("switchMoist").c_str());
//   break;
// case 13:    
//// motionsensor
//   strcpy(BS, request->arg("behBS").c_str());
//   strcpy(switchcdwn, request->arg("cdwn").c_str());
//   break;
// case 14:   
//// lichtsensor
//   if (request->arg("lichtHL") == "1") {
//    switchHL[2] = '1'; } else {switchHL[2]='0';}   
//    strcpy(switchLicht, request->arg("switchLicht").c_str());
//   break;
// case 15:    
//// digital sensor
//   if (request->arg("digitalHL") == "1") {
//      switchHL[3] = '1'; } else { switchHL[3] = '0'; }
//   break;
//  case 16:    
//// sensor keuze en calibratie en meetresolutie
//   strcpy(sensor, request->arg("sensor").c_str());
//   strcpy(tempCal, request->arg("tcal").c_str());
//   meetRes = request->arg("mf").toInt();
//   break;
//}
//// deze funties werken
// 
// //DebugPrintln("we gaan een nieuwe webpage inlezen in toSend");
// //toSend = FPSTR(CONFIRM);
// // toSend = html3; // de confirm pagina
// // de ip adreswaarden vervangen en wat zaken toevoegen
//  //toSend.replace("HAIPS" , String(swName));
//  
// //request->send_P(200, "text/html", CONFIRM); //send the html code to the client
//
//
// 
// check_mismatch(); // als timer en sensor niet in overeenstemming dan wordt timer nul
// timerConfigsave(); // alles opslaan in SPIFFS
// basisConfigsave(); // voor sensor, tempcal en meetres 
// DebugPrintln("sensor settings saved");
//
//actionFlag = 10; // reboot
//  }


void place_sensorpage() {
    if (tKeuze == 11) {
    // thermostaat 
        toSend.replace("<irame name>" , FPSTR(SENSOR_TS)); 
        // het plusteken komt niet tevoorschijn bij temp_aan dus vallen we terug op de tempaan uit spiffs
        DebugPrint("switchTemp = ");
        DebugPrintln(String(switchTemp));
        // de temperatuurgegevens terugzetten
        if (sensor[0] == '1' || sensor[0] == '2' || sensor[0] == '3') {
           toSend.replace("n/a &#8451;" , String(temp_c,1) + " &#8451;");
        }
        toSend.replace("value='+00'", "value='" + String(switchTemp) + "'");
        if (switchHL[0] == '0') {
              toSend.replace("tempHL0" , "selected"); //hoger
        } else {
          toSend.replace("tempHL1" , "selected");
        }
    }
    if (tKeuze == 12) {
    // hygrostaat 
        toSend.replace("<irame name>" , FPSTR(SENSOR_HS));
        // de vochtigheidsgegevens terugzetten
        if (sensor[0] == '2' || sensor[0] == '3') {
        toSend.replace("n/a %" , String(humidity,1) + " %");
        }
        toSend.replace("value='000A'", "value='" + String(switchMoist) + "'");
        if (switchHL[1] == '0') {
              toSend.replace("hygHL0" , "selected"); //hoger
        } else {
          toSend.replace("hygHL1" , "selected"); // lager
        }
    } 
    if (tKeuze==13) {
    //motion sensor
          toSend.replace("<irame name>" , FPSTR(SENSOR_MS));  
          // de bewegingssensor gegevens terugzetten
          switch (BS[0]) {
          case '0': 
              toSend.replace ("bew0", "selected");
              break;
          case '1': // 
              toSend.replace ("bew1", "selected");
              break;
          case '2':  
              toSend.replace ("bew2", "selected");
              break;
          case '3':
              toSend.replace ("bew3", "selected");
              break;
          case '4':
              toSend.replace ("bew4", "selected");
              break;    
          } 
          toSend.replace("{cdwn}", "" + String(switchcdwn));
    }
    
    if (tKeuze == 14){
      //licht sensor
    toSend.replace("<irame name>" , FPSTR(SENSOR_LS));  
     // de lichtgegevens terugzetten
       if (sensor[0] == '6') {
       toSend.replace("n/a Lux", String(p,1) + " Lux<br>");
       }
       toSend.replace("{00000}", String(switchLicht));
       if (switchHL[2] == '0') {
          toSend.replace("lichtHL0" , "selected"); //hoger
       } else {
          toSend.replace("lichtHL1" , "selected"); // lager
       } 
    }  
    
    if (tKeuze==15) { 
        //sensor keuze
        toSend.replace("<irame name>" , FPSTR(SENSOR_DS));  
        // de digital sensor gegevens terugzetten
        if (switchHL[3] == '0') {
            toSend.replace("digitalHL0" , "selected"); //hoger
        } else {
            toSend.replace("digitalHL1" , "selected"); // lager
        }   
    }
    
    if (tKeuze==16) { 
        //de gekozen sensor hardware
        toSend.replace("<irame name>" , FPSTR(SENSOR_WS));  
        // de gekozen optie voor sensor terugzetten
        switch (sensor[0]) {
          case '0': // geen
        toSend.replace ("sensor0", "selected");
            break;
          case '1': // bmp
        toSend.replace ("sensor1", "selected");
            break;
          case '2': // dht 
        toSend.replace ("sensor2", "selected");
            break;
          case '3': // bme
        toSend.replace ("sensor3", "selected");
            break;
          case '4': // bewegingssensor
        toSend.replace ("sensor4", "selected");
            break;
          case '5': // drukknop
        toSend.replace ("sensor5", "selected");
            break;
          case '6': // light sensor
        toSend.replace ("sensor6", "selected");
            break; 
          case '7': // digital sensor
        toSend.replace ("sensor7", "selected");
            break;      
          }
        // domoticz link gegevens terugzetten
        toSend.replace("name='tcal' value='+00'", "name='tcal' value='" + String(tempCal) + "'");
        toSend.replace("name='mf' value='300'", "name='mf' value='" + String(meetRes) + "'");
    }
}  
