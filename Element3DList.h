/* Element3DList ********************************************/
/* Purpose:		Defines a collection of elements as a		*/
/*				derived Element3D class	, enabling nested	*/
/*				collections in the data structure.			*/
/************************************************************/
#ifndef __element3dlist_h__
#define __element3dlist_h__


#include "SubElements.h"
#include <io.h>
#include <direct.h>




/* max number of elements created */
#define MAXELEMENTS	512



class Element3DList :  public Element3D
{

public:
	Element3DList(Element * parent, char * dir=NULL);
	~Element3DList();
	void draw(void);
	void init(void);
	void actkey(int key, int x, int y);
	void idle(void);
	void populateList(char *dir);
	GLdouble getState(int stateName);
	void setState(int stateName, GLdouble value);
	void setElementAt( GLint index, Element * element);
	Element * getElementAt( GLint index );
	GLint getSize();
protected:
	Element * elementArray[MAXELEMENTS];
	Element * pParentElement;
	GLint	size;
	GLint	selectedItem;
	char *  dirpath;
	struct	_finddata_t theFiles[MAXFILES];
	bool showList;
	bool active;
	void drawDirFrame();
	void checkSelected(GLint id);	
};


#endif                  /* __elelment3dlist_h__ */
