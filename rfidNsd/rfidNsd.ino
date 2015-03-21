#include <SoftwareSerial.h>
#include <SD.h>

#define admin "0200736D928E" 
#define listAll "0200736D978B"

SoftwareSerial rfid = SoftwareSerial(6, 8);

File root;

void setup()  
{
  Serial.begin(9600);
  Serial.println("Serial Ready");

  rfid.begin(9600);
  Serial.println("RFID Ready");
}

void loop(){
   
  char c; 
  String msg,command;
 
  if(Serial.available()){
    command=Serial.readString();
    if(command=="files\n"){
        Serial.println(command);
      listAllFiles();
    }
  }
  
  while(rfid.available()>0){
    c=rfid.read(); 
    msg += c;
  }
  msg=msg.substring(1,13);
  
  if(msg.length()>10){
    Serial.println(msg);
    if(msg==admin){
      Serial.println("Admin Access:");
    }
    else if(msg==listAll){
    
    }
  }
  msg=""; 
}
void listAllFiles()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(4, OUTPUT);

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  root = SD.open("/");
  
  printDirectory(root, 0);
  
  Serial.println("done!");
}


void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}



