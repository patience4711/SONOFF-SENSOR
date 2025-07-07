
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
  <center><table><tr><td style='width:140px;'>active automation<td style='width:280px;'><select name='autoMate' id='timer' class='sb1'>
    <option value='0' tM0>none</option>
    <option value='1' tM1>on/off timers</option>
    <aption1 value='3' tM3>thermostate</option>
    <aption2 value='4' tM4>hygrostate</option>
    <aption3 value='6' tM6>light sensor</option>
    <aption4 value='7' tM7>digital sensor</option>
    </select></td></tr></table>
  <irame name>
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
<table><tr><td style='width:140px;'>used sensor<td style='width:280px'><select class='sb1' name='sensor'> 
<option value='0' sensor0>no sensor</option> 
<option value='1' sensor1>DS18B20 sensor</option>
<option value='2' sensor2>DHT22 sensor</option>
<option value='3' sensor3>BME280 sensor</option>
<option value='4' sensor4>motionsensor</option>
<option value='5' sensor5>push/touchbutton</option>
<option value='6' sensor6>MAX44009 lightsensor</option>
<option value='7' sensor7>generic digital sensor</option>
</select>
<tr><td>temp. calibration<td><input onchange="subMit()" class='inp2' name='tcal' value='+00' size='4'  required pattern='^([+-]{1}[0-9]{1}[.][0-9])$' title=' i.g. +02.4 or -1.2'>
</tr>
<tr><td style='width:140px;'>measure interval<td><input class='inp2' type='number' min='10' max='3600' name='mf' value='300' size='4' >
</tr></table>
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
&nbsp:&nbsp&nbsp <input class='inp2 hoogte'  name='switchTemp' value='+00' size='5'  required pattern='^([+-]{1}[0-9]{1,2}[.][0-9])$' title=' bijv. +09.4'>&nbsp&deg;C
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
<table><tr><td style=width:110px>behaviour:<td style='width:280px'><select name='behBS' class='sb1'>
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
consoleOut("sendPageSensors");
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

    consoleOut("zendPageRelevantSensors");
    consoleOut("tKeuze = " + String(tKeuze));
    // we gaan nu de gegevens in de getoonde pagina goed ztten, afhankelijk van tKeuze  
    place_sensorpage();
  
//    server.send(200, "text/html", toSend); //send the html code to the client 
//    delay(500); //wait half a second after sending the data 
}






