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

	/*                            ���� ���� �浹                            */
	static void C2DImpurse(C2DColliderCircle *pCir1, C2DColliderCircle *pCir2, float fRest, float fFric, C2DVector* colPos);

	/*                            �簢���� ���� �浹                            */
	static void C2DImpurse(C2DColliderCircle *pCir, C2DColliderRect *pRect, float fRest, float fFric, C2DVector* colPos);
	static void C2DImpurse(C2DColliderRect *pRect, C2DColliderCircle *pCir, float fRest, float fFric, C2DVector* colPos);

	/*                            �簢���� �簢���� �浹                            */
	static void C2DImpurse(C2DColliderRect *pRect1, C2DColliderRect *pRect2, float fRest, float fFric, C2DVector* colPos);

	/*                            ���� �ݶ��̴����� �浹                            */
	static void C2DImpurse(C2DColliderCircle *pCir, C2DColliders *pCols, float fRest, float fFric, C2DVector* colPos);
	static void C2DImpurse(C2DColliders *pCols, C2DColliderCircle *pCir, float fRest, float fFric, C2DVector* colPos);

	/*                            �簢���� �ݶ��̴����� �浹                            */
	static void C2DImpurse(C2DColliderRect *pRect, C2DColliders *pCols, float fRest, float fFric, C2DVector* colPos);
	static void C2DImpurse(C2DColliders *pCols, C2DColliderRect *pRect, float fRest, float fFric, C2DVector* colPos);

	/*                            �ݶ��̴���� �ݶ��̴����� �浹                            */
	static void C2DImpurse(C2DColliders *pCols1, C2DColliders *pCols2, float fRest, float fFric, C2DVector* colPos);
};