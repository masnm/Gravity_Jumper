#ifndef __DRAWING_ROUTINES_H__
#define __DRAWING_ROUTINES_H__

void fill_rect ( int x, int y, int width, int height, const float* color );
void textured_rect ( int x, int y, int width, int height, size_t ind );
void textured_rect_partial ( int x, int y, int width, int height, size_t ind, float tx, float ty, float tw, float th );
void textured_rect_partial_ind ( int x, int y, int width, int height, size_t ind, int row, int col );

#endif // __DRAWING_ROUTINES_H__
