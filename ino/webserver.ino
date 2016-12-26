#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Config.h>
#include <WebSocketsServer.h>

// RGB LED
#define ledred 16
#define ledgreen 5
#define ledblue 4

// BMP180
#define scl 0
#define sda 2


WiFiServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
int dimValueRed=0;
int dimValueGreen=0;
int dimValueBlue=0;
bool flasher=false;
const String header_ok="HTTP/1.1 200 OK\r\n";
const String header_notfound="HTTP/1.1 404 OK\r\n";
const String header_error="HTTP/1.1 500 OK\r\n";
const String contentTypeText="Content-Type: text/html\r\n";
const String contentTypeJson="Content-Type: application/json\r\n";
const String contentTypeJsonp="Content-Type: application/javascript\r\n";
const String contentTypeXml="Content-Type: application/xml\r\n";
const String htmlhead="\r\n<!DOCTYPE HTML>\r\n<html><head><title>esp6288</title>  <link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\"></head><body>\r\n<div class=\"jumbotron\"><div class=\"container\"><h1>IoT with ESP8266</h1><img src=\"https://hackadaycom.files.wordpress.com/2014/11/esp.png?w=400\"></div></div>";
const String htmlOnButton = "\r\n<form method=\"GET\" action=\"/on\"><button type=\"Submit\" class=\"btn btn-danger btn-lg btn-block\">ON</button></form>";
const String htmlOffButton = "\r\n<form method=\"GET\" action=\"/off\"><button type=\"Submit\" class=\"btn btn-success btn-lg btn-block\">OFF</button></form>";
const String htmltail="\r\n</body></html>\r\n";
const String jsonStart="{\r\n \"dimValueRed\":";
const String jsonEnde="\r\n}";
const String xmlStart="<esp8266>\r\n <dimValueRed>";
const String xmlEnde="\r\n</esp8266>";
const String jsonpStart="callback(";
const String jsonpEnde=");";
unsigned long tickerMeasure = 0;
unsigned long tickerLog = 0;
const char* SSID = ssid;
Adafruit_BMP085 bmp180;
float temperature;
int32_t pressure;


// https://script.google.com/macros/s/AKfycbzJqwSzEzhEhr9KIRHGrhXSypd-5MbWLsigN4BUvg-ivvq9GPsq/exec?temperature=1&pressure=2&signal_dB=12
 
const char* host = "script.google.com";
//const char* host = "service.joerg-tuttas.de";
long rssi;

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
    s+=jsonEnde; 
    return s;    
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

void setup() {
  Serial.begin(115200);
  Serial.write("\r\n\r\nSet Output Pin2");
  pinMode(ledred,OUTPUT);
  pinMode(ledgreen,OUTPUT);
  pinMode(ledblue,OUTPUT);

  WiFi.mode(WIFI_STA);
  Serial.write("\r\nConnect to WLAN");
  WiFi.begin(SSID, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.write(".");
  }

  Serial.write("\r\nWiFi connected");
  delay(1000);
  Serial.write("\r\nIP address: ");
  Serial.println(WiFi.localIP());
  rssi = WiFi.RSSI();
  Serial.print("Signal strength: ");
  Serial.print(rssi);
  Serial.println("dB");
  Serial.write("\r\nInitialisiere BMP180");
  Wire.begin(sda,scl);
  if (bmp180.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while(1); // Pause forever.
  }  
  Serial.print("Temperature = ");
  temperature=bmp180.readTemperature();
  Serial.print(temperature);
  Serial.println(" Celsius");
  Serial.print("Pressure = ");
  pressure=bmp180.readPressure();
  Serial.print(pressure);
  Serial.println(" Pascal");
  Serial.println();

  Serial.write("\r\nStarte Server");
  server.begin();
  Serial.write("\r\nStarte Websocket");
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.write("\r\nfinished Setup!");
  
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
      if (tickerLog+(1000*60*15)<millis()) {
        tickerLog=millis();
        // Daten zum Logger (Google Tabelle) senden
        WiFiClientSecure client;
        if (!client.connect(host, 443)) {
         Serial.println("connection failed");
         return;
        }
        // /macros/s/AKfycbzJqwSzEzhEhr9KIRHGrhXSypd-5MbWLsigN4BUvg-ivvq9GPsq/exec?temperature=1&pressure=2&signal_dB=12
        String url = "/macros/s/AKfycbzJqwSzEzhEhr9KIRHGrhXSypd-5MbWLsigN4BUvg-ivvq9GPsq/exec?temperature=";
        String sTemp = String(temperature);                
        sTemp.replace(".",",");
        url += sTemp;
        url +=String("&pressure=");
        url +=String(pressure);
        url +=String("&signal_dB=");
        url+=String(rssi);
        Serial.print("Requesting URL: ");
        Serial.println(url);
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
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
    else if (req.endsWith("/index.html")) {
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
    else if (req.startsWith("/xml") || req.startsWith("/json") || req.startsWith("/jsonp")) {
      
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
      s+=xmlEnde;          
      Serial.println("Output:"+s);   
    }
    else {
      s+=contentTypeText;
      s+=htmlhead;
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
      s+=htmltail;
    }


    // HTTP Antwort zurück zum Client
    clientS.print(s);
    clientS.stop();
   }
}


