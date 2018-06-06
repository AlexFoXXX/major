#include <ESP8266WiFi.h> 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SimpleDHT.h>
SimpleDHT11 dht11;

int val; 
int val1; 
int valtest; 
int pinDHT11 = D4;
int stepdelay=50; ////////////////////////steper test

//const char* ssid = "111"; 
//const char* password = "123456"; 
const char* ssid = "2222"; 
const char* password = "222222"; 

boolean x = false; 

WiFiServer server(80); 

//DS18B20
#define ONE_WIRE_BUS D3 //Pin to which is attached a temperature sensor
#define ONE_WIRE_MAX_DEV 15 //The maximum number of devices

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
int numberOfDevices; //Number of temperature devices found
DeviceAddress devAddr[ONE_WIRE_MAX_DEV];  //An array device temperature sensors
float tempDev[ONE_WIRE_MAX_DEV]; //Saving the last measurement of temperature
float tempDevLast[ONE_WIRE_MAX_DEV]; //Previous temperature measurement
long lastTemp; //The last measurement
const int durationTemp = 5000; //The frequency of temperature measurement

//------------------------------------------
//Convert device id to String
String GetAddressToString(DeviceAddress deviceAddress){
  String str = "";
  for (uint8_t i = 0; i < 8; i++){
    if( deviceAddress[i] < 16 ) str += String(0, HEX);
    str += String(deviceAddress[i], HEX);
  }
  return str;
}

void SetupDS18B20(){
  DS18B20.begin();
  numberOfDevices = DS18B20.getDeviceCount();
  Serial.print( "Device count: " );
  Serial.println( numberOfDevices );
  lastTemp = millis();
  DS18B20.requestTemperatures();
}

//Loop measuring the temperature
void TempLoop(long now){
  if( now - lastTemp > durationTemp ){ //Take a measurement at a fixed time (durationTemp = 5000ms, 5s)
    for(int i=0; i<numberOfDevices; i++){
      float tempC = DS18B20.getTempC( devAddr[i] ); //Measuring temperature in Celsius
      tempDev[i] = tempC; //Save the measured value to the array
    }
    DS18B20.setWaitForConversion(false); //No waiting for measurement
    DS18B20.requestTemperatures(); //Initiate the temperature measurement
    lastTemp = millis();  //Remember the last time measurement
  }
}



