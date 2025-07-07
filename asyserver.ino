//{check value

// ***********************************************************************************
void start_asyserver() {
// Handle Web Server Events

events.onConnect([](AsyncEventSourceClient *client){
//  if(client->lastId()){
//    Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
//  }
});

server.addHandler(&events);
// Simple Firmware Update
  server.on("/FWUPDATE", HTTP_GET, [](AsyncWebServerRequest *request){
    loginBoth(request, "admin");
    request->send_P(200, "text/html", UPDATE_FORM); 
    });
  server.on("/handleFwupdate", HTTP_POST, [](AsyncWebServerRequest *request){
    Serial.println("FWUPDATE requested");
    if( !Update.hasError() ) {
    toSend="<br><br><center><h2>UPDATE SUCCESS !!</h2><br><br>";
    toSend +="click here to reboot<br><br><a href='/REBOOT'><input style='font-size:4vw;' type='submit' value='REBOOT'></a>";
    } else {
    toSend="<br><br><center><kop>update failed<br><br>";
    toSend +="click here to go back <a href='/FWUPDATE'>BACK</a></center>";
    }
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", toSend);
    response->addHeader("Connection", "close");
    request->send(response);
  
  },[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
    //Serial.println("filename = " + filename);
    if(filename != "") {
    if(!index){
      Serial.printf("start firmware update: %s\n", filename.c_str());
      Update.runAsync(true);
      if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)){
        Update.printError(Serial);
      }
    }
    } else {
      Serial.println("filename empty, aborting");
//     Update.hasError()=true;
    }
    if(!Update.hasError()){
      if(Update.write(data, len) != len){
        Serial.println("update failed with error: " );
        Update.printError(Serial);
      }
    }
    if(final){
      if(Update.end(true)){
        Serial.printf("firmware Update Success: %uB\n", index+len);
      } else {
        Update.printError(Serial);
      }
    }
  });
// ***********************************************************************************
//                                     homepage
// ***********************************************************************************
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
 
    //consoleOut("homepage requested");
    loginBoth(request, "both");
    request->send_P(200, "text/html", HOMEPAGE );
});

server.on("/STYLESHEET", HTTP_GET, [](AsyncWebServerRequest *request) {
   request->send_P(200, "text/css", STYLESHEET);
});

server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("favicon requested");
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", FAVICON, FAVICON_len);
    request->send(response);
});
server.on("/MENU", HTTP_GET, [](AsyncWebServerRequest *request) {
  if(checkRemote( request->client()->remoteIP().toString()) ) request->redirect( "/DENIED" );
  loginBoth(request, "admin");
  toSend = FPSTR(HTML_HEAD);
  toSend += FPSTR(MENUPAGE);
request->send(200, "text/html", toSend);
});

// ***********************************************************************************
//                                    timers
// ***********************************************************************************
server.on("/TIMERCONFIG", HTTP_GET, [](AsyncWebServerRequest *request) {
    strcpy( requestUrl, request->url().c_str() ); // remember this to come back after reboot
    loginBoth(request, "admin");
    if (tKeuze > 4) tKeuze = 0;
    zendpageTimers();
    request->send(200, "text/html", toSend);
});


server.on("/TIMER", HTTP_GET, [](AsyncWebServerRequest *request) {
    int i = atoi(request->arg("welke").c_str()) ;
    tKeuze = i;
    //consoleOut("tKeuze is " + String(i));
    //nu roepen we zendpageRelevant aan
    zendPageRelevantTimers(); // prepare the page
    request->send(200, "text/html", toSend); //send the html code to the client
    //delay(500); //wait half a second after sending the data
});


// ***********************************************************************************
//                                   basisconfig
// ***********************************************************************************
server.on("/BASISCONFIG", HTTP_GET, [](AsyncWebServerRequest *request) {
    //loginAdmin(request);
    strcpy( requestUrl, request->url().c_str() );// remember this to come back after reboot
    loginBoth(request, "admin");
    zendPageBasis(request);
    //request->send(200, "text/html", toSend);
});

