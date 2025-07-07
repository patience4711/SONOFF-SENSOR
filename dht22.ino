#ifdef SENSORS
// we dont want it to hang in this so we restrict the attempts

void gettemperature() {
//wdt_disable();
int aantal = 3; // restricted attempts
  do {
    //consoleOut("measure temperature");  
    // Wait at least 2 seconds seconds between measurements.
    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    humidity = dht.readHumidity();          // Read humidity (percent)
    temp_c = dht.readTemperature();     // Read temperature as celc
    //temp_c = (temp_f - 32)/1.8;
    // Check if read failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp_c)) {
     //consoleOut("read error DHT sensor!");
     temp_c = 1000;
      //wait now 2 sec before repetition
      delay(2000);
      }
  aantal -= 1;
  //consoleOut(String(temp_c,2));
  yield();
  }
  while (temp_c == 1000 && aantal > 0);
}
#endif
