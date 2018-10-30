/**************************************************************************/
/* Element3DList.cpp: 
/*	Implementation of the Element3DList class.
/**************************************************************************/

#include "Element3DList.h"
#include <math.h>




/* Constructor
/* Element3DList(Element * parent, char * dir)
/*		Sets this elementList parent and creates
/*		its elements.
/*	args	parent	this list parent
/*			dir		the directory used to 
/*					populate list
/****************************************************/
Element3DList::Element3DList(Element * parent, char * dir)
{
	showList = false;
	active = false;
	pParentElement = parent;
	dirpath = dir;
	selectedItem = -1;
	populateList( dir );

}



/* populateList(char *dir)
/*		Reads filenames from specified
/*		directory and creates elements acording
/*		to their type.
/*	args	dir		the directory used to 
/*					read filenames from
/***********************************************/
void Element3DList::populateList(char *dir)
{
	double pi = 3.1415926535;
//	struct	_finddata_t theFiles[MAXFILES];
	int		nFiles; char	* ptr;
	GLfloat x , z, y, angle;
	Util * u = u->getInstance();

	// Read files
	if ( (nFiles = u->readDir( dir,  theFiles )) == -1 ) {	// read dir
		u->oops("Error reading directory");					// error
		exit(0);
	}
	

	// Set starting Y pos.
	y = ((int)nFiles/7)-1;
	if (nFiles%7 > 0) y++;


	// traverse names and create elements
	for(int nElem = angle = 0;  nElem < nFiles; nElem ++, angle += 30) {
//		if (strcmp(theFiles[nElem].name, "_RESTORE") == 0)
//			continue;
		// set x,y,z loc of this element
		if (angle>180) angle = 0, y -= 2;
		x = cos(angle*pi/180)*5;
		z = -sin(angle*pi/180)*5;

		if ( theFiles[nElem].attrib >= 16 &&
			 theFiles[nElem].attrib <= 18    ) {		// its a dir ?
			Element * parent = new ImageApplet			// load dir map
				( theFiles[nElem].name, "C:\\ALM\\3DRP\\dir.png", NULL,
				x,y,z, 1.3f,1,1, 0,angle-90,0 );
			char * path = new char[256];
			if (strcmp(dir,"C:\\") == 0)
				strcat(strcpy(path,dir), theFiles[nElem].name);
			else
				strcat(strcat(strcpy(path,dir),"\\"), theFiles[nElem].name);
			elementArray[nElem] = new Element3DList(parent, path);
			_chdir(dirpath);						// return to this list dir

			continue;
		}

		ptr = strchr(theFiles[nElem].name, '.');	// get the extension of the file

		if (! ptr)	{								// without extension ?
			elementArray[nElem] = new ImageApplet	// create default image
				( theFiles[nElem].name, "C:\\ALM\\3DRP\\default.png", NULL,
				x,y,z, 1.3f,1,1, 0,angle-90,0 );
			continue;
		}

		if (strcmp(ptr,".png") == 0 || strcmp(ptr,".PNG") == 0 ) {				// its a .png file?
			elementArray[nElem] = new ImageApplet	// load .png image file
				( theFiles[nElem].name, theFiles[nElem].name, NULL,
				x,y,z, 1.3f,1,1, 0,angle-90,0 );
			continue;
		}
		
		elementArray[nElem] = new ImageApplet		// create default image
			( theFiles[nElem].name, "C:\\ALM\\3DRP\\default.png", NULL,
			x,y,z, 1.3f,1,1, 0,angle-90,0 );


	}
	elementArray[nElem] = NULL;	// null element list terminator

	size = nFiles - 1;			// Cero based element list size value
}



/* Destructor
/*		Removes memory allocations.
/***********************************************/
Element3DList::~Element3DList()
{

//REVISE DELETE WITH BASE CLASS TYPE(ERROR THROWN) !!!!!!!!!!!!!!!
	for (int i=0; elementArray[i]; i++)
		delete elementArray[i];

	if (pParentElement)	delete pParentElement;

	delete dirpath;
}


/* init()
/*		Initializes the list states, the parent
/*		and its created elements.
/***********************************************/
void Element3DList::init()
{

	// init parent element
	if (pParentElement)	pParentElement->init();

	// send this action across the element list
	for (int i=0; elementArray[i]; i++) {
		if (elementArray[i]->getState(ELIST) && dirpath)	{
			_chdir(dirpath); 
		}
		elementArray[i]->init();
	}


}





/* getSize()
/*		Retrieves the number of elements in
/*		this list
/***********************************************/
GLint Element3DList::getSize()
{
	return size+1;

}




/* setElementAt( GLint index, Element * element)
/*		Sets an element of the list with
/*		the specified element.
/***********************************************/
void Element3DList::setElementAt( GLint index, Element * element)
{
	elementArray[index] = element;

}



