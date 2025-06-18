const char PORTAL_HEAD[] PROGMEM = R"=====(
<!DOCTYPE html><html><html lang=\"en\"><head><meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
<title>hansiart wificonfig</title>
<head>
<style>
#msect {
  display: flex;
  flex-basis: 100%;
  align-items: center;
  margin: 0px 0px;
  width:96vw;
}
#msect:before,
#msect:after {
  content: "";
  flex-grow: 1;
  margin: 0px 0px;
}
kop {
  font-size: 4vw;
  font-weight:bold;
  padding-top: 1vh;
  padding-bottom: 1vh;
}
div,input{padding:5px;font-size:1em;} 
input{width:95%;} 
body{ text-align: center; font-family:verdana;} 
button{border:0;border-radius:0.3rem;background-color:#009933;color:#fff;line-height:2.4rem;font-size:30px;width:100%;}  
.q{float: right;width: 64px;text-align: right;} 
.btn,h2{font-size: 20px}
.btn{width:300px;height:60px;background:#0066FF; outline: none;}
.btn2{width:260px;height:60px;background:#CC00FF; outline: none; font-size: 14px}
@media only screen and (max-width: 800px) {
body { 
  font-size:14px;
  }
</style>
</head>
  )=====";


const char PORTAL_HOMEPAGE[] PROGMEM = R"=====( 
<body><script>
function c(l){
  document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();
  }
</script>
<script>
if (window.location.hostname != "192.168.4.1") {
   window.location.href = 'http://192.168.4.1'; 
} 
</script>
<br><h2>HANSIART WIFI CONFIG</h2>
<h3>device mac adres : {ma}<h3>
<form action='wifiForm' method='get'>
<div hadden><button class="btn">WIFI CONFIGURATION</button></div>
</form><br><br>
<div id='close' hidden><form action='wifiClose' method='get'>
<button class='btn'>CLOSE</button>
</form>
<br><br></div>
<strong>Currently not connected to a wifi network!</strong><br>
<br>

</body></html>
  )=====";

// ******************************************************************************************
//                           THE WIFI FORM
// ******************************************************************************************
const char PORTAL_WIFIFORM[] PROGMEM = R"=====( 
<body>
<script>
function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}
function showdiv() { 
  document.getElementById('submitdiv').style.display = 'block';
  setTimeout(function(){ window.location.href='/wifiConfirm'; }, 3000 );
}
</script>
<div id="msect" style='background:lightblue;'>
<center>
<div id='submitdiv' style='opacity:1.0; color:white; position:fixed; top:0; left:0; height:100%; width:100%; background-color:rgba(100,100,100,0.75);z-index:1000; display:none;'>
<br><br><br><h1>trying to connect..</h1><br>
</div>

<kop>HANSIART CONFIG PORTAL </kop></center></div>
</div>
<div id="msect">
  <div style='height:50vh; width:99vw; border: 3px solid #f03467'><kop style=font-size:3vw;'>scanned wifi networks</kop>
  aplijst
  </div>
</div>
<div id='msect'>
  <div style='border: 3px solid #f03467; background:lightyellow; width:99vw'>
  <form method='get' action='wifiCon'>
  <center><table><tr><td style='width:120px;'>wifi network<td><input id='s' name='s' length=32 placeholder='SSID' required></input></td></tr>
  <tr><td>wifi passwd<td><input id='p' name='p' length=64 type='text' placeholder='password' required></input></tr>
  <tr><td>admin passwd<td><input  name='pw' length='11'  value='{pw}' pattern='.{4,10}' title='between 4 en 10 chars'></input></tr> 
  <tr><td>security level<td><input type='number' min='0' max="9" name='sl' value='{sl}'></input></tr> 
  </table><br/>
  <button type='submit' onclick='showdiv()' style='float:left'>save</button>
  </form></div>
</div></body></html>
  )=====";

//<script type='text/javascript'>setTimeout(function(){ window.location.href='/CONCLUDE'; }, 3000 ); </script>
const char PORTAL_CONFIRM[] PROGMEM = R"=====( 
<body style='font-size:16px;'>
<h1>  CONNECT TO WIFI</h1><br><br><br>
<strong>please wait until the led has flashed and is on again, than click OK</strong><br><br>
<br><br>In case of troubles you can disconnect and reconnect from the AP now.<br><br>
<form action='/home' method='get'><button class='btn' type='submit'>OK</button></form>
<br><br>
</body></html>
  )=====";  

const char PORTAL_LIST[] PROGMEM = R"=====( 
  <div><a href="#p" onclick="c(this)">{v}</a>&nbsp;<span class="q {i}">{r}dBm</span></div>
  )=====";   
