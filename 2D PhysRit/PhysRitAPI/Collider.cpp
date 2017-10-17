#include "Collider.h"
#include <math.h>


CCollider::CCollider() : m_fMass(1.0f)
{
}

CCollider::CCollider(float fMass)
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

C2DCollider::C2DCollider() : m_fIzz(1.0f), m_fAngularA(0.0f), m_fAngularV(0.0f)
{
}

C2DCollider::C2DCollider(float fMass) : CCollider(fMass), m_fIzz(1.0f), m_fAngularA(0.0f), m_fAngularV(0.0f)
{

}

C2DCollider::~C2DCollider()
{
}

void C2DCollider::SetRelative(C2DVector& vector)
{
	m_vRelative.m_fX = vector.m_fX;
	m_vRelative.m_fY = vector.m_fY;
}

void C2DCollider::SetPosition(C2DVector& vector)
{
	m_mtxWorld.m_f31 = vector.m_fX;
	m_mtxWorld.m_f32 = vector.m_fY;
	m_mtxWorld.m_f33 = 1.0f;
}

void C2DCollider::SetAngularA(float fAngularA)
{
	m_fAngularA = fAngularA;
}

void C2DCollider::SetAngularV(float fAngularV)
{
	m_fAngularV = fAngularV;
}

void C2DCollider::SetLinearA(C2DVector& vLinearA)
{
	m_vLinearA.Set(vLinearA);
}

void C2DCollider::SetLinearV(C2DVector& vLinearV)
{
	m_vLinearV.Set(vLinearV);
}

void C2DCollider::Move(C2DVector& vector)
{
	m_mtxWorld.m_f31 += vector.GetX();
	m_mtxWorld.m_f32 += vector.GetY();
	m_mtxWorld.m_f33 = 1.0f;
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
	return C2DVector(m_mtxWorld.m_f31, m_mtxWorld.m_f32);
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
	
	mtxRotate.m_f11 = cos(fRadian); mtxRotate.m_f12 = sin(fRadian); mtxRotate.m_f13 = 0.0f;
	mtxRotate.m_f21 = -sin(fRadian); mtxRotate.m_f22 = cos(fRadian); mtxRotate.m_f23 = 0.0f;

	CVectorOperation::C2DTransform(&this->m_mtxWorld, &mtxRotate, &this->m_mtxWorld);
}

void C2DCollider::Update(float fElapsedTime)
{
	float fUpdatedAngularV = 0.0f;
	float fRad = 0.0f;
	C2DVector vUpdatedLinearV;
	C2DVector vMove;

	fUpdatedAngularV = m_fAngularV + m_fAngularA * fElapsedTime;
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
	CalculateInertiaTensor();
}

C2DColliderCircle::C2DColliderCircle(float fMass, float fRadius) : C2DCollider(fMass)
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

}

C2DColliderRect::C2DColliderRect(float fMass, float fWidth, float fHeight) : C2DCollider(fMass)
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
