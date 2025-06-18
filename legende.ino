// it programma is afgeleid van WiFiManager library.
// We willen dit nu gebruiken als voorbeeld sketch
// Op een ESP12 is de blauwe led led_onb aangesloten op pin 2 
// verder hebben we een ontwikkelboard waar we een led kunnen aansluiten op een willekeurig pin
// en de knop op pin 0
// voor de hardware reset kunnen we geen pin gebruiken dus we moeten hier terugvallen op ESP.reset() of ESP.restart()
// er is een interrupt service routine die funtionaliteit in de knopbediening brengt.
// na opslaan van nieuwe gegevens in SPIFFS volgt een reset omdat anders de nieuwe poort niet meteen werkt.


// De esp haalt de tijd op van een NTP server tijdens de opstart.
// deze tijd wordt in de interne klok ingevoerd en bijgehouden
// Nu kunnen we dingen op bepaalde tijden laten gebeuren


//let op nieuwe geheugenchip chip flashen in dout mode

 

// wifimanager is aangepast om DHCP mogelijk te maken

// let op: deze versie is gecompileerd met 2.4.2 boardmanager. in start wifi een wificonfig lijn ingevoegd na de dhcp verbinding door wifimanager
// omdat anders dns het niet doet en geen tijd wordt opgehaald
// flashen met 1M 64k spiffs dout ???
// deze versie kent de puya chip niet. Daarom in esp.cpp en esp.h enkele veranderingen aangebracht.
// voor de puya geheugenchip: in c:/users/hans/appdata/arduino15/packages/esp8266/hardware/esp8266/2.4.0-rc2/ 
// esp.cpp en esp.h vervangen door de exemplaren in de map puya
// de nieuwste wijziging is dat externe links van binnen het netwerk zijn toegestaan zonder authentication

// deze sw is voor de nieuwe sonoff met esp 8255 on board
// heeft geen pin14 meer in plaats daarvan gebruiken we pin 1 tx() nu hebben we geen debug meer op serial

// als er een ap wordt opgestart dan zal dit iedere 5 minuten herstarten als er geen verbinding mee wordt gemaakt
// hierdoor zal hij verbinden als na een stroomstoring de router niet up is.
// update portal, restart AP after connection, prevents client disconnect from ap
// added a function to erase the wifi credentials in a certain sector of the flash
// the portal is restarted after connection was made in AP_STA
// save the current weblocation in eeprom

// flashed als esp8285 1M 64k spiffs ota 470 internetverbinding werkt.

// flash problems sonoff r2 wouldn't connect to wifi. 
// flashed it as 8285 1M fs none erased flash relay 12 led13
// this worked but ofcause no flash
// Now i made a 8285.bin with flash 1M 64k spiffs and ota it on the device works also led works oke not button
// now i made a 8266.bin 1M 64K spiffs relay 12 led 13
