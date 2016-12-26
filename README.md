# IoT With ESP8266
This Project realized an example how to connect to a Webservice running on the ESP8266 with a temperature / pressure Sensor, the BMP180 and an RGB LED

![IoT with ESP8266](http://img.youtube.com/vi/_6Ki8T2blc0/0.jpg)](http://www.youtube.com/watch?v=_6Ki8T2blc0)

### Wirering
![Wireing](doc/aufbau_Steckplatine.png)

### Features
  - Webservice JSON,XML,JSONP
  - Embedded Webserver
  - HTML5 Client
  - .Net Client in Powershell
  - logging into a google spreadsheet

### WebApi
[![Webapi](doc/requests.png)]

### Powershell Examples

**Get the current Sensor Data**
```sh
Invoke-RestMethod http://service.joerg-tuttas.de:8266/json
```
**Dim red LED to 50%**
```sh
Invoke-RestMethod http://service.joerg-tuttas.de:8266/json/dim/r=512/
```
**Dim red LED to 10%,green LED to 50% and blue LED to 100%**
```sh
Invoke-RestMethod http://service.joerg-tuttas.de:8266/json/dim/r=102/g=512/b=1023/
```
**Get Sensor Data as XML**
```sh
Invoke-RestMethod http://service.joerg-tuttas.de:8266/xml
```

### used Ressources

### Examples

- Take a look at the [html5] client
- ESP8266 [Webserver] page
- Get sensor data as [json]
- Get Sensor data as [xml]

   [html5]: <http://htmlpreview.github.io/?https://github.com/jtuttas/ESP8266/blob/final/public_html/index.html>
   [Webserver]: <http://service.joerg-tuttas.de:8266/index.html>
   [json]: <http://service.joerg-tuttas.de:8266/json>
   [xml]: <http://service.joerg-tuttas.de:8266/xml>
   
