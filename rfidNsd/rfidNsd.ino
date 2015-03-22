#include <SoftwareSerial.h>
#include <SD.h>

#define admin "0200736D928E" 
#define new_entry "0200736D978B"

SoftwareSerial rfid = SoftwareSerial(6, 8);
char c;
String tag, command;

void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  pinMode(4, OUTPUT);

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  Serial.println("Serial Ready");

  rfid.begin(9600);
  Serial.println("RFID Ready");
}

void loop() {

  if (Serial.available()) {
    command = Serial.readString();
    if (command == "dump\n" || command == "dump") {
      Serial.println(command);
      dumpFile();
    }
  }

  while (rfid.available() > 0) {
    c = rfid.read();
    tag += c;
  }
  tag = tag.substring(1, 13);

  if (tag.length() > 10) {
    Serial.println(tag);
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

void dumpFile() {
  File dataFile = SD.open("db.txt");
  if (dataFile) {
    while (dataFile.available()) {
      //Serial.write(dataFile.read());
      String line=dataFile.readString();
      Serial.println(line);
    }
    Serial.println("Dumping done!");
    dataFile.close();
  } 
  else
    Serial.println("error opening db.txt");

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
  File dataFile=SD.open("db.txt",FILE_WRITE);
  if(dataFile){
    Serial.println(tag);
    dataFile.println(tag);
    dataFile.close();
  }
  Serial.println("Registry Complete");

}



