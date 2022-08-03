#include "../kernel/stdio.h"
#include "../kernel/string.h"
#include "../drivers/keyboard.h"
#include "./shell.h"
#include "../drivers/ports.h"
#include "../kernel/util.h"
/*
    we need some basic commands bro
    info
    exit
    show hollywood undead notes
    date & time
*/

void splash(){
    POSX=0;
    POSY=0;
    log_at("Sour shell - really shitty shell to control Lemon OS",0xA,0x0,0,0);
    POSY++;
    POSX=0;
    log("type 'help' to see available commands.");
    POSY++;
    POSX=0;
    log("creator: nitrodegen");
    POSY++;

}
void helloshell(){
    splash();
    POSY=8;
    POSX=0;
    log_at("marko",0xd,0x0,POSX,POSY);
    log_at("@lemonos~$ %",0x3,0x0,POSX+1,POSY);
    POSX=0;
}
int get_update_in_progress_flag() {
      io_write(0x70, 0x0A);
      return (io_read(0x71) & 0x80);
}
uint32_t read_cmos(uint32_t reg){
    asm volatile("cli");
    io_write(0x70, reg);
    uint32_t hr = io_read(0x71);

    asm volatile("sti");
    return hr;

}
void time(){

    uint32_t hours = read_cmos(0x04);
    char hrbuf[64];
    itoa(hours,hrbuf);
    uint32_t mins = read_cmos(0x02);
    char minbuf[64];
    itoa(mins,minbuf);
    POSY++;
    log("Current time is ");
    char result[6];
    result[0]= hrbuf[0];
    result[1]=hrbuf[1];
    result[2]=':';
    result[3]=minbuf[0];
    result[4]=minbuf[1];
    log(result);

}
void command(char* str){
    int pos = 0;
   if(str[0] == 'E' && str[1] == 'X' && str[2] =='I' && str[3] == 'T'){
        //log_at("exiting..",0xf,0x0,5,10);
        //io_wrdwrite(0x604, 0x2000); // qemu exit
        //asm volatile("jmp 0xdeadbeef");
        char *m = (char*)malloc(128);
        char *test="if you see this, malloc works.";
        for(int i =0;i<_strlen(test);i++){
                m[i]=test[i];
        }
        log(m);
        asm volatile("out %%ax ,%%dx"::"a"(0x2000),"d"(0x604));//io_wrdwrite syscall
       
   }
    if(str[0] == 'I' && str[1] == 'N' && str[2] =='F' && str[3] == 'O'){
        log_at("Lemon OS - kernel v1.0a",0x4,0x0,0,POSY+2);
        log_at("Build 2 Aug. 2k22 by nitrodegen",0x6,0x0,0,POSY+3);
        log_at("First OS i ever made. it sucks.",0x7,0x0,0,POSY+4);
    }
    if(str[0] == 'T' && str[1] == 'I' && str[2] =='P'){
        log_at("Here's a tip, you're a dick.",0x3,0x0,0,POSY+2);
    }
    if(str[0] == 'C' && str[1] == 'L' && str[2] =='E' && str[3] == 'A' && str[4] == 'R'){
        clearsc();
        POSY=8;
        POSX=0;
        log_at("marko",0xd,0x0,POSX,POSY);
        log_at("@lemonos~$ %",0x3,0x0,POSX+1,POSY);
        POSX=0; 
    }

    if(str[0] == 'C' && str[1] == 'R' && str[2] =='A' && str[3] == 'S'  && str[4] == 'H'){
        asm volatile("int $3");
    }
    if(str[0] == 'T' && str[1] == 'I' && str[2] =='M' && str[3] == 'E' ){
        time();
    }
    if(str[0] == 'H' && str[1] == 'E' && str[2] =='L' && str[3] == 'P' ){
        clearsc();
        log_at("HELP - LemonOS  help section (im so tired of this project)",0xA,0x0,0,0);
        log_at("CRASH - causes kernel panic",0xf,0x0,0,1);
        log_at("INFO - shows sysinfo",0xf,0x0,0,2);
        log_at("TIP - tells you tips",0xf,0x0,0,3);
        log_at("CALC - basic calculator example: CALC 2+2",0xf,0x0,0,4);
        log_at("TIME - shows current time",0xf,0x0,0,5);
        log_at("UNDEAD - ;)",0xf,0x0,0,6);
        log_at("EXEC - ;)",0xf,0x0,0,7);
        log_at("EXIT - should reboot (works on qemu)",0xf,0x0,0,8);
        log_at("planning to add to boot from hdd , but idk, i wanna do AI now.",0xf,0x0,0,9);
    }


    if(str[0] == 'U' && str[1] == 'N' && str[2] =='D' && str[3] == 'E'  && str[4] == 'A'&& str[5] == 'D'){
        clearsc();
        POSY=0;
        POSX=0;
        log_at("Bitches, I hope you know",0xc,0x0,0,0);
        log_at("Bitches, I hope you know",0xf,0x0,0,1);
        log_at("Bitches, I hope you know",0xc,0x0,0,2);
        log_at("I won't stop till I hit that hoe",0xf,0x0,0,3);
        log_at("Baby, come say \"hello\"",0xc,0x0,0,4);
        log_at("And get your drunk ass over here, let's bone",0xf,0x0,0,5);
        log_at("Bitches, I hope you know",0xc,0x0,0,6);
        log_at("I won't stop till I hit that hoe",0xf,0x0,0,7);
        log_at("Shorty, come say \"hello\"",0xc,0x0,0,8);
        log_at("And get your drunk ass over here, let's bone",0xf,0x0,0,9);
  
    }
    if(str[0] == 'E' && str[1] == 'X' && str[2] =='E' && str[3] == 'C'){
        log_at("I'm sorry Dave, I'm afraid I can't do that.",0x7,0x0,0,POSY+2);
        log_at("https://hallsys.net/",0x4,0x0,0,POSY+3);
        log_at("https://hallsys.net/",0x4,0x0,0,POSY+3);
    }
    if(str[0] == 'C' && str[1] == 'A' && str[2] =='L' && str[3] == 'C'  && str[4] == ' '){
       POSY++;
       int error_flag=0;
       int k = 5;
       int l = 0;
       int n1=0,n2=0;
       char symbol;
       char n1buf[64];
       char n2buf[64];
       _memset(&n1buf,0,64);
       _memset(&n2buf,0,64);


       while(str[k] != '+' && str[k] != '-' && str[k] != '/' && str[k] != '*'){
            if(k>64){
                log("SEGFAULT: buffer overflow (number bigger than 64 bytes)");
                error_flag=1;
                break;
            }
            n1buf[l]=str[k];
            k++;
            l++;
       
       }
       if(error_flag !=1){
       symbol=str[k];
       k++;
       l=0;
       for(int i =k;i<_strlen(str);i++){
            n2buf[l]=str[i];
            l++;
       }
       n1 = atoi(n1buf);
       n2 = atoi(n2buf);
       int result =0;
        if(symbol == '+'){
            result= n1+n2;
        }
        if(symbol=='-'){
          result=  n1-n2;
        }
        if(symbol=='*'){
         result= n1*n2;
        }
        if(symbol=='/'){
         result=   n1/n2;
        }
        char buff[64];
  
        itoa(result,buff);
        log("Result:");
        log(buff);
        
       }
       //TODO:boot to hdd
    
    }
}
