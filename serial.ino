void handle_Serial () {
      DebugPrintln("we are in handle serial");  
      int SerialInByteCounter = 0;
      char InputBuffer_Serial[100] = "";
      byte SerialInByte;  
  
      DebugPrintln("incoming bytes available, number = " + String(Serial.available()) );
  
      while(Serial.available()) {
             SerialInByte=Serial.read(); 
             //Serial.print("+");
            
            if(isprint(SerialInByte)) {
              if(SerialInByteCounter<100) InputBuffer_Serial[SerialInByteCounter++]=SerialInByte;
            }    
            if(SerialInByte=='\n') {                                              // new line character
              InputBuffer_Serial[SerialInByteCounter]=0;
              DebugPrintln(F("found new line"));
             break; // serieel data is complete
            }
       }  
     DebugPrintln("InputBuffer_Serial = " + String(InputBuffer_Serial) );
     // evaluate the incomming data
     if (strlen(InputBuffer_Serial) > 6) {                                // need to see minimal 8 characters on the serial port
       if (strncmp (InputBuffer_Serial,"10;",3) == 0) {                 // Command from Master to RFLink
  
          if (strncasecmp(InputBuffer_Serial+3,"LIST",4) == 0) {
              Serial.println(F("*** AVAILABLE COMMANDS ***"));
              Serial.println(F("10;ERASEFLASH; (wipe flash contents)"));                            
              Serial.println(F("10;SHOWVAR; (value of a var)"));
              //Serial.println(F("10;DEL=filename; (delete file)"));
              
          } else 

                      
           if (strncasecmp(InputBuffer_Serial+3,"ERASEFLASH=",11) == 0) {  
              //input can be 10;DELETE=filename
               eraseFlash(); 
              return;             
           

          } else {
               sprintf(InputBuffer_Serial, "20;CMD UNKNOWN;"); // Node and packet number 
               Serial.println( InputBuffer_Serial );
          }
       } // end if if (strncmp (InputBuffer_Serial,"10;",3) == 0)
    } //  end if strlen(InputBuffer_Serial) > 6
} // 


 
