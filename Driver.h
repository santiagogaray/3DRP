/*****************************************************************/
/*  Driver.h: 
/*  Purpose:		Driver used to test the MainFrm features
/*****************************************************************/


#include "MainFrm.h"


class Driver 
{

public:
	Driver();
	~Driver();
	static void display(int argc, char** argv);

private:
	static MainFrm * theMainFrame;


};



/////// Excercise for Turbine Entertainent

// NO PRESENTAR =========================================

// Timer class

class Timer
{
public:
	static double GetTime();
};


//========================================================

// data types
typedef unsigned int InstanceID;
typedef unsigned int EffectType;


// global definitions

const InstanceID Invalid_IID = 0x0;

// Effect types

const EffectType Undef_EffectType    = 0x0;
const EffectType Slash_EffectType    = 0x1;
const EffectType Pierce_EffectType   = (1 << 1);
const EffectType Bludgeon_EffectType = (1 << 2);
const EffectType Fire_EffectType     = (1 << 3);
const EffectType Acid_EffectType     = (1 << 4);
const EffectType Cold_EffectType     = (1 << 5);
const EffectType Electric_EffectType = (1 << 6);

const EffectType Health_EffectType   = (1 << 7);
const EffectType Stamina_EffectType  = (1 << 8);
const EffectType Mana_EffectType     = (1 << 9);

// Effect range values
const int MaxEffectAmount						 =  1000;
const int MinEffectAmount						 = -1000;

const unsigned int MaxEffectVariance =  100;
const unsigned int MinEffectVariance =  0;

const double MaxEffectRate					 =  10.0;	//in seconds
const double MinEffectRate					 =  0.1;	//in seconds


// Counts number of entities created
static InstanceID InstanceCounter		 = 0;



/*
===================

LiskedIDList class
------------------
	data structure chosen to handle
	the target objects ids colliding with
	the HotSpot entity.

===================
*/

class LinkedIDList
{
private:
	// the node
	struct node
		{ InstanceID item;  struct node *next; };

	// first,last and temp node
	struct node *head, *z, *t;

	// current node pointer
	struct node *rPos;

public:
	// constructor
	LinkedIDList();

	// destructor
	~LinkedIDList();

	// cleans list items
	void cleanList();

	// sets pointer to first node
	void getHeadPosition();

	// checks if a next node exists in list
	bool isNext();

	// returns the item(ID) of the next node
	InstanceID nextItem();

	// adds a new item to list
	bool insertItem(InstanceID item);

	// deletes a specific item(ID) from list 
	bool removeItem(InstanceID item);

};



/*
===================

Entity class
------------------
	base class of all entities.

===================
*/
class Entity
{
private:
	// entities id 
	const InstanceID ID;

public:
	//constructor
	Entity();

  // returns the unique instance id of the entity
  InstanceID GetID() const;

  virtual void EnterWorld()=0;
  virtual void ExitWorld()=0;
  virtual void HandleCollision( Entity* object )=0;
  virtual void HandleCollisionEnd( Entity* object )=0;  
  virtual void Affect( EffectType type, int amount )=0;
  virtual bool IsPlayer() const =0;
  virtual bool IsAI() const =0;
  virtual bool IsCreature() const =0;
  virtual bool IsItem() const =0;
};


 
/*
===================

Messenger class
---------------
	Used by entites as a message system
	The messages are sheduled and qeued to
	be dispatched at specific server times.

===================
*/

class Messenger
{
public:
	// add the entity to the list of entities
	// receiving scheduled messages.
	// stores a pointer of the entity 
	// in a table(ID, Entity*) for future method calls 
	void AddMessageListener( Entity * e );

	// removes the entity pointer from the entity table 
	// and the message system.
	void RemoveMessageListener( Entity * e );

	// sends an Affect message to a specific entity
	// with the required values at a specified ifime.
	bool SendAffectMessage(	InstanceID target,
						EffectType type,
						int intensity,
						double time = -1.0 ) const;

	// sends an AffectTargets message to a spefific hot spot
	// at a specified server time.
	bool SendAffectTargetsMessage( InstanceID hotSpotTarget, 
													double time = -1.0 ) const;

};

/*
===================================
Effect
	struct used by the Hot Spot entity.
	holds in a single data structure
	all the values needed to apply
	an effect to the target entities.
====================================
*/
struct Effect 
{
		EffectType		type;
		int						amount;		
		unsigned int  variance;
		double				rate;
};



/*
===================

HotSpot class
---------------
	A entity derived class used to periodically
	damage or heal other object colliding with it.

===================
*/

class HotSpot : public Entity, private Messenger
{
private:
	// holds the effect configuration
	Effect theEffect;

	// server time of the next scheduled AffectTargets
	// message dispatch
	double affectTargetsTime;

	// holds the objects' ids colliding with the HotSpot
	LinkedIDList Targets;

	// sets the HotSpot values
	void InitEffect(Effect effect);

public:
	
	// constructor using a list of values
	// to set up the HotSpot entity
	HotSpot(EffectType efType, int efAmount, 
		unsigned int efVar, double efRate );

	// constructor using the Effect struct
	// to set up the HotSpot entity
	HotSpot(Effect effect);

	// HotSpot destructor
	~HotSpot();

	// set-up the HotSpot to be ready to
	// interact with other entities
	void EnterWorld();

	// removes entity from the interaction
	// with other entities
	void ExitWorld();

	// invoked by the phisics system when an entity
	// collides with the HotSpot entity.
	// includes the entity id in the target list
	// in order to receive an affect messages periodically
	void HandleCollision( Entity* object );

	// invoked by the phisics system when an entity
	// stops colliding with the HotSpot entity.
	// removes the entity id from the target list
	// in order to receive an affect messages periodically 
	void HandleCollisionEnd( Entity* object );
	
	// this method is called by the message system 
	// to affect this entity using a specific effect type
	// and amount. 
	void Affect( EffectType type, int amount );

	// this method is called by the message system 
	// periodically to affect all target entities
	// applying the hot spot effect values. 
	void AffectTargets();

	// entity identificacion methods
	bool IsPlayer() const;
	bool IsAI() const;
	bool IsCreature() const;
	bool IsItem() const;


};


