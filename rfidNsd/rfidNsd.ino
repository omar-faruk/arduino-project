#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <SD.h>

LiquidCrystal lcd(8, 7, 6, 5, 3, 2);
SoftwareSerial rfid = SoftwareSerial(9, 13);
char c;
String tag = "", command, mode;

void setup() {
  Serial.begin(9600);
  rfid.begin(9600);
  pinMode(4, OUTPUT);

  if (!SD.begin(4)) {
    return;
  }
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
}

void loop() {
  if (Serial.available()) {
    command = Serial.readString();
    lcd.clear();
    if (command == "dump\n" || command == "dump") {
      dumpFile();
    }
    if (command == "attendance-mode" || command == "attendance-mode\n") {
      mode = "atmode";
      lcd.println("Attendance Mode");
    }
    if (command == "registration-mode" || command == "registration-mode\n") {
      mode = "regmode";
      lcd.println("Registration Mode");
    }
  }
  while (rfid.available() > 0) {
    c = rfid.read();
    tag += c;
  }
  tag = tag.substring(1, 13);

  if (tag.length() > 10) {
    newEntry();
  }
  delay(50);
}

void newEntry() {
  File dataFile;
  if (mode == "regmode") {
    if (!isRegistered(tag)) {
      dataFile = SD.open("db.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.println(tag);
        dataFile.close();
      }
      lcd.clear();
      lcd.println("Registration Complete");
    }
    else{
      lcd.clear();
      lcd.println("Registered User");
    }
  }

  else if(mode == "atmode"){
    if (isRegistered(tag)) {
      dataFile = SD.open("atd.txt", FILE_WRITE);
      if (!Attended()) {
        if(dataFile){
          dataFile.println(tag);
          dataFile.close();
          lcd.clear();
          lcd.println("Attendance Complete");
        }
      }
      else {
        lcd.clear();
        lcd.println("Already Taken");
      }
    }
    else{
      lcd.clear();
      lcd.println("User Not Registered");
    }
  }
  tag = "";
  delay(2000);
  lcd.clear();
  if (mode == "atmode") {
    lcd.println("Attendance Mode");
  }
  if (mode == "regmode") {
    lcd.println("Registration Mode");
  }

}

void dumpFile() {
  File dataFile = SD.open("db.txt");
  if (dataFile) {
    while (dataFile.available()) {
      String line = dataFile.readString();
      Serial.println(line);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("Dumping done!");
    dataFile.close();
  }
}


boolean isRegistered(String tag) {
  String regTag;
  char c;
  int len = 0;
  File  dataFile = SD.open("db.txt");
  if (dataFile) {
    while (dataFile.available()) {
      if (len == 12) {
        if (regTag.equals(tag)) {
          dataFile.close();
          return true;
          break;
        }
        regTag = "";
        len = 0;
      }
      else {
        c = dataFile.read();
        regTag += c;
        len++;
      }
    }
    return false;
  }
}

boolean Attended(){
  String regTag;
  char c;
  int len = 0;
  File  dataFile = SD.open("atd.txt");
  if (dataFile) {
    while (dataFile.available()) {
      if (len == 12) {
        ;
        if (regTag.equals(tag)) {
          dataFile.close();
          return true;
          break;
        }
        regTag = "";
        len = 0;
      }
      else {
        c = dataFile.read();
        regTag += c;
        len++;
      }
    }
    return false;
  }
}





