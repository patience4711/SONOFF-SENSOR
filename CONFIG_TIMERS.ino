//{check value
void zendpageTimers() {
consoleOut("we are at zendPageTimers");

toSend = FPSTR(HTML_HEAD);
toSend += FPSTR(TIMERCONFIG_START);

toSend.replace("tieTel", dvName );

// initial the last visited timer is opened
//tKeuze=0;
zendPageRelevantTimers();

}
// ****************************************************************************
// we gaan nu een pagina tonen met de voor te timer relevante inputs
// ****************************************************************************
void zendPageRelevantTimers() {
  toSend = FPSTR(HTML_HEAD);
  toSend += FPSTR(TIMERCONFIG_START);
  toSend.replace( "{title}" , String(dvName)) ;
  toSend.replace( "{device}" , String(dvName)) ;
  //eerst weer de select en keuze terugzetten
  //replace_timerselectbox();
  
consoleOut("we are at zendPageRelevantTimers, tkeuze = " + String(tKeuze));
  plaats_timerpage();  //plaats de timerpage voor de gekozen timer
}






// **********************************************************************
//        replace the timerpage with the actual values
// **********************************************************************
void plaats_timerpage() {
// we must plate the timerpage with the actual values 
    toSend.replace("<irame name>" , FPSTR(TIMER_GENERAL));  
    toSend.replace("{nr}" , String(tKeuze)); // vervang timer nummer
    if(timerActive[tKeuze] == '1') toSend.replace("tActive", "checked");
    // replace the value fo program to start
    //toSend.replace("'{prg}'" , "'" + String(defaultProg[tKeuze]) + "'") ;
    // we put back "selected" for the option in the selectbox zonattaanwelke_1 2 3 4 or 5 
    consoleOut("page_replace");
    
    toSend.replace(zonatt_replace(String(relToSunOn[tKeuze]), "zonattaan"), "selected"); 
    toSend.replace(zonatt_replace(String(relToSunOff[tKeuze]), "zonattuit"), "selected"); 
    
     // put back the checked selectboxes 
    char *grap[] = {"selzo", "selma", "seldi", "selwo", "seldo", "selvr", "selza"};
    String vervang = "";
    //weekDag
     consoleOut("replace checkboxes to show the checked ones");
    // voor deze timer doen, 7x
    //int i = tKeuze;
        for(int x=0; x<7; x++){ // bij 3 is dit van 21 tot 27 kleiner dan 28

          vervang = String(grap[x]); // als i=3 dan van 21-21 naar 27

               if (weekDag[tKeuze*7+x] == 'y') { toSend.replace(vervang, "checked");}
         }
    
    
      consoleOut("replace the time values");
      toSend.replace("{inx}"  , "" + String(switchOn[tKeuze*5]) + String(switchOn[tKeuze*5+1]) + String(switchOn[tKeuze*5+2]) + String(switchOn[tKeuze*5+3]) + String(switchOn[tKeuze*5+4]));
      toSend.replace("{uitx}" , "" + String(switchOff[tKeuze*5]) + String(switchOff[tKeuze*5+1]) + String(switchOff[tKeuze*5+2]) + String(switchOff[tKeuze*5+3]) + String(switchOff[tKeuze*5+4])); 
    }  
    // placed the page for the chosen timer


// this function returns the value of vervang
// input is p.e. "1" and zooattaan
String zonatt_replace( String argument1, String argument2) {

String vervang = "";
switch (argument1[0]) {
  case '0': //absoluut
    vervang = argument2 + "_1";
    break;
  case '1': // before sunset
    vervang = argument2 + "_2";
    break;
  case '2': // after sunset
    vervang = argument2 + "_3";
    break;
  case '3': // before sunrise
    vervang = argument2 + "_4";
    break;
  case '4': // after sunrise
    vervang = argument2 + "_5";
    break;
    }
return vervang;
}


