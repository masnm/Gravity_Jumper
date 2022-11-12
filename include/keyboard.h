#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdbool.h>

typedef enum {
	arrow_up = 0,
	arrow_right = 1,
	arrow_down = 2,
	arrow_left = 3,
} arrow_key ;

void keyboard_input ( unsigned char key, int x, int y );
void arrow_keys ( int key, int x, int y );
bool get_key ( unsigned char key );
bool get_arrow ( arrow_key key );
void key_cleanup ( void );

#endif // __KEYBOARD_H__
