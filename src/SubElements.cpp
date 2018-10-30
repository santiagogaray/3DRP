/********************************************************************/
/* Element3D.cpp:										  */
/*		Implementation of Element derived subclasses	  */
/********************************************************************/

#include "SubElements.h"




//////////////////////////////////////////////////////////////////////
// Bitmap /// Base class: Element
//////////////////////////////////////////////////////////////////////

/* Bitmap Contructor
/*		Creates a default buffer
/*		used to place the bitmap.
/***********************************************/
Bitmap::Bitmap()
{
	buffer = new GLubyte[6400];
	if (buffer == NULL) {
		fprintf(stderr,"Out of Memry");
		exit(0);
	}
}


/* Bitmap Destructor
/*		Cleans memory allocs.
/***********************************************/
Bitmap::~Bitmap()
{
	delete[] buffer;
}


/* init()
/*		Creates a default bitmap data.
/***********************************************/
void Bitmap::init()
{

	for (int i=0; i < 6400; i++) {
		buffer[i] = i%256;
	}

}



/* draw()
/*		Places the bitmap on screen.
/*************************************************/
void Bitmap::draw()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glColor3f(0,0,1);
	glRasterPos2i(0, 0);
	glBitmap(80, 80, 0.0, 0.0, 0.0, 0.0, buffer);

}


/* actkey(int key, int x, int y) 
/*		Any keyboard input must be placed here
/*************************************************/
void Bitmap::actkey(int key, int x, int y) 
{

}



/* idle() 
/*		Idle actions must be placed here
/*************************************************/
void Bitmap::idle() 
{

}






//////////////////////////////////////////////////////////////////////
// Image /// Base class: Element
//////////////////////////////////////////////////////////////////////



/* Image Contructor
/*		Creates a default buffer
/*		used to place the image.
/*		Sets default values.
/***********************************************/
Image::Image()
{
	offset_x = offset_y = 0; zf = 1.0f;
	width = 64; heigth = 64; nComp = 4;

	buffer = new GLubyte[width*heigth*nComp];
	if (buffer == NULL) {
		fprintf(stderr,"Out of Memry");
		exit(0);
	}
}



/* Image Contructor
/* Image(int w, int h, int d)
/*		Creates a default buffer
/*		used to place the image.
/*	args:	w	image width
/*			h	image height
/*			d	image pixel depth
/***********************************************/
Image::Image(int w, int h, int c)
{
	offset_x = offset_y = 0; zf = 1.0f;
	width = w; heigth = h; nComp = c;

	buffer = new GLubyte[width*heigth*nComp];
	if (buffer == NULL) {
		fprintf(stderr,"Out of Memry");
		exit(0);
	}
}


/* Image Destructor
/*		Cleans memory allocs.
/***********************************************/
Image::~Image()
{
	delete[] buffer;
}




/* init()
/*		Creates a default image data.
/***********************************************/
void Image::init()
{
	// modify image size if neccesary
	GLint newsize[2], size[2];
	size[0] = width;
	size[1] = heigth;
	for (int d = 0; d < 2; d++) {
		for (int w = 1; w <= 10; w++) {
			if (size[d] >= pow(2, w) )
				newsize[d] = pow(2, w);
			else
				break;
		}
	}

	// if image size modified scale image accordingly
	if ( size[0]!=newsize[0] || size[0]!=newsize[0]) {
		width  = newsize[0];
		heigth = newsize[1];
	// rgb and rgba support
		GLint format = ( nComp == 3? GL_RGB : GL_RGBA );

		gluScaleImage( format, size[0], size[1], GL_UNSIGNED_BYTE,
			buffer, newsize[0], newsize[1], GL_UNSIGNED_BYTE, buffer );

	}

	// Creates a randomly generated image for 
	// testing purposes
/*	for (int j=0; j < width; j++) {
		for (int i=0; i < heigth; i++) {
			int offset = rand()%3+1;
			buffer[j*width*depth+i*depth] = j+(256-j);
			buffer[j*width*depth+i*depth+1] = i + j +(256-i-j)/offset;
			buffer[j*width*depth+i*depth+2] = i +(256-i)/offset;
			if (depth > 3) {
				buffer[j*width*depth+i*depth+3] = j%256 * 4;
			}
		}
	}

	for (int j=0; j < width; j++) {
		for (int i=0; i < heigth; i++) {
			buffer[j*i+1] = j;
			buffer[j*i+1] = j;
			buffer[j*i+2] = j;
			buffer[j*i+3] = j;
		}
	}

*/

}



