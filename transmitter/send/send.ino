int time=0,i;
void setup() {
  pinMode(6,OUTPUT);
  Serial.begin(9600);
}

void loop(){
  i=0;
  if(Serial.available()) {
    String s=Serial.readString();
    Serial.println(s);
  }
  for(i=1;i<=time;i++){
    digitalWrite(6,1);
    delay(150);
    digitalWrite(6,0);
    delay(100);
  }
  time=0;
}




