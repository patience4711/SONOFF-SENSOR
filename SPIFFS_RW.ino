// ******************   spiffs lezen  *************************
// ******************   spiffs lezen  *************************


void SPIFFS_read() {

  consoleOut("mounting FS...");
 if (LittleFS.begin()) {
    consoleOut("mounted file system");

       if( file_open_for_read("/timerconfig.json") ) {     
             consoleOut("timerconfig read");
          } else {
          consoleOut("timerconfig.json not loaded");
        }
       if( file_open_for_read("/basisconfig.json") ) {     
             consoleOut("basisconfig read");
          } else {
          consoleOut("basisconfig.json not loaded");
        }  
       if( file_open_for_read("/mqttconfig.json") ) {     
             consoleOut("mqttconfig read");
          } else {
          consoleOut("mqttconfig.json not loaded");
        }
       if( file_open_for_read("/wificonfig.json") ) {
                consoleOut("wificonfig read");
          } else {
             consoleOut("wificonfig.json not loaded");
          }

  } else {
        consoleOut("failed to mount FS");
   }
 // einde spiffs lezen 3 bestanden

// als er geen swName of dts is gelezen dan zetten we deze op default
      if (dvName[0] == '\0') {
           String s = "SONOFF-" + String(ESP.getChipId());
           s.toCharArray(dvName, 21);
              }      
 
 tijd_convert(); // omrekenen van tijdwaarden voor cdwn aso en switchtime

}  


// **************************************************************************** 
//                      de gegevens opslaan in SPIFFS                         *  
// ****************************************************************************
void wifiConfigsave() {
   consoleOut("saving config");
    JsonDocument doc;
    //JsonObject root = doc.to<JsonObject>();   
    doc["pswd"] = pswd; //char
    doc["securityLevel"] = securityLevel; //int
    doc["longi"] = longi; // float
    doc["lati"] = lati; // float
    doc["gmtOffset"] = gmtOffset;  // char
    doc["dts"] = dts; //uint_8

    File configFile = LittleFS.open("/wificonfig.json", "w");
    if (!configFile) {
      consoleOut("cannot open config file");
      return;      
    }
    consoleOut("wrote wificonfig.json");
    if(diagNose)serializeJson(doc, Serial); Serial.println("\n");

    serializeJson(doc, configFile);
    configFile.close();
}

void timerConfigsave() {   
    consoleOut("saving config");
    JsonDocument doc;
    //JsonObject root = doc.to<JsonObject>();
    //json["timerActive"] = timerActive;
//    JsonArray array = root.createNestedArray("timerActive");
//    for (int i = 0; i < 4; i++) {array.add(timerActive[i]);}    
    doc["timerActive"] = timerActive; // this is a byet
    doc["weekDag"] = weekDag;
    doc["relToSunOn"] = relToSunOn;
    doc["relToSunOff"] = relToSunOff;   
    doc["switchOn"] = switchOn;   
    doc["switchOff"] = switchOff;  
    doc["switchcdwn"] = switchcdwn;
    doc["switchHL"] = switchHL; // this is a byte
    doc["switchTemp"] = switchTemp; //float
    doc["switchMoist"] = switchMoist; //int
    doc["switchLicht"] = switchLicht; // float
    doc["Bds"] = Bds;
    
    File configFile = LittleFS.open("/timerconfig.json", "w");
    if (!configFile) {
      consoleOut("cannot open timerconfig");
      return;
    }
    consoleOut("wrote timerconfig.json");
    if(diagNose) {
    serializeJson(doc, Serial);
    Serial.println("\n");     
    }
    serializeJson(doc, configFile);
    configFile.close();
}
void basisConfigsave() {
   consoleOut("saving basis config");
    JsonDocument doc;
    //JsonObject root = doc.to<JsonObject>(); 
    doc["dvName"] = dvName;
    doc["userPasswd"] = userPasswd;
    doc["aso"] = aso;
    doc["diagNose"] = diagNose;
    doc["autoMate"] = autoMate;
    doc["senSor"] = senSor;
    doc["tempCal"] = tempCal; 
    doc["meetRes"] = meetRes;   

    File configFile = LittleFS.open("/basisconfig.json", "w");
    if (!configFile) {
      consoleOut("cannot open basisconfig,json");
      return;
    }
    consoleOut("wrote basisconfig.json");
    if(diagNose)serializeJson(doc, Serial); Serial.println("\n");     
    serializeJson(doc, configFile);
    configFile.close();
}

