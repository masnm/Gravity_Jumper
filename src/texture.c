#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/texture.h"
#include <GL/gl.h>

bool is_ok = false;
GLuint* textures;
bool create_texture ( size_t n )
{
	textures = (GLuint*) malloc ( sizeof(GLuint) * n );
	glGenTextures ( n, textures );
	GLenum code = glGetError ();
	if ( code == GL_INVALID_VALUE )
		fprintf ( stderr, "Texture generation failed\n" );
	return true;
}

bool texture_from_image ( char* filename, size_t ind )
{
	int width = 0, height = 0, channels = 0;
	stbi_uc *image = stbi_load( filename, &width, &height, &channels, 4 );
	if ( image != NULL ) {
		glBindTexture ( GL_TEXTURE_2D, textures[ind] );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
		//glPixelStorei ( GL_UNPACK_ALIGNMENT, 4 );
		stbi_image_free ( image );
	} else {
		fprintf ( stderr, "Stb image failed to load image\n" );
		return false;
	}
	return true;
}

bool use_texture ( size_t ind )
{
	glEnable ( GL_TEXTURE_2D );
	glBindTexture ( GL_TEXTURE_2D, textures[ind] );
	return true;
}

bool unuse_texture ( size_t ind )
{
	glDisable ( GL_TEXTURE_2D );
	return true;
}

bool destroy_texture ( void )
{
	free ( textures );
	return true;
}