/* draw()
/*		Places the image on screen.
/*************************************************/
void Image::draw()
{

	glPushMatrix();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, offset_y);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, offset_x);
	glPixelZoom(zf, zf);
	glRasterPos2i(offset_x/2, offset_y/2);
	glDrawPixels(width-offset_x, heigth-offset_y, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	glPopMatrix();

}




/* actkey(int key, int x, int y) 
/*		Changes the zoom factor and the portion
/*		of the image displayed(offset).
/*************************************************/
void Image::actkey(int key, int xp, int yp) 
{
	switch (key) {
	case 'o':
		offset_x+=1; offset_y+=1;
		break;
	case 'p':
		offset_x-=1; offset_y-=1;
		break;
	case '+':
		zf+=0.1f;
		break;
	case '-':
		zf-=0.1f;
		break;
	default:
		break;
	}

}




/* idle() 
/*		Idle actions must be placed here
/*************************************************/
void Image::idle() 
{

}








//////////////////////////////////////////////////////////////////////
// Element3D: Abstract Class /// Base class: Element
//////////////////////////////////////////////////////////////////////


/* Contructor
/*		Init states.
/***********************************************/
Element3D::Element3D()
{
	selectFlag = FALSE;

}



/* Element Destructor
/*		Any element3d-level cleanning must be done here
/*****************************************************/
Element3D::~Element3D()
{

}




/* getState(int stateName)
/*		Returns the state if found or -1 on error
/*	args:	
/*		stateName	the name of the state inquired
/*************************************************/
GLdouble Element3D::getState(int stateName)
{
	GLdouble retState = Element::getState(stateName);
	if (retState != -1 )
		return  retState;

	if (stateName == EL_SELECTED)
		return (GLdouble) selectFlag;


	if (stateName == EL_LOCX)
		return (GLdouble) locX;

	if (stateName == EL_LOCY)
		return (GLdouble) locY;

	if (stateName == EL_LOCZ)
		return (GLdouble) locZ;

	if (stateName == EL_ROTX)
		return (GLdouble) rotX;

	if (stateName == EL_ROTY)
		return (GLdouble) rotY;

	if (stateName == EL_ROTZ)
		return (GLdouble) rotZ;


	return  -1;
}



/* setState(int stateName, GLdouble value)
/*		Sets the state if found.
/*	args:	
/*		stateName	the name of the state set.
/*		value		the new value of the sate.
/*************************************************/
void Element3D::setState(int stateName, GLdouble value)
{

	if (stateName == EL_SELECTED) {
		(value <= 0L? selectFlag = false : selectFlag = true);
		return;
	}

	if (stateName == EL_LOCX) {
		locX = (GLfloat) value;
		return;
	}
	if (stateName == EL_LOCY) {
		locY = (GLfloat) value;
		return;
	}
	if (stateName == EL_LOCZ) {
		locZ = (GLfloat) value;
		return;
	}


	if (stateName == EL_ROTX) {
		rotX = (GLfloat) value;
		return;
	}
	if (stateName == EL_ROTY) {
		rotY = (GLfloat) value;
		return;
	}
	if (stateName == EL_ROTZ) {
		rotZ = (GLfloat) value;
		return;
	}

	Element::setState(stateName, value);

}


/* actkey(int key, int x, int y)
/*		The plane-level actions are triggered with
/*		the included key strokes.
/***************************************************/
void Element3D::actkey(int key, int x, int y) 
{
	if (selectFlag) {
		switch (key) {
		case 'w':
			rotX+=2;
			break;
		case 'W':
			rotX-=2;
			break;
		case 'e':
			rotY+=2;
			break;
		case 'E':
			rotY-=2;
			break;
		case 'r':
			rotZ+=2;
			break;
		case 'R':
			rotZ-=2;
			break;
			
		default:
			break;
		}
	}

}

