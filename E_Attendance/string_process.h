String tag = "", command,mode,date="";
String course[6];

void processTag(){
  tag+='c';
  int i;
  for(i=tag.length()-1;i>8;i--){
    tag[i]=tag[i-1];
  }
  tag[8]='.';
}
void toChar(char result[],String value){
  int i,n=value.length();
  for(int i=0;i<n;i++){
	  result[i]=value[i];
  }
  result[n]=0;
} 

boolean isNum(char c){
	if(c=='0') return 1;
	if(c=='1') return 1;
	if(c=='2') return 1;
	if(c=='3') return 1;
	if(c=='4') return 1;
	if(c=='5') return 1;
	if(c=='6') return 1;
	if(c=='7') return 1;
	if(c=='8') return 1;
	if(c=='9') return 1;
	else return 0;
}
