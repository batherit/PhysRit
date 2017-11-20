#include "Collider.h"
#include "Colliders.h"
#include "CollisionDetector.h"
#include "CollisionSolver.h"
#include <math.h>


CCollider::CCollider() : m_EF_Flags(0), m_fMass(1.0f), m_eType(COLLIDER_TYPE_NONE)
{
}

CCollider::CCollider(float fMass, UINT EF_Flags) : m_EF_Flags(EF_Flags), m_eType(COLLIDER_TYPE_NONE)
{
	if (fMass <= 0.0f)
	{
		m_fMass = 1.0f;
	}
	else
	{
		m_fMass = fMass;
	}
}


CCollider::~CCollider()
{
}

void CCollider::SetMass(float fMass)
{
	if (fMass <= 0.0f)
	{
		m_fMass = 1.0f;
	}
	else
	{
		m_fMass = fMass;
	}
}

float CCollider::GetMass(void)
{
	return m_fMass;
}

UINT CCollider::GetEF_Flags(void)
{
	return m_EF_Flags;
}

COLLIDER_TYPE CCollider::GetType(void)
{
	return m_eType;
}

C2DCollider::C2DCollider() : m_pParent(nullptr), m_fIzz(0.0f), m_fAngularA(0.0f), m_fAngularV(0.0f)
{
}

C2DCollider::C2DCollider(float fMass, UINT EF_Flags) : CCollider(fMass, EF_Flags), m_pParent(nullptr), m_fIzz(0.0f), m_fAngularA(0.0f), m_fAngularV(0.0f)
{

}

C2DCollider::~C2DCollider()
{
}

void C2DCollider::SetParent(C2DCollider* pParent)
{
	m_pParent = pParent;
}

void C2DCollider::SetRelative(C2DVector& vector)
{
	m_vRelative.m_fX = vector.m_fX;
	m_vRelative.m_fY = vector.m_fY;
}

void C2DCollider::SetPosition(C2DVector& vector)
{
	if (m_pParent)
	{
		m_pParent->SetPosition(vector);
	}
	else
	{
		m_mtxWorld.m_f31 = vector.m_fX;
		m_mtxWorld.m_f32 = vector.m_fY;
		m_mtxWorld.m_f33 = 1.0f;
	}
}

void C2DCollider::SetAngularA(float fAngularA)
{
	if (m_pParent)
	{
		m_pParent->SetAngularV(fAngularA);
	}
	else
	{
		m_fAngularV = fAngularA;
	}
}

void C2DCollider::SetAngularV(float fAngularV)
{
	if (m_pParent)
	{
		m_pParent->SetAngularV(fAngularV);
	}
	else
	{
		m_fAngularV = fAngularV;
	}
}

void C2DCollider::SetLinearA(C2DVector& vLinearA)
{
	if (m_pParent)
	{
		m_pParent->SetLinearA(vLinearA);
	}
	else
	{
		m_vLinearA.Set(vLinearA);
	}
}

void C2DCollider::SetLinearV(C2DVector& vLinearV)
{
	if (m_pParent)
	{
		m_pParent->SetLinearV(vLinearV);
	}
	else
	{
		m_vLinearV.Set(vLinearV);
	}
}

void C2DCollider::Move(C2DVector& vector)
{
	if (m_pParent)
	{
		m_pParent->Move(vector);
	}
	else
	{
		m_mtxWorld.m_f31 += vector.GetX();
		m_mtxWorld.m_f32 += vector.GetY();
		m_mtxWorld.m_f33 = 1.0f;
	}
}

C2DCollider* C2DCollider::GetParent(void)
{
	return m_pParent;
}

float C2DCollider::GetIzz(void)
{
	return m_fIzz;
}

C2DVector C2DCollider::GetRelative(void)
{
	return m_vRelative;
}

