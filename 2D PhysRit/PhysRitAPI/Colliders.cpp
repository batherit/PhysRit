#include "Colliders.h"
#include "CollisionDetector.h"



C2DColliders::C2DColliders()
{
	m_EF_Flags = 0;
	m_eType = COLLIDER_TYPE_COLLIDERS;
	m_fMass = 0.0f;
	m_fIzz = 0.0f; 
	m_fAngularA = 0.0f;
	m_fAngularV = 0.0f;
}

C2DColliders::C2DColliders(UINT EF_Flags) : C2DCollider(0.0f, EF_Flags)
{
	m_EF_Flags = EF_Flags;
	m_eType = COLLIDER_TYPE_COLLIDERS;
	m_fMass = 0.0f;
	m_fIzz = 0.0f;
	m_fAngularA = 0.0f;
	m_fAngularV = 0.0f;
}


C2DColliders::~C2DColliders()
{
}

void C2DColliders::CalculateInertiaTensor(void)
{
	C2DVector vSumPrimaryMoment;
	C2DVector vOldCenterOfMass;
	C2DVector vRelative;

	m_fMass = 0.0f;
	for (auto &collider : m_fvtColliders)
	{
		m_fMass += collider->GetMass();
		vSumPrimaryMoment += collider->GetPosition() * collider->GetMass();
	}

	vOldCenterOfMass = m_vCenterOfMass;
	m_vCenterOfMass = C2DVector(0.0f, 0.0f);
	/*m_mtxCenterOfMass.Identify();*/

	if (m_fMass > 0.0f)
	{
		m_vCenterOfMass = vSumPrimaryMoment / m_fMass;								// 나누기는 분모 != 0 이라는 사실을 절대 잊지 말 것 'ㅅ'/
																						//m_mtxCenterOfMass.m_f31 = -m_vCenterOfMass.GetX();
																						//m_mtxCenterOfMass.m_f32 = -m_vCenterOfMass.GetY();		// 로컬 좌표계에서 정의되는 질량 중심의 좌표를 일단 행렬에 저장.
	}

	vRelative = m_vCenterOfMass - vOldCenterOfMass;
	CVectorOperation::C2DTransform(&vRelative, &vRelative, &m_mtxWorld);
	vRelative.Set(vRelative.GetX() - m_mtxWorld.m_f31, vRelative.GetY() - m_mtxWorld.m_f32);
	SetLinearV(C2DVector(-vRelative.GetY() * m_fAngularV + m_vLinearV.GetX(), vRelative.GetX() * m_fAngularV + m_vLinearV.GetY()));
	//SetAngularV(m_fAngularA);

	m_fIzz = 0.0f;
	for (auto &collider : m_fvtColliders)
	{
		collider->SetRelative(collider->GetPosition() - m_vCenterOfMass);

		// 축 전송 공식
		m_fIzz +=
			collider->GetIzz() + collider->GetMass()
			* (collider->GetRelative().GetX() * collider->GetRelative().GetX()
				+ collider->GetRelative().GetY() * collider->GetRelative().GetY());
	}
}

void C2DColliders::SetPosition(C2DVector& vector)
{
	m_mtxWorld.m_f31 = vector.m_fX;
	m_mtxWorld.m_f32 = vector.m_fY;
	m_mtxWorld.m_f33 = 1.0f;
}

void C2DColliders::SetAngularA(float fAngularA)
{
	m_fAngularA = fAngularA;
}

void C2DColliders::SetAngularV(float fAngularV)
{
	m_fAngularV = fAngularV;
}

void C2DColliders::SetLinearA(C2DVector& vLinearA)
{
	m_vLinearA.Set(vLinearA);
}

void C2DColliders::SetLinearV(C2DVector& vLinearV)
{
	m_vLinearV.Set(vLinearV);
}

C2DVector C2DColliders::GetPosition(void)
{
	return C2DVector(m_mtxWorld.m_f31, m_mtxWorld.m_f32);
}

float C2DColliders::GetAngularA(void)
{
	return m_fAngularA;
}

float C2DColliders::GetAngularV(void)
{
	return m_fAngularV;
}

C2DVector C2DColliders::GetLinearA(void)
{
	return m_vLinearA;
}

C2DVector C2DColliders::GetLinearV(void)
{
	return m_vLinearV;
}

C2DVector C2DColliders::GetWorldUpV(void)
{
	return C2DVector(m_mtxWorld.m_f21, m_mtxWorld.m_f22);
}

