#pragma once
#include<vector>
#include<algorithm>
#include"VectorOperation.h"
#include"Collider.h"
#include<Windows.h>

class C2DAssembledColliders
{
private:
	std::vector<C2DCollider*> m_fvtColliders;
	float m_fTotalMass;								// 총 질량
	C2DVector m_vCenterOfMass;						// 질량 중심	
	float m_fTotalIzz;								// 총 관성텐서
	float m_fAngularA;			// z축에 대한 각가속도 ; rad/s^2
	float m_fAngularV;			// z축에 대한 갱신 각속도 ; rad/s
	C2DVector m_vLinearA;		// xy평면에 대한 선가속도
	C2DVector m_vLinearV;		// xy평면에 대한 선속도 ; m/s

public:
	C2DMatrix m_mtxCenterOfMass;					
	C2DMatrix m_mtxWorld;
	
private:
	void CalculateInertiaTensor(void);
	
public:
	C2DAssembledColliders();
	~C2DAssembledColliders();

	void SetPosition(C2DVector& vector);
	void SetAngularA(float fAngularA);
	void SetAngularV(float fAngularV);
	void SetLinearA(C2DVector& vLinearA);
	void SetLinearV(C2DVector& vLinearV);

	C2DVector GetPosition(void);
	float GetAngularA(void);
	float GetAngularV(void);
	C2DVector GetLinearA(void);
	C2DVector GetLinearV(void);
	C2DVector GetWorldUpV(void);
	C2DVector GetWorldRightV(void);
	int GetNumOfCollider(void);
	float GetMass(void);
	C2DVector GetCenterOfMass(void);
	float GetIzz(void);

	bool IsIn(C2DCollider *pCollider);

	void AttachCollider(C2DVector inPos, float fRotRadian, C2DCollider* pCollider);
	void DetachCollider(C2DCollider* pCollider);

	void MoveCollider(C2DVector &v, C2DCollider* pCollider);

	void Move(C2DVector& vector);
	void RotateACCoordZ(float fRadian);
	void RotateCMCoordZ(float fRadian);

	void Update(float fElapsedTime);		// 주어진 각속도, 선속도에 근거하여 회전운동과 선운동을 갱신.

	void RenderAPI(HDC hdc, C2DCamera *pCamera = nullptr);
};

