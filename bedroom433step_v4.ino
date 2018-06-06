#include <ESP8266WiFi.h> 
#include <RCSwitch.h> 
RCSwitch mySwitch = RCSwitch(); 

int val; 
int valtest; 

int stp = D5; //connect pin 13 to step D5 
int dir = D6; // connect pin 12 to dir D6 
int led = D4; // connect pin 12 to dir D4 
int ena = D7; // connect pin 12 to dir D7 
int D8in = D8; // connect pin 12 to dir D8 
int a = 0; // gen counter 

//const char* ssid = "novacom"; 
//const char* password = "1234569871qaz"; 
const char* ssid = "HOME12test"; 
const char* password = "home1234567890"; 
int stepdelay=10; 
boolean x = false; 

WiFiServer server(80); 

void setup() { 
mySwitch.enableReceive(D2); 
Serial.begin(115200); 
delay(10); 
Serial.println(); 
Serial.println(); 
Serial.print("Connecting to "); 
Serial.println(ssid); 

pinMode(stp, OUTPUT); //D5 
pinMode(dir, OUTPUT); //D6 
pinMode(led, OUTPUT); //D4 
pinMode(ena, OUTPUT); //D7 pir 
digitalWrite(ena, HIGH); 
pinMode(D8in, INPUT); //D8 pir 
////////////////////////////////////////////////////// 
pinMode(D1, INPUT); //switch UP 
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password); 

while (WiFi.status() != WL_CONNECTED) { 
delay(500); 
Serial.print("."); 
} 
Serial.println(""); 
Serial.println("WiFi connected"); 

// Start the server 
server.begin(); 
Serial.println("Server started"); 

// Print the IP address 
Serial.println(WiFi.localIP()); 
} 

