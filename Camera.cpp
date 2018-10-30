/********************************************************************/
/* Camera.cpp:						  				    */
/*		Implementation of Camera class				      */
/********************************************************************/
#include "Camera.h"

extern GLfloat LAST_FRAME_TICKS;
extern GLfloat LAST_KEY_TIME;

#define	 fps_coef(n)	(LAST_FRAME_TICKS/n)


/* Contructor
/*		memory allocs. can be made here
/***********************************************/
Camera::Camera()
{
	locX = 0; locY = 6; locZ = 0;
	tarX = 0; tarY = 0; tarZ = 0;
	activity = NULL;
	follow   = false;
	speed    = 20;

}



/* Contructor
/*		Init. the camera location and target
/***********************************************/
Camera::Camera(GLfloat xloc, GLfloat yloc, GLfloat zloc,
	GLfloat xtar, GLfloat ytar, GLfloat ztar )
{

	locX = xloc; locY = yloc; locZ = zloc;
	tarX = xtar; tarY = ytar; tarZ = ztar;
	activity = NULL;
	follow   = false;
	speed    = 20;

}



/* Camera Destructor
/*		Any object cleanning must be done here
/*****************************************************/
Camera::~Camera()
{
	delete activity;
}



/* init()
/*		Camera inits are placed here.
/*******************************************/
void Camera::init()
{

}




/* getState(int stateName)
/*		Returns the state if found or -1 on error
/*	args:	
/*		stateName	the name of the state inquired
/*************************************************/
GLdouble Camera::getState(int stateName)
{
	GLdouble retState = Element::getState(stateName);
	if (retState != -1 )
		return  retState;

	if (stateName == EL_LOCX)
		return (GLdouble) locX;

	if (stateName == EL_LOCY)
		return (GLdouble) locY;

	if (stateName == EL_LOCZ)
		return (GLdouble) locZ;

	if (stateName == EL_TARX)
		return (GLdouble) tarX;

	if (stateName == EL_TARY)
		return (GLdouble) tarY;

	if (stateName == EL_TARZ)
		return (GLdouble) tarZ;

	if (stateName == CAM_FOLLOW)
		return (GLdouble) follow;

	if (stateName == CAM_SPEED)
		return (GLdouble) speed;


	return -1;
}




/* setState(int stateName, GLdouble value)
/*		Sets the state if found.
/*	args:	
/*		stateName	the name of the state set.
/*		value		the new value of the sate.
/*************************************************/
void Camera::setState(int stateName, GLdouble value)
{

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


	if (stateName == EL_TARX) {
		tarX = (GLfloat) value;
		return;
	}

	if (stateName == EL_TARY) {
		tarY = (GLfloat) value;
		return;
	}

	if (stateName == EL_TARZ) {
		tarZ = (GLfloat) value;
		return;
	}

	if (stateName == CAM_FOLLOW) {
	(value <= 0L? follow = false : follow = true);
		return;
	}

	if (stateName == CAM_SPEED) {
		speed = (GLint) value;
		return;
	}

	// check the Element states
	Element::setState(stateName, value);

}





/* draw()
/*************************************************/
void Camera::draw()
{
	

}




/* actkey(int key, int x, int y)
/*		The camera actions are triggered with
/*		the included key strokes.
/***************************************************/
void Camera::actkey(int key, int x, int y) 
{
	GLint sp;
	if (LAST_KEY_TIME < 200) sp = speed;
	else
		sp = speed*5;

	switch (key) {
	case '3':		// reset camera location
		if (activity) delete activity;
		activity = new Act_camCenter(this);
		break;
	case 's':
	case 'S':		//selection follow on/off
		follow = ! follow;
		if (follow) {
			if (activity) delete activity;
			activity = new Act_camCloseUp(this);
		}
		break;
	case ',':		// trigger
	case '<':		//
	case '.':		//
	case '>':		// a follow-selection
	case 'o':		//
	case 'O':		//
	case 'l':		//
	case 'L':		// action
		if (follow) {
			if (activity) delete activity;
			activity = new Act_camCloseUp(this);
		}
		break;
	case '1':
		// activate and enter new element list
		if (activity) return;
		activity = new Act_camEnterList(this);
		break;
	case '2':
		// exit element list and activate parent list
		if (activity) return;
		activity = new Act_camExitList(this);
		break;
	case 'z':
	case 'Z':
		dolly(+0.1f*fps_coef(sp));
		break;
	case 'x':
	case 'X':
		dolly(-0.1f*fps_coef(sp));
		break;
	case GLUT_KEY_LEFT:
		rotate(1.0f*fps_coef(20));
		break;
	case GLUT_KEY_RIGHT:
		rotate(-1.0f*fps_coef(20));
		break;
	case GLUT_KEY_UP:
		locY += 0.1f*fps_coef(sp);
		tarY += 0.1f*fps_coef(sp);
		break;
	case GLUT_KEY_DOWN:
		locY -= 0.1f*fps_coef(sp);
		tarY -= 0.1f*fps_coef(sp);
		break;
	default:
		break;
	}

}



void Camera::dolly(GLfloat d)
{
	// Calculate actual angle
	GLdouble pi   = 3.1415926535;
	GLfloat dist  = sqrt( pow(tarX-locX,2) + pow(tarZ-locZ,2) );
	GLfloat cosx  = (tarX-locX)/dist;
	GLfloat angle = acos(cosx)*180/pi;
	angle += (90-angle)*2;
	if (tarZ > locZ) {
		angle +=(180-angle)*2;
	}
	// change distance
	dist += d;

	// calculate new xz coords
	locX = tarX+cos(angle*pi/180)*dist;
	locZ = tarZ+sin(angle*pi/180)*dist;


}





void Camera::rotate(GLfloat ang)
{

	// Calculate actual angle
	GLdouble pi   = 3.1415926535;
	GLfloat dist  = sqrt( pow(tarX-locX,2) + pow(tarZ-locZ,2) );
	GLfloat cosx  = (tarX-locX)/dist;
	GLfloat angle = acos(cosx)*180/pi;
	angle += (90-angle)*2;
	if (tarZ > locZ) {
		angle +=(180-angle)*2;
	}
	// change angle
	angle += ang;

	// calculate new xz coords
	locX = tarX+cos(angle*pi/180)*dist;
	locZ = tarZ+sin(angle*pi/180)*dist;


}



/* idle() 
/*		Idle actions must be placed here
/*************************************************/
void Camera::idle()
{
	if (activity) {
		GLint ret = activity->execute();
		if ( ret == 1 || ret == -1 ) {
			delete activity;
			activity = NULL;
		}
	}

}

