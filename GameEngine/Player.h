#pragma once
#include "entity.h"

namespace playerNS
{
	const int EDGE_TOP = -8;
	const int EDGE_BOTTOB = 8;
	const int EDGE_LEFT = -14;
	const int EDGE_RIGHT = 14;
}

class Player :
	public Entity
{
public:
	Player(void);
	~Player(void);
};

