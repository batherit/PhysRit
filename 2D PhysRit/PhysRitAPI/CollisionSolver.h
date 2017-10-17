#pragma once
class CCollisionSolver
{
public:
	CCollisionSolver();
	~CCollisionSolver();
};

class C2DCollisionSolver : public CCollisionSolver
{
public:
	C2DCollisionSolver();
	~C2DCollisionSolver();

	/*                            원과 원의 충돌                            */
	static void C2DImpurse(C2DColliderCircle *pCir1, C2DColliderCircle *pCir2, float fRest, float fFric, C2DVector* colPos);
	static void C2DImpurse(C2DColliderCircle *pCir, C2DColliderRect *pRect, float fRest, float fFric, C2DVector* colPos);
	static void C2DImpurse(C2DColliderRect *pRect, C2DColliderCircle *pCir, float fRest, float fFric, C2DVector* colPos);
	static void C2DImpurse(C2DColliderRect *pRect1, C2DColliderRect *pRect2, float fRest, float fFric, C2DVector* colPos);
};