C2DVector C2DCollider::GetPosition(void)
{
	C2DVector vPos(m_mtxWorld.m_f31, m_mtxWorld.m_f32);

	if (m_pParent) CVectorOperation::C2DTransform(&vPos, &vPos, &m_pParent->m_mtxWorld);

	return vPos;
}

float C2DCollider::GetAngularA(void)
{
	return m_fAngularA;
}

float C2DCollider::GetAngularV(void)
{
	return m_fAngularV;
}

C2DVector C2DCollider::GetLinearA(void)
{
	return m_vLinearA;
}

C2DVector C2DCollider::GetLinearV(void)
{
	return m_vLinearV;
}

C2DVector C2DCollider::GetWorldUpV(void)
{
	return C2DVector(m_mtxWorld.m_f21, m_mtxWorld.m_f22);
}

C2DVector C2DCollider::GetWorldRightV(void)
{
	return C2DVector(m_mtxWorld.m_f11, m_mtxWorld.m_f12);
}

void C2DCollider::RotateZ(float fRadian)
{
	C2DMatrix mtxRotate;

	if (m_pParent)
	{
		m_pParent->RotateZ(fRadian);
	}
	else
	{
		mtxRotate.m_f11 = cos(fRadian); mtxRotate.m_f12 = sin(fRadian); mtxRotate.m_f13 = 0.0f;
		mtxRotate.m_f21 = -sin(fRadian); mtxRotate.m_f22 = cos(fRadian); mtxRotate.m_f23 = 0.0f;

		CVectorOperation::C2DTransform(&this->m_mtxWorld, &mtxRotate, &this->m_mtxWorld);
	}
}

void C2DCollider::Update(float fElapsedTime)
{
	float fUpdatedAngularV = 0.0f;
	float fRad = 0.0f;
	C2DVector vUpdatedLinearV;
	C2DVector vMove;

	fUpdatedAngularV = m_fAngularV + m_fAngularA * fElapsedTime;

	m_vLinearA.m_fY = m_EF_Flags & EF_GRAVITY ?  -GRAVITATIONAL_ACCELERATION_MAGNITUDE : m_vLinearA.m_fY ;
	vUpdatedLinearV = m_vLinearV + m_vLinearA * fElapsedTime;
	
	fRad = (fUpdatedAngularV + m_fAngularV) / 2.0f * fElapsedTime;
	vMove = (vUpdatedLinearV + m_vLinearV) / 2.0f * fElapsedTime;
	
	RotateZ(fRad);
	Move(vMove);

	m_vLinearV = vUpdatedLinearV;
	m_fAngularV = fUpdatedAngularV;
}

C2DColliderCircle::C2DColliderCircle() : m_fRadius(1.0f)
{
	m_eType = COLLIDER_TYPE_CIRCLE;
	CalculateInertiaTensor();
}

C2DColliderCircle::C2DColliderCircle(float fMass, float fRadius, UINT EF_Flags) : C2DCollider(fMass, EF_Flags)
{
	m_eType = COLLIDER_TYPE_CIRCLE;

	if (fRadius <= 0.0f)
	{
		m_fRadius = 1.0f;
	}
	else
	{
		m_fRadius = fRadius;
	}

	CalculateInertiaTensor();
}

C2DColliderCircle::~C2DColliderCircle()
{

}

void C2DColliderCircle::CalculateInertiaTensor(void)
{
	m_fIzz = 0.5f * m_fMass * m_fRadius * m_fRadius;
}

void C2DColliderCircle::DefineModelInLocal(float fRadius)
{
	if (fRadius <= 0.0f)
	{
		m_fRadius = 1.0f;
	}
	else
	{
		m_fRadius = fRadius;
	}

	CalculateInertiaTensor();
}

float C2DColliderCircle::GetRadius(void)
{
	return m_fRadius;
}

bool C2DColliderCircle::IsCollided_(C2DColliderCircle* circle, C2DVector* colPos)
{
	return C2DCollisionDetector::C2DIsCollided_(*this, *circle, colPos);
}

