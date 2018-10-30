/*********************************************************/
/* Driver.cpp: 
/*	implementation of the Driver class.
/*********************************************************/

#include "Driver.h"
#include "assert.h"

MainFrm * Driver::theMainFrame;


Driver::Driver()
{

}


Driver::~Driver()
{

}





/* Used this section as driver code for creating	*/ 
/* and testing the graphic elements implemented		*/
void Driver::display(int argc, char** argv)
{
	MainFrm * m = m->getInstance();
	m->mainCall(argc, argv);


}





/////// Excercise for Turbine Entertainent



// NO PRESENTAR =========================================

#include <time.h>
// Timer class methods

double Timer::GetTime()
{
	time_t theTime;
	return (double)time(&theTime);

}


// Messanger class methods

Entity *theEntities;

void Messenger::AddMessageListener(Entity * e)
{
	theEntities  =  e;
}

void Messenger::RemoveMessageListener(Entity * e)
{
	theEntities  =  NULL;
}

bool Messenger::SendAffectMessage(	InstanceID target, 
													EffectType type, int intensity,
													double time ) const
{
	theEntities->Affect(type, intensity);
	return true;
}

bool Messenger::SendAffectTargetsMessage(
													InstanceID hotSpotTarget, 
													double time ) const
{
	HotSpot *hp = (HotSpot *) theEntities;
		hp->AffectTargets();
	return true;
}





//===============================================
//LinkedIDList methods



/*
==================
LinkedIDList()
	constructor. creates the initial nodes
	and sets the position pointer
	on the first node.
==================
*/
LinkedIDList::LinkedIDList()
{
	head = new node;
	z    = new node;
	head->next = z; z->next = z;
	head->item = z->item = Invalid_IID;
	rPos = head;
}

/*
==================
~LinkedIDList() 
	destructor. cleans the list
==================
*/
LinkedIDList::~LinkedIDList()
{
	cleanList();
}


/*
==================
cleanList()
	deletes all existing
	nodes from the list
==================
*/
void LinkedIDList::cleanList()
{
	struct node *temp1, *temp2 = head;

	while (temp2 != temp2->next) {
		temp1 = temp2;
		temp2 = temp2->next;
		delete temp1;
	}
	delete z;
}


/*
==================
getHeadPosition()
	sets the position pointer
	in the first node(head)
==================
*/
void LinkedIDList::getHeadPosition()
{
	rPos = head;
}

/*
==================
isNext()
	returns true only if there is
	another element after the
	current position
==================
*/
bool LinkedIDList::isNext()
{
	if (rPos->next != rPos->next->next)
		return true;
	else
		return false;
}


/*
==================
nextItem()
	returns the InstanceID value of the
	next item of the list
==================
*/
InstanceID LinkedIDList::nextItem()
{
	rPos = rPos->next;
	return rPos->item;
}


/*
==================
insertItem( InstanceID item )
	adds a new node at the
	end of the list containing
	the specified id item.
==================
*/
bool LinkedIDList::insertItem( InstanceID item )
{
	struct node *temp = head;

	// go to last node of list
	while (true) {
		if (temp->item == item)		// check for id duplication
			return false;						// return if id already stored
		if (temp->next == temp->next->next) // if reached the end
			break;														// exit loop
		temp = temp->next;				// keep traversing list 
	}

	// insert new node
	t = new node;
	t->item = item; t->next = z;
	temp->next = t;

	return true;
}


/*
==================
removeItem( InstanceID item )
	deletes a node from the list
	if contains the specified
	id item.
==================
*/
bool LinkedIDList::removeItem( InstanceID item )
{
	struct node *temp = head;

	// look for item in list
	while (true) {
		if (temp->next == temp->next->next)	// if end of list
			return false;											// exit without changes
		if (temp->next->item == item)				// if id found
			break;														// break loop 
		temp = temp->next;									// or keep traversing list
	}

	// remove item from list
	t = temp->next;
	temp->next = temp->next->next;
	delete t;

	return true;
}



//===============================================
// Entity methods

/*
==================
Entity() 
	constructor. defines the ID value
	of the entity.
==================
*/
Entity::Entity() : ID(++InstanceCounter) { }


/*
==================
GetID()
	returns id value
	of the entity
==================
*/
InstanceID Entity::GetID() const
{
	return ID;
}





//===============================================
// HotSpot methods



/*
==================
HotSpot(EffectType efType,  int efAmount, 
				unsigned int efVar, double efRate )
	constructor. defines a hot spot entity with
	the following parameters:
			efType	 effect type
			efAmount amount of the effect
			efVar		 effect variation
			efRate	 effect rate in seconds
==================
*/
HotSpot::HotSpot(EffectType efType,  int efAmount, 
								 unsigned int efVar, double efRate )
{
	Effect effect = {efType, efAmount, efVar, efRate};
	InitEffect(effect);
}



/*
==================
HotSpot( Effect effect ) 
	constructor. defines a hot spot entity 
	specifying an effect struct
==================
*/
HotSpot::HotSpot(Effect effect)
{

	InitEffect(effect);

}



