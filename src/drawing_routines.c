#include <GL/glut.h>
#include "../include/drawing_routines.h"
#include "../include/texture.h"

void fill_rect ( int x, int y, int width, int height, const float* color )
{
	glColor4fv ( color );
	glBegin ( GL_QUADS );
		glVertex2i (         x, y          );
		glVertex2i ( x + width, y          );
		glVertex2i ( x + width, y + height );
		glVertex2i (         x, y + height );
	glEnd ();
}

void textured_rect ( int x, int y, int width, int height, size_t ind )
{
	use_texture ( ind );
	glBegin ( GL_QUADS );
		glTexCoord2f ( 0.0, 0.0 ); glVertex2i (         x, y          );
		glTexCoord2f ( 1.0, 0.0 ); glVertex2i ( x + width, y          );
		glTexCoord2f ( 1.0, 1.0 ); glVertex2i ( x + width, y + height );
		glTexCoord2f ( 0.0, 1.0 ); glVertex2i (         x, y + height );
	glEnd ();
	unuse_texture ( ind );
}

void textured_rect_partial ( int x, int y, int width, int height, size_t ind, float tx, float ty, float tw, float th )
{
	use_texture ( ind );
	glBegin ( GL_QUADS );
		glTexCoord2f (      tx, ty      ); glVertex2i (         x, y          );
		glTexCoord2f ( tx + tw, ty      ); glVertex2i ( x + width, y          );
		glTexCoord2f ( tx + tw, ty + th ); glVertex2i ( x + width, y + height );
		glTexCoord2f (      tx, ty + th ); glVertex2i (         x, y + height );
	glEnd ();
	unuse_texture ( ind );
}

void textured_rect_partial_ind ( int x, int y, int width, int height, size_t ind, int row, int col )
{
	float tx = (((float)row*64.0)/256.0), ty = (((float)col*64.0)/256.0);
	float tw = 64.0/256.0, th = 64.0/256.0;
	use_texture ( ind );
	glBegin ( GL_QUADS );
		glTexCoord2f (      tx, ty      ); glVertex2i (         x, y          );
		glTexCoord2f ( tx + tw, ty      ); glVertex2i ( x + width, y          );
		glTexCoord2f ( tx + tw, ty + th ); glVertex2i ( x + width, y + height );
		glTexCoord2f (      tx, ty + th ); glVertex2i (         x, y + height );
	glEnd ();
	unuse_texture ( ind );
}
