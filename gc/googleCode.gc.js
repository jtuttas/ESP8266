
function doGet(e) { // change to doPost(e) if you are recieving POST data

    var ss = SpreadsheetApp.openById("1La_C9DYzRMM6PpUrHrfhvxG7Hw6Ffd1kavU8_1yWKKo");
    var out = "";
    if (e != undefined && e.queryString != null) {
        if (e.parameter["mac"] != undefined) {
            var sheet = ss.getSheetByName(e.parameter["mac"]);

            if (sheet != null) {
                sheet.insertRowBefore(2);
                sheet.getRange('b2').setValue(e.parameter["temperature"]);
                sheet.getRange('c2').setValue(e.parameter["pressure"]);
                sheet.getRange('d2').setValue(e.parameter["signal_dB"]);
                var dat = new Date();
                sheet.getRange('a2').setValue(dat.getDate() + "." + (dat.getMonth() + 1) + "." + dat.getFullYear() + " " + dat.getHours() + ":" + dat.getMinutes() + ":" + dat.getSeconds());
                // nach 3 Tagen löschen
                sheet.deleteRow(288);
            } else {
                out = "Kein Tabellenblatt mit Namen " + e.parameter["mac"] + " gefunden!";
            }
        } else {
            out = "Kein Parameter 'mac' gefunden";
        }
    } else {
        out = "kein query String enthalten!";
    }
    return ContentService.createTextOutput("Logger Server V1.4:" + out);
}    