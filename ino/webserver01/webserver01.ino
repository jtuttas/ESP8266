#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "FS.h"


// BMP180
#define scl 0
#define sda 2

bool flasher=true;

// Deep Sleep Mode
const bool deepSleep = true;
// alle 15 Minuten aufwachen
#define deepSleepS 15*60*1000*1000

#define configFilename "config4.dat"

WiFiServer server(80);
const String header_ok = "HTTP/1.1 200 OK\r\n";
const String header_notfound = "HTTP/1.1 404 OK\r\n";
const String header_error = "HTTP/1.1 500 OK\r\n";
const String header_forbidden = "HTTP/1.1 403 OK\r\n";
const String contentTypeText = "Content-Type: text/html\r\n";
const String contentTypeJson = "Content-Type: application/json\r\n";
const String contentTypeJsonp = "Content-Type: application/javascript\r\n";
const String contentTypeXml = "Content-Type: application/xml\r\n";
const String htmlhead = "\r\n<!DOCTYPE HTML>\r\n<html><head><title>esp6288</title>  <link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\"></head><body>\r\n<div class=\"jumbotron\"><div class=\"container\"><h1>IoT with ESP8266</h1><img src=\"https://hackadaycom.files.wordpress.com/2014/11/esp.png?w=400\"></div></div>";
const String configForm = " <form method=\"GET\" action=\"/conf\"><div class=\"form-group\"><label for=\"usr\">SSID:</label>";
const String htmltail = "\r\n</body></html>\r\n";
const String jsonStart = "{\r\n \"dimValueRed\":";
const String jsonEnde = "\r\n}";
const String xmlStart = "<esp8266>\r\n <dimValueRed>";
const String xmlEnde = "\r\n</esp8266>";
const String jsonpStart = "callback(";
const String jsonpEnde = ");";
Adafruit_BMP085 bmp180;
float temperature;
int32_t pressure;
char MAC_char[18];
String confSSID;
String confPasswd;
String confUrl;
bool AP_Mode = false;


// https://script.google.com/macros/s/AKfycbzJqwSzEzhEhr9KIRHGrhXSypd-5MbWLsigN4BUvg-ivvq9GPsq/exec?temperature=1&pressure=2&signal_dB=12

