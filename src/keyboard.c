#include <GL/glut.h>
#include "../include/keyboard.h"
#include <string.h>

bool key_state[128];
bool arrow_state[4];

void keyboard_input ( unsigned char key, int x, int y )
{
	key_state[key] = true;
}

void arrow_keys ( int key, int x, int y )
{
	switch ( key ) {
		case GLUT_KEY_UP:
			arrow_state[arrow_up] = true;
			break;
		case GLUT_KEY_DOWN:
			arrow_state[arrow_down] = true;
			break;
		case GLUT_KEY_LEFT:
			arrow_state[arrow_left] = true;
			break;
		case GLUT_KEY_RIGHT:
			arrow_state[arrow_right] = true;
			break;
		default:
			break;
	}
}

bool get_key ( unsigned char key )
{
	return key_state[key];
}

bool get_arrow ( arrow_key key )
{
	return arrow_state[key];
}

void key_cleanup ( void )
{
	memset ( key_state, false, sizeof(bool)*128 );
	memset ( arrow_state, false, sizeof(bool)*4 );
}
