#include <WiFi.h> 
#include <WebServer.h> 
#include <LiquidCrystal_I2C.h> 
#include <DHT.h> 
// WiFi AP settings 
const char* ssid = "SmartMirrorAP"; 
const char* password = "12345678"; 
// DHT11 settings 
#define DHTPIN 9   
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); 
// LCD settings 
LiquidCrystal_I2C lcd(0x27, 20, 4); 
// Web server 
WebServer server(80); 
// Time variables 
int currentHour = 0, currentMinute = 0, currentSecond = 0; 
int currentDay = 16, currentMonth = 9, currentYear = 2025; 
// Switch states 
int sw1 = 0, sw2 = 0, sw3 = 0, sw4 = 0; 
// ===== Helper for JSON (used in AJAX refresh) ===== 
String getStatusJSON() { 
float h = dht.readHumidity(); 
float t = dht.readTemperature(); 
String json = "{"; 
json += "\"time\":\"" + String(currentHour < 10 ? "0" : "") + String(currentHour) + ":" + 
(currentMinute < 10 ? "0" : "") + String(currentMinute) + ":" + 
(currentSecond < 10 ? "0" : "") + String(currentSecond) + "\","; 
json += "\"date\":\"" + String(currentDay < 10 ? "0" : "") + String(currentDay) + "-" + 
(currentMonth < 10 ? "0" : "") + String(currentMonth) + "-" + 
String(currentYear) + "\","; 
json += "\"temp\":" + String(t, 1) + ","; 
json += "\"hum\":" + String(h, 1) + ","; 
json += "\"sw1\":" + String(sw1) + ","; 
json += "\"sw2\":" + String(sw2) + ","; 
json += "\"sw3\":" + String(sw3) + ","; 
json += "\"sw4\":" + String(sw4); 
json += "}"; 
return json; 
} 
// ===== Time sync handler ===== 
void handleTimeSync() { 
if (server.hasArg("time") && server.hasArg("date")) { 
String timeStr = server.arg("time");  
String dateStr = server.arg("date");  
int h = timeStr.substring(0, 2).toInt(); 
int m = timeStr.substring(3, 5).toInt(); 
int s = timeStr.substring(6, 8).toInt(); 
int d = dateStr.substring(0, 2).toInt(); 
int mo = dateStr.substring(3, 5).toInt(); 
int y = dateStr.substring(6, 10).toInt(); 
if (h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60 && 
d >= 1 && d <= 31 && mo >= 1 && mo <= 12 && y >= 2023) { 
currentHour = h; currentMinute = m; currentSecond = s; 
currentDay = d; currentMonth = mo; currentYear = y; 
server.send(200, "text/plain", "Date & Time updated"); 
} else { 
server.send(400, "text/plain", "Invalid format"); 
} 
} else { 
server.send(400, "text/plain", "Missing params"); 
} 
} 
// ===== Switch Handlers ===== 
void handleSwitch1() { sw1 = server.arg("state").toInt(); server.send(200, "text/plain", "OK"); } 
void handleSwitch2() { sw2 = server.arg("state").toInt(); server.send(200, "text/plain", "OK"); } 
void handleSwitch3() { sw3 = server.arg("state").toInt(); server.send(200, "text/plain", "OK"); } 
void handleSwitch4() { sw4 = server.arg("state").toInt(); server.send(200, "text/plain", "OK"); } 
// ===== Root Page (Blynk theme UI) ===== 
void handleRoot() { 
String html = "<!DOCTYPE html><html><head><meta name='viewport' 
content='width=device-width,initial-scale=1'>"; 
html += "<title>Smart Mirror</title>"; 
html += "<style>"; 
html += "body{font-family:Arial;background:#121212;color:#fff;text
align:center;padding:20px}"; 
html += ".card{background:#1e1e1e;padding:15px;margin:10px;border-radius:12px;box
shadow:0 0 10px #000}"; 
html += "button{padding:10px 20px;margin:5px;border:none;border-radius:8px;font
size:16px;}"; 
html += ".on{background:#00c853;color:#fff;} .off{background:#d32f2f;color:#fff;}"; 
html += "</style></head><body>"; 
html += "<h1>Smart Mirror Control</h1>"; 
html += "<div class='card'><h2>Date & Time</h2><p id='time'>--:--:--</p><p id='date'>-
</p></div>"; 
html += "<div class='card'><h2>Environment</h2><p>Temp: <span id='temp'>--</span> °C</p>"; 
html += "<p>Humidity: <span id='hum'>--</span> %</p></div>"; 
html += "<div class='card'><h2>Switches</h2>"; 
html += "<button id='sw1' onclick=\"toggle('switch1')\">Switch 1</button>"; 
html += "<button id='sw2' onclick=\"toggle('switch2')\">Switch 2</button><br>"; 
html += "<button id='sw3' onclick=\"toggle('switch3')\">Switch 3</button>"; 
html += "<button id='sw4' onclick=\"toggle('switch4')\">Switch 4</button>"; 
html += "</div>"; 
html += "<div class='card'><h2>Set Date & Time</h2>"; 
html += "<form action='/sync' method='GET'>"; 
html += "Time: <input type='text' name='time' placeholder='12:34:56'><br><br>"; 
html += "Date: <input type='text' name='date' placeholder='15-09-2025'><br><br>"; 
html += "<input type='submit' value='Update'></form></div>"; 
html += "<script>"; 
html += "function refresh(){fetch('/status').then(r=>r.json()).then(data=>{"; 
html += "document.getElementById('time').innerText=data.time;"; 
html += "document.getElementById('date').innerText=data.date;"; 
html += "document.getElementById('temp').innerText=data.temp;"; 
html += "document.getElementById('hum').innerText=data.hum;"; 
html += "['sw1','sw2','sw3','sw4'].forEach((id,i)=>{"; 
html += "let state=data[id];let b=document.getElementById(id);"; 
html += "if(state==1){b.className='on';}else{b.className='off';}});});}"; 
html += "function toggle(sw){let id=sw.replace('switch','');"; 
html += "let btn=document.getElementById('sw'+id);let state=btn.className=='on'?0:1;"; 
html += "fetch('/'+sw+'?state='+state).then(refresh);}setInterval(refresh,2000);refresh();"; 
html += "</script>"; 
html += "</body></html>"; 
server.send(200, "text/html", html); 
} 
// ====== Setup ====== 
void setup() { 
Serial.begin(115200); 
lcd.init(); lcd.backlight(); lcd.clear(); 
lcd.setCursor(0, 0); lcd.print("Starting Smart Mirror"); 
WiFi.softAP(ssid, password); 
Serial.println("AP IP: " + WiFi.softAPIP().toString()); 
dht.begin(); 
server.on("/", handleRoot); 
server.on("/sync", handleTimeSync); 
server.on("/switch1", handleSwitch1); 
server.on("/switch2", handleSwitch2); 
server.on("/switch3", handleSwitch3); 
server.on("/switch4", handleSwitch4); 
server.on("/status", []() { server.send(200, "application/json", getStatusJSON()); }); 
server.begin(); 
lcd.clear(); 
lcd.setCursor(0, 0); lcd.print("WiFi AP:"); 
lcd.setCursor(0, 1); lcd.print(ssid); 
lcd.setCursor(0, 2); lcd.print("IP:" + WiFi.softAPIP().toString()); 
lcd.setCursor(0, 3); lcd.print("Open browser to sync"); 
} 
// ====== Loop ====== 
void loop() { 
server.handleClient(); 
static unsigned long lastTimeUpdate = 0; 
  if (millis() - lastTimeUpdate >= 1000) {   // update once per second 
    lastTimeUpdate = millis(); 
    // Time update 
    currentSecond++; 
    if (currentSecond >= 60) { currentSecond = 0; currentMinute++; } 
    if (currentMinute >= 60) { currentMinute = 0; currentHour++; } 
    if (currentHour >= 24) { currentHour = 0; currentDay++; } 
    if (currentDay > 30) { currentDay = 1; currentMonth++; } 
    if (currentMonth > 12) { currentMonth = 1; currentYear++; } 
    // Read sensors 
    float h = dht.readHumidity(); 
    float t = dht.readTemperature(); 
    // === Update LCD without clearing === 
    lcd.setCursor(0, 0); 
    lcd.print("Time:"); 
    lcd.print(currentHour < 10 ? "0" : ""); lcd.print(currentHour); 
    lcd.print(":"); lcd.print(currentMinute < 10 ? "0" : ""); lcd.print(currentMinute); 
    lcd.print(":"); lcd.print(currentSecond < 10 ? "0" : ""); lcd.print(currentSecond); 
    lcd.print("   "); // overwrite old chars if any 
    lcd.setCursor(0, 1); 
    lcd.print("Date:"); 
    lcd.print(currentDay < 10 ? "0" : ""); lcd.print(currentDay); 
    lcd.print("-"); lcd.print(currentMonth < 10 ? "0" : ""); lcd.print(currentMonth); 
    lcd.print("-"); lcd.print(currentYear); 
    lcd.print("   "); 
    lcd.setCursor(0, 2); 
    lcd.print("T:"); lcd.print(t, 1); lcd.print("C "); 
    lcd.print("H:"); lcd.print(h, 1); lcd.print("%   "); 
    lcd.setCursor(0, 3); 
    if (sw1) lcd.print("My proj review 11-09   "); 
    else if (sw2) lcd.print("Oct 2nd Dussehra Holi "); 
    else if (sw3) lcd.print("Balosthav in Nov      "); 
    else if (sw4) lcd.print("Guide: M.S.Rao sir    "); 
    else lcd.print("                      "); 
  } 
} 