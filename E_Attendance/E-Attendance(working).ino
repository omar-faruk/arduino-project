#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <SdFat.h>
#include "string_process.h"


LiquidCrystal lcd(10, 7, 6, 5, 3, 2); //set 1st pin to 8 if ethernet shield else set to 10
SoftwareSerial rfid = SoftwareSerial(9, 13);
int button_data,course_count,dateConfirmed;
SdFat SD;

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
	if(date.length()<3 && SD.exists("date.txt")){
		File dateFile=SD.open("date.txt");
		if(dateFile){
		char cc;
		int len=8;
			while(dateFile.available() && len>=1){
				cc=dateFile.read();
				date+=cc;
				len--;
			}
		dateFile.close();
		}
		dateFile.close();
		lcd.clear();
		lcd.print("Today: ");
		lcd.print(date);
	}
}


void loop() {
	if (Serial.available()) {
		command = Serial.readString();
		lcd.clear();
		if (command == "dump-atd\n" || command == "dump-atd") {
			dumpFile("atd.txt");
			SD.remove("atd.txt");
		}
		
		if(dateConfirmed==0){
			/*/This piece of block makes every thing bullshit...... grrrrr....
			 if(command[0]=='d' && command[1]=='t'){
				String dt;
				int i=2;
				for(i=2;i<command.length();i++){
					dt+=command[i];
				}
				if(SD.exists("date.txt")){
					SD.remove("date.txt");
				}
				SdFile setDate;
				if(setDate.open("date.txt",O_RDWR | O_CREAT | O_AT_END)){
					lcd.print(dt);
					setDate.print(dt);
					setDate.close();
				}
				setDate.close();
			}
			*///
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
				dataFile.print("");
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
		if (isRegistered(dirTag)){
			if(!Attended(dirTag)){
				SdFile uid;
				if(uid.open(charTag,O_RDWR | O_CREAT | O_AT_END)){
					uid.println(date);
					uid.close();
					lcd.clear();
					lcd.print("Attendance Complete");
				}
				else{
					Serial.println("Cannot open File!!");
				}
				uid.close();
				
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


boolean isRegistered(String ptag) {
	SD.chdir("/");
    char *dir=(char*)malloc(course[course_count-1].length());
    toChar(dir,course[course_count-1]);
    SD.chdir(dir);
	char *charTag=(char*) malloc(ptag.length());
	toChar(charTag,ptag);
	Serial.println("Checking existance "+ptag);
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

boolean Attended(String dirTag) {
	
	char *charTag=(char*) malloc(dirTag.length());
	toChar(charTag,dirTag);
	String atDate;
	char cdt;
	boolean flag=0;
	SdFile oid;
	
	if(oid.open(charTag,O_READ)){
		while(oid.available()){
			cdt=oid.read();
			if(isNum(cdt)){
				atDate+=cdt;
			}
			if(atDate.length()==8){
				Serial.println(atDate);
				if(atDate==date){
					flag=1;
					atDate="";
					break;
				}
				atDate="";
			}
		}
		oid.close();
	}
	else{
		Serial.println("Failed to open File");
		return 1;
	}
	oid.close();
	free(charTag);
	Serial.print("Returninng at Flag ");
	Serial.println(flag);
	return flag;
}

void deRegistration(String ptag) {
	SD.chdir("/");
	char *charTag=(char*) malloc(ptag.length());
	toChar(charTag,ptag);
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