const char* host = "script.google.com";
long rssi;


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
  urlChars.replace("%7F", "Ã‚");
  urlChars.replace("%80", "`");
  urlChars.replace("%81", "Ã‚");
  urlChars.replace("%82", "Ã¢");
  urlChars.replace("%83", "Ã†");
  urlChars.replace("%84", "Ã¢");
  urlChars.replace("%85", "Ã¢Â¦");
  urlChars.replace("%86", "Ã¢");
  urlChars.replace("%87", "Ã¢Â¡");
  urlChars.replace("%88", "Ã‹");
  urlChars.replace("%89", "Ã¢Â°");
  urlChars.replace("%8A", "Ã…");
  urlChars.replace("%8B", "Ã¢Â¹");
  urlChars.replace("%8C", "Ã…");
  urlChars.replace("%8D", "Ã‚");
  urlChars.replace("%8E", "Ã…Â½");
  urlChars.replace("%8F", "Ã‚");
  urlChars.replace("%90", "Ã‚");
  urlChars.replace("%91", "Ã¢");
  urlChars.replace("%92", "Ã¢");
  urlChars.replace("%93", "Ã¢");
  urlChars.replace("%94", "Ã¢");
  urlChars.replace("%95", "Ã¢Â¢");
  urlChars.replace("%96", "Ã¢");
  urlChars.replace("%97", "Ã¢");
  urlChars.replace("%98", "Ã‹");
  urlChars.replace("%99", "Ã¢Â¢");
  urlChars.replace("%9A", "Ã…Â¡");
  urlChars.replace("%9B", "Ã¢Âº");
  urlChars.replace("%9C", "Ã…");
  urlChars.replace("%9D", "Ã‚");
  urlChars.replace("%9E", "Ã…Â¾");
  urlChars.replace("%9F", "Ã…Â¸");
  urlChars.replace("%A0", "Ã‚");
  urlChars.replace("%A1", "Ã‚Â¡");
  urlChars.replace("%A2", "Ã‚Â¢");
  urlChars.replace("%A3", "Ã‚Â£");
  urlChars.replace("%A4", "Ã‚Â¤");
  urlChars.replace("%A5", "Ã‚Â¥");
  urlChars.replace("%A6", "Ã‚Â¦");
  urlChars.replace("%A7", "Ã‚Â§");
  urlChars.replace("%A8", "Ã‚Â¨");
  urlChars.replace("%A9", "Ã‚Â©");
  urlChars.replace("%AA", "Ã‚Âª");
  urlChars.replace("%AB", "Ã‚Â«");
  urlChars.replace("%AC", "Ã‚Â¬");
  urlChars.replace("%AE", "Ã‚Â®");
  urlChars.replace("%AF", "Ã‚Â¯");
  urlChars.replace("%B0", "Ã‚Â°");
  urlChars.replace("%B1", "Ã‚Â±");
  urlChars.replace("%B2", "Ã‚Â²");
  urlChars.replace("%B3", "Ã‚Â³");
  urlChars.replace("%B4", "Ã‚Â´");
  urlChars.replace("%B5", "Ã‚Âµ");
  urlChars.replace("%B6", "Ã‚Â¶");
  urlChars.replace("%B7", "Ã‚Â·");
  urlChars.replace("%B8", "Ã‚Â¸");
  urlChars.replace("%B9", "Ã‚Â¹");
  urlChars.replace("%BA", "Ã‚Âº");
  urlChars.replace("%BB", "Ã‚Â»");
  urlChars.replace("%BC", "Ã‚Â¼");
  urlChars.replace("%BD", "Ã‚Â½");
  urlChars.replace("%BE", "Ã‚Â¾");
  urlChars.replace("%BF", "Ã‚Â¿");
  urlChars.replace("%C0", "Ãƒ");
  urlChars.replace("%C1", "Ãƒ");
  urlChars.replace("%C2", "Ãƒ");
  urlChars.replace("%C3", "Ãƒ");
  urlChars.replace("%C4", "Ãƒ");
  urlChars.replace("%C5", "Ãƒ");
  urlChars.replace("%C6", "Ãƒ");
  urlChars.replace("%C7", "Ãƒ");
  urlChars.replace("%C8", "Ãƒ");
  urlChars.replace("%C9", "Ãƒ");
  urlChars.replace("%CA", "Ãƒ");
  urlChars.replace("%CB", "Ãƒ");
  urlChars.replace("%CC", "Ãƒ");
  urlChars.replace("%CD", "Ãƒ");
  urlChars.replace("%CE", "Ãƒ");
  urlChars.replace("%CF", "Ãƒ");
  urlChars.replace("%D0", "Ãƒ");
  urlChars.replace("%D1", "Ãƒ");
  urlChars.replace("%D2", "Ãƒ");
  urlChars.replace("%D3", "Ãƒ");
  urlChars.replace("%D4", "Ãƒ");
  urlChars.replace("%D5", "Ãƒ");
  urlChars.replace("%D6", "Ãƒ");
  urlChars.replace("%D7", "Ãƒ");
  urlChars.replace("%D8", "Ãƒ");
  urlChars.replace("%D9", "Ãƒ");
  urlChars.replace("%DA", "Ãƒ");
  urlChars.replace("%DB", "Ãƒ");
  urlChars.replace("%DC", "Ãƒ");
  urlChars.replace("%DD", "Ãƒ");
  urlChars.replace("%DE", "Ãƒ");
  urlChars.replace("%DF", "Ãƒ");
  urlChars.replace("%E0", "Ãƒ");
  urlChars.replace("%E1", "ÃƒÂ¡");
  urlChars.replace("%E2", "ÃƒÂ¢");
  urlChars.replace("%E3", "ÃƒÂ£");
  urlChars.replace("%E4", "ÃƒÂ¤");
  urlChars.replace("%E5", "ÃƒÂ¥");
  urlChars.replace("%E6", "ÃƒÂ¦");
  urlChars.replace("%E7", "ÃƒÂ§");
  urlChars.replace("%E8", "ÃƒÂ¨");
  urlChars.replace("%E9", "ÃƒÂ©");
  urlChars.replace("%EA", "ÃƒÂª");
  urlChars.replace("%EB", "ÃƒÂ«");
  urlChars.replace("%EC", "ÃƒÂ¬");
  urlChars.replace("%ED", "ÃƒÂ­");
  urlChars.replace("%EE", "ÃƒÂ®");
  urlChars.replace("%EF", "ÃƒÂ¯");
  urlChars.replace("%F0", "ÃƒÂ°");
  urlChars.replace("%F1", "ÃƒÂ±");
  urlChars.replace("%F2", "ÃƒÂ²");
  urlChars.replace("%F3", "ÃƒÂ³");
  urlChars.replace("%F4", "ÃƒÂ´");
  urlChars.replace("%F5", "ÃƒÂµ");
  urlChars.replace("%F6", "ÃƒÂ¶");
  urlChars.replace("%F7", "ÃƒÂ·");
  urlChars.replace("%F8", "ÃƒÂ¸");
  urlChars.replace("%F9", "ÃƒÂ¹");
  urlChars.replace("%FA", "ÃƒÂº");
  urlChars.replace("%FB", "ÃƒÂ»");
  urlChars.replace("%FC", "ÃƒÂ¼");
  urlChars.replace("%FD", "ÃƒÂ½");
  urlChars.replace("%FE", "ÃƒÂ¾");
  urlChars.replace("%FF", "ÃƒÂ¿");

  return urlChars;
}


