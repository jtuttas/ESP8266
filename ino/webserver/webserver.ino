#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <WebSocketsServer.h>
#include "FS.h"


// RGB LED
#define ledred 14
#define ledgreen 5
#define ledblue 4

// BMP180
#define scl 0
#define sda 2

// Analog Eingang zum Messen der Versorgungsspannung
#define battery A0

// Deep Sleep Mode
const bool deepSleep=true;
// alle 15 Minuten aufwachen
#define deepSleepS 15*60*1000*1000

#define configFilename "config4.dat"

WiFiServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
int dimValueRed=0;
int dimValueGreen=0;
int dimValueBlue=0;
bool flasher=false;
const String header_ok="HTTP/1.1 200 OK\r\n";
const String header_notfound="HTTP/1.1 404 OK\r\n";
const String header_error="HTTP/1.1 500 OK\r\n";
const String header_forbidden="HTTP/1.1 403 OK\r\n";
const String contentTypeText="Content-Type: text/html\r\n";
const String contentTypeJson="Content-Type: application/json\r\n";
const String contentTypeJsonp="Content-Type: application/javascript\r\n";
const String contentTypeXml="Content-Type: application/xml\r\n";
const String htmlhead="\r\n<!DOCTYPE HTML>\r\n<html><head><title>esp6288</title>  <link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\"></head><body>\r\n<div class=\"jumbotron\"><div class=\"container\"><h1>IoT with ESP8266</h1><img src=\"https://hackadaycom.files.wordpress.com/2014/11/esp.png?w=400\"></div></div>";
const String htmlOnButton = "\r\n<form method=\"GET\" action=\"/index.html/on\"><button type=\"Submit\" class=\"btn btn-danger btn-lg btn-block\">ON</button></form>";
const String htmlOffButton = "\r\n<form method=\"GET\" action=\"/index.html/off\"><button type=\"Submit\" class=\"btn btn-success btn-lg btn-block\">OFF</button></form>";
const String configForm = " <form method=\"GET\" action=\"/conf\"><div class=\"form-group\"><label for=\"usr\">SSID:</label>";
const String htmltail="\r\n</body></html>\r\n";
const String jsonStart="{\r\n \"dimValueRed\":";
const String jsonEnde="\r\n}";
const String xmlStart="<esp8266>\r\n <dimValueRed>";
const String xmlEnde="\r\n</esp8266>";
const String jsonpStart="callback(";
const String jsonpEnde=");";
unsigned long tickerMeasure = 0;
unsigned long tickerLog = 0;
Adafruit_BMP085 bmp180;
float temperature;
int32_t pressure;
float voltage;
char MAC_char[18];
String confSSID;
String confPasswd;
String confUrl;
bool AP_Mode=false;


// https://script.google.com/macros/s/AKfycbzJqwSzEzhEhr9KIRHGrhXSypd-5MbWLsigN4BUvg-ivvq9GPsq/exec?temperature=1&pressure=2&signal_dB=12
 
const char* host = "script.google.com";
long rssi;

void debug(String msg) {
  if (debug) {
    Serial.println(msg);
  }
}

