#pragma once
#include "VectorOperation.h"

class CCollider
{
protected:
	float m_fMass;

public:
	CCollider();
	CCollider(float);
	virtual ~CCollider();

	void SetMass(float fMass);
	float GetMass(void);
};

class C2DCollider : public CCollider
{
protected:					
	C2DVector m_vRelative;		// 질량 중심에서 콜라이더로의 벡터 (콜라이더셋) ; 어셈블드콜라이더즈 사용 시 유효
	float m_fIzz;				// z축에 대한 회전 관성							; 질량, 위치 그리고 모양이 정의되어야 함

public:
	C2DMatrix m_mtxWorld;		// 종속된 좌표계에서 표현되는 위치 및 회전 정보를 담은 행렬

public:
	C2DCollider();
	C2DCollider(float);
	~C2DCollider();

	void SetRelative(C2DVector& vector);
	void SetPosition(C2DVector& vector);

	float GetIzz(void);
	
	C2DVector GetRelative(void);
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

private:
	void CalculateInertiaTensor(void);
	
public:
	C2DColliderCircle();
	C2DColliderCircle(float, float);
	~C2DColliderCircle();

	void DefineModelInLocal(float fRadius);
	float GetRadius(void);
};

class C2DColliderRect : public C2DCollider
{
private:
	float m_fWidth;
	float m_fHeight;

private:
	void CalculateInertiaTensor(void);

public:
	C2DColliderRect();
	C2DColliderRect(float, float, float);
	~C2DColliderRect();

	void DefineModelInLocal(float fWidth, float fHeight);
	float GetWidth(void);
	float GetHeight(void);
};