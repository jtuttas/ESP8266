#include <ESP8266WiFi.h>
#include "FS.h"
#include <RCSwitch.h>

// Data Verbindung des RC Senders
#define gpioData 2

// Code für den Sender
const char* housecode="11001";

// Codes der Steckdosen
char* socketcodes[] = {"10000","01000"};

// Name des Config Files
#define configFilename "config.dat"

long delays[] ={0,0};
int states[] = {0,0};
RCSwitch mySwitch = RCSwitch();
WiFiServer server(80);
String delaytime;
const String header_ok="HTTP/1.1 200 OK\r\n";
const String header_notfound="HTTP/1.1 404 OK\r\n";
const String header_error="HTTP/1.1 500 OK\r\n";
const String header_forbidden="HTTP/1.1 403 OK\r\n";
const String contentTypeText="Content-Type: text/html\r\n";
const String contentTypeJson="Content-Type: application/json\r\n";
const String contentTypeJsonp="Content-Type: application/javascript\r\n";
const String htmlhead="\r\n<!DOCTYPE HTML>\r\n<html><head><title>esp6288</title>  <link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\"></head><body>\r\n<div class=\"jumbotron\"><div class=\"container\"><h1>IoT with ESP8266</h1><img src=\"https://hackadaycom.files.wordpress.com/2014/11/esp.png?w=400\"></div></div>";
const String configForm = " <form method=\"GET\" action=\"/conf\"><div class=\"form-group\"><label for=\"usr\">SSID:</label>";
const String htmltail="\r\n</body></html>\r\n";
const String jsonStart="{\r\n";
const String jsonEnde="\r\n}";
const String jsonpStart="callback(";
const String jsonpEnde=");";
char MAC_char[18];
String confSSID;
String confPasswd;
bool AP_Mode=false;
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
    s+=String("\"switches\": [\r\n");
    for (int i=0;i<sizeof(socketcodes)/4;i++) {
      s+=String("{\r\n\"code\":\"");
      s+=socketcodes[i];
      s+=String("\",\r\n\"state\":");
      s+=states[i];
      s+=String(",\r\n\"delay\":");
      s+=delays[i];
      if (i==(sizeof(socketcodes)/4)-1) {
        s+=String("\r\n}");
      }
      else {
        s+=String("\r\n},");
      }
    }
    s+=String("\r\n]");
    s+=String(",\r\n\"signal_dB\":");
    s+=String(rssi);
    s+=String(",\r\n\"mac\": \"");
    s+=String(MAC_char);
    s+=String("\"");
    s+=jsonEnde; 
    return s;    
}

String getQuery(String key,String query) {
  String value = query.substring(query.indexOf(key)+key.length()+1);
  if (value.indexOf("&")==-1) {
    return value;
  }
  else {
    return value.substring(0,value.indexOf("&"));
  }
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
      Serial.println("finished reading config.json");  
      f.close();
    }
    return true;
}


void setup() {
  Serial.begin(115200);
  delay(1000);
  uint8_t MAC_array[6];  
  Serial.write("\r\nRead MAC Adress: ");
  WiFi.macAddress(MAC_array);
  for (int i = 0; i < sizeof(MAC_array); ++i){
    sprintf(MAC_char,"%s%02x:",MAC_char,MAC_array[i]);
  }  
  Serial.println(MAC_char);

  
  Serial.write("\r\nOpen File System:\r\n");
  SPIFFS.begin();
  delay(1000);

  bool result = readConfig(configFilename);
  SPIFFS.end();
  if (!result) {
    Serial.println("open config.dat failed");
    AP_Mode=true;
     // AP mode
    WiFi.mode(WIFI_AP);
    Serial.println("Starte WLAN Access Point");
    char* AP_SSID="ESP8266-RC";
    WiFi.softAP(AP_SSID, "");
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
    delay(1000);
    int n=0;
    while (WiFi.status() != WL_CONNECTED && n<20) {
      delay(1000);
      Serial.write(".");
      n++;
      Serial.println(WiFi.localIP());
      if (WiFi.localIP()!=0) {
        break;
      }
    }
    if (WiFi.status() == WL_CONNECTED || WiFi.localIP()!=0) {
      Serial.write("\r\nWiFi connected");
      delay(1000);
      Serial.write("\r\nIP address: ");
      Serial.println(WiFi.localIP());      
      Serial.println("enable Transmit");
      mySwitch.enableTransmit(gpioData);
      Serial.println("Turn OFF");
      for (int i=0;i<sizeof(socketcodes)/4;i++) {
        mySwitch.switchOff(housecode, socketcodes[i]);
      }
    }
    else {
      Serial.println("\r\nConnection failed!");
      AP_Mode=true;
       // AP mode
      WiFi.mode(WIFI_AP);
      Serial.println("Starte WLAN Access Point");
      char* AP_SSID="ESP8266-RC";
      WiFi.softAP(AP_SSID, "");
      delay(2000);
    }
  } 

  rssi = WiFi.RSSI();
  Serial.print("Signal strength: ");
  Serial.print(rssi);
  Serial.println("dB");

  Serial.write("\r\nStarte Server");
  server.begin();
}