// ********************************************************************************************
// *                   switching by the timers or manual                                               *
// ********************************************************************************************
void schakelen() {
    // *******************************************************************************************
    //                             switch by timer 0  
    // *******************************************************************************************
    if (timerActive[0]=='1' && mustSwitch[0] && !hasSwitched[1] && !hasSwitched[2] && !hasSwitched[3] ) {  //als niet door timer1 of 2 is ingeschakeld
    test_schakel_in(0);
    test_schakel_uit(0);
    }
    // *******************************************************************************************
    //                             switch by timer 1  
    // *******************************************************************************************
    if (timerActive[1]=='1' && mustSwitch[1] && !hasSwitched[0] && !hasSwitched[2] && !hasSwitched[3] ) {  //als niet door timer0 of 2is ingeschakeld  
    test_schakel_in(1);
    test_schakel_uit(1);
    }
    //// *******************************************************************************************
    ////                           switch by timer 2 
    //// *******************************************************************************************
    if (timerActive[2]=='1' && mustSwitch[2] && !hasSwitched[0] && !hasSwitched[1] && !hasSwitched[3] ) { // als niet door timer 0 of 1 of 3 
    test_schakel_in(2);
    test_schakel_uit(2);
    }
    //// *******************************************************************************************
    ////                           switch by timer 3 
    //// *******************************************************************************************
    if (timerActive[3]=='1' && mustSwitch[3] &&  !hasSwitched[0] && !hasSwitched[1] && !hasSwitched[2])  {  
    test_schakel_in(3);
    test_schakel_uit(3);
    }
}


void test_schakel_in(int welke) {
            if ( now() > (inschakeltijd[welke]) && now() < uitschakeltijd[welke] && !hasSwitched[welke]) { 
                String who = "timer " + String(welke);
                switchOnNow(true, false, "timer " + String(welke)); // meteen de lamp aan
                hasSwitched[welke] = true;
                value = 3 + welke; // 3 of 4 of 5 of 6
                delay(100); // 
            }
}
 
void test_schakel_uit(int welke) {
         if ( now() > uitschakeltijd[welke] && hasSwitched[welke] ) { // als event 3 4 5 of 6 is
              String who = "timer " + String(welke);
              switchOffNow(true, false, who); //meteen de lamp uit en mqtt message en checkTimers
              mustSwitch[welke] = false;
              hasSwitched[welke] = false; // prevent repetitions
              Serial.println("switched off by timer"+ String(welke));
             }
}

void checkTimers() {
            // if switched off manual this should prevent that a timer puts it on again
            // so we disarm a timer that is active at that moment 
            if ( hasSwitched[0] ) mustSwitch[0] = hasSwitched[0] = false; //hasSwitched[0]=false; // to prevent that its switches on again
            if ( hasSwitched[1] ) mustSwitch[1] = hasSwitched[1] = false;// to prevent that its switches on again
            if ( hasSwitched[2] ) mustSwitch[2] = hasSwitched[2] = false;// to prevent that its switches on again
            if ( hasSwitched[3] ) mustSwitch[3] = hasSwitched[3] = false;// to prevent that its switches on again 
}

void switchOnNow(bool zend, bool check, String logID) {

      switchOnmoment = now();
      if ( digitalRead(RELAY_PIN) != SWITCH_AAN ) {
      digitalWrite(RELAY_PIN, SWITCH_AAN); // schakel de lamp in als die uit was
      }
      if( zend ) { mqttSwitchupdate(); }// mqtt message about switch state 
      if( check ) {checkTimers();} // disarm timer if needed
      if(logID != "") Update_Log(logID, "on"); 
      eventSend();
    }

    void switchOffNow(bool zend, bool check, String logID) {
    if ( digitalRead(RELAY_PIN) != SWITCH_UIT ) {
        digitalWrite(RELAY_PIN, SWITCH_UIT); // switch off
    }
        value = 0;
        if( zend ) { mqttSwitchupdate(); }// mqtt message about switch state
        if( check ) {checkTimers();} // disarm timer if needed
        if(logID != "") Update_Log(logID, "off");
        eventSend();
    }
//}   
