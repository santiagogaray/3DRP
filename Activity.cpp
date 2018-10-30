/********************************************************************/
/* Activity.cpp:						  				    */
/*		Implementation of Activity derived classes		   */
/********************************************************************/


#include "Activity.h"
#include "MainFrm.h"

// globals and definitions
extern GLfloat LAST_FRAME_TICKS;

#define	 fps_coef(n)	(LAST_FRAME_TICKS/n)
#define  PI				3.1415926535


// Act_camCenter Class
//////////////////////////////////////////////////////////////////////


/* Contructor
/*		Stores the parent handler of this
/*		activity.
/***********************************************/
Act_camCenter::Act_camCenter( Element * parent )
{
	handler = parent;
}


/* execute()
/*		This activity places the camera on its
/*		initial position using a radial path. 
/*		Keeps motion ratio based on frame rate.
/***********************************************/
int Act_camCenter::execute()
{
	// get actual coords.
	GLfloat locx = handler->getState(EL_LOCX);
	GLfloat locy = handler->getState(EL_LOCY);
	GLfloat locz = handler->getState(EL_LOCZ);
	GLfloat tarx = handler->getState(EL_TARX);
	GLfloat tarz = handler->getState(EL_TARZ);


	// Calculate actual angle
	GLfloat dist  = sqrt( pow(locx,2) + pow(locz,2) );
	GLfloat cosx  = locx/dist;
	GLfloat angle = acos(cosx)*180/PI;
	if (locz < 0)	angle += (180-angle)*2;

	// make angle closer to 90°
	if (angle < 90-(1*fps_coef(20)) || angle > 270)
		angle +=1.0f * fps_coef(20);
	else
		if (angle > 90+(1*fps_coef(20)))
			angle -=1.0f*fps_coef(20);
		else
			angle = 90;

	// make distance center/camera closer to 6
	if  (dist <= 6-(0.1*fps_coef(20)))		dist += 0.1f*fps_coef(20);
	else {
		if  (dist >= 6+(0.1*fps_coef(20)))	dist -= 0.1f*fps_coef(20);
		else { dist = 6; }
	}
	// calculate new xz coords
	locx = cos(angle*PI/180)*dist;
	locz = sin(angle*PI/180)*dist;

	// make y coord. closer to 0
	if (locy >= (0.1*fps_coef(20)))		locy -= 0.1f*fps_coef(20);
	else {
		if (locy <= (-0.1*fps_coef(20)))locy += 0.1f*fps_coef(20);
		else { locy = 0; }
	}
	// make target x coord. closer to 0.
	if (tarx >= 0 + 0.1*fps_coef(20))		tarx -= 0.1f*fps_coef(20);
	else {
		if (tarx <= 0 - 0.1*fps_coef(20))	tarx += 0.1f*fps_coef(20);
		else { tarx = 0; } 
	}
	// make target z coord. closer to 0.
	if (tarz >= 0 + 0.1*fps_coef(20))		tarz -= 0.1f*fps_coef(20);
	else {
		if (tarz <= 0 - 0.1*fps_coef(20))	tarz += 0.1f*fps_coef(20);
		else { tarz = 0; }
	}


	// send new data to handler object
	handler->setState(EL_LOCX, locx);
	handler->setState(EL_LOCY, locy);
	handler->setState(EL_TARY, locy);
	handler->setState(EL_LOCZ, locz);
	handler->setState(EL_TARX, tarx);
	handler->setState(EL_TARZ, tarz);

	// stop action when reached end point
	if (locy == 0   &&
		locx <= 0.1 && locx >= - 0.1  &&
		locz <= 6.1 && locz >=   5.9  &&  
		tarx == 0   && tarz == 0)
		return 1;	// end this activity

	return 0;		// continue active
}




// Act_camCloseUp Class
//////////////////////////////////////////////////////////////////////


/* Contructor
/*		Stores the parent handler of this
/*		activity.
/***********************************************/
Act_camCloseUp::Act_camCloseUp( Element * parent )
{
	handler = parent;
	if (parent) {
		distance   = 0.9f;	// distance from target
		targOffset = 0;		// target offset from element

		if ( init() == -1 ) {
			handler = NULL;
		}

		rotStep	   = 1.0f*fps_coef(2);		// rotation animation amount
		transStep  = 0.1*fps_coef(speed);	// transl. animation amount

	}
}



