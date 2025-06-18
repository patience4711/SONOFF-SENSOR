//<script>
//var interval = 100;
//setInterval(function loadTime() {
//  var xhttp = new XMLHttpRequest();
//  xhttp.onreadystatechange = function() {
//    if (this.readyState == 4 && this.status == 200) {
//      var antwoord = this.responseText;
//      var obj = JSON.parse(antwoord);
//      var hr = obj.uur;
//      var mn = obj.min;
//      var tijd= hr + ":" + mn;
//      document.getElementById('tijdveld').innerHTML=tijd;
//      }
//  };
//  xhttp.open("GET", "get.currentTime", true);
//  xhttp.send();
//  interval=2000;
//},interval);

const char INFOPAGE [] PROGMEM = R"=====(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta http-equiv="refresh" content="60">
<meta charset='utf-8'>
<link rel="icon" type="image/x-icon" href="/favicon.ico" />

<title>ESP INFO</title>
<style>
table, th, td {border: 1px solid blue; width:font-size:12px;} 
body {margin:10px 10px 0px 20px; font-family:'lato',Verdana,Sans-serif;font-size:12px;}
.main-sect {
  margin: 0px 0px;
  width:96vw;
}
</style>
</head><body onload='loadTime()'>
<div id='msect'>
)=====";

