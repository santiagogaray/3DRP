/**********************************************/
/* Texture.cpp:
/*	Implementation of the Texture class  
/**********************************************/
#include <math.h>
#include "SubElements.h"
#include <direct.h>




//GLuint  Element3D::textName;


GLfloat * Texture::mapCube;

// the Texture Table
struct textureInfo * Texture::textTable[MAXTEXTURES];



Texture::Texture() {

	if (! mapCube)  initStatic();

	name      = "default";
	imageFile = "C:\\ALM\\3DRP\\default.png";
	img       = NULL;
	subImg    = NULL;
	border	  = 0;

}



Texture::Texture(char * name, Image * img, Image * subImg,GLint border)
{

	if (! mapCube)  initStatic();
		
	this->name      = name;
	this->img       = img;
	this->subImg    = subImg;
	this->border	= border;
	this->imageFile = NULL;

}



Texture::Texture(char * name, char * imageFile, GLint border)
{

	if (! mapCube)  initStatic();

	this->name      = name;
	this->imageFile = imageFile;
	this->border	= border;
	this->img       = NULL;
	this->subImg    = NULL;

}



Texture::~Texture()
{

	if (img)      delete   img;
	if (subImg)   delete   subImg;

	if (mapCube)  delete[] mapCube;

	for (int i=0; textTable[i] && i < MAXTEXTURES; i++) {
		delete textTable[i];
	}

}



void Texture::initStatic()
{

	textTable[0] = NULL;

	// XXXXXXXX Convert this to a mapping table!!!!! XXXXXXXXXXXXXXXXXXXXXXX
	GLfloat mapping[8] = { 1, 1, 0, 0,	 0, 0, 1, 0.5f};
	mapCube = new GLfloat[8];
	for (int i=0; i < 8; i++) {
		mapCube[i] = mapping[i];
	}


}




void Texture::init()
{

	textureInfo * storedTex;

	// look for already stored texture or create one
	if ( (storedTex=getTexture( name, imageFile )) == NULL ) {

		// create image
		if (imageFile)						  // load the image
			loadImageFile( imageFile );		  // from disk ?
													
		img->init();						  // init. the images 
		if ( subImg ) subImg->init();

		setTexture2D(&textName, img, subImg, border); // and create texture

		addTexture(name, img, subImg, imageFile, textName);

	} else {
		// copy data from existing image
		this->img       = storedTex->img;
		this->subImg    = storedTex->subImg;
		this->imageFile = storedTex->imageFile;
		this->textName  = storedTex->textName;

	}


}




textureInfo * Texture::getTexture(char * name, char * imageFile) 
{
	
	for (int i=0; textTable[i] && i < MAXTEXTURES; i++) {
		if (name && textTable[i]->name){
			if ( strcmp(textTable[i]->name, name) == 0)
				return textTable[i];
		}
		if (imageFile && textTable[i]->imageFile) {
			if ( strcmp(textTable[i]->imageFile, imageFile) == 0 )
				return textTable[i];
		}
	}


	return NULL;

}




void Texture::addTexture(char * name, Image * img, Image * subImg,
		char * imageFile, GLuint textName)
{

	textureInfo * newTex  = new textureInfo();

	newTex->name      = name;
	newTex->img       = img;
	newTex->subImg    = subImg;
	newTex->imageFile = imageFile;
	newTex->textName  = textName;

	for (int i=0; textTable[i]  && i < MAXTEXTURES; i++)
		;

	textTable[i] = newTex;
	textTable[i+1] = NULL;

}



void Texture::loadImageFile( char * pngFile )
{

	pngRawInfo info;

	if ( pngLoadRaw(pngFile, &info) == -1)
		exit(1);

	GLint comps;
		if ( info.Alpha == 8 )	comps = 4;
	else
		if ( info.Alpha == 0 )	comps = 3;
	
	img = new Image( info.Width, info.Height, comps );

	img->buffer = info.Data;

	
}




pngInfo Texture::loadTexture2D(GLuint * textName, char * pngFile)
{
	
	pngInfo info;

	glGenTextures(1, textName);

	glBindTexture(GL_TEXTURE_2D, *textName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	if (!pngLoad( pngFile , PNG_NOMIPMAP, PNG_ALPHA , &info)) 
			exit(1);
	return info;

}




void Texture::setTexture2D(GLuint * textName, Image * img,
						   Image * subImg, GLint border)
{

	// Generate texture with this image data
	glGenTextures( 1, textName );
	glBindTexture( GL_TEXTURE_2D, *textName );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	// rgb and rgba support
	GLint format = ( img->nComp == 3? GL_RGB : GL_RGBA );

	//////// with border doesn't work !!!!!!
	glTexImage2D( GL_TEXTURE_2D, 0, format, img->width+2*border,
		img->heigth+2*border, border, format, GL_UNSIGNED_BYTE,
		img->buffer );

	// sub image applied in texture if exists 
	// Incomplete (sizes not checked!!!)
	if(subImg) {
		glTexSubImage2D( GL_TEXTURE_2D, 0, 20,20, 32, 32, GL_RGBA,
			GL_UNSIGNED_BYTE, subImg->buffer );
	}


}



void Texture::setMapMode ( int mapMode )
{

	if ( mapMode == MM_AUTO ) {
		glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
		glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
		glTexGenfv( GL_S, GL_OBJECT_PLANE, mapCube );
		glTexGenfv( GL_T, GL_OBJECT_PLANE, mapCube+4 );
		
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}

}




