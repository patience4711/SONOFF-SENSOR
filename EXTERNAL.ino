// find out if the request comes from inside the network
//check if the first 9 characters of the router's ip ( 192.168.0 ) is in the url
bool checkRemote(String url) {
   if(securityLevel==0) return false; //no security so never remote
   if ( url.indexOf(WiFi.gatewayIP().toString().substring(0, securityLevel)) == -1 ) return true; else return false;
//return false;
}

// hier komen we als een onbekende request is gedaan
void handleNotFound(AsyncWebServerRequest *request) {
  
String serverargument = request->url().c_str();
DebugPrintln("we zijn in handleNotFound");
DebugPrint("serverargument = "); DebugPrintln(serverargument);
// als de client van buiten het netwerk komt mag hij niks

String addy = request->client()->remoteIP().toString();
bool intern = false;
String router=WiFi.gatewayIP().toString();
router = router.substring(0, 9);
DebugPrint("router substring 0-9 = "); DebugPrintln(router);

if ( addy.indexOf(router) == -1 ) { // indexOf geeft -1 als niet gevonden
  DebugPrintln("the client comes from outside the network");
  intern = false; 
  } else {
  DebugPrintln("the client comes from inside the network"); 
  intern = true; }

if ( intern ) { 
             
             if(serverargument.indexOf("SWITCH=ON") >-1 ){
              value = 3;
              switchOnNow(true, true, "web ext");
              request->send(200, "text/plain", "ok switched on");  
             } else
             if(serverargument.indexOf("SWITCH=OFF") >-1 ){
              value = 0;
              switchOffNow(true, true, "web ext");
              request->send(200, "text/plain", "ok switched off");  
             } else            
            
             // no valid api found    
             request->send ( 200, "text/plain", "<h2>ERROR this link is invalid, go back <--</h2>" );//send not found message
             }             

    else { // not intern
           //if not internal, this user should be logged in
      
      DebugPrint("\t\t\t\t unauthorized, not from inside the network : ");
      request->send ( 200, "text/plain", "<h2>ERROR you are not authorised, go back <--</h2>" );//send not found message
    }
}
