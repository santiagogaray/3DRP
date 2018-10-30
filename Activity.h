/**********************************************************************/
/* Activity.h:
/*	Definitions of the Activity class and its derived subclasses.
/**********************************************************************/



/* Activity Base Class **************************************/
/* Purpose:		Interface used for executing diverse	 	*/
/*				acivities derived from it.					*/
/************************************************************/
#ifndef __activity_h__
#define __activity_h__

#include "Element.h"


class Activity
{

public:
	virtual int execute()=0;
};


/* Act_camCenter Class / Activity derived class *************/
/* Purpose:	This activity places the camera on its			*/
/*			initial position using a radial path.			*/
/************************************************************/
class Act_camCenter : public Activity
{
public:
	Act_camCenter(Element * element);
	int execute();

private:
	Element * handler;

};



/* Act_camCloseUp Class / Activity derived class ************/
/* Purpose:	This activity places the camera focusing the	*/
/*			currently selected element						*/
/************************************************************/
class Act_camCloseUp : public Activity
{
public:
	Act_camCloseUp(Element * element=NULL);
	int execute();

protected:
	Element * handler;
	GLfloat selAngle;
	GLfloat selLocX;
	GLfloat selLocY;
	GLfloat selLocZ;
	GLfloat targOffset;
	GLfloat distance;
	GLfloat rotStep;
	GLfloat transStep;
	GLint   speed;
	GLint   init();

};



/* Act_camEnterList Class / Activity derived class **********/
/* Purpose:	This activity enters the current selected list	*/
/*			and activates it for further actions.			*/
/************************************************************/
class Act_camEnterList : public Act_camCloseUp
{

public:
	Act_camEnterList(Element * element);
	int execute();

protected:
	GLint init();

};



/* Act_camExitList Class / Activity derived class ***********/
/* Purpose:	This activity enters the current selected list	*/
/*			and activates it for further actions.			*/
/************************************************************/
class Act_camExitList : public Act_camCloseUp
{

public:
	Act_camExitList(Element * element);
	int execute();

protected:
	GLint init();

};





#endif                  /* __activity_h__ */
