
// deze functie meet de weersensoren en schakelt zonodig
void meetENschakel() {
  consoleOut("meet en schakel functie ");  
  DebugPrintln(minute());
// als er een sensor aanwezig is dan meten we die voor domoticz 
    if (sensor[0] == '0' || sensor[0] == '\0') {return;}
    if (sensor[0] == '1' || sensor[0] == '2' || sensor[0] == '3' || sensor[0] == '6' || sensor[0] == '7') { // alleen als er een temperatuur of licht sensor is ingesteld
          meten(); // stel de meetwaarde vast en verzend mqtt indien van toepassing
         // meteen ook schakelen. Dit kan alleen als er een thermo hygro licht digital is ingesteld
         // als er een foute meting is gedaan dan gaan we terug
         // foute metingen zijn temp_c 1000, 300, -172, p500000 
         if (sensor[0] == '1' || sensor[0] == '2' || sensor[0] == '3') { 
            if ( temp_c == 1000 || temp_c == -127 ) {return;} // sla de thermostaat en mqtt verzending over
         }
         if (sensor[0] == '6' && p == 500000) {
          DebugPrintln("lightsensor error p=500000" );
          return;}
         //nu hebben we alle foutmetingen afgevangen ook voor de luchtdruk, die is fout als de temperatuur fout is
         // de foute metingen worden niet gerapporteerd via mqtt en 'staten werken niet
         if (timer[0] == '3' ) { // thermostaat
                 thermostaat(); // als er geschakeld is door de thermostaat dan zijn de gegevens niet verzonden
              }

         if (timer[0] == '4' ) {
                 hygrostaat();  // als er geschakeld is door de thermostaat dan zijn de gegevens niet verzonden
              }

         if ( timer[0] == '6' ) {
            lichtSensor(); // als er geschakeld is door de thermostaat dan zijn de gegevens niet verzonden
          }

         if (timer[0] == '7' ) {
             digitalSensor();
             }
     // de meetwaarden zijn nu bekend en als er een thermostaat actief is, ook geschakeld
     // dus nu kan mqtt verstuurd worden        
     sendMqttsensor(); //verzend de meetwaarden                    
   }
}

// *******************************************************************************************
//                                    METEN
//*********************************************************************************************
void meten() { 
  
  consoleOut("function meten, time is ");
  DebugPrintln(minute());
          if(sensor[0] == '1') { 
               DebugPrintln("the ds18B20 is polled");
               sensors.requestTemperatures();
               temp_c = sensors.getTempCByIndex(0);
               temp_c = temp_c + atof(tempCal);
               //domoticz(0, true); 
               } // de ds18B20 sensor lezen
          if(sensor[0] == '2') { 
               DebugPrintln("the dht22 is polled");
               gettemperature(); // de dh22sensor lezen
               temp_c = temp_c + atof(tempCal);
              // deze gegevens naar domoticz seinen
              // domoticz(0);
               } 
           if(sensor[0] == '3') { 
               DebugPrintln("the bme280 is polled");
               getWeather();
               temp_c = temp_c + atof(tempCal);
               if ( humidity == 0 ) {temp_c = 1000; } // als er geen vochtigheid is polled
               } 

//           if( sensor[0] == '7' ) {
//           if (digitalRead(3) == HIGH ) { domoticz(1, false); } else { domoticz(2, false); } 
//           }
           
           if(sensor[0] == '6') { 
               DebugPrintln("the max44009 is polled");
               p = licht.get_lux();
               DebugPrint("the lichtvalue in lux = ");
               DebugPrintln(String(p));
               DebugPrintln("");
               if (licht.begin()) {
                    //Serial.println("geen max44009 sensor aangesloten");
                    p=500000;
                   }
               }        
 eventSend(); // send acknowledge to the client
 }


void getWeather() {
   // count the pollings cannot be >10
  float gemiddeld = 0;
  float som = 0;
  
  humidity = bme.readHumidity();
  temp_c = bme.readTemperature(); // in celsius
  if ( isnan(temp_c) ) 
  {
      temp_c = 1000;
      return;
  }
  //t = t*1.8+32.0;
  dp = temp_c-0.36*(100.0-humidity);
    
  p = bme.readPressure()/100.0F;
  pin = 0.02953*p;

 luchtdruktabel[0]= p;
  // de waarde van p opslaan in een array
  // nu de reeds gedane metingen verplaatsen
  for (int x=weerteller; x>0; x--) 
  {
     luchtdruktabel[x] = luchtdruktabel[x-1];
     som +=luchtdruktabel[x];
  }
 
   // multiply the values by 10 and make it an integer
   gemiddeld = int(som/weerteller*10);
   int huidig = int(luchtdruktabel[0]*10);
    
   if (huidig > gemiddeld) {
    DebugPrintln("the pressure rises");
    ldString="↑↑↑";
  } 
  if (huidig < gemiddeld) {
    DebugPrintln("the pressure lowers");
    ldString="↓↓↓" ;
  } 
  if (huidig == gemiddeld)  {
    DebugPrintln("the pressure unchanged");
    ldString="←→";
  }
 if (weerteller<10) {
  weerteller += 1;  
 }
}
