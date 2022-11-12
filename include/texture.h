#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <stdbool.h>

bool create_texture ( size_t n );
bool texture_from_image ( char* file_name, size_t ind );
bool use_texture ( size_t ind );
bool unuse_texture ( size_t ind );
bool destroy_texture ( void );

#endif // __TEXTURE_H__