/* getElementAt( GLint index )
/*		Retrieves an element of the list
/***********************************************/
Element * Element3DList::getElementAt( GLint index )
{
	return elementArray[index];

}




/* getState(int stateName)
/*		Returns the state if found or -1 on error
/*	args:	
/*		stateName	the name of the state inquired
/*************************************************/
GLdouble Element3DList::getState(int stateName)
{
	GLdouble retState;

	// returns the states of the list parent if exists
	if (pParentElement) {
		retState = pParentElement->getState(stateName);
		if (retState != -1 )
			return  retState;
	}

	// check for the element-level states
	retState = Element::getState(stateName);
	if (retState != -1 )
		return  retState;

	// here check the list specific states
	if (stateName == ELIST)
		return 1;
	if (stateName == ELIST_ACTIVE)
		return active;
	if (stateName == ELIST_SHOW)
		return showList;
	if (stateName == ELIST_SELECTED_ITEM)
		return selectedItem;

	return -1;
}




/* setState(int stateName, GLdouble value)
/*		Sets the state if found.
/*	args:	
/*		stateName	the name of the state set.
/*		value		the new value of the sate.
/*************************************************/
void Element3DList::setState(int stateName, GLdouble value)
{
	
	if (pParentElement)
		pParentElement->setState(stateName, value);

	if (stateName == EL_SELECTED) {
		(value <= 0L? showList = false : showList = true);
		return;
	}

	// list-level states here
	if (stateName == ELIST_ACTIVE) {
		(value <= 0L? active = false : active = true);
		return;
	}
	if (stateName == ELIST_SHOW) {
		(value <= 0L? showList = false : showList = true);
		return;
	}
	if (stateName == ELIST_TRYSELECT) {		// check if selected id
		checkSelected((GLint) value);		// is in this list
	}
	if (stateName == ELIST_SELECTED_ITEM) {	// change selected item
		if (value < getSize() ) {					
			if (selectedItem >= 0) {		// unselect previous sel.
				getElementAt(selectedItem)->setState(EL_SELECTED, 0);
			}
			if (value >= 0)					// select new item
				getElementAt((GLint)value)->setState(EL_SELECTED,1);
			selectedItem = (GLint) value;
		}

	}

	// check the Element states
	Element3D::setState(stateName, value);


	// send call to elements on list
//	Element * pe;
//	for (int j=0; (pe = getElementAt(j)); j++) {
//		pe->setState(stateName, value);
//	}


}




void Element3DList::checkSelected(GLint id)
{
	Element *pse;
	for (int j=0; (pse = getElementAt(j)); j++) {

		// check for an image plane selection
//		pse = pe->getSubElement(EL_IMAGE_PLANE);
		if (pse->getState(EL_ID) == id) {
			pse->setState(EL_SELECTED, 1);
			setState(ELIST_SELECTED_ITEM, j);
			break;
		}
		// check for sub list selection
		if (pse->getState(ELIST) && pse->getState(ELIST_SHOW)) {

			int selectid = pse->getState(ELIST_SELECTED_ITEM);	// get sub list sel.
			pse->setState(ELIST_TRYSELECT, id);					// try id selected
			selectid -= pse->getState(ELIST_SELECTED_ITEM);		// get the diff.

			if (selectid) {								// if not cero
				pse->setState(EL_SELECTED, 1);			// keep this element
				setState(ELIST_SELECTED_ITEM, j);		// selected		
				break;
			}
		}
		// check for an title plane selection
//		pse = pe->getSubElement(EL_TITLE_PLANE);

//		if (pse->getState(EL_ID) == id) {
//			pse->setState(EL_SELECTED, 1);
//			selected = j;
//		}

		
	}
}






/* draw()
/*		Draws the list elements and its
/*		parent element, if present.
/*************************************************/
void Element3DList::draw()
{
	if (showList) {
		if (pParentElement) {
			// draw new element list extending from
			// the parent elment and scaled down
			GLfloat pLocX, pLocY, pLocZ;
			GLfloat pRotX, pRotY, pRotZ;
			pLocX = pParentElement->getState(EL_LOCX);
			pLocY = pParentElement->getState(EL_LOCY);
			pLocZ = pParentElement->getState(EL_LOCZ);
			pRotX = pParentElement->getState(EL_ROTX);
			pRotY = pParentElement->getState(EL_ROTY);
			pRotZ = pParentElement->getState(EL_ROTZ);

			// Apply transformations in new matrix
			glPushMatrix();	

			glTranslatef( pLocX, pLocY, pLocZ );

			glRotatef( pRotX, 1, 0 , 0 );
			glRotatef( pRotY, 0, 1 , 0 );
			glRotatef( pRotZ, 0, 0 , 1 );
			
			glTranslatef( 0, 0, -1.8f );	// separate from list

			glScalef( 0.3f , 0.3f, 0.3f );
		}

		// draw the frame
		drawDirFrame();

		// send this action across the element list
		for (int i=0; elementArray[i]; i++) {
			elementArray[i]->draw();
		}

		if (pParentElement)		// return to the previous matrix
			glPopMatrix();

	}


	// Draw parent element
	if (pParentElement)	pParentElement->draw();


}




