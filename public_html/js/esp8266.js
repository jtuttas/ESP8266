var s1Value=512;
var s2Value=512;
var s3Value=512;
var dimSocket;
var tempContext
var pressureContext;
var frogImage;
var glasImage;

/**
 * JSONP Callback Funktion
 * @param {type} json JSON Data
 */
function callback(json) {
    console.log("Results");
    console.log(json);
    console.log("Temperature="+json.temperature);
    $("#dimValueRed").text(json.dimValueRed);
    $("#dimValueGreen").text(json.dimValueGreen);
    $("#dimValueBlue").text(json.dimValueBlue);
    $("#signal_dB").text(json.signal_dB);
    $("#temperature").text(json.temperature);
    $("#pressure").text(json.pressure);
    $('#ex1').slider('setValue', json.dimValueRed);
    $('#ex2').slider('setValue', json.dimValueGreen);
    $('#ex3').slider('setValue', json.dimValueBlue);
    tempContext.fillStyle="#FFFFff";
    tempContext.fillRect(18,317,24,-266);    
    tempContext.fillStyle="#CC0001";
    temp = json.temperature;
    //temp=28.0
    
    h=((temp-18)*266)/18
    console.log("h temp="+h);
    tempContext.fillRect(17,318,25,-h);

    p = json.pressure;
    //p=101500;    
    h=((p-100000)*316)/4000;
    console.log("h pressure="+h);

    pressureContext.drawImage(glasImage,0,0);
    pressureContext.drawImage(frogImage,85,316-h);

}



$("#on").click(function () {    
    console.log("dim to "+s1Value);    
    $.ajax({
        url: "http://service.joerg-tuttas.de:8266/jsonp/dim/r="+s1Value+"/g="+s2Value+"/b="+s3Value+"/",
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
        
    drawing = new Image();
    var canvas=document.getElementById("thermometer");
    tempContext=canvas.getContext('2d');
    drawing.onload = function() {
        console.log("Bild  Thermometer zeichen");
        tempContext.drawImage(drawing,0,0);
        
        glasImage = new Image();
        canvas=document.getElementById("barometer");
        pressureContext=canvas.getContext('2d');
        glasImage.onload = function() {
            console.log("Bild  Wasserglas zeichen");
            pressureContext.drawImage(glasImage,0,0);

            frogImage = new Image();
            frogImage.onload = function () {
                console.log("Frosch geladen zeichen");

                dimSocket = new WebSocket("ws://service.joerg-tuttas.de:8267");

                dimSocket.onmessage = function (event) {
                    console.log("Websocket receive data:"+event.data);
                    callback(JSON.parse(event.data));       
                };
            };
            frogImage.src = "img/frog.png"; 
        };           
        glasImage.src = "img/waterglas.png"; 
    };
    drawing.src = "img/thermometer.png"; 

});



function setPreview() {
    r=Math.round(s1Value/4);
    g=Math.round(s2Value/4);
    b=Math.round(s3Value/4);
    $("#preview").css("background-color", "rgb("+r+"," + g + "," + b + ")");
}

/*
function doPoll() {
    setTimeout(function () {
        $.ajax({
            url: "http://service.joerg-tuttas.de:8266/jsonp",
            dataType: 'jsonp',
        });
        doPoll();
    }, 2000);
}
*/

$('#ex1').slider({
	formatter: function(value) {
                             s1Value=value;
                             setPreview();
		return 'red Value: ' + value;
	}
});
$('#ex2').slider({
	formatter: function(value) {
                             s2Value=value;
                             setPreview();
		return 'green Value: ' + value;
	}
});

$('#ex3').slider({
	formatter: function(value) {
                             s3Value=value;
                             setPreview();
		return 'blue Value: ' + value;
	}
});