GLint Act_camCloseUp::init()
{

	MainFrm * m = m->getInstance();
	Element3DList * el = m->getActiveList();
	GLint selected = el->getState(ELIST_SELECTED_ITEM);

	if (selected < 0)
		return -1;		// no element selected;
	
	// get selected elem. props. by traversing the list path.
	selAngle = 90;
	selLocX  =  selLocY  =  selLocZ = 0;
	Element * e;
	GLfloat sc_fctr	= 1;	// list grid scale factor
	GLfloat gapz	= 0;	// calculates  gap
	GLfloat gapx	= 0;	// between list grids
	Element3DList * list = m->getElementList();
	while (true) {
		selected    = list->getState(ELIST_SELECTED_ITEM);
		e			= list->getElementAt(selected);

		GLfloat dx	= e->getState(EL_LOCX);
		GLfloat dz	= e->getState(EL_LOCZ);
		GLfloat hyp	= sqrt( pow(dx,2) + pow(dz,2) );

		// calculate the absolute angle of this element
		selAngle	= (e->getState(EL_ROTY)+90) - (90 - selAngle);

		dx = cos(selAngle*PI/180)*hyp;				// x and z values
		dz = sin(selAngle*PI/180)*-hyp;				// of this element

		selLocX    += dx*sc_fctr + gapx;			// add values to
		selLocZ    += dz*sc_fctr + gapz;			// the coordinates
		selLocY    += e->getState(EL_LOCY)*sc_fctr;	// holders

		gapx = cos(selAngle*PI/180)*(1.8f*sc_fctr);	// 1.8 gap between
		gapz = sin(selAngle*PI/180)*(-1.8f*sc_fctr);// list grids
		
		if ( list->getState(ELIST_ACTIVE) == 1 ) {
			break;
		}

		sc_fctr *= 0.3f;		// reduce scale factor
		list = (Element3DList *) list->getElementAt(selected);
	}

	distance   *= sc_fctr;			// scale distance from target
	selAngle	= 180 - selAngle;	// and reverse cam. final angle 
	GLfloat tar_offx = cos(selAngle*PI/180)*( targOffset*sc_fctr);
	GLfloat tar_offz = sin(selAngle*PI/180)*( targOffset*sc_fctr);
	selLocX    += tar_offx;			// place offset 
	selLocZ    += tar_offz;			// from element

	// get current cam. speed
	speed = (GLint) handler->getState(CAM_SPEED);	

	return 0;
}




/* execute()
/*		This activity places the camera facing
/*		the selected element from the inner side
/*		of the element list grid.
/*		Keeps motion ratio based on frame rate.
/***********************************************/
int Act_camCloseUp::execute()
{
	if (handler == NULL)	// error during act. init.
		return -1;			// terminate activity

	// get actual coords.
	GLfloat locx = handler->getState(EL_LOCX);
	GLfloat locy = handler->getState(EL_LOCY);
	GLfloat locz = handler->getState(EL_LOCZ);
	GLfloat tarx = handler->getState(EL_TARX);
	GLfloat tarz = handler->getState(EL_TARZ);



	// Animate camera properties to match final state //


	// make target x coord. closer to final x coord.
	if (tarx >= selLocX + transStep)
		tarx -= transStep;
	else {
		if (tarx <= selLocX - transStep)
			tarx += transStep;
		else  { tarx = selLocX; } 
	}
	// make target z coord. closer to final z coord.
	if (tarz >= selLocZ + transStep)
		tarz -= transStep;
	else {
		if (tarz <= selLocZ - transStep)
			tarz += transStep;
		else { tarz = selLocZ; }
	}
	// make y coord. closer to final y coord.
	if (locy >= selLocY + transStep)
		locy -= transStep;
	else {
		if (locy <= selLocY - transStep)
			locy += transStep;
		else { locy = selLocY; }
	}

	// Calculate actual angle
	GLfloat dist  = sqrt( pow(tarx-locx,2) + pow(tarz-locz,2) );
	GLfloat cosx  = (tarx-locx)/dist;
	if ( cosx >  1 ) { cosx =  1; }	// correct
	if ( cosx < -1 ) { cosx = -1; }	// in case of error 
	GLfloat angle = acos(cosx)*180/PI;

	// make angle special adjustments.
	angle += (90-angle)*2;
	if (tarz > locz) {
		angle +=(180-angle)*2;
	}
	if (360-angle+selAngle < angle-selAngle)	// convert angles
		angle -=360;							// to get the
	if (360+angle-selAngle < selAngle-angle )	// minimum diff.
		angle +=360;							// possible.

	// make angle closer to selAngle
	if (angle < selAngle - rotStep )
		angle += rotStep;
	else {
		if (angle > selAngle + rotStep )
			angle -= rotStep;
		else { angle = selAngle; }
	}
	// make distance center/camera closer to 
	if  (dist <= distance - transStep)
		dist += transStep;
	else {
		if  (dist >= distance + transStep)
			dist -= transStep;
		else { dist = distance; }
	}
	// calculate new xz coords
	locx = tarx+cos(angle*PI/180)*dist;
	locz = tarz+sin(angle*PI/180)*dist;


	// send new data to camera object
	handler->setState(EL_LOCX, locx);
	handler->setState(EL_LOCY, locy);
	handler->setState(EL_LOCZ, locz);
	handler->setState(EL_TARX, tarx);
	handler->setState(EL_TARY, locy);
	handler->setState(EL_TARZ, tarz);

	// stop action when reached end point
	if ( locy  == selLocY  &&  
		 angle == selAngle &&  dist == distance &&
		 tarx  == selLocX  &&  tarz == selLocZ ) {
		return 1;	// end this activity
	}

	return 0;		// continue active
}






