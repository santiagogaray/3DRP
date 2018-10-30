/****************************************************************/
/*  MainFrm.h: 
/*		Definition of MainFrm class singleton.
/*		Handles the GLUT callbacks in a single object 
/*		and hosts the Geometric Data Structure
/****************************************************************/

#include "Camera.h"



class MainFrm  
{
public:
	static MainFrm * getInstance();
	int mainCall(int argc, char** argv);
	Element3DList * getElementList();
	Element3DList * getActiveList();

private:
	static MainFrm * theMainFrame;
	static Camera * camera;
	static Element3DList * pElementList;
	MainFrm();
	~MainFrm();
	static void specialKeys( int key, int x, int y );
	static void keyboard( unsigned char key, int x, int y );
	static void mouse( int button, int state, int x, int y );
	static void motion( int x, int y );
	static void display( void );
	static void myReshape( GLsizei w, GLsizei h );
	static void myinit( void );
	static void idle( void );
	static void processHits(GLint hits, GLuint buffer[]);

};

