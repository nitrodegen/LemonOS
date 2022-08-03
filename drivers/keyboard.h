#ifndef KEYBOARD_H_
#define KEYBOARD_H_
#define ENTER (char)0xFF
#define ERR (char)256
#define ESC (char)257
#define TAB (char)259
#define LCTRL (char)400
#define BACKSPACE (char) 258
#define RSHIFT (char)402
#define LALT (char)403
#define STAR (char)404
#define UP (char)512
#define LSHIFT (char) 401
static char last_pressed;
static uint8_t enter_shell= 0;
static uint8_t read_input = 0;
static char key_buffer[256];
static int looper=0;
void init_keyboard();

#endif