/* idle() 
/*		Idle actions must be placed here
/*************************************************/
void Element3D::idle() 
{
// Place Elment3D level idle-actions here

}





//////////////////////////////////////////////////////////////////////
// InStepsDrawing /// Interface to be implemented in Panel derived objects
//////////////////////////////////////////////////////////////////////

/* drawBeforeMatrix()
/*************************************************/
void InStepsDrawing::drawBeforeMatrix()
{

}

/* drawInMatrix()
/*************************************************/
void InStepsDrawing::drawInMatrix()
{

}

/* drawAfterMatrix()
/*************************************************/
void InStepsDrawing::drawAfterMatrix()
{

}





//////////////////////////////////////////////////////////////////////
// Plane: Abstract Class /// Base class: Element3D/ InStepsDrawing
//////////////////////////////////////////////////////////////////////


/* Contructor
/*		memory allocs. can be made here
/***********************************************/
Plane::Plane()
{
// Place Plane const. work here


}



/* Element Destructor
/*		Any Plane-level cleanning must be done here
/*****************************************************/
Plane::~Plane()
{
	if (texture)	delete texture;
}



/* init()
/*		Plane-level inits are placed here.
/*		Initializes the texture member.
/*******************************************/
void Plane::init()
{
	if (texture)	texture->init();

}




/* getState(int stateName)
/*		Returns the state if found or -1 on error
/*	args:	
/*		stateName	the name of the state inquired
/*************************************************/
GLdouble Plane::getState(int stateName)
{
	GLdouble retState = Element3D::getState(stateName);
	if (retState != -1 )
		return  retState;

	if (stateName == EL_TXTNAME)
		return texture->textName;

	return -1;
}





/* draw()
/*		Draws the plane with the InStepsDrawing
/*		calls included.
/*		Applies texture, transformations,
/*		naming(for selection) and mapping.
/*************************************************/
void Plane::draw()
{
	
	drawBeforeMatrix(); // This stuff must be implemented
						// in derived classes
	glColor4f( 0.1f, 0.1f, 0.1f, 0.1f );
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->textName);

	glPushMatrix();

	// Apply transformations
	glTranslatef( locX, locY, locZ );

	glRotatef( rotX, 1, 0 , 0 );
	glRotatef( rotY, 0, 1 , 0 );
	glRotatef( rotZ, 0, 0 , 1 );

	glScalef( sizeX, sizeY, sizeZ );

	// id name sent for picking hits
	glPushName(id);

	// Draw plane
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex3f(-0.5, -0.5,0);
	glTexCoord2f(0,1); glVertex3f(-0.5,  0.5,0);
	glTexCoord2f(1,1); glVertex3f( 0.5,  0.5,0);
	glTexCoord2f(1,0); glVertex3f( 0.5, -0.5,0);
	glEnd();


	glDisable(GL_TEXTURE_2D);

	drawInMatrix();		// This stuff must be implemented
						// in derived classes.
	glPopName();		// remove name from name stack

	glPopMatrix();

	drawAfterMatrix();	// This stuff must be implemented
						// in derived classes.

}






/* actkey(int key, int x, int y)
/*		The Plane-level actions are triggered 
/*		with the included key strokes.
/***************************************************/
void Plane::actkey(int key, int x, int y) 
{
	Element3D::actkey( key, x, y );

}








//////////////////////////////////////////////////////////////////////
// ImagePlane /// Base class: Plane
//////////////////////////////////////////////////////////////////////



