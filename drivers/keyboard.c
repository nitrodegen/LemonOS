#include"ports.h"
#include"keyboard.h"
#include"../cpu/isr.h"
#include "../kernel/shell.h"
#include"../kernel/stdio.h"


char DefineKey(uint8_t code){
    char result;
    switch (code) {
        case 0x0:
            result=ERR;
            break;
        case 0x1:
            result=ESC;
            break;
        case 0x2:
            result='1';
            break;
        case 0x3:
            result='2';
            break;
        case 0x4:
            result='3';
            break;
        case 0x5:
            result='4';
            break;
        case 0x6:
            result='5';
            break;
        case 0x7:
            result='6';
            break;
        case 0x8:
            result='7';
            break;
        case 0x9:
            result='8';
            break;
        case 0x0A:
            result='9';
            break;
        case 0x0B:
            result='0';
            break;
        case 0x0C:
            result='-';
            break;
        case 0x0D:
            result='+';
            break;
        case 0x0E:
            result=BACKSPACE;
            break;
        case 0x0F:
            result=TAB;
            break;
        case 0x10:
            result='Q';
            break;
        case 0x11:
            result='W';
            break;
        case 0x12:
            result='E';
            break;
        case 0x13:
            result='R';
            break;
        case 0x14:
            result='T';
            break;
        case 0x15:
            result='Y';
            break;
        case 0x16:
            result='U';
            break;
        case 0x17:
            result='I';
            break;
        case 0x18:
            result='O';
            break;
        case 0x19:
            result='P';
            break;
		case 0x1A:
			result='[';
			break;
		case 0x1B:
			result=']';
			break;
		case 0x1C:
			result=ENTER;
			break;
		case 0x1D:
			result=LCTRL;
			break;
		case 0x1E:
			result='A';
			break;
		case 0x1F:
			result='S';
			break;
        case 0x20:
            result='D';
            break;
        case 0x21:
            result='F';
            break;
        case 0x22:
            result='G';
            break;
        case 0x23:
            result='H';
            break;
        case 0x24:
            result='J';
            break;
        case 0x25:
            result='K';
            break;
        case 0x26:
            result='L';
            break;
        case 0x27:
            result=';';
            break;
        case 0x28:
            result='\'';
            break;
        case 0x29:
            result='`';
            break;
		case 0x2A:
			result=LSHIFT;
			break;
		case 0x2B:
			result='\\';
			break;
		case 0x2C:
			result='Z';
			break;
		case 0x2D:
			result='X';
			break;
		case 0x2E:
			result='C';
			break;
		case 0x2F:
			result='V';
			break;
        case 0x30:
            result='B';
            break;
        case 0x31:
            result='N';
            break;
        case 0x32:
            result='M';
            break;
        case 0x33:
            result=',';
            break;
        case 0x34:
            result='.';
            break;
        case 0x35:
            result='/';
            break;
        case 0x36:
            result=RSHIFT;
            break;
        case 0x37:
            result=STAR;
            break;
        case 0x38:
            result=LALT;
            break;
        case 0x39:
            result=' ';
            break;
        default:
            result=UP;
            break;
    }
    return result;
}

static void keyboard_callback(registers_t regs){
    uint8_t scancode = io_read(0x60);
    char key = DefineKey(scancode);
    if(enter_shell ==1){
        clearsc();
        helloshell();

        POSX=19;
        POSY=8;
        enter_shell=-1;
         
    }
    
    if(key !=(char)UP){
        if(key != (char)ENTER && enter_shell!=0){
            
            char bob[2];
            bob[0]=key;
            dispchar(key);
            key_buffer[looper]=key;
            looper++;
            
        }
        if(key == (char)ENTER && enter_shell!=0){
            
            clearsc();
            char bp[256];
            helloshell();
            POSX=19;
            POSY=8;
            command(key_buffer);
            //log_at(key_buffer,0xf,0x0,5,10);
            _memset(key_buffer,0,256);
            looper=0;


        }
    }
    
    if(key ==(char)ENTER && enter_shell==0){
        if(enter_shell!=-1){
            enter_shell=1;
            
        }
    }
   
    
  
    char sc_ascii[256];
    itoa(scancode,sc_ascii);
  
    //log(word); // for debugging purpouses (testing keyboard , shitty IRQs AND PICs!)
}

void init_keyboard(){
    register_ihandle(IRQ1,keyboard_callback);
}
