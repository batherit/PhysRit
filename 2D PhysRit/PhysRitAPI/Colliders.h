#pragma once
#include<vector>
#include<algorithm>
#include"VectorOperation.h"
#include"Collider.h"
#include<Windows.h>

class C2DColliders : public C2DCollider
{
private:
	std::vector<C2DCollider*> m_fvtColliders;
	C2DVector m_vCenterOfMass;						// 질량 중심	

private:
	void CalculateInertiaTensor(void);

public:
	C2DColliders();
	C2DColliders(UINT EF_Flags);
	virtual ~C2DColliders();

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
	int GetNumOfColliders(void);
	C2DCollider* GetColliderByIndex(int idx);
	float GetMass(void);
	C2DVector GetCenterOfMass(void);
	float GetIzz(void);

	bool IsIn(C2DCollider *pCollider);

	void AttachCollider(C2DVector inPos, float fRotRadian, C2DCollider* pCollider);
	void DetachCollider(C2DCollider* pCollider);

	void MoveCollider(C2DVector &v, C2DCollider* pCollider);

	void Move(C2DVector& vector);
	void RotateACCoordZ(float fRadian);			// 로컬 중심 회전
	void RotateCMCoordZ(float fRadian);			// 질량 중심 회전

	/*bool IsCollided_Index(int idx, C2DColliderCircle* circle, C2DVector* colPos);
	bool IsCollided_Index(int idx, C2DColliderRect* rect, C2DVector* colPos);
	bool IsCollided_Index(int idx, C2DColliders* colliders, C2DVector* colPos);*/
	bool IsCollided_(C2DColliderCircle* circle, C2DVector* colPos);
	bool IsCollided_(C2DColliderRect* rect, C2DVector* colPos);
	bool IsCollided_(C2DColliders* colliders, C2DVector* colPos);
	void Impurse(C2DColliderCircle *pCircle, float fRest, float fFric, C2DVector* colPos);
	void Impurse(C2DColliderRect *pRect, float fRest, float fFric, C2DVector* colPos);
	void Impurse(C2DColliders *pColliders, float fRest, float fFric, C2DVector* colPos);
	void Update(float fElapsedTime);		// 주어진 각속도, 선속도에 근거하여 회전운동과 선운동을 갱신.

	void RenderAPI(HDC hdc, C2DCamera *pCamera = nullptr);
	void RenderAPI(HDC hdc, C2DMatrix *AssColsM = nullptr, C2DCamera *pCamera = nullptr);
};

