#include <Servo.h> 
#include <IRremote.h>

Servo myservo,servo2;
//Servo servo2;
int pos,j;
IRrecv irrecv(6);
String button;
decode_results results;
char c;
void setup() 
{ 
  Serial.begin(9600);
  irrecv.enableIRIn();
  myservo.attach(9);
  servo2.attach(10);
  pos=1;
  myservo.write(0);  // set servo to mid-point
  servo2.write(60);
} 
void loop(){
  if(irrecv.decode(&results)){
    button=String(results.value,HEX);
          Serial.println(button);
	if(button=="f0771735" || button=="40bd08f7"){
		Serial.println(button);
		for(j=pos;j<=pos+10;j++){
				myservo.write(j);
			}
			if(pos<=170){
				pos+=10;
			}
			
		}
	else if(button=="40bd8877" || button=="80eb69e1"){
		Serial.println(button);
		for(j=pos;j>=pos-10;j--){
				myservo.write(j);
			}
			if(pos>=10){
				pos-=10;
			}
		}
        else if(button=="40bdc837" || button=="f93dd945"){
          clock();
          Serial.println(button);
        }
        else if(button=="aa01a849" || button=="40bd48b7"){
          counterClock();
          Serial.println(button);
        }
        else if (button=="40bda857" || button=="40bda857"){
          midServo();
        }
	irrecv.resume();
	delay(500);
	}
 }
 
void counterClock(){
  int i;
  
  for(i=46;i<=125;i++){
    servo2.write(i);
    delay(10);
  }
}

void midServo(){
  int i;
  
  for(i=46;i<=100;i++){
    servo2.write(i);
    delay(10);
  }
}

void clock(){
  int i;
  for(i=125;i>45;i--){
    servo2.write(i);
    delay(10);
  }
}

