
function doGet(e) { // change to doPost(e) if you are recieving POST data

    var ss = SpreadsheetApp.openById("1La_C9DYzRMM6PpUrHrfhvxG7Hw6Ffd1kavU8_1yWKKo");
    Logger.log(ss.getName());

    if (e != undefined && e.queryString != null) {
        if (e.parameter["temperature"] != undefined) {
            ss.insertRowBefore(2);
            ss.getRange('b2').setValue(e.parameter["temperature"]);
            ss.getRange('c2').setValue(e.parameter["pressure"]);
            ss.getRange('d2').setValue(e.parameter["signal_dB"]);
            var dat = new Date();
            ss.getRange('a2').setValue(dat.getDate() + "." + (dat.getMonth() + 1) + "." + dat.getFullYear() + " " + dat.getHours() + ":" + dat.getMinutes() + ":" + dat.getSeconds());
            // nach 3 Tagen löschen
            ss.deleteRow(288);
        }
    }
    return ContentService.createTextOutput("Logger Server V1.2");
}    