String urlDecode(String urlChars)
{

  urlChars.replace("%0D%0A", String('\n'));
  urlChars.replace("+",   " ");
  urlChars.replace("%20", " ");
  urlChars.replace("%21", "!");
  urlChars.replace("%22", String(char('\"')));
  urlChars.replace("%23", "#");
  urlChars.replace("%24", "$");
  urlChars.replace("%25", "%");
  urlChars.replace("%26", "&");
  urlChars.replace("%27", String(char(39)));
  urlChars.replace("%28", "(");
  urlChars.replace("%29", ")");
  urlChars.replace("%2A", "*");
  urlChars.replace("%2B", "+");
  urlChars.replace("%2C", ",");
  urlChars.replace("%2D", "-");
  urlChars.replace("%2E", ".");
  urlChars.replace("%2F", "/");
  urlChars.replace("%30", "0");
  urlChars.replace("%31", "1");
  urlChars.replace("%32", "2");
  urlChars.replace("%33", "3");
  urlChars.replace("%34", "4");
  urlChars.replace("%35", "5");
  urlChars.replace("%36", "6");
  urlChars.replace("%37", "7");
  urlChars.replace("%38", "8");
  urlChars.replace("%39", "9");
  urlChars.replace("%3A", ":");
  urlChars.replace("%3B", ";");
  urlChars.replace("%3C", "<");
  urlChars.replace("%3D", "=");
  urlChars.replace("%3E", ">");
  urlChars.replace("%3F", "?");
  urlChars.replace("%40", "@");
  urlChars.replace("%41", "A");
  urlChars.replace("%42", "B");
  urlChars.replace("%43", "C");
  urlChars.replace("%44", "D");
  urlChars.replace("%45", "E");
  urlChars.replace("%46", "F");
  urlChars.replace("%47", "G");
  urlChars.replace("%48", "H");
  urlChars.replace("%49", "I");
  urlChars.replace("%4A", "J");
  urlChars.replace("%4B", "K");
  urlChars.replace("%4C", "L");
  urlChars.replace("%4D", "M");
  urlChars.replace("%4E", "N");
  urlChars.replace("%4F", "O");
  urlChars.replace("%50", "P");
  urlChars.replace("%51", "Q");
  urlChars.replace("%52", "R");
  urlChars.replace("%53", "S");
  urlChars.replace("%54", "T");
  urlChars.replace("%55", "U");
  urlChars.replace("%56", "V");
  urlChars.replace("%57", "W");
  urlChars.replace("%58", "X");
  urlChars.replace("%59", "Y");
  urlChars.replace("%5A", "Z");
  urlChars.replace("%5B", "[");
  urlChars.replace("%5C", String(char(65)));
  urlChars.replace("%5D", "]");
  urlChars.replace("%5E", "^");
  urlChars.replace("%5F", "_");
  urlChars.replace("%60", "`");
  urlChars.replace("%61", "a");
  urlChars.replace("%62", "b");
  urlChars.replace("%63", "c");
  urlChars.replace("%64", "d");
  urlChars.replace("%65", "e");
  urlChars.replace("%66", "f");
  urlChars.replace("%67", "g");
  urlChars.replace("%68", "h");
  urlChars.replace("%69", "i");
  urlChars.replace("%6A", "j");
  urlChars.replace("%6B", "k");
  urlChars.replace("%6C", "l");
  urlChars.replace("%6D", "m");
  urlChars.replace("%6E", "n");
  urlChars.replace("%6F", "o");
  urlChars.replace("%70", "p");
  urlChars.replace("%71", "q");
  urlChars.replace("%72", "r");
  urlChars.replace("%73", "s");
  urlChars.replace("%74", "t");
  urlChars.replace("%75", "u");
  urlChars.replace("%76", "v");
  urlChars.replace("%77", "w");
  urlChars.replace("%78", "x");
  urlChars.replace("%79", "y");
  urlChars.replace("%7A", "z");
  urlChars.replace("%7B", String(char(123)));
  urlChars.replace("%7C", "|");
  urlChars.replace("%7D", String(char(125)));
  urlChars.replace("%7E", "~");
  urlChars.replace("%7F", "Â");
  urlChars.replace("%80", "`");
  urlChars.replace("%81", "Â");
  urlChars.replace("%82", "â");
  urlChars.replace("%83", "Æ");
  urlChars.replace("%84", "â");
  urlChars.replace("%85", "â¦");
  urlChars.replace("%86", "â");
  urlChars.replace("%87", "â¡");
  urlChars.replace("%88", "Ë");
  urlChars.replace("%89", "â°");
  urlChars.replace("%8A", "Å");
  urlChars.replace("%8B", "â¹");
  urlChars.replace("%8C", "Å");
  urlChars.replace("%8D", "Â");
  urlChars.replace("%8E", "Å½");
  urlChars.replace("%8F", "Â");
  urlChars.replace("%90", "Â");
  urlChars.replace("%91", "â");
  urlChars.replace("%92", "â");
  urlChars.replace("%93", "â");
  urlChars.replace("%94", "â");
  urlChars.replace("%95", "â¢");
  urlChars.replace("%96", "â");
  urlChars.replace("%97", "â");
  urlChars.replace("%98", "Ë");
  urlChars.replace("%99", "â¢");
  urlChars.replace("%9A", "Å¡");
  urlChars.replace("%9B", "âº");
  urlChars.replace("%9C", "Å");
  urlChars.replace("%9D", "Â");
  urlChars.replace("%9E", "Å¾");
  urlChars.replace("%9F", "Å¸");
  urlChars.replace("%A0", "Â");
  urlChars.replace("%A1", "Â¡");
  urlChars.replace("%A2", "Â¢");
  urlChars.replace("%A3", "Â£");
  urlChars.replace("%A4", "Â¤");
  urlChars.replace("%A5", "Â¥");
  urlChars.replace("%A6", "Â¦");
  urlChars.replace("%A7", "Â§");
  urlChars.replace("%A8", "Â¨");
  urlChars.replace("%A9", "Â©");
  urlChars.replace("%AA", "Âª");
  urlChars.replace("%AB", "Â«");
  urlChars.replace("%AC", "Â¬");
  urlChars.replace("%AE", "Â®");
  urlChars.replace("%AF", "Â¯");
  urlChars.replace("%B0", "Â°");
  urlChars.replace("%B1", "Â±");
  urlChars.replace("%B2", "Â²");
  urlChars.replace("%B3", "Â³");
  urlChars.replace("%B4", "Â´");
  urlChars.replace("%B5", "Âµ");
  urlChars.replace("%B6", "Â¶");
  urlChars.replace("%B7", "Â·");
  urlChars.replace("%B8", "Â¸");
  urlChars.replace("%B9", "Â¹");
  urlChars.replace("%BA", "Âº");
  urlChars.replace("%BB", "Â»");
  urlChars.replace("%BC", "Â¼");
  urlChars.replace("%BD", "Â½");
  urlChars.replace("%BE", "Â¾");
  urlChars.replace("%BF", "Â¿");
  urlChars.replace("%C0", "Ã");
  urlChars.replace("%C1", "Ã");
  urlChars.replace("%C2", "Ã");
  urlChars.replace("%C3", "Ã");
  urlChars.replace("%C4", "Ã");
  urlChars.replace("%C5", "Ã");
  urlChars.replace("%C6", "Ã");
  urlChars.replace("%C7", "Ã");
  urlChars.replace("%C8", "Ã");
  urlChars.replace("%C9", "Ã");
  urlChars.replace("%CA", "Ã");
  urlChars.replace("%CB", "Ã");
  urlChars.replace("%CC", "Ã");
  urlChars.replace("%CD", "Ã");
  urlChars.replace("%CE", "Ã");
  urlChars.replace("%CF", "Ã");
  urlChars.replace("%D0", "Ã");
  urlChars.replace("%D1", "Ã");
  urlChars.replace("%D2", "Ã");
  urlChars.replace("%D3", "Ã");
  urlChars.replace("%D4", "Ã");
  urlChars.replace("%D5", "Ã");
  urlChars.replace("%D6", "Ã");
  urlChars.replace("%D7", "Ã");
  urlChars.replace("%D8", "Ã");
  urlChars.replace("%D9", "Ã");
  urlChars.replace("%DA", "Ã");
  urlChars.replace("%DB", "Ã");
  urlChars.replace("%DC", "Ã");
  urlChars.replace("%DD", "Ã");
  urlChars.replace("%DE", "Ã");
  urlChars.replace("%DF", "Ã");
  urlChars.replace("%E0", "Ã");
  urlChars.replace("%E1", "Ã¡");
  urlChars.replace("%E2", "Ã¢");
  urlChars.replace("%E3", "Ã£");
  urlChars.replace("%E4", "Ã¤");
  urlChars.replace("%E5", "Ã¥");
  urlChars.replace("%E6", "Ã¦");
  urlChars.replace("%E7", "Ã§");
  urlChars.replace("%E8", "Ã¨");
  urlChars.replace("%E9", "Ã©");
  urlChars.replace("%EA", "Ãª");
  urlChars.replace("%EB", "Ã«");
  urlChars.replace("%EC", "Ã¬");
  urlChars.replace("%ED", "Ã­");
  urlChars.replace("%EE", "Ã®");
  urlChars.replace("%EF", "Ã¯");
  urlChars.replace("%F0", "Ã°");
  urlChars.replace("%F1", "Ã±");
  urlChars.replace("%F2", "Ã²");
  urlChars.replace("%F3", "Ã³");
  urlChars.replace("%F4", "Ã´");
  urlChars.replace("%F5", "Ãµ");
  urlChars.replace("%F6", "Ã¶");
  urlChars.replace("%F7", "Ã·");
  urlChars.replace("%F8", "Ã¸");
  urlChars.replace("%F9", "Ã¹");
  urlChars.replace("%FA", "Ãº");
  urlChars.replace("%FB", "Ã»");
  urlChars.replace("%FC", "Ã¼");
  urlChars.replace("%FD", "Ã½");
  urlChars.replace("%FE", "Ã¾");
  urlChars.replace("%FF", "Ã¿");

  return urlChars;
}

