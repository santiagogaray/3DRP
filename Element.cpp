/********************************************************************/
/* Element.cpp:						  				    */
/*		Implementation of Element class				      */
/********************************************************************/

#include <math.h>

#include "SubElements.h"



//////////////////////////////////////////////////////////////////////
// Element Class
//////////////////////////////////////////////////////////////////////

/* Element Contructor
/*		Sets this element id number.
/***********************************************/
Element::Element()
{
	id = idCounter;
	idCounter++;

}

/* Element Destructor
/*		Any element-level cleanning must be done here
/*****************************************************/

Element::~Element()
{
	
}


/* getState(int stateName)
/*		Returns the state if found or -1 on error
/*	args:	
/*		stateName	the name of the state inquired
/*************************************************/
GLdouble Element::getState(int stateName)
{
	if (stateName == EL_ID)
		return (GLdouble) id;

	return  -1;
}



/* setState(int stateName, GLdouble value)
/*		Sets the state if found.
/*	args:	
/*		stateName	the name of the state set.
/*		value		the new value of the sate.
/*************************************************/
void Element::setState(int stateName, GLdouble value)
{

	if (stateName == EL_ID) {
		 id = (GLint) value;
		 return;
	}


}



/* getSubElement(int target)
/*		Returns the element if found or NULL
/*		on error.
/*	args:	
/*		target		the name of the element 
/*					required
/*************************************************/
Element * Element::getSubElement(int target)
{

	return NULL;
}




