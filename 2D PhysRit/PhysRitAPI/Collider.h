#pragma once
#include "VectorOperation.h"
//#include "PhysicalOperation.h"
#include "C2DCamera.h"
#include<Windows.h>

class CCollider
{
protected:
	float m_fMass;				// kg

public:
	CCollider();
	CCollider(float);
	virtual ~CCollider();

	void SetMass(float fMass);
	float GetMass(void);
	virtual void Update(float fElapsedTime) {}
	virtual void RenderAPI(HDC hdc, C2DMatrix *AssColsM = nullptr, C2DCamera *pCamera = nullptr) {}
};

class C2DCollider : public CCollider
{
protected:					
	C2DVector m_vRelative;		// 질량 중심에서 콜라이더로의 벡터 (콜라이더셋) ; 어셈블드콜라이더즈 사용 시 유효
	float m_fIzz;				// z축에 대한 회전 관성							; 질량, 위치 그리고 모양이 정의되어야 함
	float m_fAngularA;			// z축에 대한 각가속도 ; rad/s^2
	float m_fAngularV;			// z축에 대한 갱신 각속도 ; rad/s
	C2DVector m_vLinearA;		// xy평면에 대한 선가속도
	C2DVector m_vLinearV;		// xy평면에 대한 선속도 ; m/s
	//C2DKinematicProcessUnit m_Kinematic;

public:
	C2DMatrix m_mtxWorld;		// 종속된 좌표계에서 표현되는 위치 및 회전 정보를 담은 행렬

public:
	C2DCollider();
	C2DCollider(float);
	~C2DCollider();

	void SetRelative(C2DVector& vector);
	void SetPosition(C2DVector& vector);
	void SetAngularA(float fAngularA);
	void SetAngularV(float fAngularV);
	void SetLinearA(C2DVector& vLinearA);
	void SetLinearV(C2DVector& vLinearV);
	
	float GetIzz(void);
	C2DVector GetRelative(void);
	C2DVector GetPosition(void);
	float GetAngularA(void);
	float GetAngularV(void);
	C2DVector GetLinearA(void);
	C2DVector GetLinearV(void);
	C2DVector GetWorldUpV(void);
	C2DVector GetWorldRightV(void);

	void Move(C2DVector& vector);
	void RotateZ(float fRadian);

	void Update(float fElapsedTime);		// 주어진 각속도, 선속도에 근거하여 회전운동과 선운동을 갱신.
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

	void RenderAPI(HDC hdc, C2DMatrix *AssColsM, C2DCamera *pCamera);		// 간접 가상 함수는 어떻게 작동할까?
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

	void RenderAPI(HDC hdc, C2DMatrix *AssColsM, C2DCamera *pCamera);
};