String getJson() {
    String s="";
    s+=jsonStart;
    s+=String(dimValueRed);
    s+=String(",\r\n\"dimValueGreen\":");
    s+=String(dimValueGreen);
    s+=String(",\r\n\"dimValueBlue\":");
    s+=String(dimValueBlue);
    s+=String(",\r\n\"signal_dB\":");
    s+=String(rssi);
    s+=String(",\r\n\"temperature\":");
    s+=String(temperature);
    s+=String(",\r\n\"pressure\":");
    s+=String(pressure);
    s+=String(",\r\n\"vcc\":");
    s+=String(voltage);
    s+=String(",\r\n\"mac\": \"");
    s+=String(MAC_char);
    s+=String("\"");
    s+=jsonEnde; 
    return s;    
}

bool readConfig(String fname) {
    File f = SPIFFS.open(fname, "r");
    if (!f) {
      Serial.println("open "+fname+" failed");
      f.close();
      return false;
    }
    else {   
      confSSID= f.readStringUntil('\n');   
      Serial.println("SSID");   
      Serial.println(confSSID);
      Serial.println("Password");   
      confPasswd  = f.readStringUntil('\n');      
      Serial.println(confPasswd);
      Serial.println("PushURL");   
      confUrl  = f.readStringUntil('\n');      
      Serial.println(confUrl);
      Serial.println("finished reading config.json");  
      f.close();
    }
    return true;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
              IPAddress ip = webSocket.remoteIP(num);
              Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);        
              // send message to client
              String s = getJson();
              webSocket.sendTXT(num, s);
            }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);

            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
        case WStype_BIN:
            Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
            hexdump(payload, lenght);

            // send message to client
            // webSocket.sendBIN(num, payload, lenght);
            break;
    }

}

