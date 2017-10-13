#include "C2DAssembledColliders.h"



C2DAssembledColliders::C2DAssembledColliders()
{
	m_fTotalMass = 0.0f;
	m_vCenterOfMass = C2DVector(0.0f, 0.0f);
	m_fTotalIzz = 0.0f;
}


C2DAssembledColliders::~C2DAssembledColliders()
{
}

void C2DAssembledColliders::CalculateInertiaTensor(void)
{
	C2DVector vSumPrimaryMoment;

	m_fTotalMass = 0.0f;
	for (auto &collider : m_fvtColliders)
	{
		m_fTotalMass += collider->GetMass();
		vSumPrimaryMoment +=  collider->GetPosition() * collider->GetMass();
	}

	m_vCenterOfMass = C2DVector(0.0f, 0.0f);
	m_mtxCenterOfMass.Identify();

	if (m_fTotalMass > 0.0f)
	{
		m_vCenterOfMass = vSumPrimaryMoment / m_fTotalMass;								// ������� �и� != 0 �̶�� ����� ���� ���� �� �� '��'/
		m_mtxCenterOfMass.m_f31 = -m_vCenterOfMass.GetX();
		m_mtxCenterOfMass.m_f32 = -m_vCenterOfMass.GetY();		// ���� ��ǥ�迡�� ���ǵǴ� ���� �߽��� ��ǥ�� �ϴ� ��Ŀ� ����.
	}

	m_fTotalIzz = 0.0f;
	for (auto &collider : m_fvtColliders)
	{
		collider->SetRelative(collider->GetPosition() - m_vCenterOfMass);
		m_fTotalIzz += 
			collider->GetIzz() + collider->GetMass() 
			* (collider->GetRelative().GetX() * collider->GetRelative().GetX()
				+ collider->GetRelative().GetY() * collider->GetRelative().GetY());
	}
}

void C2DAssembledColliders::SetPosition(C2DVector& vector)
{
	m_mtxWorld.m_f31 = vector.m_fX;
	m_mtxWorld.m_f32 = vector.m_fY;
	m_mtxWorld.m_f33 = 1.0f;
}

C2DVector C2DAssembledColliders::GetPosition(void)
{
	return C2DVector(m_mtxWorld.m_f31, m_mtxWorld.m_f32);
}

C2DVector C2DAssembledColliders::GetWorldUpV(void)
{
	return C2DVector(m_mtxWorld.m_f21, m_mtxWorld.m_f22);
}

C2DVector C2DAssembledColliders::GetWorldRightV(void)
{
	return C2DVector(m_mtxWorld.m_f11, m_mtxWorld.m_f12);
}

int C2DAssembledColliders::GetNumOfCollider(void)
{
	return m_fvtColliders.size();
}

float C2DAssembledColliders::GetMass(void)
{
	return m_fTotalMass;
}
C2DVector C2DAssembledColliders::GetCenterOfMass(void)
{
	return m_vCenterOfMass * m_mtxWorld;
}
float C2DAssembledColliders::GetIzz(void)
{
	return m_fTotalIzz;
}

void C2DAssembledColliders::AttachCollider(C2DVector inPos, float fRotRadian, C2DCollider* pCollider)
{
	if (!pCollider) return;

	pCollider->RotateZ(fRotRadian);
	pCollider->SetPosition(inPos);
	m_fvtColliders.push_back(pCollider);			// �������ݶ��̴��� ��ǥ�迡 ���ǵ�.

	CalculateInertiaTensor();
}

void C2DAssembledColliders::DetachCollider(C2DCollider* pCollider)
{
	if (!pCollider) return;

	std::vector<C2DCollider*>::iterator iter;

	if ((iter = std::find(m_fvtColliders.begin(), m_fvtColliders.end(), pCollider)) != m_fvtColliders.end())
	{
		(*iter)->SetRelative(C2DVector(0.0f, 0.0f));		// ������ �ݶ��̴��� �ǹǷ� ��� ���ʹ� (0, 0);
		m_fvtColliders.erase(iter);

		CalculateInertiaTensor();
	}
}

void C2DAssembledColliders::MoveCollider(C2DVector &v, C2DCollider* pCollider)
{
	if (!pCollider) return;

	std::vector<C2DCollider*>::iterator iter;

	if ((iter = std::find(m_fvtColliders.begin(), m_fvtColliders.end(), pCollider)) != m_fvtColliders.end())
	{
		(*iter)->Move(v);

		CalculateInertiaTensor();
	}
}

void C2DAssembledColliders::RotateACCoordZ(float fRadian)
{
	C2DMatrix mtxRotate;

	mtxRotate.m_f11 = cos(fRadian); mtxRotate.m_f12 = sin(fRadian); mtxRotate.m_f13 = 0.0f;
	mtxRotate.m_f21 = -sin(fRadian); mtxRotate.m_f22 = cos(fRadian); mtxRotate.m_f23 = 0.0f;

	CVectorOperation::C2DTransform(&this->m_mtxWorld, &mtxRotate, &this->m_mtxWorld);
}

void C2DAssembledColliders::RotateCMCoordZ(float fRadian)
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

void C2DAssembledColliders::RenderAPI(HDC hdc, C2DCamera *pCamera)
{
	C2DMatrix mtxWorld;
	//C2DMatrix mtxCenterOfMassW;

	//CVectorOperation::C2DTransform(&mtxCenterOfMassW, &m_mtxCenterOfMass, &m_mtxWorld);
	//CVectorOperation::C2DTransform(&mtxWorld, &m_mtxWorld, &m_mtxCenterOfMass);

	for (auto &collider : m_fvtColliders)
	{
		collider->RenderAPI(hdc, &m_mtxWorld, pCamera);
	}
}