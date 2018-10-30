/******************************************************************/
/* Camera.h 
/*	Purpose:
/*		Defines a Camera class extended from 
/*		Element base class defining camera-related
/*		features 
/******************************************************************/


#ifndef __camera_h__
#define __camera_h__


#include "Element3DList.h"
#include "Activity.h"


class Camera : public Element
{

public:
	Camera();
	Camera(GLfloat xloc, GLfloat yloc, GLfloat zloc,
		GLfloat xtar, GLfloat ytar, GLfloat ztar );
	~Camera();
	void init();
	void draw();
	void actkey(int key, int x, int y);
	void idle();
	GLdouble getState(int stateName);
	void setState(int stateName, GLdouble value);
private:
	Activity * activity;
	GLfloat  locX ,  locY ,  locZ ; // Transformation states
	GLfloat  tarX ,  tarY ,  tarZ ;
	GLint speed;
	bool follow;
	void rotate(GLfloat ang);
	void dolly(GLfloat dist);
};

#endif                  /* __camera_h__ */
