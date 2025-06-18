//{check value
const char MQTTCONFIG[] PROGMEM = R"=====(
<body>
<div id='msect'>
  <div id='help'>
  <span class='close' onclick='sl();'>&times;</span><h3>MOSQUITTO HELP</h3>
  Mosquitto can be used to send short messages back and forth to an mqtt server (broker).
  With these messages you can switch or update sensors. <br><br>
    <b>address:</b><br>The (ip) address of the mqtt-broker cq domoticz<br><br>
  <b>mqtt messages:</b><br>Outgoing sensor update: {"idx":123,"nvalue":0,"svalue":"123.45"}<br>
  outgoing switch update: {"command":"switchlight","idx":456,"switchcmd":"On or Off"}<br>
  Listens to json messages like {"idx":123,"cmd":"Off"}
  </div>
</div>

<div id='msect'>
<div id='menu' style="align-items:left;">
<a href="#" class='close' onclick='cl();'>&times;</a>
<a href="#" id="sub" onclick='submitFunction()'>save</a>
<a href='/RUNACTION?mosqtest'>test</a>
<a href='#' onclick='helpFunction();'>&quest;</a>
</div></div>
<div id='msect'><kop>SONOFF MOSQUITTO</kop></div>
<div id='msect'>
  <div class='divstijl'>
  <center>
  <form id='formulier' method='get' action='submitForm' oninput='showSubmit();'>
  
  <table>
  <tr><td style='width:130px;'>enable ?<td><input type='checkbox' style='width:30px; height:30px;' name='mqtEn' #check></input></tr>
  <tr><td >address<td><input class='inp6' name='mqtAdres' value='{mqttAdres}' size='31' placeholder='broker adres'></tr>
  <tr><td >port<td><input class='inp2' name='mqtPort' value='{mqttPort}' size='31' placeholder='mqtt port'></tr>
  
  <tr><td>topic:&nbsp<td><input class='inp6' name='mqtoutTopic' value='{mqttoutTopic}' placeholder='mqtt topic receive' length='60'></tr>
  <tr><td>intopic:&nbsp<td><input class='inp6' readonly name='mqtinTopic' value='{mqttinTopic}' length='60'></tr>
  <tr><td>switch idx:&nbsp<td><input class='inp2' name='swidx' value='{swidx}' size='4' length='4'></tr>
  <tr><td>sensor idx:&nbsp<td><input class='inp2' name='seidx' value='{seidx}' size='4' length='4'></tr> 
  </form>
  </td></table>
  </div><br>
</div>
<script>
function helpFunction() {
alert('mosquitto send formats: \nFormat 1:{\"idx\":917,\"nvalue\":0}\nFormat 2: {\"idx\":917,\"state\":\"on\"}');   
}
</script>
</body></html>
)=====";
  //<li><a href='/MQTT_TEST' >test</a></ul>

void zendPageMQTTconfig(AsyncWebServerRequest *request) {

String webPage;
webPage = FPSTR(HTML_HEAD);
webPage += FPSTR(MQTTCONFIG);  

  //altijd de mqtt gegevens terugzetten
if (mqttEnabled){ webPage.replace("#check","checked"); } 
webPage.replace("{mqttAdres}",    String(mqttBroker)   );
webPage.replace("{mqttPort}",     String(mqttPort)     );
webPage.replace("{mqttoutTopic}", String(mqttOuttopic) );
String sub = getChipId(false) + "/in";
webPage.replace("{mqttinTopic}", sub );
//webPage.replace("{mqtu}",         String(Mqtt_Username) );
//webPage.replace("{mqtp}",         String(Mqtt_Password) );
webPage.replace("{swidx}"          , String(idxSwitch) );
webPage.replace("{seidx}"          , String(idxSensor) ); 
//webPage.replace("{mqtc}"         , String(Mqtt_Clientid) );

    request->send(200, "text/html", webPage);
    webPage="";
}

//void handleMQTTconfig(AsyncWebServerRequest *request) {
//
//  //collect serverarguments
//  mqttBroker = request->arg("mqtAdres");
//  //DebugPrint("mqttBroker na strcpy = "); DebugPrintln(mqttBroker); // oke
//
//  mqttPort = request->arg("mqtPort");
//  //DebugPrint("mqttPort = "); DebugPrintln(mqttPort); // ok
//
//  mqttIntopic = request->arg("mqtinTopic");
//  //DebugPrint("mqttIntopic na strcpy = "); DebugPrintln(String(mqttIntopic)); // oke
//
//  mqttOuttopic = request->arg("mqtoutTopic");
//  //DebugPrint("mqttOuttopic na strcpy = "); DebugPrintln(String(mqttOuttopic)); // oke
//  
////  Mqtt_Username = request->arg("mqtUser");
////  DebugPrint("Mqtt_Username na strcpy = "); DebugPrintln(Mqtt_Username); // oke
////
////  Mqtt_Password = request->arg("mqtPas");
////  DebugPrint("Mqtt_Username na strcpy = "); DebugPrintln(Mqtt_Password); // oke
//
//  idxSwitch = request->arg("mqidx1").toInt();
//  idxSensor = request->arg("mqidx2").toInt();   
//// de selectbox
//   char tempChar[1] = "";
//   String dag = request->arg("mqtEn");  // mqselect
//   if ( dag == "on") { mqttEnabled = true; } else { mqttEnabled = false; }
//
//  DebugPrintln("saved mqttconfig");
//  mqttConfigsave();  // 
//  actionFlag = 10; // takes care for reboot
//}

//} check value