void send2Google() {
  WiFiClientSecure client;
  if (!client.connect(host, 443)) {
   Serial.println("connection failed");
   return;
  }
  // /macros/s/AKfycbzJqwSzEzhEhr9KIRHGrhXSypd-5MbWLsigN4BUvg-ivvq9GPsq/exec?temperature=1&pressure=2&signal_dB=12
  String url = confUrl.substring(0,confUrl.length()-1);
  url+=String("?temperature=");
  String sTemp = String(temperature);                
  sTemp.replace(".",",");
  url += sTemp;
  url +=String("&pressure=");
  url +=String(pressure);
  url +=String("&signal_dB=");
  url+=String(rssi);
  url +=String("&mac=");
  url+=String(MAC_char);
  url +=String("&vcc=");
  String sVcc = String(voltage);                
  sVcc.replace(".",",");
  url+=String(sVcc);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" + 
         "Content-Length: 0\r\n"
         "Connection: close\r\n\r\n");
  delay(10);
  Serial.println("Response:");
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}

void setup() {
  Serial.begin(115200);
  Serial.write("\r\n\r\nSet Output Pin");
  pinMode(ledred,OUTPUT);
  pinMode(ledgreen,OUTPUT);
  pinMode(ledblue,OUTPUT);
  pinMode(battery,INPUT);

  uint8_t MAC_array[6];  
  Serial.write("\r\nRead MAC Adress: ");
  WiFi.macAddress(MAC_array);
  for (int i = 0; i < sizeof(MAC_array); ++i){
    sprintf(MAC_char,"%s%02x:",MAC_char,MAC_array[i]);
  }  
  Serial.println(MAC_char);

  Serial.write("\r\nOpen File System:\r\n");
  SPIFFS.begin();

  bool result = readConfig(configFilename);
  
  if (!result) {
    Serial.println("open config.dat failed");
    AP_Mode=true;
     // AP mode
    WiFi.mode(WIFI_AP);
    Serial.println("Starte WLAN Access Point");
    char* AP_SSID="ESP8266";
    WiFi.softAP(AP_SSID, "");
    analogWrite(ledred,1023);
  }
  else {   
    WiFi.mode(WIFI_STA);
    Serial.write("\r\nConnect to WLAN (SSID,passwd)\r\n");
    Serial.println(confSSID);
    Serial.println(confPasswd);
    String sid=confSSID.substring(0,confSSID.length()-1);
    String spw=confPasswd.substring(0,confPasswd.length()-1);
    const char* ssid = sid.c_str();
    const char* wlan_pwd =spw.c_str();
    WiFi.begin(ssid, wlan_pwd);
    int n=0;
    while (WiFi.status() != WL_CONNECTED && n<10) {
      delay(1000);
      Serial.write(".");
      n++;
    }
    if (WiFi.status()==WL_CONNECTED) {
      Serial.write("\r\nWiFi connected");
      delay(1000);
      Serial.write("\r\nIP address: ");
      Serial.println(WiFi.localIP());      
    }
    else {
      Serial.println("\r\nConnection failed!");
      AP_Mode=true;
       // AP mode
      WiFi.mode(WIFI_AP);
      Serial.println("Starte WLAN Access Point");
      char* AP_SSID="ESP8266";
      WiFi.softAP(AP_SSID, "");
      delay(2000);
      analogWrite(ledred,1023);
    }
  }

  

  rssi = WiFi.RSSI();
  Serial.print("Signal strength: ");
  Serial.print(rssi);
  Serial.println("dB");
  Serial.write("\r\nInitialisiere BMP180");
  Wire.begin(sda,scl);
  if (bmp180.begin()) {
    Serial.println("BMP180 init success");
    Serial.print("Temperature = ");
    temperature=bmp180.readTemperature();
    Serial.print(temperature);
    Serial.println(" Celsius");
    Serial.print("Pressure = ");
    pressure=bmp180.readPressure();
    Serial.print(pressure);
    Serial.println(" Pascal");
    Serial.println();
  }
  else
  {
    Serial.println("BMP180 init fail !\n\n");
    temperature=0.0;
    pressure=0;
  }  


  voltage = (5*0.66*analogRead(A0))/198;
  Serial.write("\r\nVersorgungsspannung:");
  Serial.println (voltage);

  if (!deepSleep) {
    Serial.write("\r\nStarte Server");
    server.begin();
    Serial.write("\r\nStarte Websocket");
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.write("\r\nfinished Setup!");  
  }
  else {
    send2Google();
    Serial.write("\r\nfinished Setup turn into deep sleep!");  
    ESP.deepSleep(deepSleepS);
  }
}