void setup() { 
Serial.begin(115200); 
delay(10); 
Serial.println(); 
Serial.println(); 
Serial.print("Connecting to "); 
Serial.println(ssid); 

pinMode(D0, OUTPUT);
pinMode(D1, OUTPUT);
pinMode(D2, INPUT); //switch 
//pinMode(D4, OUTPUT); //led
pinMode(D5, INPUT); //pir
pinMode(D6, INPUT); //sound
//////////////////////////////////////////
pinMode(D7, OUTPUT); //dir
pinMode(D8, OUTPUT); //step
/////////////////////////////////////////

digitalWrite(D0, HIGH);
digitalWrite(D1, HIGH);
//digitalWrite(D4, LOW);

//pinMode(13, OUTPUT); 
//pinMode(15, OUTPUT); 

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
void initHardware() 
{ 
pinMode(D7, OUTPUT); //dir
pinMode(D8, OUTPUT); //step
//pinMode(13, OUTPUT); 
//pinMode(15, OUTPUT); 
} 

void loop() { 

 int flagD2 = digitalRead(D2);
 if (flagD2==HIGH)
 {
digitalWrite(D0, !digitalRead(D0));
x=!x;
delay(500);
 }

// Check if a client has connected 
WiFiClient client = server.available(); 
if (!client) { 
return; 
} 

// Read the first line of the request 
String req = client.readStringUntil('\r'); 

if (req.indexOf("/gpioD0/on") >0) { 
digitalWrite(D0, LOW); 
x=true;
val = 1; 
} 

if (req.indexOf("/gpioD0/off") >0) { 
digitalWrite(D0, HIGH); 
x=false;
val = 0; 
} 

if (req.indexOf("/gpioD1/on") >0) { 
//digitalWrite(D1, LOW); 

digitalWrite(D7, HIGH);
for (int i=0; i <= 512; i++) 
{ 
digitalWrite(D8, HIGH); 
delay(stepdelay); 
digitalWrite(D8, LOW); 
delay(stepdelay); 
int valueD1 = digitalRead(D5); //switch UP 
if (valueD1 == HIGH) { 
i=0; 
break; 
} 
} 
digitalWrite(D7, LOW); digitalWrite(D8, LOW);
val = 1; 
} 

if (req.indexOf("/gpioD1/off") >0) { 
//digitalWrite(D1, HIGH); 

digitalWrite(D7, LOW);
for (int i=0; i <= 512; i++) 
{ 
digitalWrite(D8, HIGH); 
delay(stepdelay); 
digitalWrite(D8, LOW); 
delay(stepdelay); 
int valueD1 = digitalRead(D5); //switch UP 
if (valueD1 == HIGH) { 
i=0; 
break; 
} 
} 
digitalWrite(D7, LOW); digitalWrite(D8, LOW);
val1 = 0; 
}

if (req.indexOf("/reset") >0) { 
ESP.restart(); 
} 

///////////////////////////////////////////////
byte temperature = 0;
  byte humidity = 0;
  if (dht11.read(pinDHT11, &temperature, &humidity, NULL)) {
    Serial.print("Read DHT11 failed.");
    return;
  }
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" %");
  delay(1000);

/////////////////////////////////////////

  
client.flush(); 
client.println("<HTML>"); client.println("<HEAD>"); client.println("<TITLE>ChildRoom</TITLE>"); client.println("</HEAD>"); client.println("<BODY>"); 
// followed by the HTML for the customised button GUI. 
client.println("<br>"); // new line 
client.print("<input type=button style=width:100px;height:45px value= D0_ON onmousedown=location.href='/gpioD0/on'>"); 
client.print("<input type=button style=width:100px;height:45px value= D0_OFF onmousedown=location.href='/gpioD0/off'>"); 
client.println("<br>");
client.print("<input type=button style=width:100px;height:45px value= D1_ON onmousedown=location.href='/gpioD1/on'>"); 
client.print("<input type=button style=width:100px;height:45px value= D1_OFF onmousedown=location.href='/gpioD1/off'>"); 
client.println("<br>");
client.print("<input type=button style=width:100px;height:45px value= Reset onmousedown=location.href='/reset'>"); 
client.println("<br>"); 
client.print("Analog input A0: "); //analog light 
client.print("<level>"); 
client.print(analogRead(A0)); 
client.print("</level>"); 
client.println("<br>"); 
client.print("Digital output D0: "); //podswetka 
client.print("<D0>"); 
client.print(digitalRead(D0)); 
client.print("</D0>"); 
client.println("<br>");
client.print("Digital output D1: "); //podswetka 2
client.print("<D1>"); 
client.print(digitalRead(D1)); 
client.print("</D1>"); 
client.println("<br>"); 
client.print("Digital input D2: "); //button
client.print("<D2>"); 
client.print(digitalRead(D2)); 
client.print("</D2>"); 
client.println("<br>"); 
client.print("Digital input PIR D5: "); //pir
client.print("<D5>"); 
client.print(digitalRead(D5)); 
client.print("</D5>");
client.println("<br>"); 
client.print("Digital input sound D6: "); //sound
client.print("<D6>"); 
client.print(digitalRead(D6)); 
client.print("</D6>");  
client.println("<br>"); 
//client.print("Digital output D4 led: "); //led
//client.print("<D4>"); 
//client.print(digitalRead(D4)); 
//client.print("</D4>"); 
client.println("<br>"); 
client.print("Virtual switch B: "); //button B 
client.print("<V1>"); 
client.print(x); 
client.print("</V1>"); 
client.println("<br>"); 

DS18B20.begin();
  numberOfDevices = DS18B20.getDeviceCount();
  Serial.print( "Device count: " );
  Serial.println( numberOfDevices );
  lastTemp = millis();
  DS18B20.requestTemperatures();

  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++){
    // Search the wire for address
    if( DS18B20.getAddress(devAddr[i], i) ){
      //devAddr[i] = tempDeviceAddress;
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: " + GetAddressToString(devAddr[i]));
      Serial.println();
    }else{
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }

    //Get resolution of DS18b20
    Serial.print("Resolution: ");
    Serial.print(DS18B20.getResolution( devAddr[i] ));
    Serial.println();

    //Read temperature from DS18b20
    float tempC = DS18B20.getTempC( devAddr[i] );
   client.print("D3 Temp C: ");
   client.print("<temp>");
    client.println(tempC);
     }
  client.print("</temp>");
  client.println("<br>"); 
client.print("DHT temp C: "); //analog light 
client.print("<dhtT>"); 
client.print((int)temperature); 
client.print("</dhtT>");
client.println("<br>"); 
client.print("DHT hum %: "); //analog light 
client.print("<dhtH>"); 
client.print((int)humidity); 
client.print("</dhtH>");
client.println("<br>"); 
client.println("</BODY>"); 
client.println("</HTML>"); 
//stopping client 
client.stop(); 
return; 
String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now "; 
s += (val)?"high":"low"; 
s += (val1)?"high":"low"; 
s += "</html>\n"; 
 
 
 long t = millis();
  TempLoop( t );
//  server.handleClient();
}