bool C2DColliderCircle::IsCollided_(C2DColliderRect* rect, C2DVector* colPos)
{
	return C2DCollisionDetector::C2DIsCollided_(*this, *rect, colPos);
}

bool C2DColliderCircle::IsCollided_(C2DColliders* colliders, C2DVector* colPos)
{
	return C2DCollisionDetector::C2DIsCollided_(*this, *colliders, colPos);
}

void C2DColliderCircle::Impurse(C2DColliderCircle *pCircle, float fRest, float fFric, C2DVector* colPos)
{
	C2DCollisionSolver::C2DImpurse(this, pCircle, fRest, fFric, colPos);
}

void C2DColliderCircle::Impurse(C2DColliderRect *pRect, float fRest, float fFric, C2DVector* colPos)
{

}

void C2DColliderCircle::Impurse(C2DColliders *pColliders, float fRest, float fFric, C2DVector* colPos)
{
	int iNumOfColliders = pColliders->GetNumOfColliders();

	if (iNumOfColliders <= 0) return;
	
	for (int i = 0; i < iNumOfColliders; i++)
	{
		if (pColliders->GetColliderByIndex(i)->IsCollided_(this, colPos))
		{
			pColliders->GetColliderByIndex(i)->Impurse(this, 0.8f, 0.0f, colPos);
		}
	}
}

void C2DColliderCircle::RenderAPI(HDC hdc, C2DMatrix *AssColsM, C2DCamera *pCamera)
{
	C2DMatrix mtxWorld;
	C2DVector vVertex;
	POINT rectVertex[2];

	mtxWorld.Set(m_mtxWorld);

	if (AssColsM)
	{
		CVectorOperation::C2DTransform(&mtxWorld, &m_mtxWorld, AssColsM);
	}

	if (pCamera)
	{
		pCamera->TransformView(&mtxWorld, &mtxWorld);
		pCamera->TransformScale(&mtxWorld, &mtxWorld);
		pCamera->TransformProj(&mtxWorld, &mtxWorld);
		pCamera->TransformScreen(&mtxWorld, &mtxWorld);
	}

	vVertex.Set(-this->GetRadius(), -this->GetRadius());
	if (pCamera)
	{
		pCamera->TransformScale(&vVertex, &vVertex);
		pCamera->TransformProj(&vVertex, &vVertex);
		pCamera->TransformRatio(&vVertex, &vVertex);
	}
	rectVertex[0].x = vVertex.GetX();
	rectVertex[0].y = vVertex.GetY();

	vVertex.Set(this->GetRadius(), this->GetRadius());
	if (pCamera)
	{
		pCamera->TransformScale(&vVertex, &vVertex);
		pCamera->TransformProj(&vVertex, &vVertex);
		pCamera->TransformRatio(&vVertex, &vVertex);
	}
	rectVertex[1].x = vVertex.GetX();
	rectVertex[1].y = vVertex.GetY();

	Ellipse(hdc, mtxWorld.m_f31 + rectVertex[0].x, mtxWorld.m_f32 + rectVertex[0].y, mtxWorld.m_f31 + rectVertex[1].x, mtxWorld.m_f32 + rectVertex[1].y);
}

C2DColliderRect::C2DColliderRect() : m_fWidth(1.0f), m_fHeight(1.0f)
{
	m_eType = COLLIDER_TYPE_RECT;
}

C2DColliderRect::C2DColliderRect(float fMass, float fWidth, float fHeight, UINT EF_Flags) : C2DCollider(fMass, EF_Flags)
{
	m_eType = COLLIDER_TYPE_RECT;

	if (fWidth <= 0.0f)
	{
		m_fWidth = 1.0f;
	}
	else
	{
		m_fWidth = fWidth;
	}

	if (fHeight <= 0.0f)
	{
		m_fHeight = 1.0f;
	}
	else
	{
		m_fHeight = fHeight;
	}

	CalculateInertiaTensor();
}

C2DColliderRect::~C2DColliderRect()
{

}

