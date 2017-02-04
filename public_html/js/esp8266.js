var s1Value = 512;
var s2Value = 512;
var s3Value = 512;
var dimSocket;
var pressureContext;
var frogImage;
var glasImage;
var pollMac = ["5c:cf:7f:d0:1f:21:", "a0:20:a6:13:2e:26:"];
var tempContexts=[2];
var tempOffsets=[-18,+10];
var tempArear=[18,45];



/**
 * JSONP Callback Funktion
 * @param {type} json JSON Data
 */
function callback(json) {
    console.log("Results");
    console.log(json);
    console.log("Temperature=" + json.temperature);
    $("#dimValueRed").text(json.dimValueRed);
    $("#dimValueGreen").text(json.dimValueGreen);
    $("#dimValueBlue").text(json.dimValueBlue);
    $("#signal_dB").text(json.signal_dB);
    $("#temperature").text(json.temperature);
    $("#pressure").text(json.pressure);
    $('#ex1').slider('setValue', json.dimValueRed);
    $('#ex2').slider('setValue', json.dimValueGreen);
    $('#ex3').slider('setValue', json.dimValueBlue);

    p = json.pressure;
    //p=101500;    
    h = ((p - 98000) * 316) / 6000;
    console.log("h pressure=" + h);

    pressureContext.drawImage(glasImage, 0, 0);
    pressureContext.drawImage(frogImage, 85, 316 - h);

}



$("#on").click(function () {
    console.log("dim to " + s1Value);
    $.ajax({
        url: "http://service.joerg-tuttas.de:8266/jsonp/dim/r=" + s1Value + "/g=" + s2Value + "/b=" + s3Value + "/",
        dataType: 'jsonp',
    });
});

$("#off").click(function () {
    $.ajax({
        url: "http://service.joerg-tuttas.de:8266/jsonp/off",
        dataType: 'jsonp',
    });
});


$(document).ready(function () {
    console.log("Start..");
    setPreview();
    $("#sdata").empty();
    pollMac.forEach(function (mac) {
        console.log("Polling MAC:" + mac);
        fetchGoogleData(mac);
    });
    doPoll();
    drawing = new Image();
    var canvas = document.getElementById("thermometer");
    tempContexts[0]=canvas.getContext('2d');
    
    drawing.onload = function () {
        console.log("Bild  Thermometer zeichen");
        tempContexts[0].drawImage(drawing, 0, 0);

        aImage = new Image();
        var ca = document.getElementById("athermometer");
        tempContexts[1]=ca.getContext('2d');
        aImage.onload = function () {
            console.log("Bild  AußenThermometer zeichen");
            tempContexts[1].drawImage(aImage, 0, 0);

            glasImage = new Image();
            canvas = document.getElementById("barometer");
            pressureContext = canvas.getContext('2d');
            glasImage.onload = function () {
                console.log("Bild  Wasserglas zeichen");
                pressureContext.drawImage(glasImage, 0, 0);

                frogImage = new Image();
                frogImage.onload = function () {
                    console.log("Frosch geladen zeichen");

                    dimSocket = new WebSocket("ws://service.joerg-tuttas.de:8267");

                    dimSocket.onerror = function (event) {
                        console.log("Error");
                        $("#controller").empty();
                        $("#wsdata").empty();
                        $("#controller").append('<h1>failed to connect to ESP8266 Websocket</h1>');
                    }
                    dimSocket.onmessage = function (event) {
                        console.log("Websocket receive data:" + event.data);
                        callback(JSON.parse(event.data));
                    };
                };
                frogImage.src = "img/frog.png";
            };
            glasImage.src = "img/waterglas.png";
        };
        aImage.src = "img/athermometer.png";
        console.log("fertig");
    };
    drawing.src = "img/thermometer.png";

});



function setPreview() {
    r = Math.round(s1Value / 4);
    g = Math.round(s2Value / 4);
    b = Math.round(s3Value / 4);
    $("#preview").css("background-color", "rgb(" + r + "," + g + "," + b + ")");
}


function pollCallback(data) {
    console.log("Poll callback:" + JSON.stringify(data));
    var html = ' <table class="table table-striped">' +
            '<thead>' +
            '<tr>' +
            '<th>Sensor (MAC=' + data.mac + ')</th>' +
            '<th>Value</th>' +
            '</tr>' +
            '</thead>' +
            '<tbody>' +
            '<tr>' +
            '<td >last measurement</td>' +
            '<td>' + data.date + '</td>' +
            '</tr>' +
            '<tr>' +
            '<td >temperature</td>' +
            '<td>' + data.temperature + ' °C</td>' +
            '</tr>' +
            '<tr>' +
            '<td >pressure</td>' +
            '<td>' + data.pressure + ' Pa</td>' +
            '</tr>' +
            '<tr>' +
            '<td >signal</td>' +
            '<td>' + data.signal_db + ' dB</td>' +
            '</tr>' +
            '<tr>' +
            '<td >Vcc</td>' +
            '<td>' + data.vcc + ' V</td>' +
            '</tr>' +
            '</tbody>' +
            '</table>';
    $("#sdata").append(html);

    // Termometer zeichen
    for (i=0;i<pollMac.length;i++) {
        if (pollMac[i]==data.mac) {
            console.log("Zeichne Termometer "+i);
            tempContexts[i].fillStyle = "#FFFFff";
            tempContexts[i].fillRect(18, 317, 24, -266);
            tempContexts[i].fillStyle = "#CC0001";
            temp = data.temperature;
            //temp=28.0

            h = ((temp + tempOffsets[i]) * 266) / tempArear[i];
            console.log("h temp=" + h);
            tempContexts[i].fillRect(17, 318, 25, -h);
        }
    }
}

function fetchGoogleData(mac) {
    $.ajax({
        url: "https://script.google.com/macros/s/AKfycbzJqwSzEzhEhr9KIRHGrhXSypd-5MbWLsigN4BUvg-ivvq9GPsq/exec?mac=" + mac + "&prefix=pollCallback",
        dataType: 'jsonp',
    });
}

function doPoll() {
    setTimeout(function () {
        $("#sdata").empty();
        pollMac.forEach(function (mac) {
            console.log("Polling MAC:" + mac);
            fetchGoogleData(mac);
        });
        doPoll();
    }, 1000 * 60 * 15);
}


$('#ex1').slider({
    formatter: function (value) {
        s1Value = value;
        setPreview();
        return 'red Value: ' + value;
    }
});
$('#ex2').slider({
    formatter: function (value) {
        s2Value = value;
        setPreview();
        return 'green Value: ' + value;
    }
});

$('#ex3').slider({
    formatter: function (value) {
        s3Value = value;
        setPreview();
        return 'blue Value: ' + value;
    }
});