C2DVector C2DColliders::GetWorldRightV(void)
{
	return C2DVector(m_mtxWorld.m_f11, m_mtxWorld.m_f12);
}

int C2DColliders::GetNumOfColliders(void)
{
	return m_fvtColliders.size();
}

C2DCollider* C2DColliders::GetColliderByIndex(int idx)
{
	if (idx < 0 || idx >= m_fvtColliders.size()) return nullptr;
	return m_fvtColliders[idx];
}

float C2DColliders::GetMass(void)
{
	return m_fMass;
}
C2DVector C2DColliders::GetCenterOfMass(void)
{
	return m_vCenterOfMass * m_mtxWorld;
}
float C2DColliders::GetIzz(void)
{
	return m_fIzz;
}

bool C2DColliders::IsIn(C2DCollider *pCollider)
{
	if (!pCollider) return false;

	std::vector<C2DCollider*>::iterator iter;

	if ((iter = std::find(m_fvtColliders.begin(), m_fvtColliders.end(), pCollider)) != m_fvtColliders.end())
	{
		return true;
	}
	return false;
}

void C2DColliders::AttachCollider(C2DVector inPos, float fRotRadian, C2DCollider* pCollider)
{
	if (!pCollider) return;

	pCollider->RotateZ(fRotRadian);
	pCollider->SetPosition(inPos);
	pCollider->SetParent(this);
	m_fvtColliders.push_back(pCollider);			// 어셈블드콜라이더즈 좌표계에 정의됨.

	CalculateInertiaTensor();
}

void C2DColliders::DetachCollider(C2DCollider* pCollider)
{
	if (!pCollider) return;

	std::vector<C2DCollider*>::iterator iter;

	if ((iter = std::find(m_fvtColliders.begin(), m_fvtColliders.end(), pCollider)) != m_fvtColliders.end())
	{
		C2DVector vRelative;
		vRelative.Set((*iter)->GetRelative());
		CVectorOperation::C2DTransform(&vRelative, &vRelative, &m_mtxWorld);
		vRelative.Set(vRelative.GetX() - m_mtxWorld.m_f31, vRelative.GetY() - m_mtxWorld.m_f32);
		(*iter)->SetLinearV(C2DVector(-vRelative.GetY() * m_fAngularV + m_vLinearV.GetX(), vRelative.GetX() * m_fAngularV + m_vLinearV.GetY()));
		(*iter)->SetRelative(C2DVector(0.0f, 0.0f));		// 유일한 콜라이더가 되므로 상대 벡터는 (0, 0);

		CVectorOperation::C2DTransform(&vRelative, &(*iter)->GetPosition(), &m_mtxWorld);
		(*iter)->SetPosition(vRelative);
		(*iter)->SetParent(nullptr);
		m_fvtColliders.erase(iter);

		CalculateInertiaTensor();
	}
}

void C2DColliders::MoveCollider(C2DVector &v, C2DCollider* pCollider)
{
	if (!pCollider) return;

	std::vector<C2DCollider*>::iterator iter;

	if ((iter = std::find(m_fvtColliders.begin(), m_fvtColliders.end(), pCollider)) != m_fvtColliders.end())
	{
		(*iter)->Move(v);

		CalculateInertiaTensor();
	}
}

void C2DColliders::Move(C2DVector& vector)
{
	m_mtxWorld.m_f31 += vector.GetX();
	m_mtxWorld.m_f32 += vector.GetY();
	m_mtxWorld.m_f33 = 1.0f;
}

void C2DColliders::RotateACCoordZ(float fRadian)
{
	C2DMatrix mtxRotate;

	mtxRotate.m_f11 = cos(fRadian); mtxRotate.m_f12 = sin(fRadian); mtxRotate.m_f13 = 0.0f;
	mtxRotate.m_f21 = -sin(fRadian); mtxRotate.m_f22 = cos(fRadian); mtxRotate.m_f23 = 0.0f;

	CVectorOperation::C2DTransform(&this->m_mtxWorld, &mtxRotate, &this->m_mtxWorld);
}

