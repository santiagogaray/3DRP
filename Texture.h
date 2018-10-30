/* Texture Class ********************************************/
/* Purpose:		Defines a data class and interface to make  */
/*				3D elements 'texture-renderable'.			*/
/*															*/
/************************************************************/

#ifndef __texture_h__
#define __texture_h__


#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glpng.h>

#define MAXTEXTURES		20

// Defined in Element.h
class Image;

// global enum mapping specification
static enum mapMode {MM_AUTO, MM_VECTOR, MM_VERTEX };

// the texture entry used in table
struct textureInfo {
	char  * name, 
		  * imageFile;
	Image * img,  
		  * subImg;
	GLuint  textName;
};




class Texture
{

public:
	GLuint  textName;	// must  be private!!!!!!!!!!!!!!!!!

	Texture();
	Texture(char * name, Image * img, Image * subImg=NULL, GLint border=0);
	Texture(char * name, char * imgeFile=NULL, GLint border=0);
	~Texture();
	void init();
	static void			setMapMode( int mapMode );

		// must be private !!!!!!!!
	char  * name;
	Image * img;
	Image * subImg;
	char  * imageFile;
	GLint	border;

private:

	void				loadImageFile( char * pngFile );
	static struct		textureInfo * textTable[MAXTEXTURES];
	static void			initStatic();
	static GLfloat *	mapCube;
	static pngInfo		loadTexture2D( GLuint * textName, char * pngFile );
	static void			setTexture2D( GLuint * textName, Image * img,
							Image * subImg = NULL, GLint border=0);
	static textureInfo  * getTexture(char * name, char * imageFile);
	static void			addTexture(char * name, Image * img, Image * subImg,
							char * imageFile, GLuint textName);
};


#endif                  /* __texture_h__ */
