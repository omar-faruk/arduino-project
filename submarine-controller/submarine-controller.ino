#include<Servo.h>
Servo servo1,servo2,servo3,esc;
int servo1_angle=90,servo2_angle=90,servo3_angle=90;
int propeller_flag=0;


boolean Direction;
int duration;


String c;
char cd;

int sv=0,sv_flag=0,pmp=0,pmp_flag=0;
int ssv=0,ssv_flag=0,spmp=0,spmp_flag=0;

int bldc_speed;

void setSpeed(int speed){
  int angle = map(speed, 0, 100, 0, 180);
  esc.write(angle);
}

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);  //this serial for bluetooth communication
  
  //PWM pins from 2-13 & 44-46
  servo1.attach(8);
  servo2.attach(9);
  
  servo3.attach(10); //this servo is for radar control
  
  esc.attach(11); //BLDC attached to pin 11 for speed control
  bldc_speed=10;
  setSpeed(bldc_speed);
  
  servo1.write(90);
  servo2.write(90);
  
  //this servo is for radar
  servo3.write(90);
    
    
  //2 pins for 2 solenoid valves
  pinMode(22,OUTPUT);
  pinMode(24,OUTPUT);
  
  digitalWrite(22,HIGH);
  digitalWrite(24,HIGH);
  
  //4 pins for 4 pumps  
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(26,OUTPUT);
  
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(26,HIGH);  
  
  //set pin 11 to ouput mode for controlling bldc
  pinMode(11,OUTPUT);
}


void loop(){  
  
  if(Serial.available()) c=Serial.readString();
  
  if(Serial1.available()){
    c=Serial1.readString();
    Serial.println(c);
  }
  
  if(c=="propeller"){
    propeller_flag=(!propeller_flag);
    Serial.println("propeller state"+propeller_flag);
  }
  
  
  if(c=="rdrm"){
    servo3_angle-=5;
    servo3.write(servo3_angle);    
  }
  if(c=="rdrp"){
    servo3_angle+=5;
    servo3.write(servo3_angle);
  }
  
  
    //two servo control for sail/stern plane
  if(c=="sp1"){
    servo1_angle-=10;
    servo2_angle+=10;
    if(servo1_angle<0) servo1_angle=0;
    if(servo2_angle>180) servo2_angle=180;
    servo1.write(servo1_angle);
    servo2.write(servo2_angle);
    
  }
    //two servo control for sail/stern plane
  if(c=="sm1"){
    servo1_angle+=10;
    servo2_angle-=10;
    if(servo1_angle>180) servo1_angle=180;
    if(servo2_angle<0) servo2_angle=0;
    servo1.write(servo1_angle);
    servo2.write(servo2_angle);
  }
 
  //this servo controls the radar to move left or right
  if(c=="rdrp"){
    servo3_angle+=10;
    if(servo3_angle>180) servo3_angle=180;
    servo3.write(servo3_angle);
  }
  
  if(c=="rdrm"){
    servo3_angle-=10;
    if(servo3_angle<0) servo3_angle=0;
    servo3.write(servo3_angle);
  }
 
  
  //sv controls solenoid valve, pmp controls the pumps
  if(c=="sv" || c=="sv\n"){
    if(!propeller_flag){
      sv=(!sv);
      sv_flag=1;
    }
    else{
      bldc_speed-=1;
      if(bldc_speed<10){
        bldc_speed=10;
      }
      setSpeed(bldc_speed);
      delay(500);
      Serial.println(bldc_speed);
    }
  }

  
  
  if(c=="pmp" || c=="pmp\n"){
    if(!propeller_flag){
      pmp=(!pmp);
      pmp_flag=1; 
    }
    else{
      bldc_speed+=1;
      if(bldc_speed>100){
        bldc_speed=100;
      }
      setSpeed(bldc_speed);
      delay(500);
      Serial.println(bldc_speed);
    }
  }
  
  if(c=="spmp" || c=="spmp\n"){
    spmp=(!spmp);
    spmp_flag=1;
  }

  if(sv && sv_flag){
      digitalWrite(22,HIGH);
      digitalWrite(24,HIGH); 
      Serial.println("solenoid valve "+sv);
  }
  
  
  if(!sv & sv_flag) {
    digitalWrite(22,LOW);
    digitalWrite(24,LOW);
    Serial.println("solenoid valve "+sv);
  }
  
  
  
  if(pmp && pmp_flag){
      digitalWrite(5,HIGH);
      digitalWrite(6,HIGH);
      Serial.println("pump "+pmp);
  }
  
  else if(!pmp & pmp_flag) {
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    Serial.println("pump "+pmp);
  }
  
  if(spmp && spmp_flag){
      digitalWrite(7,HIGH);
      digitalWrite(26,HIGH);
  }
  
  else if(!spmp && spmp_flag){
      digitalWrite(7,LOW);
      digitalWrite(26,LOW);
  }
  
  sv_flag=0;
  pmp_flag=0;
  ssv_flag=0;
  spmp_flag=0;
  c="";
}
