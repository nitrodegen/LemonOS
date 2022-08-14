#include <stdarg.h>
#include <stdint.h>
#include <stdarg.h>
#include "string.h"
#include "../drivers/keyboard.h"
#include "./stdio.h"
#include "./stdlib.h"
int pos_x= 0;
int pos_y = 0; 
void reset_args(){
    pos_x=0;
    pos_y=0;
}
//gandalf's version of strlen (mine), returns length of const char* obj
int _strlen(const char *str){
    int lol =0;
    while(1){
        if(str[lol] == '\0'){
          break;
        }
        lol++;
    }
    return lol;
}
/*
    Kernel VGA display API
        *it should never be used in final system* 
            just in debugging!
*/

inline void putchar(char c,int fore,int bckg,int x,int y){
    int posx = pos_x;
    int posy = pos_y;

    if(x<0 && y<0){
        posx = pos_x;
        posy = pos_y;
    }
    else{
        posx = x;
        posy=y;
    }
    uint16_t clr = (bckg<<4) | (fore & 0x0F);
    volatile uint16_t* mem=( volatile uint16_t*)0xb8000+(pos_y*80+pos_x);
    *mem=c|(clr<<8);
    if(pos_y>80){
        clearsc();
        pos_x=0;
        pos_y=0;
    }
    if(pos_x>=80){
        pos_x=0;
        pos_y++;
    }
    pos_x++;

}

//log is basic function for logging on console, it is not  to be mixed with "printf"
void log_at(const char*str,int fore,int bckg,int x,int y){
    
    if(x <0 && y <0 && fore <0 && bckg <0){
        int len = _strlen(str);
        for(int i =0;i<len;i++){
            putchar(str[i],0xf,0x0,-1,-1);
        }
        //pos_x++;
        
    }
    else{
        int len = _strlen(str);
        int x_rise=x;
        for(int i =0;i<len;i++){
            putchar(str[i],fore,bckg,x_rise,y);
            x_rise++;
        }
    }
   
}
void log(char *msg){
    log_at(msg,-1,-1,-1,-1);
} 
int kprintf( char* msg, ...){
    va_list arg;    
    va_start (arg, msg);
    int len = _strlen(msg);
    char fixed[len*12];
    for(int i =0;i<len;i++){
        fixed[i] =msg[i];
    }
    for(int i =len;i<len*12;i++){
        fixed[i] = 0;
    }
    unsigned int zz;
    int x = pos_x;
    int y = pos_y;
    int j = 0;
    for(int i =0;i<len;i++){
    
        char ch = msg[i];
        if(ch == '\n'){
            fixed[i] = ' ';
            pos_x=0;
            pos_y++;
        }
        if(ch == '%'){
            if(msg[i+1] == '0'){
                char buff[3];
                buff[0] = msg[i+2];
                int fallback = atoi(buff);
               
                if(msg[i+3] == 'x'){
                    fixed[i] = ' ';
                    fixed[i+1] = ' ';
                    fixed[i+2] = ' ';
                    fixed[i+3 ] = ' ';
                    //%02x
                 

                    zz = va_arg(arg,unsigned int );
                                  

                    if(zz != 0x00){
                        char buff[256];
                        itoh(buff,zz,16);
                        int blen =_strlen(buff);
                        int k = i;
                        for(int h = 0 ;h<fallback;h++){
                            fixed[k] = '0';
                            fixed[k+1] = buff[h];
                            k++;
                        }
                    }
                    else{
                        
                        fixed[i] = '0';
                        fixed[i+1] = '0';
                    }
                }
            }
            if(msg[i+1] =='d'){
                fixed[i+1]=' ';
                zz =va_arg(arg,int);
                char buff[256];
                itoa(zz,buff);
                int bufflen= _strlen(buff);
                int k = i;
                for(int h = 0;h<bufflen;h++){
                    fixed[k] = buff[h];
                    k++;
                }
            }
            if(msg[i+1] =='f'){
                fixed[i+1]=' ';
                zz =va_arg(arg,int);
                char buff[256];
                itoa(zz,buff);
                int bufflen= _strlen(buff);
                int k = i;
                for(int h = 0;h<bufflen;h++){
                    fixed[k] = buff[h];
                    k++;
                }
            }
            if(msg[i+1] =='c'){
                fixed[i+1]=' ';
                zz =va_arg(arg,int);
                fixed[i] = (char)zz;
            }
            if(msg[i+1] =='s'){
                fixed[i+1]=' ';
                char *buff = va_arg(arg,char*);
                int bufflen = _strlen(buff);
                int k =i;
                
                for(int h =0;h<bufflen;h++){
                    fixed[k] = buff[h];
                    k++;
                }
            }
            if(msg[i+1] == 'x'){
                fixed[i] = ' ';
                fixed[i+1] = ' ';
                zz = va_arg(arg,unsigned int );
                if(zz != 0x0){
                    char buff[256];
                    itoh(buff,zz,16);

                    int blen =_strlen(buff);
                    int k = i ;

                    for(int h = 0 ;h<blen;h++){
                        fixed[k] = buff[h];
                        k++;
                    }

                }
                else{
                    fixed[i] = '0';
                    fixed[i+1] = '0';
                }

            }

            
        }
    
    }
    log(fixed);

  
    
}  

/*
    Public usable LIBC
        *should be used always, just not when debugging*
*/

void dispchar(char c){
    int pos_y = pos_y;
    int pos_x = pos_x;
    int fore = 0xf;
    int bckg = 0x0;
    uint16_t clr = (bckg<<4) | (fore & 0x0F); //low bit bckg OR operation on low level bit fore
    volatile uint16_t* mem=( volatile uint16_t*)0xb8000+(pos_y*80+pos_x);
    *mem=c|(clr<<8);
    if(pos_y>80){
        clearsc();
        pos_x=0;
        pos_y=0;
    }
    if(pos_x>=80){
        pos_x=0;
        pos_y++;
    }
    pos_x++;
}
int  del_last_key(){
    int pos_y = pos_y-1;
    int pos_x = pos_x-1;
    int fore = 0x0;
    int bckg = 0x0;
    char c= ' ';
    uint16_t clr = (bckg<<4) | (fore & 0x0F); //low bit bckg OR operation on low level bit fore
    volatile uint16_t* mem=( volatile uint16_t*)0xb8000+(pos_y*80+pos_x);
    *mem=c|(clr<<8);
   
}
