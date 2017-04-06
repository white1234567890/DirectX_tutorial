//You Oyadomari
//Kokusai Denshi Business College
//Initial 2017/4/5
//LastUpdate 2017/4/5
#pragma once

#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "Input.h"
#include "Game.h"

namespace entityNS
{
	enum COLLISION_TYPE
	{
		NONE,
		CIRCLE,
		BOX,
		ROTATED_BOX,
		PIXEL_PERFEXT
	};
	const float GRAVITY = 6.67428e-11f;	//gravitational constant
}

class Entity :
	public Image
{
	//Entity properties
protected:
	entityNS::COLLISION_TYPE collisionType;
	VECTOR2 center;					//center of entity
	VECTOR2 collisionCenter;	//center of collision
	float radius;						//radius of collision circle
	VECTOR2 distSquared;			//use for caluculating circle collision
	float sumRadiiSquared;		//edge specifies the collision box relaticeto the center
											//left and top are typically negative numbers
	RECT edge;							//for BOX and ROTATED_BOX collision ditection
	VECTOR2 corners[4];			//for ROTATED_BOX collision detection
	VECTOR2 edge01, edge03;		//edge use for projection
	float edge01Min, edge01Max, edge03Min, edge03Max;	//min and max projections
	VECTOR2 velocity;				//velocity
	VECTOR2 deltaV;					//added to velocity during next call to update()
	float mass;						//Mass of entity
	float health;						//health 0 to 100
	float rr;							//Radius square variable
	float force;						//force of gravity
	float gravity;					//gravitational constant of the game universe
	Input *input;						//pointer to input system
	HRESULT hr;						//standard return type
	bool active;						//only active entities may collide
	bool rotatedBoxReady;			//true when rotated collision box is ready
	DWORD pixelColliding;			//number of pixel colliding in pixel perfect collision
	
	//The following functions are protected
	//because they are not intended to be call frome outside the class
	//««««««««««

	//circular collision detection 
	//pre:	&enr:	other entity
	//post:	&collision Vector:	from center to center
	virtual bool collideCircle(Entity &ent, VECTOR2 &collisionVector);

	//axis aligned box collision detection
	//pre:	&ent:	other entity
	//post:	&collisionVector:	frome center to center
	virtual bool collideBox(Entity &ent, VECTOR2 &collisionVector);

	//separating axis collision detection between boxes
	//pre:	&ent:	other entity
	//post:	&collisionVector:	perpendicular to collisoin edge
	virtual bool collideRotatedBox(Entity &ent, VECTOR2 collisionVector);

	//separating axis collision detection between box and circle
	//pre:	&ent:	other entity
	//post:	&collisionVector:	perpendicular to collision edge or center to corner
	//										when collision is with box center
	virtual bool collideRotateBoxCircle(Entity & ent, VECTOR2 collisionVector);

	//separateing axis collision detection helperfunction
	//««««««««««

	//this function must call before BOX or ROTATED_BOX collision detection 
	void computeRotatedBox();
	bool projectionOverlap(Entity &ent, VECTOR2 &collisionVector);
	bool collideCornerCircle(VECTOR2 corner, Entity &ent, VECTOR2 &collisionVector);

	//ªªªªªªªªªª

	//Pixel Perfect collision detection
	//if the graphics card does not support a stencil buffer then CIRCLE collision is used
	//pre:	&ent:	other entity
	//post: &collisionVector:	contains collision vector
	virtual bool collidePixelPerfect(Entity &ent, VECTOR2 collisionVector);

	//ªªªªªªªªªª

public:
	//constructor
	Entity(void);
	//destructor
	virtual ~Entity(void);

	//getter
	//««««««««««

	//return center of scaled Entity as screen x,y
	virtual const VECTOR2* getCenter()
	{
		center = VECTOR2(getCenterX(), getCenterY());
		return &center;
	}

	//return collision center
	virtual const VECTOR2* getCollisionCenter() {return &collisionCenter;}

	//return radius of collision circle
	virtual float getRadius() const {return radius;}

	//return RECT structure used for BOX and ROTATE_BOX collision detection
	virtual const RECT& getEdge() const {return edge;}

	//return corner c of ROTATED_BOX
	virtual const VECTOR2* getCorner(UINT c) const
	{
		if(c >= 4)
		{
			c = 0;
		}
		return &corners[c];
	}

	//return velocity vector
	virtual const VECTOR2 getVelocity() const {return velocity;}

	//return active
	virtual bool getActive() const {return active;}

	//return mass
	virtual float getMass() const {return mass;}

	//return gravitational constatn
	virtual float getGravity() const {return gravity;}

	//return health;
	virtual float getHealth() const {return health;}

	//return collision type
	virtual entityNS::COLLISION_TYPE getCollisionType() {return collisionType;}

	//return number of pixel colliding in pixel perfect collision
	virtual DWORD getPixelsColllision() const {return pixelColliding;}

	//ªªªªªªªªªª

	//setter
	//««««««««««

	//set velocity
	virtual void setVelocity(VECTOR2 v) {velocity = v;}

	//set delta velocity
	//added to velocity in update()
	virtual void setDeltaV(VECTOR2 dv) {deltaV = dv;}

	//set active
	virtual void setActive(bool a) {active = a;}

	//set health
	virtual void setHealth(float h) {health = h;}

	//set mass
	virtual void setMass(float m) {mass = m;}

	//set gravitational constant
	//default is 6.67428e - 11
	virtual void setGravity(float g) {gravity = g;}

	//set radius of collision circle
	virtual void setCollisionRadius(float r) {radius = r;}

	//set collision center
	virtual void setCollisionCenter(VECTOR2 cc) {collisionCenter = cc;}

	//set collision type
	virtual void setCollisionType(entityNS::COLLISION_TYPE ctype) {collisionType = ctype;}

	//set RECT structure used for BOX and ROTATED_BOX collision detection
	virtual void setEdge(RECT e) {edge = e;}

	//set rotatedBoxReady
	//set to false to force recalculation
	virtual void setRotateBoxReady(bool r) {rotatedBoxReady = r;}

	//ªªªªªªªªªª

	//update entity
	//typically called once per frame
	//frameTime is used to regulate the speed of move
	virtual void update(float frameTime);

	//Initialize Entity
	//pre:	*gamePtr:	pointer to Game object
	//			width:	width of Image in pixels (0 = use full texture width)
	//			height:	height of Image in pixels (0 = use full texture height)
	//			ncols:	number of columns in texture (1 to n) (0 same as 1)
	//			*textureM:	pointer to TextureManager object
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM);

	//activate entity
	virtual void activate();

	//empty ai function to allow Entity objects to be instantiated
	virtual void ai(float frameTime, Entity &ent);

	//Is this entity outside the specified rectangle?
	virtual bool outsideRect(RECT rect);

	//Does this entity collide with ent?
	virtual bool collidesWith(Entity &ent, VECTOR2 &collisionVector);

	//Damege This Entity with weapon
	virtual void damage(int weapon);

	//Entity bounces after collision with other Entity
	void bounce(VECTOR2 &collisionVector, Entity &ent);

	//Adds the gravitational force to the velocity vector of this entity
	void gravityForce(Entity &other, float frameTime);
};

