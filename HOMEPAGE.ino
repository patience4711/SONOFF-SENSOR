
const char HOMEPAGE[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8'>
<title>sonoff sensors</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" type="image/x-icon" href="/favicon.ico" />
<link rel="stylesheet" type="text/css" href="/STYLESHEET">
<style>
#td0 {width:100px;}
#td1 {width:150px;}
#td2 {width:90px;}
table, th, td {
  border: 2px solid blue; font-size:16px; padding:6px; text-align:center; border-collapse:collapse;backgound-color:#dfff80;
  }
tr {background-color:#ccffcc;}
.bth {margin-bottom: 50px;}

@media only screen and (max-width: 800px) {
th, td { font-size:11px; }
#td0 {width:80px}
#td1 {width:120px;}
#td2 {width:80px;}
.bth  {margin-bottom: 220px;}
}
</style>
</head>
<body onload='loadScript()'>

<div id='msect'>
<div id="menu">
<a id="ml" href='/MENU' style='float:right; display:none'>menu</a>
</div>
</div>
<div id='msect'>
<kop id='naamveld'>ESP</kop>
</div>
<div id='msect'>
    <div class='divstijl' id='maindiv' style='height:76vh; background: grey;'>
    <center>
    <span id='pwdby' style='font-size:11px; display:table; color: white;'>powered by Hansiart</span><br>
    <br><button class='bt' id='knop' onclick='myFunction();' >SWITCH</button>
    <p class="bth"></p>
    <center><table id='table' style="display:none;">
          <tr style='Background-color:lightblue; font-weight:bold; text-align:center; border:4px solid black;'>
          <td id="td0">SENSOR<td id="td1">READINGS<td id="td2">UNIT</td></tr>
          <tr><td id="p0"></td><td id="p1"></td><td id="p2"></td></tr></table>
    </div>
<div>
</body>
<script>
// refresh when gets focus
window.addEventListener('visibilitychange', () =>{
    if (document.visibilityState === 'visible') {
    getData();
    }
})

function loadScript() {
// the first time we load the settings at once
getData();
}


function myFunction() {
var xhttp = new XMLHttpRequest();  
var btn = document.getElementById('knop');
var veld = document.getElementById('veld'); 
    if(!btn.classList.contains('bt_on')) { // the lamp is off
          btn.classList.add('bt_on');
          xhttp.open("GET", "SW=ON", true);
          xhttp.send();
    } else {
          btn.classList.remove('bt_on');
          xhttp.open("GET", "SW=OFF", true);
          xhttp.send();
    }
    //getData(); // sets the page props
    //setTimeout(getData, 2000);
}

//setInterval(function loadData() {
//  getData();
//},6000);

function getData() {
var btn = document.getElementById('knop');
var veld = document.getElementById('veld');  
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var antwoord = this.responseText;
      var obj = JSON.parse(antwoord);
      //var name = obj.name;
      var state = obj.state;
      if (state == "0") {
        btn.classList.remove('bt_on');
      } else {
        btn.classList.add('bt_on');
      }
      var rem = obj.rm;
      if(rem == 0) {document.getElementById("ml").style.display = "block";} // show menu link 
      document.getElementById("naamveld").innerHTML=obj.name;
      if (obj.sensor !="none") {
      document.getElementById("table").style.display="table";
      document.getElementById("p0").innerHTML=obj.sensor;
      document.getElementById("p1").innerHTML=obj.readings;
      document.getElementById("p2").innerHTML=obj.unit;
      }else{
      document.getElementById("table").style.display="none";  
      }
    
    }
  }
  xhttp.open("GET", "get.Homepagedata", true);
  xhttp.send();
}

if (!!window.EventSource) {
 var source = new EventSource('/events');

 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);

 source.addEventListener('message', function(e) {
  console.log("message", e.data);
  if(e.data == "getall") {
  getData();
  }

 }, false);
}
</script>
</html>
)=====";
