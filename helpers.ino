//{ check ***********
String getChipId(bool sec) {
  if(sec) return String(ESP.getChipId()); else return "SONOFF-" + String(ESP.getChipId());
}

// function to show debug info
void consoleOut(String toLog) {
// decide to log to serial or the console 
  if(diagNose) 
  {
    Serial.println(toLog);
  } 
}

void ledblink(int i, int wait) {
  for (int x = 0; x < i; x++) {
    digitalWrite(led_onb, LED_AAN);
    delay(wait);
    digitalWrite(led_onb, LED_UIT);
    delay(wait);
  }
}

void eraseFlash() {
         noInterrupts();
         const size_t cfgSize = 0x4000;
         size_t cfgAddr = ESP.getFlashChipSize() - cfgSize;

        for (size_t offset = 0; offset < cfgSize; offset += SPI_FLASH_SEC_SIZE) {
            if (!ESP.flashEraseSector((cfgAddr + offset) / SPI_FLASH_SEC_SIZE)) {
                Serial.println("erase flash fail");
            }
        }
        interrupts();
 }


 // ****************** check the actionflags *************************
 void checkActionflags() 
 {   
        if (actionFlag == 11 ) 
        { // the button has been long pressed, start in AP
           toSend="";
           delay(1000); //time to release button
           DebugPrintln("erasing the wifi credentials, value = " + String(value) + "  actionFlag = " + String(actionFlag));
           eraseFlash(); // wipe the sector where the cerdentials are saved
           ESP.restart();
       }
       if (actionFlag == 10)
       { // the button has been pressed utill the led goes on. resetting
           delay(2000); // give the server the time to send the confirm
           DebugPrintln("rebooting on actionFlag==10");
           //write_eeprom();
           ESP.restart();
      }
      if (actionFlag == 20)
      {
          actionFlag = 0;
          value = 1;
          switchOnNow(true, true, "button");
      }  
      if (actionFlag == 21) 
      {
          actionFlag=0;
          //value = 0; not needed
          switchOffNow(true, true, "button");
      }
      if (actionFlag == 15) // the button
      {
          actionFlag=0;
          meetENschakel();
      }

      if(actionFlag == 25)  // when timers or sensors changed
      { 
         consoleOut("recalculating all timers and sensors");
         actionFlag=0;
         switchOffNow(true, false, "settings"); // make current state off
         getTijd(); // recalculate and (dis)arm all the timers (don't need reboot)
         meetENschakel(); //renew sensor readings
      }

}

void eventSend() {
     events.send( "getall", "message"); //getAll triggered
}
//} ************** 
