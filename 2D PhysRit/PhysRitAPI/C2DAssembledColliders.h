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
	float m_fTotalMass;								// �� ����
	C2DVector m_vCenterOfMass;						// ���� �߽�	
	float m_fTotalIzz;								// �� �����ټ�
	float m_fAngularA;			// z�࿡ ���� �����ӵ� ; rad/s^2
	float m_fAngularV;			// z�࿡ ���� ���� ���ӵ� ; rad/s
	C2DVector m_vLinearA;		// xy��鿡 ���� �����ӵ�
	C2DVector m_vLinearV;		// xy��鿡 ���� ���ӵ� ; m/s

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

	void Update(float fElapsedTime);		// �־��� ���ӵ�, ���ӵ��� �ٰ��Ͽ� ȸ����� ����� ����.

	void RenderAPI(HDC hdc, C2DCamera *pCamera = nullptr);
};

