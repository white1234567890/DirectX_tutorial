//You Oyadomari
//Kokusai Denshi Business College
//Initial 2017/2/17
//LastUpdate 2017/4/5

#include "Entity.h"

//////////////////////////////////////////////////////////////////////////////
//constructor
//////////////////////////////////////////////////////////////////////////////
Entity::Entity(void) : Image()
{
	radius = 1.0f;
	edge.left = -1;
	edge.top = -1;
	edge.right = 1;
	edge.bottom = 1;
	mass = 1.0f;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	deltaV.x = 0.0f;
	deltaV.y = 0.0f;
	active = true;	//defaulte entity is active
	rotatedBoxReady = false;
	collisionType = entityNS::CIRCLE;
	health = 100.0f;
	gravity = entityNS::GRAVITY;
	pixelColliding = 0;
}

//////////////////////////////////////////////////////////////////////////////
//destructor
//////////////////////////////////////////////////////////////////////////////
Entity::~Entity(void)
{
}

//////////////////////////////////////////////////////////////////////////////
//circular collision detection method
//this function is called by collision(). default collision detection method
//post:	return true if collision, false otherwise
//			if collision, set collisionVector
//////////////////////////////////////////////////////////////////////////////
bool Entity::collideCircle(Entity &ent, VECTOR2 &collisionVector)
{
	//if which one is not active, don't collide
	if(!active || !ent.getActive()) return false;

	//distance between centers
	distSquared = *getCenter() - *ent.getCenter();
	//square difference
	distSquared.x = distSquared.x * distSquared.x;
	distSquared.y = distSquared.y * distSquared.y;
	
	//sum radii(adjust for scale)
	sumRadiiSquared = (radius * getScale()) + (ent.radius * ent.getScale());
	sumRadiiSquared *= sumRadiiSquared;	//square

	//if entity collide
	if(distSquared.x + distSquared.y <= sumRadiiSquared)
	{
		//set collisionVector
		collisionVector = *ent.getCenter() - *getCenter();
		return true;
	}

	return false;	//not collision
}

