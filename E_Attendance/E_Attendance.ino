#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <SdFat.h>
#include "string_process.h"


LiquidCrystal lcd(10, 7, 6, 5, 3, 2); //set 1st pin to 8 if ethernet shield else set to 10
SoftwareSerial rfid = SoftwareSerial(9, 13);
int button_data,course_count,dateConfirmed;
SdFat SD;
SdFile file;

void setup() {
	Serial.begin(9600);
	rfid.begin(9600);
	course_count=0;
	dateConfirmed=0;
	pinMode(8, OUTPUT); //output pin mode 8 on sd shield else pin 4
	if (!SD.begin(8)) {  //chip select on pin 8 in sd shield else pin 4
		return;
	}
	initCourseList();
	lcd.begin(20, 4);
	lcd.setCursor(0, 0);
	if(date.length()<7 && SD.exists("date.txt")){
		if(file.open("date.txt",O_READ)){
		char cc;
		int len=8;
		date="";
			while(file.available() && len>=1){
				cc=file.read();
				date+=cc;
				len--;
			}
		file.close();
		}
		file.close();
		lcd.clear();
		lcd.print("Today: ");
		lcd.print(date);
		Serial.println(date+" "+date.length());
	}
}


void loop() {
	
	if (Serial.available()>0){
		command = Serial.readString();
		if (command[0]=='d' && command[1]=='u' && command[2]=='m' && command[3]=='p'){
			dumpFile(course[course_count-1]);
			//SD.remove("atd.txt");
		}
		if(dateConfirmed==0){
			  
			if(command[0]=='d' && command[1]=='t'){
				char *charDate=(char*)malloc(8);
				int i=2;
				for(i=2;i<command.length();i++){
					charDate[i-2]=command[i];
				}
				SD.chdir("/");
				if(SD.exists("date.txt")){
					SD.remove("date.txt");
				}
				if(file.open("date.txt",O_RDWR | O_CREAT | O_AT_END)){
					lcd.clear();
					lcd.print(charDate);
					file.println(charDate);
					file.close();
					Serial.println(charDate);
					free(charDate);
				}
			}
		
			dateConfirmed=1;
			command="";
		}
		
	}
	button_data=analogRead(3);
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
	else if(button_data>=450 && button_data<=550){
		mode="regmode";
		lcd.clear();
		lcd.print("Registration Mode");
	}
	else if(button_data>=600 && button_data<=700){
		if(course_count==5){
			course_count=0;
		}
		mode="";
		lcd.clear();
		lcd.print("Course: CSE-"+course[course_count]);
		course_count++;
	}
	else if(button_data>800 && button_data<900){
		Serial.println("Start dumping: "+course[course_count-1]);
		dumpFile(course[course_count-1]);
		
	}
	
	char ct;
	while (rfid.available() > 0) {
		ct = rfid.read();
		tag += ct;
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
	String dirTag=tag;
	
	 SD.chdir("/");
	char *dir=(char*)malloc(course[course_count-1].length());
    toChar(dir,course[course_count-1]);
    SD.chdir(dir);
    
	char *charTag=(char*) malloc(dirTag.length());
	toChar(charTag,dirTag);
	Serial.println(dir);
	if (mode == "regmode") {
		if (!isRegistered(dirTag)) {
			if (file.open(charTag,O_RDWR | O_CREAT | O_AT_END)) {
				file.print("");
				Serial.println(charTag);
				file.close();
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
		if (isRegistered(dirTag)){
			if(!Attended(dirTag)){
				if(file.open(charTag,O_RDWR | O_CREAT | O_AT_END)){
					file.println(date);
					file.close();
					lcd.clear();
					lcd.print("Attendance Complete");
				}
				else{
					Serial.println("Cannot open File!!");
				}
				file.close();
				
			}
			else if (Attended(dirTag)) {
				lcd.clear();
				lcd.print("Already Taken");
			}
		}
		else if (!isRegistered(dirTag)) {
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

void dumpFile(String directory) {
	SD.chdir("/");
    char *dir=(char*)malloc(directory.length());
    toChar(dir,directory);
    SD.chdir(dir);
    char dts;
    lcd.clear();
    lcd.print("Dumping Course: ");
    lcd.print(dir);
    while (file.openNext(SD.vwd(), O_READ)) {
		file.printName(&Serial);
		Serial.println("");
		while(file.available()){
			dts=file.read();
			Serial.print(dts);
		}
		file.close();
	}
}


boolean isRegistered(String ptag) {
	
	 SD.chdir("/");
    char *dir=(char*)malloc(course[course_count-1].length());
    toChar(dir,course[course_count-1]);
    SD.chdir(dir);
    
    char *charTag=(char*) malloc(ptag.length());
	toChar(charTag,ptag);
	Serial.print("Checking existance "+ptag);
	if(SD.exists(charTag)){
		Serial.println(".... registered");
		free(charTag);
		free(dir);
		return true;
	}
	else {
		Serial.println(".... not registered");
		free(charTag);
		free(dir);
		return false;
	}
}

boolean Attended(String dirTag) {
	
	char *charTag=(char*) malloc(dirTag.length());
	toChar(charTag,dirTag);
	String atDate;
	char cdt;
	boolean flag=0;
	if(file.open(charTag,O_READ)){
		while(file.available()){
			cdt=file.read();
			if(isNum(cdt)){
				atDate+=cdt;
			}
			if(atDate.length()==8){
				Serial.println(atDate+" "+date);
				if(atDate==date){
					flag=1;
					atDate="";
					break;
				}
				atDate="";
			}
		}
		file.close();
	}
	else{
		Serial.println("Failed to open File");
		return 1;
	}
	file.close();
	free(charTag);
	Serial.print("Returninng at Flag ");
	Serial.println(flag);
	return flag;
}

void deRegistration(String ptag) {
	
	char *charTag=(char*) malloc(ptag.length());
	toChar(charTag,ptag);
	
	
	SD.chdir("/");
	char *dir=(char*)malloc(course[course_count-1].length());
    toChar(dir,course[course_count-1]);
    SD.chdir(dir);
	
	
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
	char c;
	if(file.open("courses.lst",O_READ)){
		course_count=0;
		while(file.available()){
			c=file.read();
			if(c!=' '){
				course[course_count]+=c;
			}
			else{
				course_count++;
			}
		}
	}
	file.close();
}
