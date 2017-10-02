#pragma once
#include "VectorOperation.h"

class CCollider
{
public:
	CCollider();
	~CCollider();
};

class C2DCollider : public CCollider
{
public:
	C2DMatrix m_mtxWorld;

public:
	C2DCollider();
	~C2DCollider();

	void SetPosition(C2DVector& vector);
	C2DVector GetPosition(void);
	C2DVector GetWorldUpV(void);
	C2DVector GetWorldRightV(void);

	void RotateZ(float fRadian);
};

class C3DCollider : public CCollider
{
public:
	C3DCollider();
	~C3DCollider();
};
/****************************************************************************************/
/*                                                                                      */		
/*                                 2D 콜라이더                                          */
/*                                                                                      */
/****************************************************************************************/
class C2DColliderCircle : public C2DCollider
{
private:
	float m_fRadius;
	
public:
	C2DColliderCircle();
	C2DColliderCircle(float);
	~C2DColliderCircle();

	void DefineModelInLocal(float fRadius);
	float GetRadius(void);
};

class C2DColliderRect : public C2DCollider
{
private:
	float m_fWidth;
	float m_fHeight;

public:
	C2DColliderRect();
	C2DColliderRect(float, float);
	~C2DColliderRect();

	void DefineModelInLocal(float fWidth, float fHeight);
	float GetWidth(void);
	float GetHeight(void);
};