const char HTML_HEAD[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8'>
<title>sonoff sensors</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" type="image/x-icon" href="/favicon.ico" />
<link rel="stylesheet" type="text/css" href="/STYLESHEET">
<script type='text/javascript'>
function showSubmit() {
document.getElementById("sub").style.display = "block";
}
function submitFunction() {
document.getElementById('formulier').submit();
}
function cl() {
window.location.href='/MENU';
}
function sl() {
document.getElementById("help").style.display = "none";
}

function helpFunction() {
document.getElementById("help").style.display = "block";
}
</script>
</head>
)=====";

const char STYLESHEET[] PROGMEM = R"=====(
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}
body {
  background: linear-gradient(#e8f5f7, #caf5fb); 
  color:navy;
  font-family:'lato',Verdana,Sans-serif;
  font-size:16px;
  margin-left:2px;
  }

.divstijl {
  background: linear-gradient(#e8edc8, #c8eaed); padding:0px 0px 0px 0px; width: 60vw; height: 99%;  }

#msect {display: flex; flex-basis: 100%; align-items: center; margin: 0 auto; width:98vw;}
#msect:before,
#msect:after { content: ""; flex-grow: 1; margin: 0px 0px;}

p { color: #006600; font-family: 'lato',Verdana,Sans-serif; font-size:14px; }
.bt, .bt_small{
  width:30vw;
  padding:6px;
  margin:6px;
  text-align:center;
  border-radius:15px;
  color:white;
  background: #262626;
  font-weight:bold;
  font-size:4vw;
  box-shadow: 0 9px #999;
  }
.bt {height:16vw;}
.bt_small {height:8vw;}
.bt_on{
  background:yellow;
  color:black;
  box-shadow: 0px 0px 20px 15px red;
  }
kop { font-size: 3vw; font-weight:bold; padding-top: 1vh; padding-bottom: 1vh;}
input { font-size:20px; }
.inp1 { width:46px; }
.inp2 { width:62px; } 
.inp3 { width:66px; }
.inp4 { width:100px;} 
.inp5 { width:160px;}
.inp6 { width:220px;} 

option {
    direction: ltr;
}
.sb1 {font-size:20px; height:34px;} 
input, select{
  box-sizing: border-box;
  -moz-box-sizing: border-box;
  -webkit-box-sizing: border-box;
}

tr {height:36px;} 
input[type=checkbox] { width:24px; height:24px;
   }
#menu { height:48px; width: 60vw; overflow: hidden; background-color: #333; border-radius:6px;}
#menu a:link, #menu a:visited, #menu2 a:link, #menu2 a:visited  { /*only in the div menu*/
  background-color: #333;
  color: white;
  border-radius: 6px;
  text-align: center;
  text-decoration: none; /* prevent the underline */
  display: inline-block;
  width: 58px;
  line-height: 48px;
  margin-right: 2px;
}
#menu #sub { float: right; background:green;display:none }
#menu2 a:link, #menu2 a:visited {width:98px;}
#menu a:hover, #menu2 a:hover { background-color: green; color: white;}

.close { color: red !important; position: static; float:right; font-size: 42px; font-weight: bold; cursor: pointer;}

b { color: #eab3f2; margin-left: 4px; font-size:16px;}

#fleft {float:left;}

#help { background-color: #ffffff; border: solid 2px; display:none; padding:4px; width:94vw;}
b { color: #eab3f2; margin-left: 4px; font-size:16px;}

#fright {float:right;}
.hoogte {height:34px; }
@media only screen and (max-width: 800px) {
.inp3a { width:70px; font-size:18px; }
.divstijl { margin: 0 auto; width:98vw;}
.close { font-size: 32px; width:20px; padding: 0px 10px;}
input, .sb1 { font-size:16px; }
.sb1, .hoogte {height:24px;} 
body  { font-size:12px; margin-left: 2px;}
#menu {width:98vw;}
kop { font-size: 5vw;}
.bt_small {height:16vw;}
.bt {height:22vw;}
}  
)=====";
   

const char UPDATE_FORM[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8'>
<link rel="stylesheet" type="text/css" href="/STYLESHEET">
<style> 
input {
  font-size:3vw;
}  
</style>
<script>
function showDiv() {
document.getElementById('main').style.background="#b3ffd9";  
document.getElementById('hiddendiv').innerHTML="<marquee><kop>updating software, do not interrupt .....</kop></marquee>";
}
</script></head>
<body><div id='main'>
<br><br><center><kop> HANSIART FIRMWARE UPDATER</kop>
<br><br><br><table><tr><td>
<form method='POST' action='/handleFwupdate' enctype='multipart/form-data' onsubmit='showDiv()'>
<input id='inP' type='file' name='update' pattern='\S+' title='cannot be empty' required>
<tr><td><br><tr><td><input type='submit' value='update'></form>
<tr><td><br><tr><td><input type='submit' value='cancel' onclick="location.href = '/MENU';"></table><br>
</table><br>
<div id='hiddendiv' style='margin: 50px; width: 40%; background:#e7f2bd;'></div><br><br>
</div></center></body></html>  
)=====";

#define FAVICON_len 318
const uint8_t FAVICON[] PROGMEM = {
0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x10, 0x00, 0x01, 0x00, 0x04, 0x00, 0x28, 0x01,
0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00,
0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2E, 0xDB,
0xD6, 0x00, 0xD1, 0x0F, 0x18, 0x00, 0xB4, 0xDD, 0xDE, 0x00, 0x66, 0xE1, 0xE3, 0x00, 0xD3, 0xE6,
0xE5, 0x00, 0x81, 0xE1, 0xE3, 0x00, 0x9A, 0xDA, 0xDB, 0x00, 0x75, 0xDF, 0xE0, 0x00, 0xB5, 0x14,
0x9D, 0x00, 0xB6, 0xE3, 0xE2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44,
0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x49, 0x91, 0x99, 0x91, 0x19, 0x99, 0x19, 0x94, 0x49, 0x21,
0x22, 0x12, 0x21, 0x22, 0x12, 0x94, 0x49, 0x21, 0x66, 0x16, 0x61, 0x66, 0x12, 0x94, 0x49, 0x21,
0x55, 0x15, 0x51, 0x55, 0x12, 0x94, 0x49, 0x21, 0x57, 0x71, 0x17, 0x71, 0x11, 0x94, 0x49, 0x26,
0x50, 0x00, 0x00, 0x75, 0x62, 0x94, 0x49, 0x26, 0x57, 0x33, 0x30, 0x75, 0x62, 0x94, 0x48, 0x86,
0x58, 0x83, 0x80, 0x75, 0x62, 0x84, 0x48, 0x86, 0x58, 0x80, 0x88, 0x75, 0x68, 0x84, 0x48, 0x86,
0x58, 0x87, 0x88, 0x88, 0x88, 0x84, 0x48, 0x88, 0x88, 0x85, 0x58, 0x85, 0x88, 0x94, 0x48, 0x86,
0x68, 0x86, 0x66, 0x82, 0x82, 0x94, 0x48, 0x82, 0x28, 0x82, 0x22, 0x88, 0x82, 0x94, 0x48, 0x89,
0x98, 0x89, 0x99, 0x98, 0x99, 0x94, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