void loop() {
  webSocket.loop();
  WiFiClient clientS = server.available();
   if (!clientS) {
      // kein Request und alle 2 Minuten wird gemessen
      if (tickerMeasure+(1000*60*2)<millis()) {
        bool updateClients=false;
        tickerMeasure=millis();
        Serial.println("Messe .....!");
        Serial.print("Versorgungsspannung: ");
        voltage = (5*0.66*analogRead(A0))/198;
        Serial.println(voltage);
        
        rssi = WiFi.RSSI();
        Serial.print("Signal strength: ");
        Serial.print(rssi);
        Serial.println("dB");

        Serial.print("Temperature = ");
        float t = bmp180.readTemperature();
        Serial.print(t);
        Serial.println(" Celsius");
        // Müllwerte verhindern
        if (t<100.0) {
          if (t!=temperature) {
            updateClients=true;
          }
          temperature=t;
        }
        Serial.print("Pressure = ");
        int32_t p = bmp180.readPressure();
        Serial.print(p);
        Serial.println(" Pascal");
        // Müllwerte verhindern
        if (p<150000) {
          if (p!=pressure) {
            updateClients=true;
          }
          pressure=p;
        }
        Serial.println();
        if (updateClients) {
          String s = getJson();
          webSocket.broadcastTXT(s);
        }
      }
      // kein Request und alle 15 Minuten werden die Daten zu Google Tabelle gesendet
      if (tickerLog+(1000*60*15)<millis() && !AP_Mode ) {
        tickerLog=millis();
        // Daten zum Logger (Google Tabelle) senden
        send2Google();
      }
   }
   else {
    /*
    flasher=!flasher;
    // Interne LED bei jedem Request umschalten
    if (flasher) {
      digitalWrite(1,HIGH);
    }
    else {
      digitalWrite(1,LOW);
    }
    */
    Serial.write("\r\nnew Client:");
    String request = clientS.readStringUntil('\r');
    Serial.println(">"+request+"<");
    clientS.flush();

    String req=request.substring(request.indexOf("/"),request.lastIndexOf(" "));
    Serial.println(req);

    String s;
    if (req.length()==0) {
      return;      
    }
    else if (req.indexOf("/on")!=-1 || req.indexOf("/on?")!=-1) {
      dimValueRed=1023;
      dimValueGreen=1023;
      dimValueBlue=1023;
      //Ausgang setzten
      analogWrite(ledred,dimValueRed);
      analogWrite(ledgreen,dimValueGreen);
      analogWrite(ledblue,dimValueBlue);
      String s = getJson();
      webSocket.broadcastTXT(s);
    }
    else if (req.indexOf("/off")!=-1 || req.indexOf("/off?")!=-1) {
      dimValueRed=0;
      dimValueGreen=0;
      dimValueBlue=0;
      //Ausgang setzten
      analogWrite(ledred,dimValueRed);
      analogWrite(ledgreen,dimValueGreen);
      analogWrite(ledblue,dimValueBlue);
      String s = getJson();
      webSocket.broadcastTXT(s);
    }
    else if (req.indexOf("/dim/")!= -1) {
      String value;
      int dv;  
      bool upd=false;
    
      if (req.indexOf("/r=")!=-1) {
        value = req.substring(req.indexOf("/r=")+3,req.indexOf("/r=")+3+4);
        Serial.write("\r\nDimRed Red to:");
        Serial.println(value);
        dv = value.toInt();         
        if (dv>0 && dv<1024) {
          dimValueRed=dv;
          upd=true;
          //Ausgang setzten
          analogWrite(ledred,dimValueRed);
        }
      }
      if (req.indexOf("/g=")!=-1) {                
        value = req.substring(req.indexOf("/g=")+3,req.indexOf("/g=")+3+4);
        Serial.write("\r\nDimRed Green to:");
        Serial.println(value);
        dv = value.toInt(); 
        if (dv>0 && dv<1024) {
          dimValueGreen=dv;
          upd=true;
          //Ausgang setzten
          analogWrite(ledgreen,dimValueGreen);
        }
      }          
      if (req.indexOf("/b=")!=-1) {                
        value = req.substring(req.indexOf("/b=")+3,req.indexOf("/b=")+3+4);
        Serial.write("\r\nDimRed Blue to:");
        Serial.println(value);
        dv = value.toInt(); 
        if (dv>0 && dv<1024) {
          dimValueBlue=dv;
          upd=true;
          //Ausgang setzten
          analogWrite(ledblue,dimValueBlue);
        }
      }    
      if (upd) {
        String s = getJson();
        webSocket.broadcastTXT(s);      
      }
    }

    if (req.startsWith("/xml") || req.startsWith("/json") || req.startsWith("/jsonp") || req.startsWith("/index.html")) {
    }
    else if ( req.startsWith("/conf")) {
      if (!AP_Mode) {
        Serial.println ("Konfigurationsänderungen nur im AP Mode!");
        s=header_forbidden;
        clientS.print(s);
        clientS.stop();
        return;
      }
    }
    else {
      s=header_notfound;
      clientS.print(s);
      clientS.stop();
      return;
    }

    // HTTP Antwort zusammensetzten
    s=header_ok;
    if (req.startsWith("/jsonp")) {
      Serial.write("\r\nJSONP ");
      s+=contentTypeJsonp;      
      // Ende vom HTTP Header
      s+=String("\r\n");
      s+=jsonpStart;
      s+=getJson();
      s+=jsonpEnde;
      Serial.println("Output:"+s);   
    }
    else if (req.startsWith("/json")) {
      Serial.write("\r\nJSON ");
      s+=contentTypeJson;      
      // Ende vom HTTP Header
      s+=String("\r\n");
      s+=getJson();
      Serial.println("Output:"+s);   
    }
    else if (req.startsWith("/xml")) {
      Serial.write("\r\nXML ");
      s+=contentTypeXml;
      // Ende vom HTTP Header
      s+=String("\r\n");
      s+=xmlStart;
      s+=String(dimValueRed);
      s+=String("</dimValueRed>");
      s+=String("\r\n<dimValueGreen>");
      s+=String(dimValueGreen);
      s+=String("</dimValueGreen>");
      s+=String("\r\n<dimValueBlue>");
      s+=String(dimValueBlue);
      s+=String("</dimValueBlue>");      
      s+=String("\r\n\<signal_dB>");
      s+=String(rssi);
      s+=String("</signal_dB>");
      s+=String("\r\n<temperature>");
      s+=temperature;
      s+=String("</temperature>");
      s+=String("\r\n<pressure>");
      s+=pressure;
      s+=String("</pressure>");
      s+=String("\r\n<vcc>");
      s+=voltage;
      s+=String("</vcc>");
      s+=String("\r\n<mac>");
      s+=String(MAC_char);
      s+=String("</mac>");
      s+=xmlEnde;          
      Serial.println("Output:"+s);   
    }
    else {
      s+=contentTypeText;
      s+=htmlhead;
      if (req.startsWith("/index.html")) {
        s+=String("<div class=\"container\">");
        s+=String("<table class=\"table\"><thead><tr><th>dim red</th><th>dim green</th><th>dim blue</th><th>Temperature [C]</th><th>Pressure [PA]</th><th>Signal [dB]</th></tr></thead>");
        s+=String("<tbody><tr><td>");
        s+=String(dimValueRed);
        s+=String("</td><td>");
        s+=String(dimValueGreen);
        s+=String("</td><td>");
        s+=String(dimValueBlue);
        s+=String("</td><td>");
        s+=String(temperature);
        s+=String("</td><td>");
        s+=String(pressure);
        s+=String("</td><td>");
        s+=String(rssi);
        s+=String("</td></tr></tbody></table>");
        s+=String("</div><div class=\"row\"><div class=\"col-md-2 col-md-offset-5\">");            
        if (dimValueRed>0) {
          s+=htmlOffButton;
        }
        else {
          s+=htmlOnButton;
        }
        s+=String("</div></div>");
      }
      else if (req.startsWith("/config.html")) {
        s+=configForm;        
        s+=String("<input type=\"text\" class=\"form-control\" name=\"ssid\" placeholder=\"WLAN SSID\" value=\"");
        s+=String(confSSID);
        s+=String("\"><br/><label for=\"pwd\">Password:</label><input type=\"password\" class=\"form-control\" name=\"pwd\" placeholder=\"WLAN Password\" value=\"");
        s+=String(confPasswd);
        s+=String("\"><br/><label for=\"push\">Push URL:</label><input type=\"text\" class=\"form-control\" name=\"push\" placeholder=\"URL to push the Data\" value=\"");
        s+=String(confUrl);
        s+=String("\"></div><button type=\"Submit\" class=\"btn btn-danger btn-block\">Submit</button></form>");
      }
      else if (req.startsWith("/conf?ssid")) {
        s+=String("<h1>Config stored and ESP8266 doing reset!</h1>");
      }
      s+=htmltail;
    }


    // HTTP Antwort zurück zum Client
    clientS.print(s);
    clientS.stop();
    if (req.startsWith("/conf?ssid")) {
      Serial.write("\r\nSchreibe neue config Datei");   

      String id=urlDecode(req.substring(req.indexOf("?ssid=")+6,req.indexOf("&pwd")));
      String pwd=urlDecode(req.substring(req.indexOf("&pwd=")+5,req.indexOf("&push")));
      String purl=urlDecode(req.substring(req.indexOf("&push=")+6));
      File f = SPIFFS.open(configFilename, "w");
      if (!f) {
        Serial.println("open config.dat failed");
      }
      else {   
        Serial.println ("Set SSID to");
        Serial.println(id);        
        Serial.println ("Set Password to");
        Serial.println(pwd);
        Serial.println ("Set Push URL to");        
        Serial.println(purl);
        f.println(id);
        f.println(pwd);
        f.println(purl);
        Serial.println("finished writing config.dat");    
      }
      f.close();
      delay(2000);
      ESP.reset();
    }   
  }
}