/* Contructor
/*		Creates a fully init. Image Plane object.
/*		Default values are provided in delclaration.
/*	args:
/*	(opt)	title		the name of the texture
/*						(for id. purposes)
/*	(opt)	imageFile	a PNG file to be loaded
/*						and used as a texture.
/*	(opt)	theImg		an Image object used as a
/*						texture.
/*	(opt)	lX, lY, lZ	The initial 3d object location.
/*	(opt)	sX, sY, sZ	The initial 3d object scale.
/*	(opt)	rX, rY,	rZ	The initial 3d object rotation.
/***************************************************************/
ImagePlane::ImagePlane( char * title, char * imageFile, Image * theImg, 
				GLfloat lX , GLfloat lY , GLfloat lZ, 
				GLfloat sX , GLfloat sY , GLfloat sZ,
				GLfloat rX , GLfloat rY , GLfloat rZ )
{
	
	// Set texture data
	if (imageFile)	texture = new Texture( title, imageFile, 0);
	else
		if (theImg)	texture = new Texture( title, theImg, NULL, 0);
		else
				texture = new Texture();

	// 3D properties
	locX  = lX;   locY  = lY;   locZ  = lZ;
	sizeX = sX;   sizeY = sY;   sizeZ = sZ;
	rotX  = rX;   rotY  = rY;   rotZ  = rZ ;

	// States
	alphaBlend = false;
	

}




/* drawBeforeMatrix()
/*		Implements the method call declared by 
/*		InStepsDrawing interface and invoked by
/*		the Plane.draw() method.
/*		Enables blending if on.
/*************************************************/
void ImagePlane::drawBeforeMatrix()
{
	if ( alphaBlend ) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

}




/* drawInMatrix()
/*		Implements the method call declared by 
/*		InStepsDrawing interface and invoked by
/*		the Plane.draw() method.
/*		Draws a frame in plane.
/*************************************************/
void ImagePlane::drawInMatrix()
{
	Util * u = u->getInstance();	  
	if (selectFlag)
		u->drawFrame( 0.005f, 1, 1.0f, 1.0f, 1.0f, 1.00f );
	else
		u->drawFrame( 0.005f, 1,  0.25f, 0.25f, 0.25f, 1.00f );

}



/* drawAfterMatrix()
/*		Implements the method call declared by 
/*		InStepsDrawing interface and invoked by
/*		the Plane.draw() method.
/*		Disables blending when alphaBlend is on 
/*************************************************/
void ImagePlane::drawAfterMatrix()
{

	if ( alphaBlend ) {
		glDisable(GL_BLEND);
	}


}



/* actkey(int key, int x, int y)
/*		The ImagePlane-level actions are triggered 
/*		with the specified key strokes.
/***************************************************/
void ImagePlane::actkey(int key, int x, int y) 
{
	Plane::actkey( key, x, y );
	switch ( key ) {
	case 'b':
		alphaBlend = !alphaBlend;
		break;
	default:
		break;
	}

}





//////////////////////////////////////////////////////////////////////
// ImageApplet /// Base class: Plane
//////////////////////////////////////////////////////////////////////



/* Contructor
/*		Creates a fully init. ImageApplet object 
/*		Default values are provided in delclaration.
/*	args:
/*	(opt)	title		the name of the texture
/*						(for id. purposes)
/*	(opt)	imageFile	a PNG file to be loaded
/*						and used as a texture.
/*	(opt)	theImg		an Image object used as a
/*						texture.
/*	(opt)	lX, lY, lZ	The initial 3d object location.
/*	(opt)	sX, sY, sZ	The initial 3d object scale.
/*	(opt)	rX, rY,	rZ	The initial 3d object rotation.
/***************************************************************/
ImageApplet::ImageApplet( char * title, char * imageFile, Image * theImg,
				GLfloat lX , GLfloat lY , GLfloat lZ, 
				GLfloat sX , GLfloat sY , GLfloat sZ,
				GLfloat rX , GLfloat rY , GLfloat rZ )
{
	char * t_text = new char[256];
	strcpy(t_text, "map_"); 
	strcat(t_text, title );

	// create image plane
	imagePlane = new ImagePlane
		(t_text, imageFile, theImg, lX,lY,lZ, sX,sY,sZ, rX,rY,rZ);

	// create title plane
	Image * img = new Image( 128, 16, 4 );
	titlePlane = new ImagePlane
		( title, NULL, img, lX,lY-0.7f,lZ, sX,0.15f,sZ, rX,rY,rZ );

//	delete[] t_text;	// not cleaned (leak)
}



