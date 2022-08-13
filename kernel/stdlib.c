#include "./stdlib.h"
#include "./paging.h"
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
uint32_t _memcmp(void* b1,void *b2 , int n ){

    uint32_t  *str1 = (uint32_t *)b1;
    uint32_t  *str2 = (uint32_t *)b2;
    uint32_t  res =0 ;
    for(int i =0;i<n;i++){
            if(str1[i] == str2[i]){
                    res = str1-str2;
            }
            if(str1[i] > str2[i]){
                    res = str1-str2;
            }
            if(str1[i] < str2[i]){
                    res = str1-str2;
            }
    }
    return res;
}



uint32_t strtol(char *buf){
    uint32_t res;

    while(*buf){
        uint8_t byte = *buf++;
        if(byte >='0' &&  byte<='9')
        {
            byte=byte-'0';
        }
        else if(byte>='a' && byte<='f'){
            byte = byte-'a'+10;
        }
        else if(byte>='A' && byte<='F'){
            byte = byte-'A'+10;
        }
        res = (res<<4) | ( byte &0xF);
    }
    return res;
}
void itoh(char *buff , unsigned int num ,int base){
    char Representation[]= "0123456789ABCDEF";
    char buffer[250];
    int i = 250-1;
    while(num != 0 ){
        buffer[i--] = Representation[num%base];
        num/=base;
    }
    int count = 0;
    for(int j = i+1;j<=250-1;j++){
        buff[count++] = buffer[j];
    }
    buff[count] ='\0';
}
int atoi(char *str){
    int res = 0;
    for(int i =0;i<_strlen(str);i++){
        res = res*10+str[i]-'0';
    }
    return res;

}
