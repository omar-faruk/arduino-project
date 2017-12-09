
//pwm control


int E1 = 5;  
int M1 = 4; 
int E2 = 6;                      
int M2 = 7;                        

String ctrl_msg;
int value1,value2,start_flag;

void setup() 
{ 
    Serial.begin(9600);
    Serial1.begin(9600);
    pinMode(M1, OUTPUT);   
    pinMode(M2, OUTPUT); 
    value1=value2=30;
    start_flag=0;
} 

void loop() 
{ 
  if(Serial1.available()){
   ctrl_msg=Serial1.readString(); 
   Serial.println(ctrl_msg);
  }
  
  if(ctrl_msg=="left"){
    value1+=20;
    Serial.println(value1);
  }
  if(ctrl_msg=="right"){
    value2+=20;
    Serial.println(value1);
  }
  
  if(ctrl_msg=="start"){
    Serial.println("motor start");
    for(int i=1;i<30;i++){
      digitalWrite(M1,HIGH);   
      digitalWrite(M2, HIGH);       
      analogWrite(E1, value1);   //PWM Speed Control
      analogWrite(E2, value2);   //PWM Speed Control 
    }
    Serial.println(value1);
  }
  
  if(ctrl_msg=="stop"){
    value2=0;
    value1=0;
    digitalWrite(M1,HIGH);   
    digitalWrite(M2, HIGH);       
    analogWrite(E1, value1);   //PWM Speed Control
    analogWrite(E2, value2);   //PWM Speed Control 
    Serial.println("stopped");
  }
  
  if(value1>255){
    value1=0;
  }
  
  if(value2>255){
    value2=0;
  }
  
  
    digitalWrite(M1,HIGH);   
    digitalWrite(M2, HIGH);       
    analogWrite(E1, value1);   //PWM Speed Control
    analogWrite(E2, value2);   //PWM Speed Control 
    ctrl_msg="";
}
