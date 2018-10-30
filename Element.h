/***********************************************************************/
/* Element.h:
/* Purpose:
/*		Definition of the Element Abstract class. 
/*		Data type used in the Graphic tree structure.
/*		Interface used to display and interact with its elements.
/************************************************************************/


#ifndef __element_h__
#define __element_h__


#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glpng.h>
#include <math.h>

#include "Util.h"
#include "Texture.h"



//global variables
static GLint idCounter=0;

// global enum states names specification
static enum stateName	{	EL_ID, 
							EL_LOCX, EL_LOCY, EL_LOCZ,
							EL_TARX, EL_TARY, EL_TARZ,
							EL_ROTX, EL_ROTY, EL_ROTZ,
							EL_TXTNAME,	EL_SELECTED,
							ELIST, ELIST_ACTIVE, ELIST_SHOW,
							ELIST_SELECTED_ITEM, ELIST_TRYSELECT,
							CAM_FOLLOW, CAM_SPEED
						};
static enum target		{	EL_IMAGE_PLANE, EL_TITLE_PLANE };





/* Element Abstract Class  **********************************/
/* Purpose:		Defines an interface to be used to render	*/
/*				every graphic object and invoked by the		*/
/*				opengl callbacks in the main loop.			*/
/************************************************************/


class Element  
{
protected:
	GLint id;
public:
	Element();
	~Element();
	virtual GLdouble getState(int stateName);
	virtual void setState(int stateName, GLdouble value);
	virtual Element* getSubElement(int target);
	virtual void draw(void) = 0;
	virtual void init(void) = 0;
	virtual void actkey(int key, int x, int y) = 0;
	virtual void idle() = 0;

};

#endif                  /* __elelment_h__ */


