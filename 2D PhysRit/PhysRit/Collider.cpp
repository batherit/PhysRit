#include "Collider.h"
#include <math.h>


CCollider::CCollider()
{
}


CCollider::~CCollider()
{
}

C2DCollider::C2DCollider()
{
}


C2DCollider::~C2DCollider()
{
}

void C2DCollider::SetPosition(C2DVector& vector)
{
	m_mtxWorld.m_f31 = vector.m_fX;
	m_mtxWorld.m_f32 = vector.m_fY;
	m_mtxWorld.m_f33 = 1.0f;
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

}

C2DColliderCircle::C2DColliderCircle(float fRadius)
{
	if (fRadius <= 0.0f)
	{
		fRadius = 1.0f;
	}
	else
	{
		m_fRadius = fRadius;
	}
}

C2DColliderCircle::~C2DColliderCircle()
{

}

void C2DColliderCircle::DefineModelInLocal(float fRadius)
{
	if (fRadius <= 0.0f)
	{
		fRadius = 1.0f;
	}
	else
	{
		m_fRadius = fRadius;
	}
}

float C2DColliderCircle::GetRadius(void)
{
	return m_fRadius;
}

C2DColliderRect::C2DColliderRect()
{

}

C2DColliderRect::C2DColliderRect(float fWidth, float fHeight)
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
}

C2DColliderRect::~C2DColliderRect()
{

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
