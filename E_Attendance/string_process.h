char c;
String tag = "", command,mode,date="";
String course[5];

void processTag(){
  tag+='c';
  int i;
  for(i=tag.length()-1;i>8;i--){
    tag[i]=tag[i-1];
  }
  tag[8]='.';
}
void toChar(char result[],String value){
  value.toCharArray(result,13);
} 