void place_sensorpage() {
    if (tKeuze == 11) {
    // thermostaat 
        toSend.replace("<irame name>" , FPSTR(SENSOR_TS)); 
        consoleOut("switchTemp = " + String(switchTemp));

        // show the thermometer readings
        if (senSor == 1 || senSor == 2 || senSor == 3) {
           toSend.replace("n/a &#8451;" , String(temp_c,1) + " &#8451;");
        }
        //toSend.replace("value='+00'", "value='" + String(switchTemp) + "'");
        toSend.replace("value='+00'", "value='" + String(switchTemp,1) + "'");
        if (bitRead(switchHL, 0) == false) {
              toSend.replace("tempHL0" , "selected"); //hoger
        } else {
          toSend.replace("tempHL1" , "selected");
        }
    }
//replace the select for active automation
 replace_automationSelect();  //will it work? 
    
    if (tKeuze == 12) {
    // hygrostaat 
        toSend.replace("<irame name>" , FPSTR(SENSOR_HS));
        // show the humidity value
        if (senSor == 2 || senSor == 3) {
        toSend.replace("n/a %" , String(humidity,1) + " %");
        }
        //toSend.replace("value='000A'", "value='" + String(switchMoist) + "'");
        toSend.replace("value='000A'", "value='" + String(switchMoist, 1) + "'");
        if (bitRead(switchHL, 1) == false) {
              toSend.replace("hygHL0" , "selected"); //hoger
        } else {
          toSend.replace("hygHL1" , "selected"); // lager
        }
    } 
    if (tKeuze==13) {
    //motion sensor
          toSend.replace("<irame name>" , FPSTR(SENSOR_MS));  
          // de bewegingssensor gegevens terugzetten
          switch (Bds) {
          case 0: 
              toSend.replace ("bew0", "selected");
              break;
          case 1: // 
              toSend.replace ("bew1", "selected");
              break;
          case 2:  
              toSend.replace ("bew2", "selected");
              break;
          case 3:
              toSend.replace ("bew3", "selected");
              break;
          case 4:
              toSend.replace ("bew4", "selected");
              break;    
          } 
          toSend.replace("{cdwn}", "" + String(switchcdwn));
    }
    
    if (tKeuze == 14){
      //licht sensor
    toSend.replace("<irame name>" , FPSTR(SENSOR_LS));  
     // show the lux value
       if ( senSor == 6) {
       toSend.replace("n/a Lux", String(p,1) + " Lux<br>");
       }
       //toSend.replace("{00000}", String(switchLicht));
       toSend.replace("{00000}", String(switchLicht, 0 ));
       if (bitRead(switchHL, 2) == false) {
          toSend.replace("lichtHL0" , "selected"); // hoger
       } else {
          toSend.replace("lichtHL1" , "selected"); // lager
       } 
    }  
    
    if (tKeuze==15) { 
        //sensor keuze
        toSend.replace("<irame name>" , FPSTR(SENSOR_DS));  
        // de digital sensor gegevens terugzetten
        if (bitRead(switchHL, 3) == false) {
            toSend.replace("digitalHL0" , "selected"); //hoger
        } else {
            toSend.replace("digitalHL1" , "selected"); // lager
        }   
    }
    
    if (tKeuze==16) { 
        //de gekozen sensor hardware
        toSend.replace("<irame name>" , FPSTR(SENSOR_WS));  
        // put back the selected option for senSor 
        switch (senSor) {
          case 0: toSend.replace ("sensor0", "selected"); break; // none
          case 1: toSend.replace ("sensor1", "selected"); break;// bmp
          case 2: toSend.replace ("sensor2", "selected"); break;// dht 
          case 3: toSend.replace ("sensor3", "selected"); break; // bme
          case 4: toSend.replace ("sensor4", "selected"); break;// bewegingssensor
          case 5: toSend.replace ("sensor5", "selected"); break;// drukknop
          case 6: toSend.replace ("sensor6", "selected"); break; // light sensor
          case 7: toSend.replace ("sensor7", "selected");break;// digital sensor
        }
        // domoticz link gegevens terugzetten
        toSend.replace("name='tcal' value='+00'", "name='tcal' value='" + String(tempCal) + "'");
        toSend.replace("name='mf' value='300'", "name='mf' value='" + String(meetRes) + "'");
    }
}

// *********************************************************************************
//          replace the selectbox for active automation only (sensors)
// **********************************************************************************
void replace_automationSelect() {  // wordt door basisconfig gebruikt
// make the available options visible

// if there is a temperaturesensor then we have to show the button for settings
// and also the option for thermostate in the selectbox
if (senSor == 1 || senSor == 2 || senSor == 3) {
   consoleOut("de waarde van senSor is 1  2 of 3");
//  toSend.replace("'thermostaat' type='hidden'", "'thermostaat' type='button'");
  toSend.replace("aption1", "option"); // 
  } 
 // if there is a dht or BME connected, also the hygrostate
if (senSor == 2 || senSor == 3) {
  toSend.replace("aption2", "option"); //  
}
//if (sensor[0] == '4') {
//  toSend.replace("aption5", "option"); // de option voor de motion sensor

if ( senSor == 6 ) {
  toSend.replace("aption3", "option"); // the option for the light sensor
}
if ( senSor == 7 ) {
  toSend.replace("aption4", "option"); // the option for the digital sensor
}
  // put back the selected option in the select
  switch( autoMate ){
      case 0: toSend.replace ("tM0", "selected"); break;
      case 1: toSend.replace ("tM1", "selected"); break; 
      case 3: toSend.replace ("tM3", "selected"); break;
      case 4: toSend.replace ("tM4", "selected"); break;
    // not needed for the motion sensor, that always works when selected
      case 6: toSend.replace ("tM6", "selected"); break;
      case 7: toSend.replace ("tM7", "selected"); break;  
    }

}  
