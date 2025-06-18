// ************************************************************************************
// *                        START wifi
// ************************************************************************************
void start_wifi() {
int event = 0;
 WiFi.softAPdisconnect(true);
 WiFi.mode(WIFI_STA);
 WiFi.hostname(dvName);

 WiFi.mode(WIFI_STA); // geen ap op dit moment 

// we gaan 10 pogingen doen om te verbinden
// met de laatst gebruikte credentials
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print("*");
     event+=1;
     if (event==10) {break;}
  }
// als het verbinden is mislukt gaan we naar het configportal  
  if (event>9) {
     event=0;

     start_portal(); // stel het portal in
     }
event=0; // we kunnen door naar de rest
//checkFixed();
  consoleOut("wifi connected, ip = ");
  Serial.println(WiFi.localIP());

  
  start_asyserver();
}

//// ********************************************************************
////             controleer if er een static ip moet komen
//// ********************************************************************
//void checkFixed() {
//  // we komen hier alleen als we zijn verbonden
//    // we gaan GATE_WAY van een waarde voorzien, die is niet opgeslagen in SPIFFS
//    char GATE_WAY[16]="";
//    IPAddress gat=WiFi.gatewayIP();
//    sprintf(GATE_WAY, "%d.%d.%d.%d", gat[0], gat[1], gat[2], gat[3]);
//    DebugPrint("GATE_WAY in checkFixed = nu: "); DebugPrintln(String(GATE_WAY));
//    DebugPrint("static_ip in checkFixed = nu: "); DebugPrintln(String(static_ip));
//
//    if (static_ip[0] != '\0' && static_ip[0] != '0') {
//      DebugPrintln("we moeten een static ip hebben Custom STA IP/GW/Subnet");
//      IPAddress _ip,_gw,_sn(255,255,255,0); // declaratie 
//      _ip.fromString(static_ip);
//      _gw.fromString(GATE_WAY);//  if (ssid != "") {
//      WiFi.config(_ip, _gw, _sn);
//      DebugPrintln(WiFi.localIP());
//  } else {
//      DebugPrintln("proberen wificonfig kwijt te raken");
//      WiFi.config(0u, 0u, 0u);     
//  }
//}
