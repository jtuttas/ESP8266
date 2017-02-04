var pollMac = ["5c:cf:7f:d0:1f:21:", "a0:20:a6:13:2e:26:"];
var temps= [[20,25],[0,20]];

$(document).ajaxSend(function (event, request, settings) {
    console.log("show loading indicator!");
    $.mobile.loading("show", {
        theme: "a",
        text: "sending..",
        textVisible: true,
        html: ""
    });
});

$(document).ajaxComplete(function (event, request, settings) {
    $.mobile.loading("hide", {
        theme: "a",
        html: "",
    });
    console.log("hide loading indicator!");
});

var toast = function (msg) {
    $("<div class='ui-loader ui-overlay-shadow ui-body-e ui-corner-all'><h3>" + msg + "</h3></div>")
            .css({display: "block",
                opacity: 0.90,
                position: "fixed",
                padding: "7px",
                "text-align": "center",
                width: "270px",
                left: ($(window).width() - 284) / 2,
                top: $(window).height() / 2})
            .appendTo($.mobile.pageContainer).delay(1500)
            .fadeOut(400, function () {
                $(this).remove();
            });
}

$(document).ready(function () {
    loadData();
});


function callback(json) {
    console.log("Results sensor "+i);
    console.log(JSON.stringify(json));
    var id =json.mac.replace(/\:/g,'');
    console.log(" the id="+id);
    $("#t"+id).text(json.temperature+" °C");
    $("#p"+id).text(json.pressure+" hPa");
    var i=0;
    pollMac.forEach(function (mac) {
        if (mac==json.mac) {
            console.log("found mac id "+i);
            if (json.temperature<temps[i][0]) {
                $("#t"+id).css('color', '#90D1D4');
            }
            else if (json.temperature>=temps[i][0] && json.temperature<temps[i][1]) {
                $("#t"+id).css('color', '#CDD490');                
            }
            else {
                $("#t"+id).css('color', '#D49090');
            }
        }
        i++;
    });
}

$("#refresh").click(function () {
    loadData();
});


function fetchGoogleData(mac) {
    $.ajax({
        url: "https://script.google.com/macros/s/AKfycbzJqwSzEzhEhr9KIRHGrhXSypd-5MbWLsigN4BUvg-ivvq9GPsq/exec?mac=" + mac + "&prefix=callback",
        dataType: 'jsonp',
    });
}

function loadData() {
     pollMac.forEach(function (mac) {
        console.log("Polling MAC:" + mac);
        fetchGoogleData(mac);
    });
}