// Act_camEnterList Class
//////////////////////////////////////////////////////////////////////

/* Contructor
/*		Stores the parent handler of this
/*		activity.
/***********************************************/
Act_camEnterList::Act_camEnterList(Element * parent)
{ 
	handler    = parent;
	distance   =  1.8f;					// distance from target
	targOffset = -1.8f;					// target offset from element

	if (Act_camCloseUp::init() == -1 || init() == -1) {
		handler = NULL;
	}

	rotStep	   = 1.0f*fps_coef(20);		// rotation animation amount
	transStep  = 0.1*fps_coef(speed);	// transl. animation amount
}



GLint Act_camEnterList::init()
{

	// change current active element list
	MainFrm * m = m->getInstance();
	Element3DList * el = m->getActiveList();
	GLint selected = el->getState(ELIST_SELECTED_ITEM);

	if ( selected >= 0 && el->getElementAt(selected)->getState(ELIST) != -1) {

		el->getElementAt(selected)->setState(ELIST_ACTIVE, 1); // activate sublist
		el->setState(ELIST_ACTIVE, 0);		// desactivate parent list
		handler->setState(CAM_FOLLOW, 0);	// desactivate cam.follow

	} else {
		return -1;	// this action is not allowed here.
	}

	return 0;
}


/* execute()
/*		This activity places the camera facing
/*		the interior of the selected element list.
/*		Keeps motion ratio based on frame rate.
/***********************************************/
int Act_camEnterList::execute()
{	
	GLint ret = Act_camCloseUp::execute();
	if( ret == 1) {

		// if new list reached slowdown camera
		handler->setState(CAM_SPEED,(GLdouble) speed/0.3f );
	}

	return ret;
}




// Act_camExitList Class
//////////////////////////////////////////////////////////////////////

/* Contructor
/*		Stores the parent handler of this
/*		activity.
/***********************************************/
Act_camExitList::Act_camExitList(Element * parent)
{ 
	handler    = parent;
	distance   = 0.9f;					// distance from target
	targOffset = 0;						// target offset from element

	if ( init() == -1 || Act_camCloseUp::init() == -1 ) {
		handler = NULL;
	}

	rotStep	   = 1.0f*fps_coef(4);		// rotation animation amount
	transStep  = 0.1*fps_coef(40);	// transl. animation amount
}



GLint Act_camExitList::init()
{

	// activate the parent element list
	MainFrm * m = m->getInstance();
	Element3DList * pl = m->getElementList();
	Element3DList * al = m->getActiveList();
	if (pl == al)	return -1;	// this action is not allowed here.

	while (true) {
		GLint sel = pl->getState(ELIST_SELECTED_ITEM);
		al = (Element3DList *) pl->getElementAt(sel);
		if (al->getState(ELIST_ACTIVE) == 1) {
			al->setState(ELIST_ACTIVE, 0);
			pl->setState(ELIST_ACTIVE, 1);
			break;
		}
		pl = al;
	}

	handler->setState(CAM_FOLLOW, 0);	// desactivate cam.follow

	return 0;
}


/* execute()
/*		This activity places the camera facing
/*		the interior of the selected element list.
/*		Keeps motion ratio based on frame rate.
/***********************************************/
int Act_camExitList::execute()
{	
	GLint ret = Act_camCloseUp::execute();

	if(ret == 1) {

		// if new list reached slowdown camera
		handler->setState(CAM_SPEED,(GLdouble) speed*0.3f );
		handler->setState(CAM_FOLLOW, 1);	// activate cam.follow

	}
	return ret;
}