void C2DColliderRect::CalculateInertiaTensor(void)
{
	m_fIzz = m_fMass / 12.0f * (m_fHeight * m_fHeight + m_fWidth * m_fWidth);
}

void C2DColliderRect::DefineModelInLocal(float fWidth, float fHeight)
{
	if (fWidth <= 0.0f)
	{
		m_fWidth = 1.0f;
	}
	else
	{
		m_fWidth = fWidth;
	}

	if (fHeight <= 0.0f)
	{
		m_fHeight = 1.0f;
	}
	else
	{
		m_fHeight = fHeight;
	}

	CalculateInertiaTensor();
}

float C2DColliderRect::GetWidth(void)
{
	return m_fWidth;
}

float C2DColliderRect::GetHeight(void)
{
	return m_fHeight;
}

bool C2DColliderRect::IsCollided_(C2DColliderCircle* circle, C2DVector* colPos)
{
	return C2DCollisionDetector::C2DIsCollided_(*this, *circle, colPos);
}

bool C2DColliderRect::IsCollided_(C2DColliderRect* rect, C2DVector* colPos)
{
	return C2DCollisionDetector::C2DIsCollided_(*this, *rect, colPos);
}

bool C2DColliderRect::IsCollided_(C2DColliders* colliders, C2DVector* colPos)
{
	return C2DCollisionDetector::C2DIsCollided_(*this, *colliders, colPos);
}


void C2DColliderRect::Impurse(C2DColliderCircle *pCircle, float fRest, float fFric, C2DVector* colPos)
{

}

void C2DColliderRect::Impurse(C2DColliderRect *pRect, float fRest, float fFric, C2DVector* colPos)
{

}

void C2DColliderRect::Impurse(C2DColliders *pColliders, float fRest, float fFric, C2DVector* colPos)
{

}

void C2DColliderRect::RenderAPI(HDC hdc, C2DMatrix *AssColsM, C2DCamera *pCamera)
{
	C2DMatrix mtxWorld;
	C2DVector vVertex;
	POINT rectVertex[4];

	mtxWorld.Set(m_mtxWorld);

	if (AssColsM)
	{
		CVectorOperation::C2DTransform(&mtxWorld, &m_mtxWorld, AssColsM);
	}

	if (pCamera)
	{
		pCamera->TransformView(&mtxWorld, &mtxWorld);
		pCamera->TransformScale(&mtxWorld, &mtxWorld);
		pCamera->TransformProj(&mtxWorld, &mtxWorld);
		pCamera->TransformScreen(&mtxWorld, &mtxWorld);
	}

	vVertex.Set(-this->GetWidth() / 2.0f, this->GetHeight() / 2.0f);
	CVectorOperation::C2DTransform(&vVertex, &vVertex, &mtxWorld);
	rectVertex[0].x = vVertex.GetX();
	rectVertex[0].y = vVertex.GetY();
	
	vVertex.Set(this->GetWidth() / 2.0f, this->GetHeight() / 2.0f);
	CVectorOperation::C2DTransform(&vVertex, &vVertex, &mtxWorld);
	rectVertex[1].x = vVertex.GetX();
	rectVertex[1].y = vVertex.GetY();

	vVertex.Set(this->GetWidth() / 2.0f, -this->GetHeight() / 2.0f);
	CVectorOperation::C2DTransform(&vVertex, &vVertex, &mtxWorld);
	rectVertex[2].x = vVertex.GetX();
	rectVertex[2].y = vVertex.GetY();

	vVertex.Set(-this->GetWidth() / 2.0f, -this->GetHeight() / 2.0f);
	CVectorOperation::C2DTransform(&vVertex, &vVertex, &mtxWorld);
	rectVertex[3].x = vVertex.GetX();
	rectVertex[3].y = vVertex.GetY();

	Polygon(hdc, rectVertex, 4);
}

C3DCollider::C3DCollider()
{
}


C3DCollider::~C3DCollider()
{
}
