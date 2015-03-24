#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <SD.h>

#define admin "0200736D928E" 
#define new_entry "0200736D978B"

LiquidCrystal lcd(8,7,6,5,3,2);
SoftwareSerial rfid = SoftwareSerial(9, 13);
char c;
String tag, command;

void setup() {
  Serial.begin(9600);
  rfid.begin(9600);
  pinMode(4, OUTPUT);

  if (!SD.begin(4)) {
    return;
  }
  lcd.begin(20, 4);
  lcd.setCursor(0,0);
}

void loop() {

  if (Serial.available()) {
    command = Serial.readString();
    if (command == "dump\n" || command == "dump") {
      dumpFile();
    }
  }
  if(command=="attendance-mode"){
    
  }

  while (rfid.available() > 0) {
    c = rfid.read();
    tag += c;
  }
  tag = tag.substring(1, 13);

  if (tag.length() > 10) {
    if (tag == admin) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.println("attendance:");
    }
    if (tag == new_entry) {
      newEntry();
    }
  }
  tag = "";
  delay(50);
}

void newEntry() {
  delay(1000);
  tag="";
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("Ready to entry");
  rfid.flush();
  while(tag.length()<13){
    while (rfid.available() > 0) {
      c = rfid.read();
      tag += c;
    }
  }
  tag = tag.substring(1, 13);
  if(!isRegistered(tag)){
    File dataFile=SD.open("db.txt",FILE_WRITE);
    if(dataFile){
      dataFile.println(tag);
      dataFile.close();
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("Registration Complete");
  }
}

void dumpFile() {
  File dataFile=SD.open("db.txt");
  if (dataFile) {
    while (dataFile.available()) {
      String line=dataFile.readString();
      Serial.println(line);
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("Dumping done!");
    dataFile.close();
  } 
}


boolean isRegistered(String tag){
  String regTag;
  char c;
  int len=0;
  File dataFile=SD.open("db.txt");

  if(dataFile){
    while(dataFile.available()){
      if(len==12){
        if(regTag.equals(tag)){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.println("Registered");
          return true;
          break;
        }
        regTag="";
        len=0;
      }
      else {
        c=dataFile.read();
        regTag+=c;
        len++;
      }
    }
    return false;
  }
}

