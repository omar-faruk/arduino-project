#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <SD.h>

LiquidCrystal lcd(10, 7, 6, 5, 3, 2);
SoftwareSerial rfid = SoftwareSerial(9, 13);
char c;
String tag = "", command, mode="";

void setup() {
  Serial.begin(9600);
  rfid.begin(9600);
  pinMode(8, OUTPUT); //set it to 4 when using ether+sd else set to 8

  if (!SD.begin(8)) { //chip select pin in sd shield
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
      SD.remove("mode.txt");
      SD.open("mode",FILE_WRITE);
      File modeSelector=SD.open("mode.txt",FILE_WRITE);
      if(modeSelector){
        modeSelector.print("atmode");
        modeSelector.close();
      }
      lcd.clear();
      lcd.print("Attendance Mode");
    }
    if (command == "registration-mode" || command == "registration-mode\n") {
      mode = "regmode";
       SD.remove("mode.txt");
      SD.open("mode",FILE_WRITE);
      File modeSelector=SD.open("mode.txt",FILE_WRITE);
      if(modeSelector){
        modeSelector.print("regmode");
        modeSelector.close();
      }
      lcd.clear();
      lcd.print("Registration Mode");
    }
    if (command == "deregistration-mode" || command == "deregistration-mode\n") {
      mode = "dregmode";
      SD.remove("mode.txt");
      SD.open("mode",FILE_WRITE);
      File modeSelector=SD.open("mode.txt");
      if(modeSelector){
        modeSelector.print("dregmode");
        modeSelector.close();
      }
      lcd.clear();
      lcd.print("De-Registration Mode");
    }
    if(command=="cleardb" || command=="cleardb\n"){
      SD.remove("db.txt");
      lcd.clear();
      lcd.print("Database Cleared");
    }
  }
    if(SD.exists("mode.txt") && mode==""){
    File modeSelector=SD.open("mode.txt");
    if(modeSelector){
      mode=modeSelector.readString();
      modeSelector.close();
    }
    lcd.clear();
    if(mode == "atmode" || mode == "atmode\n") {
      mode = "atmode";
      lcd.print("Attendance Mode");
    }
    else if (mode == "regmode" || mode == "regmode\n") {
      mode = "regmode";
      lcd.print("Registration Mode");
    }
    else if (mode == "dregmode" || mode == "dregmode\n") {
      mode = "dregmode";
      lcd.print("De-Registration Mode");
    }
  }
  
  while (rfid.available() > 0) {
    c = rfid.read();
    tag += c;
  }
  tag = tag.substring(1, 13);
  if (tag.length() > 10) {
    Serial.println(tag);
    if (mode == "regmode" || mode == "atmode") {
      newEntry();
    }
    if (mode == "dregmode") {
      deRegistration(tag);
    }
  }
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
        lcd.print("Registration Complete");
      }
    }
    else if (isRegistered(tag)) {
      lcd.clear();
      lcd.print("Registered User");
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
          lcd.print("Attendance Complete");
        }
      }
      else if (Attended()) {
        lcd.clear();
        lcd.print("Already Taken");
      }
    }
    else if (!isRegistered(tag)) {
      lcd.clear();
      lcd.print("User Not Registered");
    }
  }
  tag = "";
  delay(1400);
  lcd.clear();
  if (mode == "atmode") {
    lcd.print("Attendance Mode");
  }
  if (mode == "regmode") {
    lcd.print("Registration Mode");
  }

}

void dumpFile(char *fileName) {
  
  if(SD.exists(fileName)){
    File dataFile = SD.open(fileName);
    if (dataFile) {
      while (dataFile.available()) {
        String line = dataFile.readString();
        Serial.println(line);
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Dumping done!");
      dataFile.close();
    }
  }
  else {
    lcd.clear();
    lcd.print("File Not Exists");
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

void deRegistration(String tag) {
  File dataFile=SD.open("db.txt");
  String data,d1,d2;
  int index;

  if(dataFile){
    data=dataFile.readString();
    dataFile.close();
  }
  index=data.indexOf(tag);
  if(index>=0){
    d1=data.substring(0,index);
    d2=data.substring(data.indexOf(tag)+12,data.length());
    SD.remove("db.txt");
    dataFile=SD.open("db.txt",FILE_WRITE);
    if(dataFile){
      dataFile.print(d1+d2);
      dataFile.close();
    }
    lcd.clear();
    lcd.print(tag+" deRegistered");
  }
  else{
    lcd.clear();
    lcd.print("User not Found");
  }
  delay(1500);
  lcd.clear();
  lcd.print("De-Registration Mode");
}



