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

C2DCollider::C2DCollider() : m_fIzz(1.0f)
{
}

C2DCollider::C2DCollider(float fMass) : CCollider(fMass)
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

C3DCollider::C3DCollider()
{
}


C3DCollider::~C3DCollider()
{
}