bool readConfig(String fname) {
  File f = SPIFFS.open(fname, "r");
  if (!f) {
    Serial.println("open " + fname + " failed");
    f.close();
    return false;
  }
  else {
    confSSID = f.readStringUntil('\n');
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


void send2Google() {
  WiFiClientSecure client;
  if (!client.connect(host, 443)) {
    Serial.println("connection failed");
    return;
  }
  // /macros/s/AKfycbzJqwSzEzhEhr9KIRHGrhXSypd-5MbWLsigN4BUvg-ivvq9GPsq/exec?temperature=1&pressure=2&signal_dB=12
  String url = confUrl.substring(0, confUrl.length() - 1);
  url += String("?temperature=");
  String sTemp = String(temperature);
  sTemp.replace(".", ",");
  url += sTemp;
  url += String("&pressure=");
  url += String(pressure);
  url += String("&signal_dB=");
  url += String(rssi);
  url += String("&mac=");
  url += String(MAC_char);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Length: 0\r\n"
               "Connection: close\r\n\r\n");
  delay(10);
  Serial.println("Response:");
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}

void setup() {
  Serial.begin(115200);
  delay(4000);
  pinMode(5,OUTPUT);
  digitalWrite(5,HIGH);
  uint8_t MAC_array[6];
  Serial.write("\r\nRead MAC Adress: ");
  WiFi.macAddress(MAC_array);
  for (int i = 0; i < sizeof(MAC_array); ++i) {
    sprintf(MAC_char, "%s%02x:", MAC_char, MAC_array[i]);
  }
  Serial.println(MAC_char);

  Serial.write("\r\nOpen File System:\r\n");
  SPIFFS.begin();

  bool result = readConfig(configFilename);

  if (!result) {
    Serial.println("open config.dat failed");
    AP_Mode = true;
    // AP mode
    WiFi.mode(WIFI_AP);
    Serial.println("Starte WLAN Access Point");
    char* AP_SSID = "ESP8266";
    WiFi.softAP(AP_SSID, "");
  }
  else {
    WiFi.mode(WIFI_STA);
    Serial.write("\r\nConnect to WLAN (SSID,passwd)\r\n");
    Serial.println(confSSID);
    Serial.println(confPasswd);
    String sid = confSSID.substring(0, confSSID.length() - 1);
    String spw = confPasswd.substring(0, confPasswd.length() - 1);
    const char* ssid = sid.c_str();
    const char* wlan_pwd = spw.c_str();
    WiFi.begin(ssid, wlan_pwd);
    int n = 0;
    while (WiFi.status() != WL_CONNECTED && n < 50) {
      flasher=!flasher;
      if (flasher) {
        digitalWrite(5,HIGH);
      }
      else {
        digitalWrite(5,LOW);
      }
      delay(1000);
      Serial.write(".");
      Serial.println(WiFi.localIP());
      n++;
    }
    if (WiFi.status() == WL_CONNECTED  || WiFi.localIP()!=0) {
      Serial.write("\r\nWiFi connected");
      delay(1000);
      Serial.write("\r\nIP address: ");
      Serial.println(WiFi.localIP());
    }
    else {
      Serial.println("\r\nConnection failed!");
      Serial.write("\r\nSetup abborded, turn into deep sleep!");
      ESP.deepSleep(60*1000*1000);
      return;
    }
  }



  rssi = WiFi.RSSI();
  Serial.print("Signal strength: ");
  Serial.print(rssi);
  Serial.println("dB");

  Serial.write("\r\nInitialisiere BMP180");
  Wire.begin(sda, scl);
  if (bmp180.begin()) {
    Serial.println("BMP180 init success");
    Serial.print("Temperature = ");
    temperature = bmp180.readTemperature();
    Serial.print(temperature);
    Serial.println(" Celsius");
    Serial.print("Pressure = ");
    pressure = bmp180.readPressure();
    Serial.print(pressure);
    Serial.println(" Pascal");
    Serial.println();
  }
  else
  {
    Serial.println("BMP180 init fail !\n\n");
    temperature = 0.0;
    pressure = 0;
  }
  if (!AP_Mode) {
    send2Google();
    Serial.write("\r\nfinished Setup turn into deep sleep!");
    ESP.deepSleep(deepSleepS);
  }
  else {
    Serial.write("\r\nStarte Server");
    server.begin();
  }
}

void loop() {
  WiFiClient clientS = server.available();
  String s;
  String req;
  if (!clientS) {
    
    return;
  }
  else {
    Serial.write("\r\nnew Client:");
    String request = clientS.readStringUntil('\r');
    Serial.println(">" + request + "<");
    clientS.flush();

    req = request.substring(request.indexOf("/"), request.lastIndexOf(" "));
    Serial.println(req);

    
    if (req.length() == 0) {
      return;
    }
    

    // HTTP Antwort zusammensetzten
    s = header_ok;
    s += contentTypeText;
    s += htmlhead;
    if (req.startsWith("/config.html")) {
      s += configForm;
      s += String("<input type=\"text\" class=\"form-control\" name=\"ssid\" placeholder=\"WLAN SSID\" value=\"");
      s += String(confSSID);
      s += String("\"><br/><label for=\"pwd\">Password:</label><input type=\"password\" class=\"form-control\" name=\"pwd\" placeholder=\"WLAN Password\" value=\"");
      s += String(confPasswd);
      s += String("\"><br/><label for=\"push\">Push URL:</label><input type=\"text\" class=\"form-control\" name=\"push\" placeholder=\"URL to push the Data\" value=\"");
      s += String(confUrl);
      s += String("\"></div><button type=\"Submit\" class=\"btn btn-danger btn-block\">Submit</button></form>");
    }
    else if (req.startsWith("/conf?ssid")) {
      s += String("<h1>Config stored and ESP8266 doing reset!</h1>");
    }
    s += htmltail;
  }


  // HTTP Antwort zurÃ¼ck zum Client
  clientS.print(s);
  clientS.stop();

  if (req.startsWith("/conf?ssid")) {
    Serial.write("\r\nSchreibe neue config Datei");

    String id = urlDecode(req.substring(req.indexOf("?ssid=") + 6, req.indexOf("&pwd")));
    String pwd = urlDecode(req.substring(req.indexOf("&pwd=") + 5, req.indexOf("&push")));
    String purl = urlDecode(req.substring(req.indexOf("&push=") + 6));
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




