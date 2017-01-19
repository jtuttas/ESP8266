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

function callback(json) {
    console.log("Results");
    console.log(JSON.stringify(json));
    toast("success");
}

$("#on").click(function () {
    adr = url + "?socket%3D0%26state%3D1";
    if ($("#ta").val() != "") {
        adr = adr + "%26delay%3D" + $("#ta").val();
    }
    console.log("adr=" + adr);
    $.ajax({
        url: adr,
        type: "GET",
        cache: false,
        dataType: 'jsonp',
        success: function (data) {
        },
        error: function (xhr, textStatus, errorThrown) {
            console.log("Fehler:" + xhr.status + " " + textStatus + " " + errorThrown);
            if (xhr.status!=200) {
                toast("Fehler " + xhr.status);
            }
        }
    });
});
$("#off").click(function () {
    adr = url + "?socket%3D0%26state%3D0";
    console.log("adr=" + adr);
    $.ajax({
        url: adr,
        type: "GET",
        cache: false,
        dataType: 'jsonp',
        success: function (data) {
        },
        error: function (xhr, textStatus, errorThrown) {
            console.log("Fehler:" + xhr + " " + textStatus + " " + errorThrown);
            if (xhr.status!=200) {
                toast("Fehler " + xhr.status);
            }
        }
    });
});