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
  consoleOut("wifi connected, ip = ");
  Serial.println(WiFi.localIP());

  
  start_asyserver();
}
