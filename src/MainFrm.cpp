/************************************************************************/
/* MainFrm.cpp:
/*	Implementation of the MainFrm class singleton.
/************************************************************************/


#include "MainFrm.h"


/* display settings definitions */
#define SET_SHADING		GL_FLAT
#define SET_DEPTH_TEST
//#define SET_ANTIALIAS
//#define SET_CULLING	GL_BACK


/* global states */
GLfloat LAST_FRAME_TICKS = 0;
GLfloat LAST_KEY_TIME    = 0;


///// MainFrm States //////////////////////////////////////

// This singleton instance
MainFrm * MainFrm::theMainFrame;

// the camera
Camera * MainFrm::camera;

// the element list
Element3DList * MainFrm::pElementList;	


///// MainFrm Functions ///////////////////////////////////





/* Constructor
/* Sets initial values of camera
/* Creates run-itime array of elements
/***************************************/
MainFrm::MainFrm()
{

	pElementList  = // create list
		new Element3DList(NULL,"C:\\ALM\\images");
	pElementList->setState(ELIST_ACTIVE, 1);
	pElementList->setState(ELIST_SHOW, 1);
	// create camera w/observer & target coords.
	camera = new Camera(0,0,6, 0,0,0);

}





/* Destructor
/* Cleans memory allocation
/*****************************/
MainFrm::~MainFrm()
{
	// clean memory alloc.
	delete camera;
	delete pElementList;
	delete theMainFrame;
}





/* getInstance()
/*		Returns the MainFrm singleton
/*		object as a reference.
/*		If object doesn't exist, it creates one first.
/*******************************************************/
MainFrm * MainFrm::getInstance() 
{

	if (theMainFrame == NULL) {
		theMainFrame = new MainFrm();
		if (theMainFrame == NULL) {
			fprintf(stderr,"Out of Memory");
			exit(1);
		}
	}

	return theMainFrame;
}





/* mainCall(int argc, char** argv)
/*		The entry point of the OpenGL visualization system.
/*		Initializes the glut library and the display window.
/*		Sets all the callback funtions and starts the loop.
/************************************************************/ 
int MainFrm::mainCall(int argc, char** argv)
{
	
	glutInit( &argc,argv );
	glutInitWindowSize( 800,600 );
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );
	glutCreateWindow( "3DRP" );
	
	
	myinit ();
	glutReshapeFunc( myReshape );
	glutKeyboardFunc( keyboard );
	glutSpecialFunc( specialKeys );
	glutMouseFunc( mouse );
	glutMotionFunc( motion );
	glutDisplayFunc( display );
	glutIdleFunc(idle);

	glutMainLoop();

	return (0);
}




Element3DList * MainFrm::getElementList()
{
	return pElementList;
}



Element3DList * MainFrm::getActiveList()
{
	Element3DList * list = pElementList;
	while (true) {
		if (list->getState(ELIST_ACTIVE) == 1 )
			break;
		int index;
		if ( (index = list->getState(ELIST_SELECTED_ITEM)) >= 0) {
			if (list->getElementAt(index)->getState(ELIST) != -1) {
				list =(Element3DList *) list->getElementAt(index);
			}
		}
	}
	return list;
}



/* myinit(void)
/*		The Main Frame general initialization.
/*		Sets visualization modes.
/*		Initializes the elements created.
/**********************************************/
void MainFrm::myinit(void)
{

	// settings

	glShadeModel(SET_SHADING);

#ifdef SET_DEPTH_TEST
	glEnable(GL_DEPTH_TEST);
#endif

#ifdef SET_ANTIALIAS
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
#endif

#ifdef SET_CULLING
	glCullFace(SET_CULLING);
	glEnable(GL_CULL_FACE);
#endif

	// Gets OpenGl impl. info
	int info;
	glGetIntegerv(GL_AUX_BUFFERS, &info);
	const char * myImplInfo;
	myImplInfo =(const char *) glGetString(GL_RENDERER);
	myImplInfo =(const char *) glGetString(GL_RENDERER);
	myImplInfo =(const char *) glGetString(GL_EXTENSIONS);
	myImplInfo =(const char *) glGetString(GL_VERSION);
//	if ( strncmp(myImplInfo, "1.1.",4) == 0 ||
//		 strncmp(myImplInfo, "1.0.",4) == 0  ) {
//		fprintf(stderr, "This application needs OpenGl version 1.2\n");
//		exit(1);
//	}

	// Checks maximum texture size in this impl.
	GLint	ret;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &ret);


	// extend the init.call to the camera
	camera->init();
	// send this action across the element list
	pElementList->init();

}



/* myReshape(GLsizei w, GLsizei h)
/*		Window reshape callback
/***********************************/
void MainFrm::myReshape(GLsizei w, GLsizei h)
{
	glViewport(0,0,w,h);
}



