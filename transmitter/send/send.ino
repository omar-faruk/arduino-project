int time=0,i;

void setup() {
  pinMode(6,OUTPUT);
  pinMode(8,OUTPUT);
  Serial.begin(9600);
}

void loop(){
 int i=0;
 String s;
  if(Serial.available()) {
    String s=Serial.readString();
    //Serial.println(s);
  int len=s.length();
  for(i=1;i<len-1;i++){
      Serial.println(int(s[i]-'0'));
      digitalWrite(6,int(s[i]-'0'));
      digitalWrite(8,int(s[i+1]-'0'));
      delay(50);
  }
  }
  
  
}