server.on("/submitForm", HTTP_GET, [](AsyncWebServerRequest *request) {
handleForms(request);
confirm(); // puts a response in toSend
request->send(200, "text/html", toSend);
});


// ***********************************************************************************
//                 mqtt  geo  log
// ***********************************************************************************
server.on("/MQTT", HTTP_GET, [](AsyncWebServerRequest *request) {
    //loginAdmin(request);
    loginBoth(request, "admin");
    strcpy( requestUrl, request->url().c_str() ); // remember this to come back after reboot
    zendPageMQTTconfig(request);
    //request->send(200, "text/html", toSend);
});

server.on("/GEOCONFIG", HTTP_GET, [](AsyncWebServerRequest *request) {
    //loginAdmin(request);
    loginBoth(request, "admin");
    strcpy( requestUrl, request->url().c_str() ); // remember this to come back after reboot
    zendPageGEOconfig(request);
    //request->send(200, "text/html", toSend);
});

// server.on("/LOGPAGE", HTTP_GET, [](AsyncWebServerRequest *request) {
//     loginBoth(request, "both");
//     zendPageLog();
//     request->send( 200, "text/html", toSend );
// });
// server.on("/CLEAR_LOG", HTTP_GET, [](AsyncWebServerRequest *request) {
//     //loginAdmin(request);
//     loginBoth(request, "admin");
//     //Serial.println(F("clear log requested"));
//     Clear_Log();
//     zendPageLog();
//     request->send( 200, "text/html", toSend );  
// });

server.on("/REBOOT", HTTP_GET, [](AsyncWebServerRequest *request) {
  Serial.println("reboot requested");
  strcpy( requestUrl, "/" ); //anders blijt hij rebooten
  loginBoth(request, "admin");
  actionFlag = 10;
  confirm();
  request->send(200, "text/html", toSend);
});

server.on("/STARTAP", HTTP_GET, [](AsyncWebServerRequest *request) {
//    if (!request->authenticate("admin", pswd)) return request->requestAuthentication();
        loginBoth(request, "admin");
        //consoleOut("erase the wifi data");
        String toSend = F("<!DOCTYPE html><html><style>body {font-size:44px;}</style><head><script type='text/javascript'>setTimeout(function(){ window.location.href='/'; }, 5000 ); </script>");
        toSend += F("</head><body><center><h3>OK the accesspoint is started.</h3>Wait unil the led goes on.<br><br>Then go to the wifi-settings on your pc/phone/tablet and connect to ");
        toSend += getChipId(false) + " !";
        request->send ( 200, "text/html", toSend ); //zend confirm
        actionFlag = 11;
        
});

server.on("/INFOPAGE", HTTP_GET, [](AsyncWebServerRequest *request) {
    loginBoth(request, "both");
    strcpy( requestUrl, request->url().c_str() );
    handleInfo(request);
});



server.on("/MQTT_TEST", HTTP_GET, [](AsyncWebServerRequest *request) {
    String toMQTT = "{\"test\":\"" + String(mqttOuttopic) + "\"}";
    Serial.println("MQTT_Client.publish the message : " + toMQTT);
    MQTT_Client.publish ( mqttOuttopic, toMQTT.c_str() );
    toSend = "sent mqtt message : " + toMQTT;
    request->send( 200, "text/plain", toSend  );
});

server.on("/SENSOR", HTTP_GET, [](AsyncWebServerRequest *request) {
    int i = atoi(request->arg("welke").c_str()) ;
    tKeuze = i;
    //consoleOut("tKeuze is " + String(i)); 
    strcpy( requestUrl, request->url().c_str() );
    //nu roepen we zendpageRelevant aan
    zendPageRelevantSensors(); 
    request->send(200, "text/html", toSend); //send the html code to the client
  });

server.on("/SENSORCONFIG", HTTP_GET, [](AsyncWebServerRequest *request) {
    loginBoth(request, "both");
    //strcpy( requestUrl, request->url().c_str() ); // remember this to come back after reboot
    zendPageSensors();
    request->send( 200, "text/html", toSend );
});