ImageApplet::~ImageApplet()
{
	delete titlePlane;
	delete imagePlane;
}



void ImageApplet::init()
{
	// load title pixel buffer
	char *	  title =	titlePlane->texture->name;
	GLubyte * buffer =	titlePlane->texture->img->buffer;
	Util * u = u->getInstance();	  u->textToImage(title, buffer );

	titlePlane->init();

	imagePlane->init();

}



void ImageApplet::actkey(int key, int x, int y)
{

	titlePlane->actkey(key, x, y);
	imagePlane->actkey(key, x, y);

}


void ImageApplet::draw()
{
	imagePlane->draw();
	titlePlane->draw();
//	titlePlane->texture->img->draw();

}



void ImageApplet::idle()
{

	titlePlane->idle();
	imagePlane->idle();

}



/* getState(int stateName)
/*		Returns the state if found or -1 on error
/*	args:	
/*		stateName	the name of the state inquired
/*************************************************/
GLdouble ImageApplet::getState(int stateName)
{

	GLdouble retState;

	// return the image plane states if state found
	if ((retState=imagePlane->getState(stateName)) != -1)
		return retState;

	// by now titlePlane ignored
//	if ((retState=titlePlane->getState(stateName)) != -1)
//		return retState;


	return -1;
}




/* setState(int stateName, GLdouble value)
/*		Sets the state if found.
/*	args:	
/*		stateName	the name of the state set.
/*		value		the new value of the sate.
/*************************************************/
void ImageApplet::setState(int stateName, GLdouble value)
{

	// send call to elements on applet
	imagePlane->setState(stateName, value);
//	titlePlane->setState(stateName, value);	// by now title plane ignored

	// check the Element states
	Element::setState(stateName, value);

}




/* getSubElement(int target)
/*		Returns the element if found or NULL
/*		on error.
/*	args:	
/*		target		the name of the element 
/*					required
/*************************************************/
Element * ImageApplet::getSubElement(int target)
{

	if (target == EL_IMAGE_PLANE)
		return imagePlane;

	if (target == EL_TITLE_PLANE)
		return titlePlane;

	return NULL;

}









//////////////////////////////////////////////////////////////////////
// Cube /// Base class: Element3D
//////////////////////////////////////////////////////////////////////




/* Contructor
/*		Creates a fully init. Cube object.
/*		Default values are provided in delclaration.
/***************************************************************/
Cube::Cube( GLfloat posX, GLfloat posY, GLfloat posZ, 
	 GLfloat dimX, GLfloat dimY, GLfloat dimZ )
{

	texture = new Texture();

	locX  = posX;   locY  = posY;   locZ  = posZ;
	sizeX = dimX;   sizeY = dimY;   sizeZ = dimZ;
	rotX  =  0;		rotY  = 90   ;  rotZ  = 0   ;

}



/* init()
/*		Cube-leve inits are placed here.
/*		Initializes the texture member.
/*******************************************/
void Cube::init()
{
	texture->init();
	texture->setMapMode( MM_AUTO );

}




/* draw()
/*		Draws the Cube. Applies texture,
/*		transformations and mapping.
/*************************************************/
void Cube::draw()
{
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0,0,0,1);

	glPushMatrix();

	// Apply transformations
	glTranslatef( locX, locY, locZ );

	glRotatef( rotX, 1, 0 , 0 );
	glRotatef( rotY, 0, 1 , 0 );
	glRotatef( rotZ, 0, 0 , 1 );

	glScalef( sizeX, sizeY, sizeZ );


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->textName);
	glutSolidCube(1);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glDisable(GL_BLEND);

}




/* actkey(int key, int x, int y)
/*		The Cube-level actions are triggered 
/*		with the included key strokes.
/***************************************************/
void Cube::actkey(int key, int x, int y) 
{
	Element3D::actkey(key, x, y);

}



/* idle() 
/*		Idle actions must be placed here
/*************************************************/
void Cube::idle() 
{

}




