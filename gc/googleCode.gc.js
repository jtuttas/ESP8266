
function doGet(e) { // change to doPost(e) if you are recieving POST data
   
  var ss = SpreadsheetApp.openById("1La_C9DYzRMM6PpUrHrfhvxG7Hw6Ffd1kavU8_1yWKKo");
  var out="";
  if (e!=undefined && e.queryString!=null) {
    if (e.parameter["mac"]!=undefined) {
      var sheet = ss.getSheetByName(e.parameter["mac"]);

      if (sheet!=null) {
        var answer = new Object();
        answer.date=sheet.getRange('a2').getValue()
        answer.temperature=sheet.getRange('b2').getValue()
        answer.pressure=sheet.getRange('c2').getValue()
        answer.signal_db=sheet.getRange('d2').getValue()
        answer.vcc=sheet.getRange('e2').getValue()
        answer.mac=e.parameter["mac"];
        if (e.parameter["prefix"]!=undefined) {
          var content = e.parameters.prefix + '(' +JSON.stringify(answer) + ')';
          return ContentService.createTextOutput(content).setMimeType(ContentService.MimeType.JAVASCRIPT); 
        }
        else {
          var content = JSON.stringify(answer);
          return ContentService.createTextOutput(content).setMimeType(ContentService.MimeType.JSON); 
        }
      }
      else {
        out ="Kein Tabellenblatt mit Namen "+e.parameter["mac"]+" gefunden!";
      }
    }
    else {
      out="Kein Parameter 'mac' gefunden";
    }    
  }
  else {
    out="kein query String enthalten!";
  }
  return ContentService.createTextOutput("Logger Server V1.4:"+out);
}    

function doPost(e) {
   
  var ss = SpreadsheetApp.openById("1La_C9DYzRMM6PpUrHrfhvxG7Hw6Ffd1kavU8_1yWKKo");
  var out="";
  if (e!=undefined && e.queryString!=null) {
    if (e.parameter["mac"]!=undefined) {
      var sheet = ss.getSheetByName(e.parameter["mac"]);

      if (sheet!=null) {
        sheet.insertRowBefore(2);
        sheet.getRange('b2').setValue(e.parameter["temperature"]);
        sheet.getRange('c2').setValue(e.parameter["pressure"]);
        sheet.getRange('d2').setValue(e.parameter["signal_dB"]);
        sheet.getRange('e2').setValue(e.parameter["vcc"]);
        var dat = new Date();      
        sheet.getRange('a2').setValue(dat.getDate()+"."+(dat.getMonth()+1)+"."+dat.getFullYear()+" "+dat.getHours()+":"+dat.getMinutes()+":"+dat.getSeconds());
        
        // Zeile f. aktuelle Werte (werden gebraucht für die Anzeigeinstrumente!)
        sheet.getRange('g1').setValue(dat.getDate()+"."+(dat.getMonth()+1)+"."+dat.getFullYear()+" "+dat.getHours()+":"+dat.getMinutes()+":"+dat.getSeconds());
        sheet.getRange('h1').setValue(e.parameter["temperature"]);
        sheet.getRange('i1').setValue(e.parameter["pressure"]);
        sheet.getRange('j1').setValue(e.parameter["signal_dB"]);
        sheet.getRange('k1').setValue(e.parameter["vcc"]);
        // nach 3 Tagen löschen
        sheet.deleteRow(288);
      }
      else {
        out ="Kein Tabellenblatt mit Namen "+e.parameter["mac"]+" gefunden!";
      }
    }
    else {
      out="Kein Parameter 'mac' gefunden";
    }    
  }
  else {
    out="kein query String enthalten!";
  }
  return ContentService.createTextOutput("Logger Server V1.4:"+out);
}    