#define r(x) analogRead(x)

int l0=0;
int l1=1;
int l2=2;
int l3=3;
int l4=4;
int l5=5;
int b0,b1,b2,b3,b4,b5;

char decodeIT(int a,int b,int c,int d,int e){
  int sum=0;
  sum+=a*16+b*8+c*4+d*2+e*1;
  return char(sum+'a'-1);
}

 void setup() {
   pinMode(l0, INPUT);
   pinMode(l1,INPUT);
   pinMode(l2, INPUT);
   pinMode(l3,INPUT);
   pinMode(l4, INPUT);
   pinMode(l5,INPUT);
   
   Serial.begin(9600);
 }
 
 void loop() {
     b5 = r(l5)>10;
     b4 = r(l4)>10;
     b3 = r(l3)>15;
     b2 = r(l2)>10;
     b1 = r(l1)>10;
     b0 = r(l0)>12;
   if(b4!=0 || b3!=0 || b2!=0 || b1!=0 || b0!=0)
   {
     char value=decodeIT(b4,b3,b2,b1,b0);
     if(value=='{') value=' ';
     Serial.print(value);
   }
    delay(78);
 }