/* display(void)
/*		Places the cammera.
/*		Draws the elements created.
/***********************************/
void MainFrm::display(void)
{

	// Get start of frame time
	static int start, end, last;
	start = glutGet(GLUT_ELAPSED_TIME);

	glClearColor (0.0f,0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Viewing Frustum
	gluPerspective(80.0, 1.2, 0.01, 50);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// camera states
	GLfloat lx, ly, lz, tx, ty, tz;
	lx = camera->getState(EL_LOCX);
	ly = camera->getState(EL_LOCY);
	lz = camera->getState(EL_LOCZ);
	tx = camera->getState(EL_TARX);
	ty = camera->getState(EL_TARY);
	tz = camera->getState(EL_TARZ);

	gluLookAt(lx, ly, lz, tx, ty, tz, 0.0, 1.0, 0.0);

	Util * u = u->getInstance();	  
	u->drawTripod(1);

	// send this action across the element list
	pElementList->draw();

	// shows cam loc. on screen 
	u->text2(1, 1, "camX: %2.2f, camY: %2.2f, camZ: %2.2f"
		, lx, ly, lz);
	// Display fps
	if ( (end - last) > 0 ) {
		LAST_FRAME_TICKS = end - last;
		u->text2(300, 1, "%.2f fps", 1000.0/LAST_FRAME_TICKS);
		last = start;
	}
	glutSwapBuffers();

	// End of frame *after* swap buffers
	end = glutGet(GLUT_ELAPSED_TIME);


}




/* keyboard (unsigned char key, int x, int y)
/*		Sends a actKey call to every element
/*		found in the list. 
/****************************************************/
void MainFrm::keyboard (unsigned char key, int x, int y)
{


	switch (key) {
	// Exit from application
	case 'q':
	case 'Q':
		exit (0);
		break;
	default:
		break;
	}

	// get last key elapse time
	LAST_KEY_TIME = glutGet(GLUT_ELAPSED_TIME) - LAST_KEY_TIME;

	// extend this key event across the element list
	pElementList->actkey(key, x, y);
	// check camera interaction
	camera->actkey(key, x, y);

	// update last key time 
  	LAST_KEY_TIME = glutGet(GLUT_ELAPSED_TIME);

	// update scene
	glutPostRedisplay();

}




/* specialKeys(int key, int x, int y)
/*		Handles arrow key strokes to 
/*		modify camera x-y position.
/**************************************/
void MainFrm::specialKeys(int key, int x, int y)
{
	LAST_KEY_TIME = glutGet(GLUT_ELAPSED_TIME) - LAST_KEY_TIME;

	camera->actkey(key, x, y);

	// update key time 
  	LAST_KEY_TIME = glutGet(GLUT_ELAPSED_TIME);

	glutPostRedisplay();


}




#define BUFSIZE 512		// the size of the hits buffer

/* mouse( int button, int state, int x, int y )
/*		Performs selection feedback to
/*		check if an objectwas picked.
/*		Processes the hits when found.
/***********************************************/
void MainFrm::mouse( int button, int state, int x, int y )
{

	GLuint	selectBuf[BUFSIZE];
	GLint	hits;
	GLint	viewport[4];

	if ( button != GLUT_LEFT_BUTTON || state != GLUT_DOWN )
		return;

	glGetIntegerv( GL_VIEWPORT, viewport );

	glSelectBuffer(BUFSIZE, selectBuf);
	(void) glRenderMode(GL_SELECT);

	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3]-y),
		5.0, 5.0, viewport);
	gluPerspective(80.0, 1.2, 0.01, 50);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// camera states
	GLfloat lx, ly, lz, tx, ty, tz;
	lx = camera->getState(EL_LOCX);
	ly = camera->getState(EL_LOCY);
	lz = camera->getState(EL_LOCZ);
	tx = camera->getState(EL_TARX);
	ty = camera->getState(EL_TARY);
	tz = camera->getState(EL_TARZ);

	gluLookAt(lx, ly, lz, tx, ty, tz, 0.0, 1.0, 0.0);


	// Draw all built elements to send hits
	pElementList->draw();


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();


	hits = glRenderMode(GL_RENDER);
	if (hits > 0)
		processHits(hits, selectBuf);

}





/* processHits(GLint hits, GLuint buffer[])
/*		Extracts the hits information
/*		and performs a selection search.
/*****************************************************/
void MainFrm::processHits(GLint hits, GLuint buffer[])
{

	int		i, id;
	GLuint	names, j, * ptr;

	printf("hits = %d\n", hits);
	ptr = (GLuint *) buffer;
	for ( i=0; i  < hits; i++ ) {
		names = *ptr;
		printf(" number of names for this hit = %d\n", names);
		ptr++;
		printf(" z1 is %g;", (float) *ptr/0x7fffffff); ptr++;
		printf(" z2 is %g;", (float) *ptr/0x7fffffff); ptr++;
		printf("  names are ");
		for (j = 0; j < names; j++) {
			printf("%d ", *ptr);
			id = (int) *ptr;
			ptr++;
		}
		printf("\n");
	}

	// find the element picked inside de list
	pElementList->setState(ELIST_TRYSELECT, id);

	GLint selitem = pElementList->getState(ELIST_SELECTED_ITEM);
	if ( selitem >= 0 )				// an element item selected
		camera->actkey('l', 0, 0);	// notify camera
		


}





/* motion( int x, int y )
/*
/***********************************************/
void MainFrm::motion( int x, int y )
{

}




/* idle()
/*		Performs actions while app.
/*		enters idle state.
/***********************************************/
void MainFrm::idle()
{

	camera->idle();
	pElementList->idle();

	glutPostRedisplay();

}



