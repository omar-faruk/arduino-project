#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <SD.h>
#include <SPI.h>

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
      dumpFile("db.txt");
    }
    if (command == "dump-atd\n" || command == "dump-atd") {
      dumpFile("atd.txt");
      SD.remove("atd.txt");
    }
    if (command == "attendance-mode" || command == "attendance-mode\n") {
      mode = "atmode";
      lcd.println("Attendance Mode");
    }
    if (command == "registration-mode" || command == "registration-mode\n") {
      mode = "regmode";
      lcd.println("Registration Mode");
    }
    if (command == "deregistration-mode" || command == "deregistration-mode\n") {
      mode = "dregmode";
      lcd.println("De-Registration Mode");
    }
    if(command=="cleardb" || command=="cleardb\n"){
      SD.remove("db.txt");
      lcd.println("Database Cleared");
    }
  }
  while (rfid.available() > 0) {
    c = rfid.read();
    tag += c;
  }
  tag = tag.substring(1, 13);

  if (tag.length() > 10) {
    if (mode == "regmode" || mode == "atmode") {
      newEntry();
    }
    if (mode == "dregmode") {
      deRegistration(tag);
    }
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
        lcd.clear();
        lcd.println("Registration Complete");
      }
    }
    else if (isRegistered(tag)) {
      lcd.clear();
      lcd.println("Registered User");
    }
  }

  else if (mode == "atmode") {
    if (isRegistered(tag)) {
      dataFile = SD.open("atd.txt", FILE_WRITE);
      if (!Attended()) {
        if (dataFile) {
          dataFile.println(tag);
          dataFile.close();
          lcd.clear();
          lcd.println("Attendance Complete");
        }
      }
      else if (Attended()) {
        lcd.clear();
        lcd.println("Already Taken");
      }
    }
    else if (!isRegistered(tag)) {
      lcd.clear();
      lcd.println("User Not Registered");
    }
  }
  tag = "";
  delay(1400);
  lcd.clear();
  if (mode == "atmode") {
    lcd.println("Attendance Mode");
  }
  if (mode == "regmode") {
    lcd.println("Registration Mode");
  }

}

void dumpFile(char *fileName) {
  File dataFile = SD.open(fileName);
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

  if (SD.exists("db.txt")) {
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
          if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')) {
            regTag += c;
            len++;
          }
        }
      }
      dataFile.close();
      return false;
    }
    dataFile.close();
  }
  else {
    return false;
  }
}

boolean Attended() {
  String regTag;
  char c;
  int len = 0;
  if (SD.exists("atd.txt")) {
    File  dataFile = SD.open("atd.txt");
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
          if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')) {
            regTag += c;
            len++;
          }
        }
      }
      dataFile.close();
      return false;
    }
  }
  else return false;
}
//Fix bugs here
void deRegistration(String tag) {
  File dataFile=SD.open("db.txt");
  String data,d1,d2;
  if(dataFile){
    data=dataFile.readString();
  }
  dataFile.close();
  d1=data.substring(0,data.indexOf(tag));
  d2=data.substring(data.indexOf(tag)+12,data.length());
  SD.remove("db.txt");
  dataFile=SD.open("db.txt",FILE_WRITE);
  dataFile.print(d1+d2);
  lcd.clear();
  lcd.println(tag+" deRegistered");
  delay(1500);
  lcd.clear();
  lcd.println("De-Registration Mode");
}