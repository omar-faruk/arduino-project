int time=0,i;

void setup() {

  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  Serial.begin(9600);
}

void loop(){
  int i=0;
  String s;
  if(Serial.available()) {
    
    String s=Serial.readString();
    digitalWrite(5,1);
    
    for(i=0;i<s.length();i+=5)
    { 
      digitalWrite(6,int(s[i+0]-'0'));
      digitalWrite(7,int(s[i+1]-'0'));
      digitalWrite(8,int(s[i+2]-'0'));
      digitalWrite(9,int(s[i+3]-'0'));  
      digitalWrite(10,int(s[i+4]-'0'));
      
      digitalWrite(6,0);
      digitalWrite(7,0);
      digitalWrite(8,0);
      digitalWrite(9,0);
      digitalWrite(10,0);
    }
    digitalWrite(5,0);    
  }
}




