#include <SoftwareSerial.h>
#include <SD.h>

#define admin "0200736D928E" 
#define new_entry "0200736D978B"

SoftwareSerial rfid = SoftwareSerial(6, 8);
char c;
String tag, command;

void setup() {
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  if (!SD.begin(4)) {
    return;
  }
  rfid.begin(9600);
}

void loop() {

  if (Serial.available()) {
    command = Serial.readString();
    if (command == "dump\n" || command == "dump") {
      //Serial.println(command);
      dumpFile();
    }
  }

  while (rfid.available() > 0) {
    c = rfid.read();
    tag += c;
  }
  tag = tag.substring(1, 13);

  if (tag.length() > 10) {
    //Serial.println(tag);
    if (tag == admin) {
      Serial.println("attendence:");
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
  Serial.println("Ready to entry");
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
      Serial.println(tag);
      dataFile.println(tag);
      dataFile.close();
    }
    Serial.println("Registry Complete");
  }
}

void dumpFile() {
  File dataFile=SD.open("db.txt");
  if (dataFile) {
    while (dataFile.available()) {
      //Serial.write(dataFile.read());
      String line=dataFile.readString();
      Serial.println(line);
    }
    //Serial.println("Dumping done!");
    dataFile.close();
  } 
  //else Serial.println("error opening db.txt");

}


boolean isRegistered(String tag){

  String regTag;
  char c;
  int len=0;
  File dataFile=SD.open("db.txt");

  if(dataFile){
    while(dataFile.available()){
      if(len==12){
        Serial.println(regTag);
        if(regTag.equals(tag)){
          Serial.println("Registered");
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