/*
==================
InitEffect( Effect effect )
	Checks effect values passed as a
	parameter and copies its values
	to the effect data member
==================
*/
void HotSpot::InitEffect( Effect effect )
{

	// check the effect type
	// frexp()returns 0.5 only for (2 raised to n) values
	// n stores the exponent (only 1 to 10 are valid, 10 effect types)

	int     n;	
	double  ret = frexp( double(effect.type), &n );
  assert (ret == 0.5 && n <= 10 ); //Invalid effect type found 

	// check the effect amount
	assert (effect.amount <= MaxEffectAmount &&
		effect.amount >= MinEffectAmount);

	if (n < 8 )										// if first 7 effect types
		assert(effect.amount > 0);	// amount must be always negative

	// check the effect variance
	assert (effect.variance <= MaxEffectVariance &&
		effect.variance >= MinEffectVariance);

	// check the effect rate
	assert (effect.rate <= MaxEffectRate &&
		effect.rate >= MinEffectRate);

	// assign values to data member
	theEffect.type     =  effect.type;
	theEffect.amount   =  effect.amount;
	theEffect.variance =  effect.variance;
	theEffect.rate     =  effect.rate;
}



/*
==================
EnterWorld()
	registers this entity with
	the message system and starts the
	AffectTarget message loop.
==================
*/
void HotSpot::EnterWorld()
{

	//register entity as a message listener
	// (Messenger class method)
	AddMessageListener(this);

	// set the next message time
	affectTargetsTime = Timer::GetTime() + theEffect.rate;

	// start the AffectTarget message loop
	// try sending message 3 times, if not successful
	// send error message to stderr and continue

	bool retVal = false;
	for (int i=0; !retVal && i<3; i++) {
		retVal = SendAffectTargetsMessage( GetID(), affectTargetsTime );
	}
	if (!retVal) {
		fprintf(stderr, "error: AffectTarget message could not be sent");
		fprintf(stderr, "				current time: %.4f, Message time: %.4f",
			Timer::GetTime(), affectTargetsTime);
	}

}



/*
==================
ExitWorld()
	unregisters this entity from the message 
	listeners list, in order to be ready
	for deletion.
==================
*/
void HotSpot::ExitWorld()
{
	// unregister object from the message 
	// listeners list (Messenger class method)

	RemoveMessageListener(this);

}



/*
==================
HandleCollision( Entity* object )
	includes a specified entity in the target
	entities list.
	only creatures (both players and AIs) can
	be handled for collision.
==================
*/
void HotSpot::HandleCollision( Entity* object )
{
	// check for entity types.
	// if an invalid entity is found send an error message
	// to stderr and return

	if (! object->IsCreature()) {	
		fprintf(stderr, 
			"HandleCollision: error - The entity must be a creature");
		return;
	}

	// try inserting the entity id into the list
	// if id already stored send an error message to
	// stderr and continue

	if ( !Targets.insertItem(object->GetID()) ) {
		fprintf(stderr, 
			"HandleCollision: error - The entity id is already stored");
	}

}



/*
==================
HandleCollisionEnd( Entity* object )
	excludes a specified entity from the target
	entities list.
==================
*/
void HotSpot::HandleCollisionEnd( Entity* object )
{
	// try deleting the entity id from the list or
	// send an error message to stderr and continue
	if ( !Targets.removeItem(object->GetID()) ) {
		fprintf(stderr, 
			"HandleCollisionEnd: error - The entity could not be deleted");
	}

}


/*
==================
Affect( EffectType type, int amount )
	Applies an amount of a given effect
	to this entity.
==================
*/
void HotSpot::Affect( EffectType type, int amount )
{
	// to design: 
	// as an extension of the HotSpot entity
	// an Affect method could be implemented 
	// where the effect amound could be used to
	// reduce or increase the hot spot's effect intensity.
	// this would enable animated effect amounts during
	// the hot spot life.
}


/*
==================
AffectTargets()
	Apply the HotSpot effect to all the
	target entities.
	re-send a scheduled AffectTargets message
	following the effect rate value in order to 
	continue the effect loop	
==================
*/
void HotSpot::AffectTargets()
{
	// calculate effect intensity to send to target entities
	// the variance ranges from
	// (-)theEffect.variance/2 to (+)theEffect.variance/2

  srand( (unsigned)time( NULL ) );
	int variance    = rand()%(theEffect.variance + 1) - 
		theEffect.variance/2;
	int intensity   = theEffect.amount + variance;

	// send effect to all target entities
	Targets.getHeadPosition();
	while (Targets.isNext()) {
		InstanceID id = Targets.nextItem();
		SendAffectMessage(id, theEffect.type, intensity, -1);
	}

	// send new AffectTargets message to continue
	// with the AffectTarget message loop

	// update next message time
	affectTargetsTime = Timer::GetTime() + theEffect.rate;
	// try sending message 3 times,
	bool retVal = false;
	for (int i=0; !retVal && i<3; i++) {
		retVal = SendAffectTargetsMessage( GetID(), affectTargetsTime );
	}
	//if no message sent print error message to stderr and continue
	if (!retVal) {	
		fprintf(stderr, "error: AffectTarget message could not be sent");
		fprintf(stderr, "				current time: %.4f, Message time: %.4f",
			Timer::GetTime(), affectTargetsTime);
	}

}


/*
==================
IsPlayer()
	entity identification method
==================
*/
bool HotSpot::IsPlayer() const
{
	return false;
}


/*
==================
IsAI()
	entity identification method 
==================
*/
bool HotSpot::IsAI() const
{
	return false;
}


/*
==================
IsCreature()
	entity identification method 
==================
*/
bool HotSpot::IsCreature() const
{
	return false;
}


/*
==================
IsItem()
	entity identification method 
	includes the HotSpot entity type
==================
*/
bool HotSpot::IsItem() const
{
	return true;
}



