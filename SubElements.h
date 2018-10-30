/*********************************************/
/* Purpose:
/*	Definition of Element derived classes
/*	used to handle specific implementations
/*	according to their element type.
/*********************************************/

#ifndef __element3d_h__
#define __element3d_h__


#include "Element.h"


/* InStepsDrawing Interface **********************************/
/* Purpose:		Defines an interface for implementing 3-Step */
/*				drawing process inside the element3D's		 */
/*				draw() routine.								 */
/*************************************************************/
class InStepsDrawing
{
public:
	virtual void drawBeforeMatrix();
	virtual void drawInMatrix();
	virtual void drawAfterMatrix();
};





/* Bitmap Class *********************************************/
/* Purpose:		Defines an object that holds a bitmap pixel	*/
/*				buffer representing an image used by the	*/
/*				Texture object as its raster data.			*/
/************************************************************/
class Bitmap : public Element
{
public:
	Bitmap();
	~Bitmap();
	void draw(void);
	void init(void);
	void actkey(int key, int x, int y);
	void idle();

private:
	GLubyte * buffer;
};




/* Image Class **********************************************/
/* Purpose:		Defines an object that holds a rgba pixel	*/
/*				buffer representing an image used by the	*/
/*				Texture object as its raster data.			*/
/************************************************************/
class Image : public Element
{
public:
	Image();
	Image(int w, int h, int d);
	~Image();
	void draw(void);
	void init(void);
	void actkey(int key, int x, int y);
	void idle();

//private:
	GLint offset_x;
	GLint offset_y;
	GLfloat zf;		//zoom factor
	GLint width;
	GLint heigth;
	GLint nComp;
	GLubyte * buffer;

};



/* Element3D Abstract Class *********************************/
/* Purpose:		Extends from Element a base class with the 	*/
/*				required data to represent a 3-D object.	*/
/************************************************************/
class Element3D : public Element
{

public:
	Element3D();
	~Element3D();
	GLdouble getState(int stateName);
	void setState(int stateName, GLdouble value);
	void actkey(int key, int x, int y);	
	void idle();	

protected:
	GLfloat  locX ,  locY ,  locZ ; // Transformation states
	GLfloat  rotX ,  rotY ,  rotZ ;
	GLfloat  sizeX,  sizeY,  sizeZ;
	bool selectFlag;

};





// Cube Class
class Cube : public Element3D
{
public:
	Cube(GLfloat posX = 0, GLfloat posY = 0, GLfloat posZ = 0, 
		GLfloat dimX = 1.3f, GLfloat dimY = 1, GLfloat dimZ = 1 );
	Texture * texture;
	void draw(void);
	void init(void);
	void actkey(int key, int x, int y);
	void idle();

};




/* Plane Abstract Class *****************************************/
/* Purpose:		Extends from Element3D a base class with the	*/
/*				required data to represent a 3-D object.		*/
/*				Also includes the 3-Step drawing interface.		*/
/*				Uses its abstract methods inside the draw()		*/
/*				function but leaves these methods undefined.	*/
/****************************************************************/
class Plane : public Element3D, protected InStepsDrawing
{
public:
	Plane();
	~Plane();
	void draw(void);
	void init(void);
	void actkey(int key, int x, int y);
	GLdouble getState(int stateName);

//protected:
	Texture * texture;



};






/* ImagePlane Class *********************************************/
/* Purpose:		Extends from Planel a final class with the		*/
/*				features required for an image to be shown as	*/
/*				a plane in 3d space.							*/
/****************************************************************/
class ImagePlane : public Plane
{
public:
	bool alphaBlend;
	ImagePlane( char * title=NULL, char * imageFile=NULL, Image * theImg=NULL,
		GLfloat lX = 0,    GLfloat lY = 0, GLfloat lZ = 0,
		GLfloat sX = 1.3f, GLfloat sY = 1, GLfloat sZ = 1,
		GLfloat rX = 0,    GLfloat rY = 0, GLfloat rZ = 0 );
	void actkey(int key, int x, int y);

private:
	void drawBeforeMatrix();	// Inherited from 
	void drawInMatrix();		// Plane abstract class, 
	void drawAfterMatrix();		// and implemented.
};




/* ImageApplet Class ********************************************/
/* Purpose:		Extends an Element object to hold a 
/*				complete file image unit (graphic icon and title) 
/*			Consists of two ImagePlanes: 
/*				1 to display the graphic icon
/*				1 to display the element title
/****************************************************************/
class ImageApplet : public Element
{
public:
	ImageApplet( char * name=NULL, char * imageFile=NULL, Image * theImg=NULL,
		GLfloat lX = 0,    GLfloat lY = 0, GLfloat lZ = 0,
		GLfloat sX = 1.3f, GLfloat sY = 1, GLfloat sZ = 1,
		GLfloat rX = 0,    GLfloat rY = 0, GLfloat rZ = 0 );
	~ImageApplet();
	void actkey(int key, int x, int y);
	void init();
	void draw();
	void idle();
	void setState(int stateName, GLdouble value);
	GLdouble getState(int stateName);
	Element * getSubElement(int target);

private:
	ImagePlane * titlePlane;
	ImagePlane * imagePlane;
};

#endif                  /* __elelment_h__ */
