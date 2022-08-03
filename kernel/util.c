#include "./util.h"
void itoa(int n ,char str[]){
    int i ,sign;
    sign = n;
    if(sign<0){
        sign = -sign;
        n =-n;
    }
    i=0;
    do{
        str[i++] = n%10+'0';
    }while((n/=10) >0);
    if(sign<0)
        str[i++]='-';
    str[i]='\0';

    //reverse string
    
    i =0;
    char c;
    int j = _strlen(str)-1;
    while(i<j){        
        c = str[j];
        str[j] = str[i];
        str[i]= c;
        i++;
        j--;
    }
    

}
int atoi(char *str){
    int res = 0;

    

    for(int i =0;i<_strlen(str);i++){
        res = res*10+str[i]-'0';
    }
    return res;

}