void mqttConfigsave() {
   consoleOut("saving mqtt config");
    JsonDocument doc;
    //JsonObject root = doc.to<JsonObject>(); 
    doc["mqttEnabled"] = mqttEnabled;
    doc["mqttBroker"] = mqttBroker;
    doc["mqttOuttopic"] = mqttOuttopic;
    doc["idxSwitch"] = idxSwitch;
    doc["idxSensor"] = idxSensor;
    
    File configFile = LittleFS.open("/mqttconfig.json", "w");
    if (!configFile) {
      consoleOut("failed to open mqttconfig.json");
      return;
    }
    consoleOut("wrote mqttconfig.json");
    if(diagNose)serializeJson(doc, Serial); Serial.println("\n");
    serializeJson(doc, configFile);
    configFile.close();
}


bool file_open_for_read(const char* bestand) 
  {
  // we try to read the configfile and when not exists we always read the fallback values
  // this saves us the trouble to declare the variables with a value
  consoleOut("file_open_for_read, file = "); consoleOut(bestand); 
  StaticJsonDocument<400> doc;
  File configFile = LittleFS.open(bestand, "r");
  if (configFile) {
        DeserializationError error = deserializeJson(doc, configFile);
        configFile.close();
        if (error) {
            Serial.print(F("Failed to parse config file: "));
            Serial.println(error.c_str());
            // Continue with fallback values
        } else {
        // no error so we can print the file
            serializeJson(doc, Serial);  // always print
        }
    } else {
        Serial.print(F("Cannot open config file: "));
        Serial.println(bestand);
        // Continue with empty doc -> all fallbacks will be used
    }
      //Serial.println("reading " + bestand);
      serializeJson(doc, Serial);  // always serial print doc
      //JsonObject root = doc.as<JsonObject>(); // for the rest
      if ( strcmp(bestand, "/wificonfig.json") == 0) {
            //we read all the values with a fallback value to prevent crashes
            strcpy(pswd, doc["pswd"] | "0000");
            securityLevel = doc["securityLevel"] | 6 ;
            longi = doc["longi"] | 5.123;
            lati = doc["lati"] | 51.234;                      
            strcpy(gmtOffset, doc["gmtOffset"] | "+120" );
            dts = doc["dts"] | false;
       } else
       if ( strcmp(bestand, "/timerconfig.json") == 0) {

            timerActive = doc["timerActive"] | 0b00000000; // byte
            strcpy(weekDag, doc["weekDag"] | "0000000000000000000000000000"); //char
            strcpy(relToSunOn, doc["relToSunOn"] | "0000"); //char
            strcpy(relToSunOff, doc["relToSunOff"] | "0000"); //char
            strcpy(switchOn, doc["switchOn"] |  "00:0000:0000:0000:00");  //char     
            strcpy(switchOff, doc["switchOff"] | "00:0000:0000:0000:00"); //char
            strcpy(switchcdwn, doc["switchcdwn"] | "00:00"); // char
            switchHL = doc["switchHL"] | 0b00000000;  // byte
            switchTemp = doc["switchTemp"] | 0.1; // float
            switchMoist = doc["switchMoist"] | 11; // int
            switchLicht = doc["switchLicht"] | 20; //float
            Bds = doc["Bds"] | 0 ; // uint8_t 

       } else

       if ( strcmp(bestand, "/basisconfig.json") == 0) {
           strcpy(dvName, doc["dvName"] | "" ); //char
           strcpy(userPasswd, doc["userPasswd"] | "1111"); //char
           strcpy(aso, doc["aso"] | "06:00"); //char
           autoMate = doc["autoMate"] | 0; //uint8_t
           senSor = doc["senSor"] | 0;  //uint8_
           strcpy(tempCal, doc["tempCal"] | "0.0"); //char
           meetRes = doc["meetRes"] | 180; // int       
           diagNose = doc["diagNose"] | false;
        } else

        if ( strcmp(bestand, "/mqttconfig.json") == 0) {
           mqttEnabled =  doc["mqttEnabled"] | false;  // bool   
           strcpy(mqttBroker, doc["mqttBroker"] | "192.168.0.100"); //char
           strcpy(mqttOuttopic, doc["mqttOuttopic"] | "domoticz/in");         
           idxSwitch = doc["idxSwitch"] | 0;
           idxSensor = doc["idxSensor"] | 0;     
        }             
       return true; // if ! error

  }
  


