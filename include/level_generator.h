#ifndef __LEVEL_GENERATOR_H__
#define __LEVEL_GENERATOR_H__
#include <stdbool.h>
#include <stddef.h>
#include "linear_algebra.h"

bool generate_level ( char* filename, int level_no );
int make_difficulty ( char* level_data, vec2i level_size, int level_no, int difficulty );

#endif // __LEVEL_GENERATOR_H__
