#ifdef SENSORS
// we willen niet dat hij in deze lus kan hangen dus we doen aantal pogingen

void gettemperature() {
//wdt_disable();
int aantal = 3; // beperkt aantal pogingen
  do {
  DebugPrintln("de temperatuur meten");  
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
//  unsigned long currentMillis = millis();
 
 // if(currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor 
//   previousMillis = currentMillis;   
// 
    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    humidity = dht.readHumidity();          // Read humidity (percent)
    temp_c = dht.readTemperature();     // Read temperature as celc
    //temp_c = (temp_f - 32)/1.8;
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp_c)) {
     DebugPrintln("Failed to read from DHT sensor!");
     temp_c = 1000;
      //wacht nu 2 sec alvorens opnieuw
      // unsigned long starttijd = millis(); // sla de tijd op in currentMillis
      // unsigned long eindtijd = millis();
      // while (eindtijd - starttijd  <= 2000) { //2000 millis = 2 sec
      // eindtijd = millis();
      //   }
      delay(1000);
      delay(1000);
    }
  aantal -= 1;
  DebugPrintln(temp_c);
  yield();
  }
  while (temp_c == 1000 && aantal > 0);
//wdt_enable();  
}
#endif