//////////////////////////////////////////////////////////////////////////////
//axis aligned box collision detection method
//this function is called by collision()
//post:	return true if collision, false otherwise
//			if collision, set collisionVector
//////////////////////////////////////////////////////////////////////////////
bool Entity::collideBox(Entity &ent, VECTOR2 &collisionVector)
{
	//if which one is not active, don't collide
	if(!active || !ent.getActive()) return false;

	//check by axis aligned box
	if((getCenterX() + edge.right * getScale() >= ent.getCenterX() + ent.getEdge().left * ent.getScale()) &&
		(getCenterX() + edge.left * getScale() <= ent.getCenterX() + ent.getEdge().right * ent.getScale()) &&
		(getCenterY() + edge.bottom * getScale() >= ent.getCenterY() + ent.getEdge().top * ent.getScale()) &&
		(getCenterY() + edge.top * getScale() <= ent.getCenterY() + ent.getEdge().bottom * ent.getScale()))
	{
		//set collisionVector
		collisionVector = *ent.getCenter() - *getCenter();
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////
//separating axis collision detection between boxes method
//this function is called by collision()
//post:	return true if collision, false otherwise
//			if collision, set collisionVector
//detect collision to use separating axis test
//separating axis test:	if two boxes projections don't overlap, they don't collide
//this method that store collision vector to use center of box is not correct, but speedy
//////////////////////////////////////////////////////////////////////////////
bool Entity::collideRotatedBox(Entity &ent, VECTOR2 collisionVector)
{
	computeRotatedBox();	//prepare rotated box
	ent.computeRotatedBox();	//prepare rotated box
	if(projectionOverlap(ent, collisionVector) && ent.projectionOverlap(*this, collisionVector))
	{
		//set collisionVector
		collisionVector = *ent.getCenter() - *getCenter();
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////
//separating axis collision detection between box and circle method
//called by collision()
//detect collision to use separatinf axis test box edges and circle radius
//if circle center is in voronoi area (out side of expand edges)
//check collision between ciecle and nearest corner
//nearest corner is detected from projection overlap
//
//voronoi0 |      | voronoi1
//        ---0---1---
//             |       |
//        ---3---2---
//voronoi3 |      | voronoi2
//
//pre:	this entity must be box, opponent entity must be circle
//post:	return true if collide, false otherwise
//if collide, set collisionVector
//////////////////////////////////////////////////////////////////////////////
bool Entity::collideRotateBoxCircle(Entity & ent, VECTOR2 collisionVector)
{
	float min01, min03, max01, max03, center01, center03;
	
	computeRotatedBox();	//prepare rotated box
	
	//project to edge01 circle center
	center01 = graphics->Vector2Dot(&edge01, ent.getCenter());
	//min and max are radius from center
	min01 = center01 - ent.getRadius() * ent.getScale();
	max01 = center01 + ent.getRadius() * ent.getScale();
	//if projections don't overlap
	if(min01 > edge01Max || min01 < edge01Min) return false;	//don't collide

	//project to edge03 circle center
	center03 = graphics->Vector2Dot(&edge03, ent.getCenter());
	//min and max are radius from center
	min03 = center03 - ent.getRadius() * ent.getScale();
	max03 = center03 + ent.getRadius() * ent.getScale();
	//if projections don't overlap
	if(min03 > edge03Max || min03 < edge03Min) return false;	//don't collide

	//if circle projection overlap box projection
	//check circle is voronoi area

	//center is voronoi0
	if(center01 < edge01Min && center03 < edge03Min) return collideCornerCircle(corners[0], ent, collisionVector);
	//center is voronoi1
	if(center01 < edge01Max && center03 < edge03Min) return collideCornerCircle(corners[1], ent, collisionVector);
	//center is voronoi2
	if(center01 < edge01Max && center03 < edge03Max) return collideCornerCircle(corners[2], ent, collisionVector);
	//center is voronoi3
	if(center01 < edge01Max && center03 < edge03Min) return collideCornerCircle(corners[3], ent, collisionVector);

	//if circle is voronoi area, circle collide a box edge
	//set collisionVector, from circle center to box center
	collisionVector = *ent.getCenter() - *getCenter();

	return true;
}

//////////////////////////////////////////////////////////////////////////////
//calculate rotated box vertexes, projection ray, and min and max of projection
//vertexes number
//0---1
//|	      |
//3---2
//////////////////////////////////////////////////////////////////////////////
void Entity::computeRotatedBox()
{
	if(rotatedBoxReady) return;
	
	float projection;

	//create vector x,y from Entity angle
	VECTOR2 rotatedX(cos(spriteData.angle), sin(spriteData.angle));
	VECTOR2 rotatedY(-sin(spriteData.angle), cos(spriteData.angle));

	//calculate vertexes
	const VECTOR2 *center = getCenter();
	corners[0] = *center + rotatedX * ((float)edge.left * getScale()) + rotatedY * ((float)edge.top * getScale());
	corners[1] = *center + rotatedX * ((float)edge.right * getScale()) + rotatedY * ((float)edge.top * getScale());
	corners[2] = *center + rotatedX * ((float)edge.right * getScale()) + rotatedY * ((float)edge.bottom * getScale());
	corners[3] = *center + rotatedX * ((float)edge.left * getScale()) + rotatedY * ((float)edge.bottom * getScale());

	//create two projection rays
	//use corners[0] as base
	//use edges from corners[0] as projection rays
	edge01 = VECTOR2(corners[1].x - corners[0].x, corners[1].y - corners[0].y);
	graphics->Vector2Normalize(&edge01);
	edge03 = VECTOR2(corners[3].x - corners[0].x, corners[3].y - corners[0].y);
	graphics->Vector2Normalize(&edge03);

	//project the entitiy to projection rays
	//min and max when this entity project to projaction ray
	projection = graphics->Vector2Dot(&edge01, &corners[0]);
	edge01Min = projection;
	edge01Max = projection;

	//project to edge01
	projection = graphics->Vector2Dot(&edge01, &corners[1]);
	if(projection < edge01Min)
	{
		edge01Min = projection;
	}
	else if(projection > edge01Max)
	{
		edge01Max = projection;
	}

	//min and max when this entity project to projaction ray
	projection = graphics->Vector2Dot(&edge03, &corners[0]);
	edge03Min = projection;
	edge03Max = projection;

	//project to edge03
	projection = graphics->Vector2Dot(&edge03, & corners[3]);
	if(projection < edge03Min)
	{
		edge03Min = projection;
	}
	else if(projection > edge03Max)
	{
		edge03Max = projection;
	}
	
	//this entity's projection min and max is corners[1] and corners[3]
	//so check projection only about corners[1] and corners[3] in this entity
	rotatedBoxReady = true;
}

//////////////////////////////////////////////////////////////////////////////
//project to edge01 and edge03 opponent entity
//called by colllideRotatedBox()
//post:	if projection overlap, return true. return false otherwise
//////////////////////////////////////////////////////////////////////////////
bool Entity::projectionOverlap(Entity &ent, VECTOR2 &collisionVector)
{
	float projection, min01, max01, min03, max03;
	
	//project to edge01 opponent box
	//prject corner 0
	projection = graphics->Vector2Dot(&edge01, ent.getCorner(0));
	min01 = projection;
	max01 = projection;

	//processing other corners
	for(int c = 1; c < 4; c++)
	{
		//project to edge01
		projection = graphics->Vector2Dot(&edge01, ent.getCorner(c));
		if(projection < min01)
		{
			min01 = projection;
		}
		else if(projection > max01)
		{
			max01 = projection;
		}
	}
	
	//if projection don't overlap
	if(min01 > edge01Max || max01 < edge01Min) return false;	//don't collide
	
	//project to edge03 opponent box
	//prject corner 0
	projection = graphics->Vector2Dot(&edge03, ent.getCorner(0));
	min03 = projection;
	max03 = projection;

	//processing other corners
	for(int c = 1; c < 4; c++)
	{
		//project to edge03
		projection = graphics->Vector2Dot(&edge03, ent.getCorner(c));
		if(projection < min03)
		{
			min03 = projection;
		}
		else if(projection > max03)
		{
			max03 = projection;
		}
	}

	//if projection don't overlap
	if(min03 > edge03Max || max03 < edge03Min) return false;	//don't collide

	return true;	//projections overlap
}

//////////////////////////////////////////////////////////////////////////////
//check box corner collide with circle that use distance
//called by collideRotateBoxCircle()
//post:	return true if collide, false otherwise
//if collide, set collsionVector
//////////////////////////////////////////////////////////////////////////////
bool Entity::collideCornerCircle(VECTOR2 corner, Entity &ent, VECTOR2 &collisionVector)
{
	distSquared = corner - *ent.getCenter();	//corner - circle
	//square difference
	distSquared.x = distSquared.x * distSquared.x;
	distSquared.y = distSquared.y * distSquared.y;

	//culculate sum radius, then square it
	sumRadiiSquared = ent.getRadius() * ent.getScale();	//(0 + radius)
	sumRadiiSquared *= sumRadiiSquared;

	//if collide corner and circle
	if(distSquared.x + distSquared.y <= sumRadiiSquared)
	{
		//set collisionVector
		collisionVector = *ent.getCenter() - corner;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////
//pixel perfect collision detection method
//called by collision()
//if the graphics card does not support stencil buffer 
//then CIRCLE collision is used
//post:	return true if collisoin, false otherwise
//			set collisionVector if collision
//////////////////////////////////////////////////////////////////////////////
bool Entity::collidePixelPerfect(Entity &ent, VECTOR2 collisionVector)
{
	//if stencil not supported
	if(graphics->getStencilSupport() == false)	
	{
		return (collideCircle(ent, collisionVector));	//use CIRCLE collision
	}

	//get fresh texture because they may have been released
	ent.spriteData.texture = ent.textureManager->getTexture();
	spriteData.texture = textureManager->getTexture();

	//if pixel are colliding
	pixelColliding = graphics->pixelCollision(ent.getSpriteInfo(), this->getSpriteInfo());
	if(pixelColliding > 0)
	{
		//set collision vector to center of entity
		collisionVector = *ent.getCenter() - *getCenter();
		collisionCenter = *getCenter();
		ent.setCollisionCenter(*ent.getCenter());
		return true;
	}
	return false;
}



//////////////////////////////////////////////////////////////////////////////
//Update entity
//typically called once per frame
//frameTime:	used to regulate the speed of move
//add deltaV to velocity, then clear deltaV 
//////////////////////////////////////////////////////////////////////////////
void Entity::update(float frameTime)
{
	velocity += deltaV;
	deltaV.x = 0.0f;
	deltaV.y = 0.0f;
	Image::update(frameTime);
	rotatedBoxReady = false;	//for collision detection of ROTATED_BOX
}

//////////////////////////////////////////////////////////////////////////////
//Initialize Entity
//pre:	*gamePtr:	pointer to Game object
//			width:	width of Image in pixels (0 = use full texture width)
//			height:	height of Image in pixels (0 = use full texture height)
//			ncols:	number of columns in texture (1 to n) (0 same as 1)
//			*textureM:	pointer to TextureManager object
//post:	return true if successfully initialize, false otherwise
//////////////////////////////////////////////////////////////////////////////
bool Entity::initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM)
{
	input = gamePtr->getInput();	//input system
	return (Image::initialize(gamePtr->getGraphics(), width, height, ncols,textureM));
}

//////////////////////////////////////////////////////////////////////////////
//activate entity
//////////////////////////////////////////////////////////////////////////////
void Entity::activate()
{
	active = true;
}

//////////////////////////////////////////////////////////////////////////////
//AI(artificial intelligence)
//typically called once per frame
//performs ai calculations, ent is passed for interaction
//this is place holder
//this is not pure virtual function, because I want to Instantiat Entity
//this method is effecially to make prototype early
//////////////////////////////////////////////////////////////////////////////
void Entity::ai(float frameTime, Entity &ent)
{
}

//////////////////////////////////////////////////////////////////////////////
//detect collision this Entiry with another Entity
//each Entity must have one type of collision method
//multiple collision tyeps may be done by treating each part as a separate
//entity or by using  PIXEL_PERFECT collision
//typically called once per frame
//the collision type:	CIRCLE, BOX, ROTATED_BOX or PIXEL_PERFECT
//post:	eturns true if collision, false otherwise
//			sets collisionVector if collision
//////////////////////////////////////////////////////////////////////////////
bool Entity::collidesWith(Entity &ent, VECTOR2 &collisionVector)
{
	//if either entity is not active, don't collide
	if(!active || !ent.getActive()) return false;

	//if each entities are circle
	if(collisionType == entityNS::CIRCLE && ent.getCollisionType() == entityNS::CIRCLE) return collideCircle(ent, collisionVector);

	//if each entities are box
	if(collisionType == entityNS::BOX && ent.getCollisionType() == entityNS::BOX) return collideBox(ent, collisionVector);
	
	//All other combinations use separated axis test
	//Å´Å´Å´Å´Å´Å´Å´Å´Å´Å´

	//each entities do not use circle
	if(collisionType != entityNS::CIRCLE && ent.getCollisionType() != entityNS::CIRCLE)
	{
		return collideRotatedBox(ent, collisionVector);
	}
	//either entity use circle
	else
	{
		//this entity is circle
		if(collisionType == entityNS::CIRCLE) return ent.collideRotateBoxCircle(*this, collisionVector);
		//another entity is circle
		else return collideRotateBoxCircle(ent, collisionVector);
	}

	//Å™Å™Å™Å™Å™Å™Å™Å™Å™Å™

	return false;
}

//////////////////////////////////////////////////////////////////////////////
//check this entity is outside the specified rectangle
//this function is useful to check player is especially area
//post:	return true if this entity is outside rectangle, false otherwise
//////////////////////////////////////////////////////////////////////////////
bool Entity::outsideRect(RECT rect)
{
	if(spriteData.x + spriteData.width * getScale() < rect.left ||
		spriteData.x > rect.right ||
		spriteData.y + spriteData.height * getScale() < rect.top ||
		spriteData.y > rect.bottom)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////////
//Damage
//Damege This Entity with weapon
//must override this function in inherited class
//////////////////////////////////////////////////////////////////////////////
void Entity::damage(int weapon)
{
}

//////////////////////////////////////////////////////////////////////////////
//if collide other entity, then bound
//////////////////////////////////////////////////////////////////////////////
void Entity::bounce(VECTOR2 &collisionVector, Entity &ent)
{
	VECTOR2 Vdiff = ent.getVelocity() - velocity;	//relational velocity
	VECTOR2 cUV = collisionVector;	//collision unit vector
	Graphics::Vector2Normalize(&cUV);
	float cUVdotVdiff = Graphics::Vector2Dot(&cUV, &Vdiff);	//length of reaction vector
	float massRatio = 2.0f;
	if(getMass() != 0)
	{
		//calculate mass ratio
		massRatio *= (ent.getMass() / (getMass() + ent.getMass()));
	}

	//if entity already move to reflected direction, bouns must huve been previously called and they are still colliding
	//moce entities apart along collisionVector
	if(cUVdotVdiff > 0)
	{
		setX(getX() - cUV.x * massRatio);
		setY(getY() - cUV.y * massRatio);
	}
	else
	{
		deltaV += ((massRatio * cUVdotVdiff) * cUV);
	}
}

//////////////////////////////////////////////////////////////////////////////
//gravity to this entity from other entity
//add gravity velocity of this entity
//Force = GRAVITY * m1 * m2 / r^2
//r^2 = (Ax - Bx)^2 + (Ay - By)^2
//////////////////////////////////////////////////////////////////////////////
void Entity::gravityForce(Entity *ent, float frameTime)
{
	//if either entity is not active, do not effect gravity
	if(!activate || !ent->getActive())
		return ;

	rr = pow((ent->getCenterX() - getCenterX()), 2) +
		pow((ent->getCenterY() - getCenterY()), 2);

	force = gravity * ent->getMass() * mass / rr;
}
