
//pwm control


int E1 = 5;  
int M1 = 4; 
int E2 = 6;                      
int M2 = 7;                        

String ctrl_msg;
int value1,value2,start_flag,target;

void setup() 
{ 
    Serial.begin(9600);
    Serial1.begin(9600);
    pinMode(M1, OUTPUT);   
    pinMode(M2, OUTPUT); 
    value1=value2=30;
    start_flag=0;
    target=60;
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
    start_flag=1;
    int i;
    
    if(start_flag==1){
      target+=10;
      Serial.println("speed up");
    }
    
    for(i=0;i<=target;i++){
      digitalWrite(M1,HIGH);   
      digitalWrite(M2, HIGH);       
      analogWrite(E1, value1);   //PWM Speed Control
      analogWrite(E2, value2);   //PWM Speed Control 
      value1=value2=i;
    }
    Serial.println(value1);
  }
  
  if(ctrl_msg=="stop"){
    start_flag=0;
    digitalWrite(M1,HIGH);   
    digitalWrite(M2, HIGH);       
    
    for(int i=value1,j=value2;i>=0,j>=0;i--,j--){
      analogWrite(E1, 1);   //PWM Speed Control
      analogWrite(E2, j);   //PWM Speed Control 
    }
    Serial.println("stopped");
    value1=0;
    value2=0;
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
