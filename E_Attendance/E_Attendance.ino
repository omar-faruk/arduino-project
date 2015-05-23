#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <SD.h>

LiquidCrystal lcd(8, 7, 6, 5, 3, 2); //set 1st pin to 8 if ethernet shield else set to 10
SoftwareSerial rfid = SoftwareSerial(9, 13);
char c;
String tag = "", command, mode="";

void setup() {
  Serial.begin(9600);
  rfid.begin(9600);
  pinMode(4, OUTPUT); //output pin mode 8 on sd shield else pin 4

  if (!SD.begin(4)) {  //chip select on pin 8 in sd shield else pin 4
    return;
  }
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
}

void processTag(){
  tag+='c';
  int i;
  for(i=tag.length()-1;i>7;i--){
    tag[i]=tag[i-1];
  }
  tag[8]='.';
}

void toChar(char result[],String value){
  value.toCharArray(result,13);
  Serial.println(result);
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
  String temp= tag.substring(2, 13);
  tag=temp;
  if (tag.length() > 10) {
    processTag();
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
  char *charTag=(char*) malloc(tag.length());
  toChar(charTag,tag);
  Serial.println(charTag);
  if (mode == "regmode") {
    if (!isRegistered(tag)) {
      dataFile = SD.open(charTag, FILE_WRITE);
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
  free(charTag);
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
  char *charTag=(char*) malloc(tag.length());
  toChar(charTag,tag);
  Serial.println(charTag);
  if(SD.exists(charTag)){
    free(charTag);
    return true;
  }
  else {
    free(charTag);
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
          if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || c=='.') {
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
  char *charTag=(char*) malloc(tag.length());
  toChar(charTag,tag);

  Serial.println(charTag);

  if(SD.exists(charTag)){
    SD.remove(charTag);
    lcd.clear();
    lcd.println("User de-registered");
    free(charTag);
  }
  else{
    free(charTag);
    lcd.clear();
    lcd.print("User not Found");
  }
  delay(1500);
  lcd.clear();
  lcd.print("De-Registration Mode");
}