// check a string to see if it is numeric Using Arduino Lib code
bool isNumericB(String str) {
  for (byte i = 0; i<str.length(); i++)
  {
    if (!isdigit(str.charAt(i))) return false;
  }
  return true;
}

void loop() {
  WiFiClient clientS = server.available();
  if (!clientS) {
    for (int i=0;i<sizeof(socketcodes)/4;i++) {
      if (delays[i]!=0 && millis()>delays[i]) {
        mySwitch.switchOff(housecode, socketcodes[i]);  
        Serial.println("Turn Off after delay");
        delays[i]=0;
        states[i]=0;
      }
      
    }
    return;
  }
  else {
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
    else if (AP_Mode==false && req.indexOf("?")!=-1) {
      String query = req.substring(req.indexOf("?")+1);

      String socket = getQuery("socket",query);
      Serial.println ("Socket");
      Serial.println (socket);      
     
      if (socket !="" && socket.toInt()>=0 && socket.toInt()<sizeof(socketcodes)/4) {
        String state=getQuery("state",query);
        if (state!="") {
          if (state=="0") {
            s=header_ok;
            mySwitch.switchOff(housecode, socketcodes[socket.toInt()]);
            states[socket.toInt()]=0;
          }
          else if (state=="1") {
            s=header_ok;
            mySwitch.switchOn(housecode, socketcodes[socket.toInt()]);  
            states[socket.toInt()]=1;
            String delaytime=getQuery("delay",query);
            if (delaytime!="" && isNumericB(delaytime)==true) {
              delays[socket.toInt()]=millis()+delaytime.toInt()*1000;
            }
          }
          else {
            Serial.println("Query Mismatch! Missig or wrong state Parameter");
            s=header_error;        
            clientS.print(s);
            clientS.stop();
            return;            
          }
        }
      }
      else {
        Serial.println("Query Mismatch!");
        s=header_error;        
        clientS.print(s);
        clientS.stop();
        return;
      }
      
      if (req.startsWith("/jsonp")) {
        s=header_ok;     
        Serial.write("\r\nJSONP ");
        s+=contentTypeJsonp;      
        // Ende vom HTTP Header
        s+=String("\r\n");
        s+=jsonpStart;
        s+=getJson();
        s+=jsonpEnde;
        Serial.println("Output:"+s);   
      }
      else {
        s=header_ok;     
        Serial.write("\r\nJSON ");
        s+=contentTypeJson;      
        // Ende vom HTTP Header
        s+=String("\r\n");
        s+=getJson();
        Serial.println("Output:"+s);   
      }
    }
    else if (AP_Mode==true) {
      s=header_ok;
      s+=contentTypeText;
      s+=htmlhead;
      if (req.startsWith("/conf?ssid")) {
        s+=String("<h1>Config stored and ESP8266 press reset!</h1>");
        Serial.write("\r\nSchreibe neue config Datei");   
  
        String id=urlDecode(req.substring(req.indexOf("?ssid=")+6,req.indexOf("&pwd")));
        String pwd=urlDecode(req.substring(req.indexOf("&pwd=")+5,req.indexOf("&push")));

        SPIFFS.begin();
        File f = SPIFFS.open(configFilename, "w");
        if (!f) {
          Serial.println("open config.dat failed");
        }
        else {   
          Serial.println ("Set SSID to");
          Serial.println(id);    
          confSSID=id;    
          Serial.println ("Set Password to");
          Serial.println(pwd);
          confPasswd=pwd;
          f.println(id);
          f.println(pwd);
          Serial.println("finished writing config.dat");    
        }
        f.close();
        SPIFFS.end();
      }
      else {
        s+=configForm;        
        s+=String("<input type=\"text\" class=\"form-control\" name=\"ssid\" placeholder=\"WLAN SSID\" value=\"");
        s+=String(confSSID);
        s+=String("\"><br/><label for=\"pwd\">Password:</label><input type=\"password\" class=\"form-control\" name=\"pwd\" placeholder=\"WLAN Password\" value=\"");
        s+=String(confPasswd);
        s+=String("\">");
        s+=String("</div><button type=\"Submit\" class=\"btn btn-danger btn-block\">Submit</button></form>");
      }
      s+=htmltail;
    }
    else if (AP_Mode==false) {
      if (req.startsWith("/jsonp")) {
        s=header_ok;
        Serial.write("\r\nJSONP ");
        s+=contentTypeJsonp;      
        // Ende vom HTTP Header
        s+=String("\r\n");
        s+=jsonpStart;
        s+=getJson();
        s+=jsonpEnde;
        Serial.println("Output:"+s);   
      }
      else {
        s=header_ok;
        Serial.write("\r\nJSON ");
        s+=contentTypeJson;      
        // Ende vom HTTP Header
        s+=String("\r\n");
        s+=getJson();
        Serial.println("Output:"+s);   
      }
    }
    else {
      s=header_notfound;
      clientS.print(s);
      clientS.stop();
      return;
    }


    // HTTP Antwort zurück zum Client
    clientS.print(s);
    clientS.stop();
  }
}



