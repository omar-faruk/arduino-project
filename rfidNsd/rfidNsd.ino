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
  if (!isRegistered(tag)) {
    if (mode == "regmode") {
      dataFile = SD.open("db.txt", FILE_WRITE);
    }
    if (mode == "atmode") {
      dataFile = SD.open("attendance.txt", FILE_WRITE);
    }
    if (dataFile) {
      dataFile.println(tag);
      dataFile.close();
    }
    lcd.clear();
    if (mode == "regmode") {
      lcd.println("Registration Complete");
    }
    if (mode == "atmode") {
      lcd.println("Attendance Complete");
    }
  }

  tag = "";
  delay(2000);
  //lcd.clear();
  if (mode == "atmode") {
    //lcd.println("Attendance Mode");
  }
  if (mode == "regmode") {
   // lcd.println("Registration Mode");
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
  File dataFile;
  if (mode == "regmode") {
    dataFile = SD.open("db.txt");
  }
  if (mode == "atmode") {
    dataFile = SD.open("attendance.txt");
  }
  if (dataFile) {
    while (dataFile.available()) {
      if (len == 12) {
        Serial.println(tag);
        if (regTag.equals(tag)) {
          lcd.clear();
          lcd.setCursor(1, 1);
          if (mode == "atmode") {
            lcd.println("Attendance Completed");
          }
          if (mode == "regmode") {
            lcd.println("Registered");
          }
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

