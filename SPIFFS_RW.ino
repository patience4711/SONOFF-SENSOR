// ******************   spiffs lezen  *************************
// ******************   spiffs lezen  *************************


void SPIFFS_read() {

  consoleOut("mounting FS...");
 if (LittleFS.begin()) {
    consoleOut("mounted file system");
// we formatteren als nodig
//if (!LittleFS.exists("/check.100")) {
//  LittleFS.format();
//    DynamicJsonDocument doc(1024);
//    JsonObject json = doc.to<JsonObject>(); 
//    json["hahaha"] = event;         
//    File configFile = LittleFS.open("/check.100", "w");
//    if (!configFile) {
//    consoleOut("failed to open config file for writing");
//    }
//    consoleOut("check file in SPIFFS geschreven");
//    serializeJson(json, Serial);
//    Serial.println(""); 
//    serializeJson(json, configFile);
//    configFile.close();
//}
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
    DynamicJsonDocument doc(1024);
    JsonObject root = doc.to<JsonObject>();   
    root["pswd"] = pswd;
    root["securityLevel"] = securityLevel;
    root["longi"] = longi;
    root["lati"] = lati;
    root["gmtOffset"] = gmtOffset;
    root["dts"] = dts; 

    File configFile = LittleFS.open("/wificonfig.json", "w");
    if (!configFile) {
      consoleOut("cannot open config file");
      return;      
    }
    consoleOut("wrote wificonfig.json");
    if(diagNose)serializeJson(root, Serial); Serial.println("\n");

    serializeJson(root, configFile);
    configFile.close();
}

void timerConfigsave() {   
    consoleOut("saving config");
    DynamicJsonDocument doc(1024);
    JsonObject root = doc.to<JsonObject>();
    //json["timerActive"] = timerActive;
//    JsonArray array = root.createNestedArray("timerActive");
//    for (int i = 0; i < 4; i++) {array.add(timerActive[i]);}    
    root["timerActive"] = timerActive; // this is a byet
    root["weekDag"] = weekDag;
    root["relToSunOn"] = relToSunOn;
    root["relToSunOff"] = relToSunOff;   
    root["switchOn"] = switchOn;   
    root["switchOff"] = switchOff;  
    root["switchcdwn"] = switchcdwn;
    root["switchHL"] = switchHL; // this is a byte
//    JsonArray arrey = root.createNestedArray("switchHL");
//    for (int i = 0; i < 4; i++) { arrey.add(switchHL[i]); } 
    
    root["switchTemp"] = switchTemp; //float
    root["switchMoist"] = switchMoist; //int
    root["switchLicht"] = switchLicht; // float
    root["Bds"] = Bds;
    
    File configFile = LittleFS.open("/timerconfig.json", "w");
    if (!configFile) {
      consoleOut("cannot open timerconfig");
      return;
    }
    consoleOut("wrote timerconfig.json");
    if(diagNose) {
    serializeJson(root, Serial);
    Serial.println("\n");     
    }
    serializeJson(root, configFile);
    configFile.close();
}
void basisConfigsave() {
   consoleOut("saving basis config");
    DynamicJsonDocument doc(1024);
    JsonObject root = doc.to<JsonObject>(); 
    root["dvName"] = dvName;
    root["userPasswd"] = userPasswd;
    root["aso"] = aso;
    root["diagNose"] = diagNose;
    root["autoMate"] = autoMate;
    root["senSor"] = senSor;
    root["tempCal"] = tempCal; 
    root["meetRes"] = meetRes;   

    File configFile = LittleFS.open("/basisconfig.json", "w");
    if (!configFile) {
      consoleOut("cannot open basisconfig,json");
      return;
    }
    consoleOut("wrote basisconfig.json");
    if(diagNose)serializeJson(root, Serial); Serial.println("\n");     
    serializeJson(root, configFile);
    configFile.close();
}

void mqttConfigsave() {
   consoleOut("saving mqtt config");
    DynamicJsonDocument doc(1024);
    JsonObject root = doc.to<JsonObject>(); 
    root["mqttEnabled"] = mqttEnabled;
    root["mqttBroker"] = mqttBroker;
    root["mqttOuttopic"] = mqttOuttopic;
    root["idxSwitch"] = idxSwitch;
    root["idxSensor"] = idxSensor;
    
    File configFile = LittleFS.open("/mqttconfig.json", "w");
    if (!configFile) {
      consoleOut("failed to open mqttconfig.json");
      return;
    }
    consoleOut("wrote mqttconfig.json");
    if(diagNose)serializeJson(root, Serial); Serial.println("\n");
    serializeJson(root, configFile);
    configFile.close();
}


bool file_open_for_read(String bestand) 
  {
  //bool checkBool = false;
  consoleOut("file_open_for_read, file = "); consoleOut(bestand); 
  File configFile = LittleFS.open(bestand, "r");
  if (!configFile) {
        Serial.println(F("cannot open config file"));
        return false;
    }
     consoleOut("opened file "); consoleOut(bestand);
     DynamicJsonDocument doc(1024);
     DeserializationError error = deserializeJson(doc, configFile);
     if (error) {
          Serial.print(F("Failed parse config file: "));
          Serial.println(error.c_str());
          return false;
      }
      //Serial.println("reading " + bestand);
      serializeJson(doc, Serial);  // always print
      JsonObject root = doc.as<JsonObject>();
      if (bestand == "/wificonfig.json") {
            if (root.containsKey("pswd"))          strcpy(pswd, root["pswd"]);
            if (root.containsKey("securityLevel")) securityLevel = root["securityLevel"];
            if (root.containsKey("longi"))         longi = root["longi"];
            if (root.containsKey("lati"))          lati = root["lati"];                      
            if (root.containsKey("gmtOffset"))     strcpy(gmtOffset, root["gmtOffset"]);
            if (root.containsKey("dts"))           dts = root["dts"];
       } else
       if ( bestand == "/timerconfig.json" ) {
//            if (root.containsKey("timerActive")) {                      
//                JsonArray array = root["timerActive"];
//                int i = 0;
//                for (bool val : array) { if ( i < 4 ) timerActive[i++] = val; }
//            }
            if (root.containsKey("timerActive")) timerActive = root["timerActive"];
            if (root.containsKey("weekDag"))     strcpy(weekDag, root["weekDag"]);
            if (root.containsKey("relToSunOn"))  strcpy(relToSunOn, root["relToSunOn"]);
            if (root.containsKey("relToSunOff")) strcpy(relToSunOff, root["relToSunOff"]);
            if (root.containsKey("switchOn"))    strcpy(switchOn, root["switchOn"]);        
            if (root.containsKey("switchOff"))   strcpy(switchOff, root["switchOff"]);
          
            if (root.containsKey("switchcdwn"))    strcpy(switchcdwn, root["switchcdwn"]);
            //if (root.containsKey("switchHL"))      strcpy(switchHL, root["switchHL"]);
//            if (root.containsKey("switchHL")) {                      
//                JsonArray array = root["switchHL"];
//                int i = 0;
//                for (bool val : array) { if ( i < 4 ) switchHL[i++] = val; }
//            }
            if (root.containsKey("switchHL"))      switchHL = root["switchHL"];
            if (root.containsKey("switchTemp"))    switchTemp = root["switchTemp"];
            if (root.containsKey("switchMoist"))   switchMoist = root["switchMoist"];
            if (root.containsKey("switchLicht"))   switchLicht = root["switchLicht"];
            if (root.containsKey("Bds"))           Bds = root["Bds"];
       } else

       if (bestand == "/basisconfig.json"){
           if (root.containsKey("dvName"))     strcpy(dvName, root["dvName"]);
           if (root.containsKey("userPasswd")) strcpy(userPasswd, doc["userPasswd"]);
           if (root.containsKey("aso"))        strcpy(aso, root["aso"]);
           if (root.containsKey("autoMate"))   autoMate = root["autoMate"];
           if (root.containsKey("senSor"))     senSor = root["senSor"]; 
           if (root.containsKey("tempCal"))    strcpy(tempCal, root["tempCal"]);
           if (root.containsKey("meetRes"))    meetRes = root["meetRes"];       
           if (root.containsKey("diagNose"))   diagNose = root["diagNose"];
        } else

        if (bestand == "/mqttconfig.json") {
           if (root.containsKey("mqttEnabled"))  mqttEnabled =  doc["mqttEnabled"];   
           if (root.containsKey("mqttBroker"))   strcpy(mqttBroker, doc["mqttBroker"]);
           if (root.containsKey("mqttOuttopic")) strcpy(mqttOuttopic, doc["mqttOuttopic"]);         
           if (root.containsKey("idxSwitch"))    idxSwitch = doc["idxSwitch"];
           if (root.containsKey("idxSensor"))    idxSensor = doc["idxSensor"];     
        }             
       return true; // if ! error

  }
//     }
// }  


// void SPIFFS_read() {

//   consoleOut("mounting FS...");
//  if (LittleFS.begin()) {
//     consoleOut("mounted file system");
// // we formatteren als nodig
// //if (!LittleFS.exists("/check.100")) {
// //  LittleFS.format();
// //    DynamicJsonDocument doc(1024);
// //    JsonObject json = doc.to<JsonObject>(); 
// //    json["hahaha"] = event;         
// //    File configFile = LittleFS.open("/check.100", "w");
// //    if (!configFile) {
// //    consoleOut("failed to open config file for writing");
// //    }
// //    consoleOut("check file in SPIFFS geschreven");
// //    serializeJson(json, Serial);
// //    Serial.println(""); 
// //    serializeJson(json, configFile);
// //    configFile.close();
// //}
//        if( file_open_for_read("/timerconfig.json") ) {     
//              consoleOut("timerconfig read");
//           } else {
//           consoleOut("timerconfig.json not loaded");
//         }
//        if( file_open_for_read("/basisconfig.json") ) {     
//              consoleOut("basisconfig read");
//           } else {
//           consoleOut("basisconfig.json not loaded");
//         }  
//        if( file_open_for_read("/mqttconfig.json") ) {     
//              consoleOut("mqttconfig read");
//           } else {
//           consoleOut("mqttconfig.json not loaded");
//         }
//        if( file_open_for_read("/wificonfig.json") ) {
//                 consoleOut("wificonfig read");
//           } else {
//              consoleOut("wificonfig.json not loaded");
//           }

//   } else {
//         consoleOut("failed to mount FS");
//    }
//  // einde spiffs lezen 3 bestanden

// // als er geen swName of dts is gelezen dan zetten we deze op default
//       if (dvName[0] == '\0') {
//            String s = "SONOFF-" + String(ESP.getChipId());
//            s.toCharArray(dvName, 21);
//               }      
 
//  tijd_convert(); // omrekenen van tijdwaarden voor cdwn aso en switchtime

// }  


// // **************************************************************************** 
// //                      de gegevens opslaan in SPIFFS                         *  
// // ****************************************************************************
// void wifiConfigsave() {
//    consoleOut("saving config");
//     DynamicJsonDocument doc(1024);
//     //doc.setCapacity(1024);
//     JsonObject root = doc.to<JsonObject>();   
//     root["pswd"] = pswd;
//     root["securityLevel"] = securityLevel;
//     root["longi"] = longi;
//     root["lati"] = lati;
//     root["gmtOffset"] = gmtOffset;
//     root["dts"] = dts; 

//     File configFile = LittleFS.open("/wificonfig.json", "w");
//     if (!configFile) {
//       consoleOut("cannot open config file");
//       return;      
//     }
//     consoleOut("wrote wificonfig.json");
//     if(diagNose)serializeJson(root, Serial); Serial.println("\n");

//     serializeJson(root, configFile);
//     configFile.close();
// }

// void timerConfigsave() {   
//     consoleOut("saving config");
//     DynamicJsonDocument doc(1024);
//     //doc.setCapacity(1024);
//     JsonObject root = doc.to<JsonObject>();
//     //json["timerActive"] = timerActive;
// //    JsonArray array = root.createNestedArray("timerActive");
// //    for (int i = 0; i < 4; i++) {array.add(timerActive[i]);}    
//     root["timerActive"] = timerActive; // this is a byet
//     root["weekDag"] = weekDag;
//     root["relToSunOn"] = relToSunOn;
//     root["relToSunOff"] = relToSunOff;   
//     root["switchOn"] = switchOn;   
//     root["switchOff"] = switchOff;  
//     root["switchcdwn"] = switchcdwn;
//     root["switchHL"] = switchHL; // this is a byte
// //    JsonArray arrey = root.createNestedArray("switchHL");
// //    for (int i = 0; i < 4; i++) { arrey.add(switchHL[i]); } 
    
//     root["switchTemp"] = switchTemp; //float
//     root["switchMoist"] = switchMoist; //int
//     root["switchLicht"] = switchLicht; // float
//     root["Bds"] = Bds;
    
//     File configFile = LittleFS.open("/timerconfig.json", "w");
//     if (!configFile) {
//       consoleOut("cannot open timerconfig");
//       return;
//     }
//     consoleOut("wrote timerconfig.json");
//     if(diagNose) {
//     serializeJson(root, Serial);
//     Serial.println("\n");     
//     }
//     serializeJson(root, configFile);
//     configFile.close();
// }
// void basisConfigsave() {
//    consoleOut("saving basis config");
//     DynamicJsonDocument doc(1024);
//     //doc.setCapacity(1024);
//     JsonObject root = doc.to<JsonObject>(); 
//     root["dvName"] = dvName;
//     root["userPasswd"] = userPasswd;
//     root["aso"] = aso;
//     root["diagNose"] = diagNose;
//     root["autoMate"] = autoMate;
//     root["senSor"] = senSor;
//     root["tempCal"] = tempCal; 
//     root["meetRes"] = meetRes;   

//     File configFile = LittleFS.open("/basisconfig.json", "w");
//     if (!configFile) {
//       consoleOut("cannot open basisconfig,json");
//       return;
//     }
//     consoleOut("wrote basisconfig.json");
//     if(diagNose)serializeJson(root, Serial); Serial.println("\n");     
//     serializeJson(root, configFile);
//     configFile.close();
// }

// void mqttConfigsave() {
//    consoleOut("saving mqtt config");
//     DynamicJsonDocument doc(1024);
//     //doc.setCapacity(1024);
//     JsonObject root = doc.to<JsonObject>(); 
//     root["mqttEnabled"] = mqttEnabled;
//     root["mqttBroker"] = mqttBroker;
//     root["mqttOuttopic"] = mqttOuttopic;
//     root["idxSwitch"] = idxSwitch;
//     root["idxSensor"] = idxSensor;
    
//     File configFile = LittleFS.open("/mqttconfig.json", "w");
//     if (!configFile) {
//       consoleOut("failed to open mqttconfig.json");
//       return;
//     }
//     consoleOut("wrote mqttconfig.json");
//     if(diagNose)serializeJson(root, Serial); Serial.println("\n");
//     serializeJson(root, configFile);
//     configFile.close();
// }


// bool file_open_for_read(String bestand) 
//   {
//   //bool checkBool = false;
//   consoleOut("file_open_for_read, file = "); consoleOut(bestand); 
//   File configFile = LittleFS.open(bestand, "r");
//   if (!configFile) {
//         Serial.println(F("cannot open config file"));
//         return false;
//     }
//      consoleOut("opened file "); consoleOut(bestand);
//      DynamicJsonDocument doc(1024);
//      DeserializationError error = deserializeJson(doc, configFile);
//      if (error) {
//           Serial.print(F("Failed parse config file: "));
//           Serial.println(error.c_str());
//           return false;
//       }
//       //Serial.println("reading " + bestand);
//       serializeJson(doc, Serial);  // always print
//       JsonObject root = doc.as<JsonObject>();
//       if (bestand == "/wificonfig.json") {
            
//             //if (root.containsKey("pswd"))          strcpy(pswd, root["pswd"]);
//             strcpy(pswd, root["pswd"] | "0000");
//             if (root["securityLevel"].is<int>())  securityLevel = root["securityLevel"].as<int>();
//             if (root["longi"].is<float>())  longi = root["longi"].as<float>();
//             if (root["lati"]. is<float>())   lati = root["lati"].as<float>();
            
//             //if (root.containsKey("securityLevel")) securityLevel = root["securityLevel"];
//             //if (root.containsKey("longi"))         longi = root["longi"];
//             strcpy(pswd, root["gmtOffset"] | "+120");    //char 
//             if (root["dts"].is<bool>())  dts = root["dts"].as<bool>();       
//             //if (root.containsKey("gmtOffset"))     strcpy(gmtOffset, root["gmtOffset"]);
//             //if (root.containsKey("dts"))           dts = root["dts"];
//        } else
//        if ( bestand == "/timerconfig.json" ) {
// //            if (root.containsKey("timerActive")) {                      
// //                JsonArray array = root["timerActive"];
// //                int i = 0;
// //                for (bool val : array) { if ( i < 4 ) timerActive[i++] = val; }
// //            }
//   // char relToSunOn[5] = "0000"; 
//   // char relToSunOff[5] = "0000"; 
//   // char switchOn[21] = "00:0000:0000:0000:00";
//   // char switchOff[21] = "00:0000:0000:0000:00";
//   // char weekDag[29] = "nnnnnnnnnnnnnnnnnnnnnnnnnnnn";
//   // char switchcdwn[6] = "00:00";
//   // byte switchHL = 0b00000000;
//   // float switchTemp = 0.1; // was 4
//   // int switchMoist = 11; // de vochtigheidsgraad
//   // float switchLicht = 20; // ambient light in lux 
//   // uint8_t Bds = 0;  // boodschap
//   // char tempCal[5] = "+0.0"; //calibation
//   // //char sensor[2] = ""; // what sensor is configured
//   // int meetRes = 180; // hebben we nodig om de mqtt verbinding te bewaken
//   // //char timer[2] = "1"; //what automation is configured
//   // uint8_t autoMate = 1; //(timers)
//   // uint8_t senSor = 1; //(timers)

//             //if (root.containsKey("timerActive")) timerActive = root["timerActive"];
//             if (root["timerActive"].is<uint8_t>()) timerActive = root["timerActive"].as<uint8_t>();
//             //if (root.containsKey("weekDag"))     strcpy(weekDag, root["weekDag"]);
//             strcpy(weekDag, root["weekDag"] | "0000000000000000000000000000");
//             //if (root.containsKey("relToSunOn"))  strcpy(relToSunOn, root["relToSunOn"]);
//             strcpy(relToSunOn, root["relToSunOn"] | "0000");
//             //if (root.containsKey("relToSunOff")) strcpy(relToSunOff, root["relToSunOff"]);
//             strcpy(relToSunOff, root["relToSunOff"] | "0000");
//             //if (root.containsKey("switchOn"))    strcpy(switchOn, root["switchOn"]);        
//             strcpy(switchOn, root["switchOn"] | "00:0000:0000:0000:00");
//             //if (root.containsKey("switchOff"))   strcpy(switchOff, root["switchOff"]);
//              strcpy(switchOff, root["switchOff"] | "00:0000:0000:0000:00");
//             if (root.containsKey("switchcdwn"))    strcpy(switchcdwn, root["switchcdwn"]);
//              strcpy(switchcdwn, root["switchcdwn"] | "00:00");
//             //if (root.containsKey("switchHL"))      switchHL = root["switchHL"];
//             if (root["switchHL"].is<uint8_t>())  switchHL = root["switchHL"].as<uint8_t>();
//             //if (root.containsKey("switchTemp"))    switchTemp = root["switchTemp"];
//             if (root["switchTemp"].is<float>())   switchTemp = root["switchTemp"].as<float>();
//             //if (root.containsKey("switchMoist"))   switchMoist = root["switchMoist"];
//             if (root["switchMoist"].is<int>())     switchMoist = root["switchMoist"].as<int>();
//             //if (root.containsKey("switchLicht"))   switchLicht = root["switchLicht"];
//             if (root["switchLicht"].is<float>())   switchLicht = root["switchLicht"].as<float>();
//             //if (root.containsKey("Bds"))           Bds = root["Bds"];
//             if (root["Bds"].is<uint8_t>())         Bds = root["Bds"].as<uint8_t>();

//        } else

//        if (bestand == "/basisconfig.json"){
//            if (root.containsKey("dvName"))     strcpy(dvName, root["dvName"]);
//            if (root.containsKey("userPasswd")) strcpy(userPasswd, doc["userPasswd"]);
//            if (root.containsKey("aso"))        strcpy(aso, root["aso"]);
//            if (root.containsKey("autoMate"))   autoMate = root["autoMate"];
//            if (root.containsKey("senSor"))     senSor = root["senSor"]; 
//            if (root.containsKey("tempCal"))    strcpy(tempCal, root["tempCal"]);
//            if (root.containsKey("meetRes"))    meetRes = root["meetRes"];       
//            if (root.containsKey("diagNose"))   diagNose = root["diagNose"];
//         } else

//         if (bestand == "/mqttconfig.json") {
//            if (root.containsKey("mqttEnabled"))  mqttEnabled =  doc["mqttEnabled"];   
//            if (root.containsKey("mqttBroker"))   strcpy(mqttBroker, doc["mqttBroker"]);
//            if (root.containsKey("mqttOuttopic")) strcpy(mqttOuttopic, doc["mqttOuttopic"]);         
//            if (root.containsKey("idxSwitch"))    idxSwitch = doc["idxSwitch"];
//            if (root.containsKey("idxSensor"))    idxSensor = doc["idxSensor"];     
//         }             
//        return true; // if ! error

//   }
// //     }
// // }  


        
