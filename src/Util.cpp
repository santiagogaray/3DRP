/****************************************************/
/* Util.cpp
/*	Implementation of the Util class.
/****************************************************/

#include "Util.h"
#include <stdio.h>
#include <io.h>
#include <direct.h>

// This singleton instance
Util * Util::theUtil;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Util::Util()
{

}

Util::~Util()
{
	delete theUtil;
}

/* getInstance()
/*		Returns the MainFrm singleton
/*		object as a reference.
/*		If object doesn't exist, it creates one first.
/*******************************************************/
Util * Util::getInstance() 
{

	if (theUtil == NULL) {
		theUtil = new Util();
		if (theUtil == NULL) {
			fprintf(stderr,"Out of Memory");
			exit(1);
		}
	}

	return theUtil;
}



//////////////////////////////////////////////////////////////////////
// Util functions
//////////////////////////////////////////////////////////////////////



void Util::text1(GLuint x, GLuint y, GLfloat scale, char* format, ...)
{
	va_list args;
	char buffer[255], *p;
	GLfloat font_scale = 119.05f + 33.33f;

	// Format buffer
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Save and reset state
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glTranslatef(x, y, 0.0);

	glScalef(scale/font_scale, scale/font_scale, scale/font_scale);

	for(p = buffer; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	
	// Restore state
	glPopAttrib();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}




void Util::text2(GLuint x, GLuint y, char* format, ...)
{
	va_list args;
	char buffer[255], *p;

	// Format buffer
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Save and reset state
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glRasterPos2f(x,y);
	for(p = buffer; *p; p++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);
	
	// Restore state
	glPopAttrib();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}




void Util::textToImage( char * text, GLubyte * buffer ) 
{


	glClearColor (0.0f,0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT );
	glColor4f( 1, 1, 1, 1 );

	text2(3,5, text);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

	glReadPixels(0,0, 128,16, GL_RGBA, 
		GL_UNSIGNED_BYTE, buffer );


}





void Util::drawTripod( GLfloat size )
{

	glColor4f( 1.0, 1.0, 1.0, 1.0 );
	glPushMatrix();
	glScalef( size, size, size );

	glPushMatrix();
	glBegin( GL_LINES );
		glVertex3f( 0, 0, 0 );
		glVertex3f( 1, 0, 0 );
		glVertex3f( 0, 0, 0 );
		glVertex3f( 0, 1, 0 );
		glVertex3f( 0, 0, 0 );
		glVertex3f( 0, 0, 1 );
	glEnd();
	glPopMatrix();
	
	drawChar( 1.1f, 0, 0, 0.001f, 'x' );
	drawChar( 0, 1.1f, 0, 0.001f, 'y' );
	drawChar( 0, 0, 1.1f, 0.001f, 'z' );

	glPopMatrix();
}




void Util::drawChar( GLfloat x, GLfloat y, GLfloat z,
					 GLfloat scale , char ch)
{
	
	glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(scale, scale, scale);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, ch);
	glPopMatrix();

}




GLint Util::checkTextureSize(GLint xtsize, GLint ytsize)
{
	//max texture2D size
	GLint width=0;
	glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_RGBA, xtsize, ytsize,
				 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	return width;

}




void Util::oops(char * errorMessage)
{
	fprintf(stderr, "%s\n", errorMessage);
}




GLint Util::readDir(char * dir, struct _finddata_t theFiles[])
{

	struct	_finddata_t afile;
	long	hFile;
	int		fcount;

	// set working directory
	if (dir && _chdir(dir) == -1) {	
		oops("Directory not found");
		return -1;
	}

	// Find first file in current directory ('.')
	if( (hFile = _findfirst( "*.*", &afile )) == -1L )
		return -1;

	_findnext( hFile, &afile );			// skip '..' entry
	// get the rest of the files
	for(fcount=0; _findnext( hFile, &afile ) == 0 && fcount < MAXFILES; fcount++ )
	{
		theFiles[fcount] = afile;
	}
	_findclose( hFile );

	return fcount;

}


/* drawFrame( GLfloat offset )
/*		Draw a line-fashion frame offsetted by
/*		a specified amount from plane border
/*		using an rgba color specification.
/*************************************************/
void Util::drawFrame( GLfloat offset, GLfloat lineWidth,  
		     GLfloat r, GLfloat g, GLfloat b, GLfloat a ) 
{
	glColor4f( r, g, b, a );

	glLineWidth(lineWidth);

	glPushMatrix();
	glBegin(GL_LINE_LOOP);
		glVertex3f( -0.5 - offset, -0.5 - offset, 0 );
		glVertex3f( -0.5 - offset,  0.5 + offset, 0 );
		glVertex3f(  0.5 + offset,  0.5 + offset, 0 );
		glVertex3f(  0.5 + offset, -0.5 - offset, 0 );
	glEnd();
	glPopMatrix();

	glLineWidth(1);
}




void Util::checkGLError()
{
	GLenum errCode;
	const GLubyte * errString;

	if ((errCode = glGetError()) != GL_NO_ERROR){
		errString = gluErrorString(errCode);
		fprintf(stderr,"OpenGl Error: %s\n", errString);
	}

}

