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

	/*                            원과 원의 충돌                            */
	static bool C2DIsCollided_(C2DColliderCircle circle1, C2DColliderCircle circle2, C2DVector* colPos);
	static bool C2DIsCollided_(C2DVector vPos1, float fR1, C2DVector vPos2, float fR2, C2DVector* colPos);
	static bool C2DIsCollided_(float fX1, float fY1, float fR1, float fX2, float fY2, float fR2, C2DVector* colPos);

	/*                            원과 사각형의 충돌                            */	
	static bool C2DIsCollided_(C2DColliderCircle circle, C2DColliderRect rect, C2DVector* colPos);
	static bool C2DIsCollided_(C2DColliderRect rect, C2DColliderCircle circle, C2DVector* colPos);

	/*                            사각형과 사각형의 충돌                            */
	static bool C2DIsCollided_Nearest(C2DColliderRect rect1, C2DColliderRect rect2, C2DVector* colPos);
	static bool C2DIsCollided_NearestOBB(C2DColliderRect rect1, C2DColliderRect rect2, C2DVector* colPos);
	static bool C2DIsCollided_OBB(C2DColliderRect rect1, C2DColliderRect rect2, C2DVector* colPos);
};