void C2DColliders::RotateCMCoordZ(float fRadian)
{
	C2DMatrix mtxTRT;
	C2DMatrix mtxRotate;
	C2DVector vCenterOfMassW;

	CVectorOperation::C2DTransform(&vCenterOfMassW, &m_vCenterOfMass, &m_mtxWorld);

	mtxTRT.m_f31 -= vCenterOfMassW.GetX();
	mtxTRT.m_f32 -= vCenterOfMassW.GetY();
	mtxRotate.m_f11 = cos(fRadian); mtxRotate.m_f12 = sin(fRadian); mtxRotate.m_f13 = 0.0f;
	mtxRotate.m_f21 = -sin(fRadian); mtxRotate.m_f22 = cos(fRadian); mtxRotate.m_f23 = 0.0f;
	CVectorOperation::C2DTransform(&mtxTRT, &mtxTRT, &mtxRotate);
	mtxTRT.m_f31 += vCenterOfMassW.GetX();
	mtxTRT.m_f32 += vCenterOfMassW.GetY();

	CVectorOperation::C2DTransform(&this->m_mtxWorld, &this->m_mtxWorld, &mtxTRT);
}

bool C2DColliders::IsCollided_(C2DColliderCircle* circle, C2DVector* colPos)
{
	bool bIsCollided = false;

	for (auto &collider : m_fvtColliders)
	{
		if (collider->IsCollided_(circle, colPos))
		{
			bIsCollided = true;
			break;
		}
	}

	return bIsCollided;
}

bool C2DColliders::IsCollided_(C2DColliderRect* rect, C2DVector* colPos)
{
	bool bIsCollided = false;

	for (auto &collider : m_fvtColliders)
	{
		if (collider->IsCollided_(rect, colPos))
		{
			bIsCollided = true;
			break;
		}
	}

	return bIsCollided;
}

bool C2DColliders::IsCollided_(C2DColliders* colliders, C2DVector* colPos)
{
	bool bIsCollided = false;

	for (auto &collider : m_fvtColliders)
	{
		if (collider->IsCollided_(colliders, colPos))
		{
			bIsCollided = true;
			break;
		}
	}

	return bIsCollided;
}

void C2DColliders::Impurse(C2DColliderCircle *pCircle, float fRest, float fFric, C2DVector* colPos)
{
	pCircle->Impurse(this, fRest, fFric, colPos);
}

void C2DColliders::Impurse(C2DColliderRect *pRect, float fRest, float fFric, C2DVector* colPos)
{

}

void C2DColliders::Impurse(C2DColliders *pColliders, float fRest, float fFric, C2DVector* colPos)
{

}

//bool C2DColliders::IsCollided_Index(int idx, C2DColliderCircle* circle, C2DVector* colPos)
//{
//	/*int iNumOfColliders = m_fvtColliders.size();
//	bool bIsCollided = false;
//
//	if (!iNumOfColliders) return false;
//	if (idx < 0 || idx >= iNumOfColliders) return false;
//
//	for (aut
//		if (m_fvtColliders[])
//		{
//			bIsCollided = true;
//			break;
//		}
//	}
//
//	return bIsCollided;*/
//}
//
//bool C2DColliders::IsCollided_Index(int idx, C2DColliderRect* rect, C2DVector* colPos)
//{
//
//}
//
//bool C2DColliders::IsCollided_Index(int idx, C2DColliders* colliders, C2DVector* colPos)
//{
//
//}

void C2DColliders::Update(float fElapsedTime)
{
	float fUpdatedAngularV = 0.0f;
	float fRad = 0.0f;
	C2DVector vUpdatedLinearV;
	C2DVector vMove;

	fUpdatedAngularV = m_fAngularV + m_fAngularA * fElapsedTime;

	m_vLinearA.m_fY = m_EF_Flags & EF_GRAVITY ? -GRAVITATIONAL_ACCELERATION_MAGNITUDE : m_vLinearA.m_fY;
	vUpdatedLinearV = m_vLinearV + m_vLinearA * fElapsedTime;

	fRad = (fUpdatedAngularV + m_fAngularV) / 2.0f * fElapsedTime;
	vMove = (vUpdatedLinearV + m_vLinearV) / 2.0f * fElapsedTime;

	RotateCMCoordZ(fRad);
	Move(vMove);

	m_vLinearV = vUpdatedLinearV;
	m_fAngularV = fUpdatedAngularV;
}

void C2DColliders::RenderAPI(HDC hdc, C2DCamera *pCamera)
{
	for (auto &collider : m_fvtColliders)
	{
		collider->RenderAPI(hdc, &m_mtxWorld, pCamera);
	}
}

void C2DColliders::RenderAPI(HDC hdc, C2DMatrix *AssColsM, C2DCamera *pCamera)
{
	for (auto &collider : m_fvtColliders)
	{
		collider->RenderAPI(hdc, &m_mtxWorld, pCamera);
	}
}