//{check value
void handleForms(AsyncWebServerRequest *request) {
     //every form submission is handled here basisconfig geoconfig mqtt-config
     // we find out which form with a parameter present 
     String serverUrl = request->url().c_str();
     //consoleOut("serverUrl = " + serverUrl); // this is /submitform 
     //consoleOut("requestUrl = " + String(requestUrl));
     if(request->hasParam("dvName")) {
         // received form basisconfig
         //strcpy(userPwd, request->arg("pw1").c_str());
         strcpy(userPasswd, request->arg("pw1").c_str());
         strcpy(dvName, request->arg("dvName").c_str());
         strcpy(aso, request->arg("bev").c_str());
         if(request->hasParam("debug")) diagNose = true;  else diagNose = false;          

          if (dvName[0] == '\0') {
               String s = "ESP-" + String(ESP.getChipId());
               s.toCharArray(dvName, 21);
          }
     strcpy(timer, request->arg("timer").c_str());
   // if this is not 1 than we have to disable all timers
   if(timer[0] != '1') {
      for (int z = 0; z<TIMERCOUNT; z++) timerActive[z] = '0';
      timerConfigsave();
   }
         basisConfigsave();  // alles opslaan
         // check if there is a mismatch between the sensor and the automatition 
         // if so, timer becomes 0
         check_mismatch();
         return; 
     } else 

      if(request->hasParam("longi")) {
        // received the geoconfig form
        longi = request->getParam("longi")->value().toFloat();
        lati = request->getParam("be")->value().toFloat();
        strcpy(gmtOffset, request->getParam("tz")->value().c_str());
        // a checkbox has only a parameter when checked so we check only "is there a param"
        if(request->hasParam("ts")) dts = true;  else  dts = false;
        wifiConfigsave();
        actionFlag=25; // recalculate with these settings 
        return;
     } else
     
     if(request->hasParam("mqtAdres")) {
        // form mosquitto received
        strcpy( mqttBroker  , request->getParam("mqtAdres")   ->value().c_str() );
        mqttPort = request->arg("mqtPort").toInt();
        strcpy( mqttOuttopic, request->getParam("mqtoutTopic")->value().c_str() );
        idxSensor     =          request->arg("seidx").toInt();
        idxSwitch      =          request->arg("swidx").toInt();
        if(request->hasParam("mqtEn")) mqttEnabled = true;  else mqttEnabled = false;
        mqttConfigsave();  // 
        actionFlag=24; // reconnect with these settings
        return;
     }      

     if(request->hasParam("zonattaan")) {
          //form timers received
          char tempChar[1] = "";
          String temp = "";
          //ledState = 0; // prevent slow down
          if( request->hasParam("ta") )  timerActive[tKeuze] = '1';  else timerActive[tKeuze] = '0';

//             temp = request->arg("prg").c_str(); // args are 0 to 6
//             temp.toCharArray(tempChar, 2);
//             defaultProg[tKeuze]=tempChar[0];  // revise the variable defaultProg            
//             //strcpy(defaultProg[tKeuze], request->arg("prg"));
//             consoleOut("defaultProg set to " + String(defaultProg));
//          }
          
          // first put back what is selected in sonnataan and zonnatuit
          temp = "";
          tempChar[0] = '\0';
         // char dag[3] = "";
          temp = request->arg("zonattaan").c_str(); // args are 0 to 6
          temp.toCharArray(tempChar, 2);
          relToSunOn[tKeuze]=tempChar[0];  // revise the variable relToSunOn 
          consoleOut("relToSunOn = " + String(relToSunOn));
          temp = request->arg("zonattuit").c_str(); // args are 0 to 4
          temp.toCharArray(tempChar, 2);
          relToSunOff[tKeuze]=tempChar[0];         
          consoleOut("relToSunOff = " + String(relToSunOff));                    
        
          //now inschakeltijd
          strcpy(tempChar, request->arg("inw").c_str()); 
          for (int i=0; i<5; i++){
             switchOn[tKeuze*5+i] = tempChar[i];
          }
          strcpy(tempChar, request->arg("uitw").c_str());
          for (int i=0; i<5; i++){
             switchOff[tKeuze*5+i] = tempChar[i];
          }      
          // put the values of the days in an array
          char *grap[] = {"su", "mo", "tu", "we", "th", "fr", "sa"};
          String wd = ""; 
          for (int x = 0; x < 7; x++) {
          // the checkbox only has a parameter when checked
          if(request->hasParam(grap[x])) weekDag[tKeuze*7+x] = 'y'; else weekDag[tKeuze*7+x] = 'n';  
          }
           // these functions work ok
           // now send the confirm 
           timerConfigsave(); // alles opslaan in SPIFFS
           Serial.println("timer instellingen opgeslagen");
           actionFlag = 25; // recalculate the timmers
           return;
    }     
     if( request->hasParam("sensorID") ) {
        consoleOut("found sensorID");
        String temp = "";
          switch (tKeuze) {
          case 11:    
          // thermostaat
             if (request->arg("tempHL") == "1") {
              switchHL[0] = '1'; } else {switchHL[0]='0';}
              strcpy(switchTemp, request->arg("switchTemp").c_str());
             break;   
           case 12:    
          // hygrostaat
             if (request->arg("hygHL") == "1") {
                 switchHL[1] = '1'; } else {switchHL[1]='0';}
             strcpy(switchMoist, request->arg("switchMoist").c_str());
             break;
           case 13:    
          // motionsensor
             strcpy(BS, request->arg("behBS").c_str());
             strcpy(switchcdwn, request->arg("cdwn").c_str());
             break;
           case 14:   
          // lichtsensor
             if (request->arg("lichtHL") == "1") {
              switchHL[2] = '1'; } else {switchHL[2]='0';}   
              strcpy(switchLicht, request->arg("switchLicht").c_str());
             break;
           case 15:    
          // digital sensor
             if (request->arg("digitalHL") == "1") {
                switchHL[3] = '1'; } else { switchHL[3] = '0'; }
             break;
            case 16:    
          // sensor keuze en calibratie en meetresolutie
             strcpy(sensor, request->arg("sensor").c_str());
             strcpy(tempCal, request->arg("tcal").c_str());
             meetRes = request->arg("mf").toInt();
             break;
          }
          // deze funties werken
           check_mismatch(); // if timer and sensor mismatch timer becomes nul
           timerConfigsave(); // alles opslaan in SPIFFS
           basisConfigsave(); // voor sensor, tempcal en meetres 
           consoleOut("sensor settings saved");
           actionFlag = 10; // reboot 
           return;   
         }
//sent by sensor form formSubmit?sensorID=sensorID&sensor=1&tcal=%2B0.0&mf=180     
     // if we are here something was wrong, no parameters found
     request->send(200, "text/html", "no valid form found");
}
//}
