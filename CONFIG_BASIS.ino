//{check value

//    <tr><td>active automation<td><select name='autoMate' id='timer' class='sb1'>
//    <option value='0' tM0>none</option>
//    <option value='1' tM1>on/off timers</option>
//    <aption1 value='3' tM3>thermostate</option>
//    <aption2 value='4' tM4>hygrostate</option>
//    <aption3 value='6' tM6>light sensor</option>
//    <aption4 value='7' tM7>digital sensor</option>
//    </select></td></tr>

const char BASISCONFIG[] PROGMEM = R"=====(
<body>
<script>
function cTrig(box) {
  if (box.checked) {
    if (!confirm("Sensors cannot work when this box is checked. Are you sure you want that?")) {
        box.checked = false;
    }
  }
}
</script>


<div id='msect'>

  <div id='help'>
  <span class='close' onclick='sl();'>&times;</span><h3>SETTINGS HELP</h3>
  <b>security:</b><br>if switched on manually, automatic switch off after this time.
  Default is 6 hours.<br><br>
    <b>switch name:</b><br>If you avoid spaces, the hostname will be reflected correctly by your router.
  <br><br><b>user passwd:</b><br>Grant housemates limited access with username  
  <span style='color:red;'>user</span> and the <span style='color:red;'>user passw.</span> 
  <br><br><b>active automation:</b><br>The desired behaviour of the automatic switching.
  <br>Eventually setup the connected sensor in the sensors menu. 
  <br><br><b>Serial debug:</b><br>When checked we can see debug info on a serial monitor. <br>Sensors will not work then, and polling them<br>
  will cause a reboot!!
  <br><br></div>
</div>

<div id='msect'>
<div id="menu">
<a href='#' onclick='helpFunction();'>&quest;</a>
<a href="#" id="sub" onclick='submitFunction()'>save</a>
<a href="#" class='close' onclick='cl();'>&times;</a>
</div>
</div>
<div id='msect'>
<kop>GENERAL SETTINGS</kop>
</div>
<div id='msect'>
  <div class='divstijl' style='width: 480px; height:56vh;'>
    <form id='formulier' method='get' action='submitForm' oninput='showSubmit()'>
    <center><table>
    
    <tr><td style='width:140px;'>device name<td><input class='inp6' name='dvName' maxlength='21' title='no spaces, see help' value='{nm}'></input></tr>
    <tr><td>user passwd<td><input  class='inp5' name='pw1' length='11' placeholder='max. 10 char' value='{pw1}' pattern='.{4,10}' title='betweeen 4 and 10 characters'></input></tr> 
    <tr><td>security<td><input class='inp2' name='bev' value='{bev}' pattern='(?:[01]|2(?![4-9])){1}[0-9]{1}:[0-5]{1}[0-9]{1}' title='hh:mm' ></input></tr> 
    <tr><td>serial debug:<td><input type='checkbox' style='width:30px; height:30px;' name='debug' #sjek onchange='cTrig(this)'></></input></td></tr>
    </table></form>
    
    </center>
  </div><br>
</div>

</body></html>
  )=====";

void zendPageBasis(AsyncWebServerRequest *request) {
  consoleOut("we are now on zendPageBasis");
  //String(webPage)="";
  toSend = FPSTR(HTML_HEAD);
  toSend += FPSTR(BASISCONFIG);
  
  // naam terugzetten
  toSend.replace("'{nm}'" , "'" + String(dvName) + "'") ;
  toSend.replace("'{pw1}'" , "'" + String(userPasswd) + "'") ;
  toSend.replace("'{bev}'" , "'" + String(aso) + "'") ;
  if (diagNose){ toSend.replace("#sjek","checked"); }
//  replace_timerselectbox(); // to set the select for active automatition
  request->send(200, "text/html", toSend);
}



// check if 
void check_mismatch() {
int klik=0;
  if ( autoMate == 3 && (senSor != 1 && senSor != 2 &&  senSor != 3 )) {
    int klik=1;
  } 
//  if ( timer[0] == '3' && (sensor[0] != '1' && sensor[0] != '2' &&  sensor[0] != '3' )) {
//    int klik=1;
//  }  
  if ( autoMate == 4  && ( senSor != 2 &&  senSor != 3 )){
    int klik=1;
  }
//  if ( timer[0] == '4'  && ( sensor[0] != '2' &&  sensor[0] != '3' )){
//    int klik=1;
//  }
  if ( autoMate == 6  &&  senSor != 6 ){
    int klik=1;
  }
//  if ( timer[0] == '6'  &&  sensor[0] != '6' ){
//    int klik=1;
//  }
  if ( autoMate == 7  &&  senSor != 7 ){
    int klik=1;
  }
//  if ( timer[0] == '7'  &&  sensor[0] != '7' ){
//    int klik=1;
//  }
//  if ( klik == 1 ) {
//    timer [0] = '0';
//  }
  if ( klik == 1 ) {
    autoMate = 0; 
  }
}



//} // check
