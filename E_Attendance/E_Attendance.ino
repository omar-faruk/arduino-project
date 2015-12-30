#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <SdFat.h>
#include "string_process.h"


LiquidCrystal lcd(10, 7, 6, 5, 3, 2); //set 1st pin to 8 if ethernet shield else set to 10
SoftwareSerial rfid = SoftwareSerial(9, 13);
int button_data,course_count;
SdFat SD;

void setup() {
	Serial.begin(9600);
	rfid.begin(9600);
	course_count=0;
	pinMode(8, OUTPUT); //output pin mode 8 on sd shield else pin 4
	if (!SD.begin(8)) {  //chip select on pin 8 in sd shield else pin 4
		return;
	}
	initCourseList();
	lcd.begin(20, 4);
	lcd.setCursor(0, 0);
	lcd.print("Ready");
}


void loop() {
	if (Serial.available()) {
		command = Serial.readString();
		lcd.clear();
		if (command == "dump-atd\n" || command == "dump-atd") {
			dumpFile("atd.txt");
			SD.remove("atd.txt");
		}
	}
	
	button_data=analogRead(1);

	if(button_data<=80){
		mode="atmode";
		lcd.clear();
		lcd.print("Attendance Mode");
	}
	else if(button_data>=250 && button_data<=400){
		mode="dregmode";
		lcd.clear();
		lcd.print("De-registration Mode");
	}
	else if(button_data>=500 && button_data<=600){
		mode="regmode";
		lcd.clear();
		lcd.print("Registration Mode");
	}
	else if(button_data>=700 && button_data<=800){
		if(course_count==5){
			course_count=0;
		}
		mode="";
		lcd.clear();
		lcd.print("Course: CSE-"+course[course_count]);
		course_count++;
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
	delay(250);
}


void newEntry() {
	File dataFile;
	String dirTag=tag;
	SD.chdir("/");
	char *dir=(char*)malloc(course[course_count-1].length());
    toChar(dir,course[course_count-1]);
    SD.chdir(dir);
	char *charTag=(char*) malloc(dirTag.length());
	toChar(charTag,dirTag);
	Serial.println(course[course_count-1]+" "+dirTag);
	if (mode == "regmode") {
		if (!isRegistered(dirTag)) {
			dataFile = SD.open(charTag, FILE_WRITE);
			if (dataFile) {
				dataFile.println(tag);
				Serial.println(charTag);
				dataFile.close();
				lcd.clear();
				lcd.print("Registration Complete");
			}
		}
		else if (isRegistered(dirTag)) {
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
	free(dir);
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
	SD.chdir("/");
    char *dir=(char*)malloc(course[course_count-1].length());
    toChar(dir,course[course_count-1]);
    SD.chdir(dir);
	char *charTag=(char*) malloc(tag.length());
	toChar(charTag,tag);
	if(SD.exists(charTag)){
		free(charTag);
		free(dir);
		return true;
	}
	else {
		free(charTag);
		free(dir);
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
	SD.chdir("/");
	char *charTag=(char*) malloc(tag.length());
	toChar(charTag,tag);
	char *dir=(char*)malloc(course[course_count-1].length());
    toChar(dir,course[course_count-1]);
    SD.chdir(dir);
	Serial.println(charTag);

	if(SD.exists(charTag)){
		SD.remove(charTag);
		lcd.clear();
		lcd.print("User de-registered");
	}
	else{
		lcd.clear();
		lcd.print("User not Found");
	}
	delay(1800);
	lcd.clear();
	lcd.print("De-Registration Mode");
	free(charTag);
}

void initCourseList(){
	File courseFile=SD.open("courses.lst");
	char c;
	if(courseFile){
		course_count=0;
		while(courseFile.available()){
			c=courseFile.read();
			if(c!=' '){
				course[course_count]+=c;
			}
			else{
				course_count++;
			}
		}
	}
	courseFile.close();
}
