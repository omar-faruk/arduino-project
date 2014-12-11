

int time=0,i;

void setup() {
  pinMode(6,OUTPUT);
  Serial.begin(9600);
}

void loop(){
   
  if(Serial.available()) {
    time=time*10+Serial.read();
    Serial.write(time);
  }
  else{
    for(i=1;i<3;i++){
      digitalWrite(6,HIGH);
      delay(50);
      digitalWrite(6,LOW);
      delay(50);
    }
    time=0;
  }
}