void loop() { 
////////////////////////////BBBBB UP///////po chasovoj///////////////////////////////////////// 
if (mySwitch.available()) { 
int value = mySwitch.getReceivedValue(); 
int valueD1 = digitalRead(D1); //switch UP 
if (value == 14379832 && valueD1 == LOW && x==false) { 
Serial.println("buttonB UP"); 
digitalWrite(dir, HIGH);//направление задает 
digitalWrite(ena, LOW);//снимает тормоз 
for (int i=0; i <= 1600; i++) 
{ 
digitalWrite(stp, HIGH); 
delay(stepdelay); 
digitalWrite(stp, LOW); 
delay(stepdelay); 

int valueD1 = digitalRead(D1); //switch UP 
if (valueD1 == HIGH) 
{ 
i=0; 
break; 
} 
} 
digitalWrite(stp, LOW); digitalWrite(stp, LOW); 
digitalWrite(ena, HIGH);//ставит тормоз 
mySwitch.resetAvailable(); 
x=true; 
delay(500); 
} 

////////////////////////////BBBBB DOWN up///////protiv chasovoj///////////////////////////////////////// 
if (value == 14379832 && valueD1 == LOW & x==true) { 
Serial.println("buttonB UP"); 
digitalWrite(dir, HIGH);//направление задает 
digitalWrite(ena, LOW);//снимает тормоз 
for (int i=0; i <= 400; i++) //поднимаем до сработки или 400 шагов 
{ 
digitalWrite(stp, HIGH); 
delay(stepdelay); 
digitalWrite(stp, LOW); 
delay(stepdelay); 

int valueD1 = digitalRead(D1); //switch UP 
if (valueD1 == HIGH) 
{ 
i=0; 
break; 
} 
} 
Serial.println("buttonB down"); 
digitalWrite(dir, LOW); //направление задает 
digitalWrite(ena, LOW);//снимает тормоз 
for (int i=0; i <= 1600; i++) 
{ 
digitalWrite(stp, HIGH); 
delay(stepdelay); 
digitalWrite(stp, LOW); 
delay(stepdelay); 

int valueD1 = digitalRead(D1); //switch UP 
if (valueD1 == LOW) 
{ 
i=0; 
break; 
} 
} 
digitalWrite(stp, LOW); digitalWrite(stp, LOW); 
digitalWrite(ena, HIGH);//снимает тормоз 
mySwitch.resetAvailable(); 
x=false; 
delay(500); 
} 




////////////////////////////BBBBB DOWN///////protiv chasovoj///////////////////////////////////////// 
if (value == 14379832 && valueD1 == HIGH) { 
Serial.println("buttonB down"); 
digitalWrite(dir, LOW); //направление задает 
digitalWrite(ena, LOW);//снимает тормоз 
for (int i=0; i <= 1600; i++) 
{ 
digitalWrite(stp, HIGH); 
delay(stepdelay); 
digitalWrite(stp, LOW); 
delay(stepdelay); 

int valueD1 = digitalRead(D1); //switch UP 
if (valueD1 == LOW) 
{ 
i=0; 
break; 
} 
} 
digitalWrite(stp, LOW); digitalWrite(stp, LOW); 
digitalWrite(ena, HIGH);//снимает тормоз 
mySwitch.resetAvailable(); 
x=false; 
delay(500); 
} 
} 
// Check if a client has connected 
WiFiClient client = server.available(); 
if (!client) { 
return; 
} 

// Read the first line of the request 
String req = client.readStringUntil('\r'); 

int valueD1 = digitalRead(D1); //switch UP 
///////////////////////////////////////////////////////UP////////////// 
if (req.indexOf("/gpio/open") >0 && valueD1 == LOW && x==false)
{ 
Serial.println("buttonB UP"); 
digitalWrite(dir, HIGH);//направление задает 
digitalWrite(ena, LOW);//снимает тормоз 
for (int i=0; i <= 1600; i++) 
{ 
digitalWrite(stp, HIGH); 
delay(stepdelay); 
digitalWrite(stp, LOW); 
delay(stepdelay); 

int valueD1 = digitalRead(D1); //switch UP 
if (valueD1 == HIGH) 
{ 
i=0; 
break; 
} 
} 
digitalWrite(stp, LOW); digitalWrite(stp, LOW); 
digitalWrite(ena, HIGH);//ставит тормоз 
x=true; 
val = 1; 
delay(500); 
} 


////////////////////////////////////////////////////DOWN up////////// 
if (req.indexOf("/gpio/close") >0 && valueD1 == LOW & x==true) { 
Serial.println("buttonB UP"); 
digitalWrite(dir, HIGH);//направление задает 
digitalWrite(ena, LOW);//снимает тормоз 
for (int i=0; i <= 400; i++) //поднимаем до сработки или 400 шагов 
{ 
digitalWrite(stp, HIGH); 
delay(stepdelay); 
digitalWrite(stp, LOW); 
delay(stepdelay); 

int valueD1 = digitalRead(D1); //switch UP 
if (valueD1 == HIGH) 
{ 
i=0; 
break; 
} 
} 
Serial.println("buttonB down"); 
digitalWrite(dir, LOW); //направление задает 
digitalWrite(ena, LOW);//снимает тормоз 
for (int n=0; n <= 1600; n++) 
{ 
digitalWrite(stp, HIGH); 
delay(stepdelay); 
digitalWrite(stp, LOW); 
delay(stepdelay); 

} 
digitalWrite(stp, LOW); digitalWrite(stp, LOW); 
digitalWrite(ena, HIGH);//снимает тормоз 
x=false; 
val = 0; 
delay(500); 
} 
////////////////////////////////////////////////////DOWN///////////////////////// 
if (req.indexOf("/gpio/close") >0 && valueD1 == HIGH) { 
Serial.println("buttonB down"); 
digitalWrite(dir, LOW); //направление задает 
digitalWrite(ena, LOW);//снимает тормоз 
for (int i=0; i <= 1700; i++) 
{ 
digitalWrite(stp, HIGH); 
delay(stepdelay); 
digitalWrite(stp, LOW); 
delay(stepdelay); 

int valueD1 = digitalRead(D1); //switch UP 
if (valueD1 == LOW) 
{ 
i=0; 
break; 
} 
} 
digitalWrite(stp, LOW); digitalWrite(stp, LOW); 
digitalWrite(ena, HIGH);//снимает тормоз 
x=false; 
val = 0; 
delay(500); 
} 


if (req.indexOf("/reset") >0) { 
ESP.restart(); 
} 

client.flush(); 
client.println("<HTML>"); client.println("<HEAD>"); client.println("<TITLE>ESPwindow</TITLE>"); client.println("</HEAD>"); client.println("<BODY>"); 
// followed by the HTML for the customised button GUI. 
client.println("<br>"); // new line 
client.print("<input type=button style=width:100px;height:45px value= Open onmousedown=location.href='/gpio/open'>"); 
client.print("<input type=button style=width:100px;height:45px value= Close onmousedown=location.href='/gpio/close'>"); 
client.print("<input type=button style=width:100px;height:45px value= Reset onmousedown=location.href='/reset'>"); 
client.println("<br>"); 
client.print("Analog input A0: "); //analog light 
client.print("<level>"); 
client.print(analogRead(A0)); 
client.print("</level>"); 
client.println("<br>"); 
client.print("Digital output D0 GPIO16: "); //podswetka 
client.print("<D0>"); 
client.print(digitalRead(D0)); 
client.print("</D0>"); 
client.println("<br>"); 
client.print("Digital input D1 GPIO05 switch UP: "); //button DOWN 
client.print("<D1>"); 
client.print(digitalRead(D1)); 
client.print("</D1>"); 
client.println("<br>"); 
client.print("Digital input D8 PIR: "); //PIR 
client.print("<D8>"); 
client.print(digitalRead(D8)); 
client.print("</D8>"); 
client.println("<br>"); 
client.print("Virtual switch B: "); //button B 
client.print("<V1>"); 
client.print(x); 
client.print("</V1>"); 
client.println("</BODY>"); 
client.println("</HTML>"); 
//stopping client 
client.stop(); 
return; 
String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now "; 
s += (val)?"high":"low"; 
s += (valtest)?"high":"low"; 
s += "</html>\n"; 
}