server.on("/METEN", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!diagNose) {
      meetENschakel(); 
      confirm();
      request->send( 200, "text/html", toSend );
    } else { request->send(200, "text/plain", "this is not possible in debug mode, go back <-");}
    });

server.on("/SW=OFF", HTTP_GET, [](AsyncWebServerRequest *request) {
     loginBoth(request, "both");
     switchOffNow(true, true, "web int");
     value = 0;
     request->send_P(200, "text/html", HOMEPAGE);
});
server.on("/SW=ON", HTTP_GET, [](AsyncWebServerRequest *request) {
    loginBoth(request, "both");
    value = 2; // for the update of the main page button
    switchOnNow(true, true, "web int");
    request->send_P(200, "text/html", HOMEPAGE);
});


server.on("/get.Homepagedata", HTTP_GET, [](AsyncWebServerRequest *request) {     

    uint8_t remote = 0;
    if(checkRemote( request->client()->remoteIP().toString()) ) remote = 1; // for the menu link
    int state = value;
    if(state > 1) state = 1;    
    // set the array into a json object
    // in senSor we find a int that reflects the sensor
    // 1 = DS18B20 2 = dht11 3 bme280 4=motion 5=button 6=MAX44009 7=digital
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    //StaticJsonDocument<160> doc; //(160);
    DynamicJsonDocument root(240); //(160);

 
// float humidity, temp_f, temp_c;

    root["name"] = String(dvName);
    root["state"] = String(state); // 0 or 1
    root["rm"] = remote;

    switch (senSor)  {
      case 0: root["sensor"]="none"; root["unit"]="&#8451;"; break;     
      case 1: 
          root["sensor"]="DS18B20" ;
          root["readings"] = String(temp_c, 1); 
          root["unit"]="&#8451;";
          break;
      case 2: 
          root["sensor"]="DHT11"; 
          root["readings"]= String(temp_c, 1) + " / " + String(humidity, 1); 
          root["unit"]="&#8451; / %";
          break;
      case 3: 
         root["sensor"]="BME280";
         root["readings"]=String(temp_c, 1) + " / " + String(humidity, 1)  + " / " + String(p, 1); 
         root["unit"]="&#8451; / % /hPa";        
         break;
      case 4: 
         root["sensor"]="motion"; 
         if(value == 18) root["readings"] = "1"; else root["readings"] = "0"; 
         root["unit"]="1 / 0";
         break;
      case 5: root["sensor"]= "button"; root["readings"] = "0"; root["unit"]="1 / 0"; break;
      case 6: 
         root["sensor"]="MAX4409"; 
         root["readings"] = String(p, 1);  
         root["unit"]="lux"; 
         break;
      case 7: root["sensor"]="digital"; root["readings"] = "0"; root["unit"]="1 / 0"; 
    }    
    //request->send(200, "text/json", json);
    //json = String();
    serializeJson(root, * response);
    request->send(response);
});


// if everything failed we come here
server.onNotFound([](AsyncWebServerRequest *request){
  Serial.println("unknown request");
  handleNotFound(request);
});

server.begin(); 
}

void loginBoth(AsyncWebServerRequest *request, String who) {
  String authFailResponse = "<h2>login failed <a href='/'>click here</a></h2>";
  if (who == "admin" ){
  const char* www_realm = "login as administrator."; 
    if (!request->authenticate("admin", pswd)) return request->requestAuthentication(www_realm, authFailResponse);
  }
  if (who == "both" ){
  const char* www_realm = "login as administrator or user."; 
    if (!request->authenticate("admin", pswd) && !request->authenticate("user", userPasswd)) return request->requestAuthentication(www_realm, authFailResponse);
  }
}

void confirm() {
toSend="<html><style>body {font-size:64px;}</style><body onload=\"setTimeout(function(){window.location.href='/';}, 3000 );\"><br><br><center>processing<br>your request,<br>please wait</body></html>";
}

//} check value