void Element3DList::drawDirFrame()
{
	double pi = 3.1415926535;
	GLfloat x, y, z;

	// Set starting Y pos.
	int rows = ((int)(size+1)/7)-1;
	if ((size+1)%7 > 0) rows++;
	y = rows+1.5;

	// x,z of arc piece
	x = cos(30*pi/180)*5 + 0.75*cos(60*pi/180);
	z = -sin(30*pi/180)*5 + 0.75*sin(60*pi/180);

	glLineStipple(1, 0xAAAA);


	// draw the wire structure
	glPushMatrix();
	for(int i=0;  i<7;  i++) {
		// line settings
		glColor4f( 0.8f, 0.8f, 0.8f, 1 );
		glLineWidth(1.0f);
		glEnable(GL_LINE_STIPPLE);
		// rotation of this column frame
		if (i> 0) {
			glRotatef( 30, 0, 1 , 0 );
		}

		glBegin(GL_LINES);
		// vertical lines
		glVertex3f(5,y, 0.75f);
		glVertex3f(5,-y, 0.75f);

		glVertex3f(5,y,-0.75f);
		glVertex3f(5,-y,-0.75f);
		// Horiz. lines
		for (GLfloat h=y-2; h > -y; h -= 2) {

			glVertex3f(5, h-0.1f,  0.75f);
			glVertex3f(5, h-0.1f, -0.75f);
			glVertex3f(5, h+1.1f,  0.75f);
			glVertex3f(5, h+1.1f, -0.75f);

			if (i!=6) {
				glVertex3f(5, h-0.1f, -0.75f);
				glVertex3f(x, h-0.1f,  z);

				glVertex3f(5, h+1.1f, -0.75f);
				glVertex3f(x, h+1.1f,  z);
			}
		}
		glEnd();

		// new lines settings
		glColor4f( 1.0f, 1.0f, 1.0f, 1 );
		glDisable(GL_LINE_STIPPLE);
		// frame curves
		glBegin(GL_LINES);
		glVertex3f(5, y,  0.75f);
		glVertex3f(5, y, -0.75f);

		glVertex3f(5, -y,  0.75f);
		glVertex3f(5, -y, -0.75f);
		if (i!=6) {
			glVertex3f(5, y, -0.75f);
			glVertex3f(x, y,  z);

			glVertex3f(5, -y, -0.75f);
			glVertex3f(x, -y,  z);
		}
		glEnd();

		// corner lines
		glLineWidth(2.0f);
		for (h=y-2; h > -y; h -= 2) {
			glBegin(GL_LINES);
			glVertex3f(5, h-0.1f, -0.70f);
			glVertex3f(5, h-0.1f, -0.80f);

			glVertex3f(5, h+1.1f, -0.70f);
			glVertex3f(5, h+1.1f, -0.80f);

			glVertex3f(5, h-0.1f, 0.70f);
			glVertex3f(5, h-0.1f, 0.80f);

			glVertex3f(5, h+1.1f, 0.70f);
			glVertex3f(5, h+1.1f, 0.80f);
			glEnd(); 
		}
	}
	glPopMatrix();

	glLineWidth(1);

}




/* actkey(int key, int x, int y)
/*		Propagates this action through
/*		all the element list and its parent element.
/***************************************************/
void Element3DList::actkey(int key, int x, int y) 
{
	if (active) {
		// get selected item
		GLint selected = getState(ELIST_SELECTED_ITEM);
		
		switch (key) {
		case ',':
		case '<':		// move next element
			if (++selected == getSize())
				selected = 0;
			break;
		case '.':
		case '>':		// move prev. element
			if (--selected < 0)
				selected = getSize() - 1;
			break;
		case 'o':
		case 'O':		// move 1 row up element
			if ( selected > 7)
				selected -= 7;
			break;
		case 'l':
		case 'L':		// move 1 row down element
			if ( selected < getSize()-8 )
				selected += 7;
			break;
		default:
			break;
		}

		// update selected item
		setState(ELIST_SELECTED_ITEM, selected);
	}



	// send the action to the parent element
	if (pParentElement) { pParentElement->actkey(key, x, y); }

	// send this action across the element list
	for (int i=0; elementArray[i]; i++) {
		elementArray[i]->actkey(key, x, y);
	}

}



/* idle() 
/*		Idle actions must be placed here
/*************************************************/
void Element3DList::idle() 
{
	for (int i=0; elementArray[i]; i++) {
		elementArray[i]->idle();
	}
}



