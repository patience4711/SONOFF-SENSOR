// find out if the request comes from inside the network
//check if the first securityLevel characters of the router's ip eg ( 192.168.0 ) is in the url
bool checkRemote(String url) {
   if(securityLevel==0) return false; //no security so never remote
   if ( url.indexOf(WiFi.gatewayIP().toString().substring(0, securityLevel)) == -1 ) return true; else return false;
//return false;
}

// we are here when an onknown request is done
void handleNotFound(AsyncWebServerRequest *request) {
  
String serverargument = request->url().c_str();
consoleOut("HandleNotFound argument = " + String(serverargument));
// if the client is remote he can't do anything
bool intern = false;
if(!checkRemote( request->client()->remoteIP().toString()) ) intern = true;

if ( intern ) { 
//             if(serverargument.indexOf("ERASE_FLASH") >-1 ){
//                    flashErase(request);
//                    //request->send(200, "text/plain", "Flash Erase and reboot!");
//             return;
//             
//             } else             
             if(serverargument.indexOf("SWITCH=ON") >-1 ){
              value = 3;
              switchOnNow(true, true, "web ext");
              request->send(200, "text/plain", "ok switched on");  
              return;
             } else
             if(serverargument.indexOf("SWITCH=OFF") >-1 ){
              value = 0;
              switchOffNow(true, true, "web ext");
              request->send(200, "text/plain", "ok switched off");  
              return;
             } else            
            
             // no valid api found    
             request->send ( 200, "text/plain", "<h2>ERROR this link is invalid, go back <--</h2>" );//send not found message
             }             

    else { // not intern
           //if not internal, this user should be logged in
      
      consoleOut("\t\t\t\t unauthorized, not from inside the network : ");
      request->send ( 200, "text/plain", "<h2>ERROR you are not authorised, go back <--</h2>" );//send not found message
    }
}
