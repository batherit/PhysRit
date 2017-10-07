#pragma once
#include<vector>
#include<algorithm>
#include"VectorOperation.h"
#include"Collider.h"

class C2DAssembledColliders
{
private:
	std::vector<C2DCollider*> m_fvtColliders;
	float m_fTotalMass;								// ÃÑ Áú·®
	C2DVector m_vCenterOfMass;						// Áú·® Áß½É	
	float m_fTotalIzz;								// ÃÑ °ü¼ºÅÙ¼­

public:
	C2DMatrix m_mtxWorld;
	
private:
	void CalculateInertiaTensor(void);
	
public:
	C2DAssembledColliders();
	~C2DAssembledColliders();

	void SetPosition(C2DVector& vector);

	C2DVector GetPosition(void);
	C2DVector GetWorldUpV(void);
	C2DVector GetWorldRightV(void);
	int GetNumOfCollider(void);
	float GetMass(void);
	C2DVector GetCenterOfMass(void);
	float GetIzz(void);

	void AttachCollider(C2DVector inPos, float fRotRadian, C2DCollider* pCollider);
	void DetachCollider(C2DCollider* pCollider);

	void RotateZ(float fRadian);
};

