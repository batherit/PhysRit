#pragma once

#include "Collider.h"

class CCollisionDetector
{
public:
	CCollisionDetector();
	~CCollisionDetector();
};

class C2DCollisionDetector : public CCollisionDetector
{
public :
	C2DCollisionDetector();
	~C2DCollisionDetector();

	static bool C2DIsCollided(C2DColliderCircle circle, C2DColliderRect rect, C2DVector* colPos);
	static bool C2DIsCollided(C2DColliderRect rect, C2DColliderCircle circle, C2DVector* colPos);
};

