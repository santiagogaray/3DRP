/*******************************************************/
/* Util.h
/*	Defines Util  as a helper class
/*******************************************************/
#ifndef __util_h__
#define __util_h__


#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glpng.h>


/* file system definitions */
#define MAXFILES		256


class Util
{
public:
	static Util * getInstance();
	static void text1(GLuint x, GLuint y, GLfloat scale, char* format, ...);
	static void text2(GLuint x, GLuint y, char* format, ...);
	static void textToImage( char * title, GLubyte * buffer);
	static void drawTripod(GLfloat size);
	static void drawChar( GLfloat x, GLfloat y, GLfloat z,
		GLfloat scale , char ch);
	static GLint checkTextureSize(GLint x, GLint y);
	static void oops( char * errorMessage );
	static int readDir(char * dir, struct _finddata_t theFiles[]);
	static void drawFrame( GLfloat offset, GLfloat lineWidth,
		GLfloat r, GLfloat g, GLfloat b, GLfloat a );
	static void checkGLError();

private:
	Util();
	virtual ~Util();
	static Util * theUtil;

};

#endif                  /* __util_h__ */