void handleInfo(AsyncWebServerRequest *request) {
DebugPrintln("we zijn op handleInfopage() ");

toSend = FPSTR(INFOPAGE);
 char temp[100]={0};
toSend.replace("tieTel", dvName );

toSend+="<center><h2>STATUS INFORMATION</h2></center>";
toSend+="<center><div style='position:fixed; margin: auto; left: 0; right: 0'>";
toSend +="<a href='/'> <button style='width:150px; height:50px;text-align:center; font-weight:bold; font-size:20px; background:#db97b9;'>close</button></a></div>";

  String zt = "summertime";
  switch (dst) {
    case 2: zt="wintertime"; break;
    case 0: zt="no dst set"; break;
  }
  sprintf( temp, "<tr><td>systemtime<td> %d:%d" , hour(), minute() );
  toSend += "<br><br><br><br><span style='font-size:20px;font-weight:bold'>&nbsp;" + String(temp) + zt + "</span></tr><br>";

  DebugPrintln("fase 0");

#ifdef SENSORS
toSend += "force measuring and switch by the sensor <a href='/METEN'><button style='width:80px; height:30px;text-align:center; font-weight:bold; font-size:14px; background:#42f5ec;'>measure</button><br></a>"; 
#endif

toSend += "firmware version : ";
#ifdef SENSORS
toSend += "Sonoff-v10_0_SENSORS<br>";
#endif
#ifdef ESP01_FET
toSend += "ESP01-v10_0_FET<br>";
#endif
#ifdef ESP01_TRIAC
toSend += "ESP01-v10_0_TRIAC<br>";
#endif
#ifdef S20
toSend += "SONOFF-v10_0_S20<br>";
#endif
#ifdef MINI
toSend += "SONOFF-v10_0_MINI<br>";
#endif
#ifdef SONOFF
toSend += "SONOFF-v10_0_SONOFF<br>";
#endif
toSend += "time retrieved today : "; if ( timeRetrieved ) { toSend += "yes<br>"; } else { toSend += "no<br>"; }

#ifdef SENSORS 
if ( (temp_c == 1000 || temp_c == -127 ) && (sensor[0]=='1' || sensor[0]=='2' || sensor[0]=='3') ) { 
  toSend += "ATTENTION: the temperaturesensor is not working!<br>";
  } else { 
      if (sensor[0] == '1') { 
      toSend += "Temperature = " + String(temp_c,1) + " &#8451; <br>";
       }
      if (sensor[0] == '2') {
      toSend += "temperature: " + String(temp_c, 1) + " &#8451;" + "   humidity: " + String(humidity,1)+"%<br>";
      }
      if (sensor[0] == '3') {
      toSend += "temperature: " + String(temp_c,1) + " &#8451;" + "  humidity: " + String(humidity,1)+"%  pressure: " + String(p,1) + " hPascal " + ldString + "<br>";
      }
  }
if ( sensor[0] == '6' ) {
   if ( p == 500000 ) { toSend += "ATTENTION: the lightsensor is not working!<br>";
   } else { toSend += "the amount of ambient light =: " + String(p,1) + " Lux<br>";}
}
DebugPrintln("fase 2");

if (sensor[0] == '7') {
  toSend += "generic sensor state = : " + String(digitalRead(3)) + "<br>";
}

#endif
//if ( optelBool ) {
//    // ingestelde zetAan
//    int aanzet = zetAan[0];
//    toSend += "the count-up timer switches on over " + String(zetAanschakeltijd - now()) + " seconds<br>";
//}
if ( mqttEnabled ) { //bool == y en er is een mqtt adres, ja kijk dan of er een sensor is ingesteld
// check of connected
    String clientId = "ESPClient#";
    clientId += (String(ESP.getChipId()));
    toSend += "de mqtt clientId = : " + clientId + "<br>";    
    if ( MQTT_Client.connected() ) 
    {
       toSend += "status mqtt connection : connected to " + String(mqttBroker) + "<br>";
    } else { 
       toSend += "status mqtt connection : not connected to " + String(mqttBroker) + "<br>";
    }
} else {
 toSend += "mqtt not enabled/configured!<br>";  
}

long rssi = WiFi.RSSI();
toSend += "the signaalstrength of the router = " + String(rssi) + "<br>";
// de variable event zegt iets over de toestand van de switch
// 1 == aan drukknop  3 = aan webpage 5 = aan timer 1 6 = aan timer 2
// 2 == uit drukknop  4 = uit webpage 7 = uit timer 1 8 = uit timer 2


if ( value == 1 || value == 2 || value == 17 || value == 18) { // handmatig ingeschakeld, zetAan of bewegingssensor
    String digit = "";
    int minuutje = minute(switchOnmoment);
    if  ( minuutje<10 ) { digit = "0"; } else {digit = ""; }
    toSend += "  time switched on : " + String(hour(switchOnmoment)) + ":" + digit + String(minute(switchOnmoment)) + "<br>";
    #ifdef SENSORS
    if ( value == 18 || timer[0] == '2' ) { // ingeschakeld door bewegingssensor, zoniet handmatig en dan moet de cdwn zijn ingeschakeld
            time_t uitschakeltijd = switchOnmoment + ((cdwnuur * 60 + cdwnminuut) * 60) ; // aantal seconden
            int minuutje = minute(uitschakeltijd);
            if  ( minuutje < 10 ) { digit = "0"; } 
            toSend += "  there will be switched off by the bodysensor at " + String(hour(uitschakeltijd)) + ":" + digit + String(minute(uitschakeltijd)) + "<br>";
        } 
    #endif    
    if ( value != 18) {// manual switched on (not by motionsensor) 
            time_t uitschakeltijd = switchOnmoment + (asouur * 60 + asominuut) * 60 ; // aantal seconden
            int minuutje = minute(uitschakeltijd);
            if  ( minuutje < 10 ) { digit = "0"; } else {digit = ""; }
        toSend += "  switched off time by security: " + String(hour(uitschakeltijd)) + ":" + String(minute(uitschakeltijd)) + "<br>";
    }
}
  int minutens = millis()/60000;
  int urens = minutens/60;
  int dagen = urens/24;
toSend += "system up-time: " + String(dagen) + " days " + String(urens-dagen*24) + " hours " + String(minutens - urens*60) + " min.<br> ";

toSend += "<br><table><TR><TH> SETTINGS</th></TR>"; 
toSend += "<tr><td>longitude:<td>" + String(longi,3) + "&deg<td> latitude:<td> " + String(lati,3) + "&deg"; // 2

time_t sunrisetijd = (sunrise - dagbegintijd)/60; //uitgedrukt in minuten voor zendpage
time_t sunsettijd = (sunset - dagbegintijd)/60; //uitgedrukt in minuten voor zendpage
 String ssuur = "0" + String(int(sunrisetijd/60)); // altijd onder 10 dus er moet een nul bij
 String ssmin = String(int(sunrisetijd -(int(sunrisetijd/60)*60)));
if (int(sunrisetijd -(int(sunrisetijd/60)*60)) < 10){
    //ssmin = "0" + int(sunrisetijd -(int(sunrisetijd/60)*60)))
    ssmin = "0" + ssmin;
}
toSend += "<tr><td>sunrise:<td> " + ssuur + ":" + ssmin + " hr.";
// nu voor de sunset
 ssuur = String(int(sunsettijd/60)); // is altijd > 10 dus geen nul nodig
 ssmin = String(int(sunsettijd -(int(sunsettijd/60)*60)));
if (int(sunsettijd -(int(sunsettijd/60)*60)) <  10) {
     //ssmin = "0" + int(sunrisetijd -(int(sunrisetijd/60)*60)))
     ssmin = "0" + ssmin;
}
toSend += "<td> sunset: <td>" + ssuur + ":" + ssmin + " hr.";
toSend += "<tr><td>gmtOffset:&nbsp<td>" + String(gmtOffset) + " min.<td>mooncycle: dag<td> " + String(mDay) + "<tr><td>maangestalte:<td>"  + maan + "<td><td></table>";

toSend += "<br><table><tr><TH> ESP INFORMATION</th></tr>";
toSend += "<tr><td>ESP CHIP ID nr: <td>" + String(ESP.getChipId());
toSend += "<td>ESP Mac adres: <td>"  + String(WiFi.macAddress()) + "</tr>";
toSend += "<tr><td>IDE Flash size: <td>" + String(ESP.getFlashChipSize()) + " bytes";
toSend += "<td>Echte Flash size<td>" +  String(ESP.getFlashChipRealSize()) + " bytes</tr>";
toSend += "<tr><td>Free memory<td>" +  String(ESP.getFreeHeap()) + " bytes<td><td>" + "</table>";


toSend += "<h3>variables dump</h3>";
//toSend += "mqttIntopic : " + mqttIntopic + "  mqttOuttopic : " + mqttOuttopic + "<br>";

toSend += "timer[0]=" + String(timer[0]) + "<br>";
toSend += "sensor[0]=" + String(sensor[0]) + "   BS[0] = " + String(BS[0]) + "   donker=" + String(donker()) +  "   boodschap=" + String(boodschap())  + "   bwswitch=" + String(bwswitch()) +  "<br>";
//toSend += "meetRes = " + String (meetRes) + "   ";
toSend += "switchHL 0=temp 1=moist 2=licht 3=digital" + String(switchHL) + "   meetRes" + String(meetRes) + "<br>";


//toSend += "dns ip 1 : " + WiFi.dnsIP().toString() + "  dns IP 2 : " + WiFi.dnsIP(1).toString()  + "<br>";
toSend += "dts=" + String(dts) + "   tKeuze = " + String(tKeuze) + "<br>";
toSend += "relToSunOn = " + String(relToSunOn) + "  relToSunOff = " + String(relToSunOff) + "<br>"; 
toSend += "switchOn = " + String(switchOn) + "<br>";
toSend += "switchOff = " + String(switchOff) + "<br>";
toSend += "weekDag = " + String(weekDag) + "<br>";
toSend += "mustSwitch = " + String(mustSwitch[0]) + String(mustSwitch[1]) + String(mustSwitch[2]) + String(mustSwitch[3]) + "<br>";
toSend += "hasSwitched = " + String(hasSwitched[0]) +String(hasSwitched[1]) + String(hasSwitched[2]) + String(hasSwitched[3]) + "<br>";

toSend += "pin 3 (tx) = " + String(digitalRead(3))  + "   value = " + String(value) ;

toSend += "<h3>Contents filesystem :</h3>";
Dir dir = LittleFS.openDir("/");
while (dir.next()) {
    toSend += String(dir.fileName()) + "   size(bytes) ";
    File f = dir.openFile("r");
    toSend += String(f.size()) + "<br>";
}
toSend += "<h4>calculated timer properties</h4>";
toSend += "unix day starttime = " + String(dagbegintijd) + "<br>"; 
for(int aantal=0; aantal<TIMERCOUNT; aantal ++ ) {
if(timerActive[aantal] == '1') {
      toSend += "timer : " + String(aantal) + "]";
      toSend += "<br>switchOnTime[] = ";
      int Uur=hour(inschakeltijd[aantal]); 
      int Min=minute(inschakeltijd[aantal]);
      int Dag=day(inschakeltijd[aantal]); 
      toSend += String(Dag) + "/" + String(Uur) + ":" + String(Min) + "  ";    
      
      toSend += "<br>switchOfTime[] = ";
       Uur=hour(uitschakeltijd[aantal]); 
       Min=minute(uitschakeltijd[aantal]);
       Dag=day(uitschakeltijd[aantal]);
      toSend += String(Dag) + "/" + String(Uur) + ":" + String(Min) + "  ";  
      toSend += "<br>****************************************<br>";
  }
}
toSend += "</body></html>"; 
 //DebugPrintln("end infopage "); 
 //DebugPrint("de lengte van toSend na de infopage = "); DebugPrintln( toSend.length() );
 request->send(200, "text/html", toSend); //send the html code to the client

 
}
