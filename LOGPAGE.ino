
// const char HTML_LOGPAGE[] PROGMEM = R"=====(
// <!DOCTYPE html><html><head><meta charset='utf-8'>
// <title>sonoff log</title>
// <meta name="viewport" content="width=device-width, initial-scale=1">
// <link rel="icon" type="image/x-icon" href="/favicon.ico"/>
// <link rel="stylesheet" type="text/css" href="/STYLESHEET">
// <style>
// #lijst {
//   font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
//   color: black;
//   border-collapse: collapse;
//   width: 98%;
//   font-size:18;
// }
// #lijst td, #customers th {
//   border: 1px solid #ddd;
//   padding: 6px;
// }
// #lijst tr:nth-child(even){background-color: #f2f2f2;}
// #lijst tr td:nth-child(1) { font-size:14px; width:28%}
// #lijst tr td:nth-child(2) { font-size:14px; width:22%}
// #lijst tr td:nth-child(3) { font-size:14px; width:47%}

// #lijst th {
//   padding-top: 5px;
//   padding-bottom: 5px;
//   padding-left: 10px;
//   text-align: left;
//   background-color: #4CAF50;
//   color: white;
// }
// tr {height:20px;}

// @media only screen and (max-width: 600px) {
// #lijst td, #customers th { padding: 4px;}
// #lijst {font-size:12px;} 
// #lijst tr td:nth-child(1) { width:32%}
// #lijst tr td:nth-child(2) { width:26%}
// #lijst tr td:nth-child(3) { width:40%}

// }

// </style>
// <script> function cl() { window.location.href='/MENU'; } </script>
// </head>
// <body>
// <div id='msect'>
//   <div id='menu'>
//   <a href="#" class='close' onclick='cl();'>&times;</a>
//   <a href="#" onClick='window.location.reload();' >refresh</a>
//   <a onclick="return confirm('wipe log, are you sure?')" href='/CLEAR_LOG'>wipe</a>
//   </div>
// </div>
// <div id='msect'>
//   <kop>SONOFF LOG</kop>
// </div>

// <div id='msect'>
//   <div class='divstijl'>
//   <table><tr><td style='width:240px;'>Last refresh : !@@!<td></table><br>
  
//   <table id='lijst'>
//   <tr><th>Time</th>
//   <th>Type</th>
//   <th>Command</th></tr> 
//   <tr><td></td><td></td><td></td></tr>   
//   <cont> 
//   </tr><tr><td></td><td></td><td></td></tr>   
//   </table></div>
// </div>
// </body>
// </html>
// )=====";

// void zendPageLog() {

//  String uur = String(hour());
//  if(hour() < 10) { 
//         uur = "0" + String(hour());
//  } 
//   String minuten = String(minute());
//   if(minute() < 10) { 
//        minuten = "0" + String(minute());
//  }

// String cont = "";
// cont += uur + " : " + minuten + " hr.";
 
// toSend=FPSTR(HTML_LOGPAGE);

// //toSend.replace("HAIPS" , String(dvName));

// toSend.replace("!@@!", cont);

// consoleOut(" zendlogpage :building eventlist");  
//   byte Log_Count = 0;
//   Log_MaxReached ? Log_Count = Log_MaxEvents : Log_Count = Log_CurrentEvent;  // determine if the max number of event is already reached

//   int j = Log_CurrentEvent;
//   String content = "";
//   for ( int i = 1; i <= Log_Count; i++ ) {
//  //Serial.println("een regel van de lijst, nummer i = "); Serial.println(i);    
//   j--; //  we get the index of the last record in the array
//  //Serial.println("een regel van de lijst, nummer j = "); Serial.println(j);
//   if (j ==-1) j = Log_MaxEvents - 1; // if we are under the first index of the array ,we go to the last
//   ////////////////// One table line ///////////////////
//   content +=             "<tr><td>";
//   content +=             Log_EventList[j].Log_date;
//   content +=             "</td><td>";
//   content +=             Log_EventList[j].Log_kind;
//   content +=             "</td><td>"; 
//   content +=             Log_EventList[j].Log_message;
//   content +=             "</td>";
// ////////////////// One table line ///////////////////
//   }
// //Serial.print(content);
// toSend.replace("<cont>", content);
// //request->send(200, "text/html", toSend); 
//  }

//  void Update_Log(String what, String message) {
//   String nu;
//         consoleOut("updating the log");
//         if(what != "clear") {
//         nu = String(day()) + "-" + String(hour()) + ":" + String(minute()) + ":" + String(second());
//         } else { 
//           nu = "";
//           what = "";}
//         //DebugPrint("nu = "); DebugPrintln(nu);
//         Log_EventList[Log_CurrentEvent].Log_date = nu;
//         Log_EventList[Log_CurrentEvent].Log_kind = what;
//         //Log_EventList[Log_CurrentEvent].Log_issued = who;
//         Log_EventList[Log_CurrentEvent].Log_message = message;
//         Log_CurrentEvent++;
//         if (Log_CurrentEvent >= Log_MaxEvents)
//         {
//             Log_CurrentEvent = 0;//start again
//             Log_MaxReached = true;
//         }
// }
// void Clear_Log() {
//   //Serial.println("clearing the log");
//   String nu="";
//   String what="";
//   String message="";
//         for (int i=0; i <= Log_MaxEvents; i++) {
//         Update_Log("clear", "");
//         }
//      Log_CurrentEvent = 0;//start again
//      Log_MaxReached = false;     
//     //Serial.println("log cleared");   
// }
