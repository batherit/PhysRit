#include "stdafx.h"
#include "CollisionSolver.h"


CCollisionSolver::CCollisionSolver()
{
}


CCollisionSolver::~CCollisionSolver()
{
}

C2DCollisionSolver::C2DCollisionSolver()
{

}

C2DCollisionSolver::~C2DCollisionSolver()
{

}

void C2DCollisionSolver::C2DImpurse(C2DColliderCircle *pCir1, C2DColliderCircle *pCir2, float fRest, float fFric, C2DVector* colPos)
{
	if (pCir1 == nullptr || pCir2 == nullptr) return;

	// �浹�� Ȯ���Ѵ�.
	float fDistance = CVectorOperation::C2DLength(&(pCir2->GetPosition() - pCir1->GetPosition()));
	float fRadiusOfCir1 = pCir1->GetRadius();
	float fRadiusOfCir2 = pCir2->GetRadius();

	if (fDistance > fRadiusOfCir1 + fRadiusOfCir2) return;

	C2DVector vNormal;					// ���� ��� ; �浹 �� �ӵ��� ���ϴµ� ���δ�.
	C2DVector vProofDistance;			// �浹 �� ���� �Ÿ�
	C2DVector vNormalV1;				// pCir1�� ���� ��ֿ� ���� �ӵ�
	C2DVector vNormalV2;				// pCir2�� ���� ��ֿ� ���� �ӵ�
	C2DVector vTan;						// ���� ���� ; �������� ���� �� ���δ�.
	C2DVector vRelativeV;				// ���ӵ� V1 - V2
	C2DVector vJn;						// Cir1�� �ۿ��ϴ� ��ݷ�
	float fJn;							// vJn�� ũ��
	C2DVector vJt;						// Cir1�� �ۿ��ϴ� ������
	C2DVector vJ;						// �� �ۿ뷮

	// ���� ����� ���Ѵ�.
	vNormal.Set(pCir2->GetPosition() - pCir1->GetPosition());
	CVectorOperation::C2DNormalize(&vNormal, &vNormal);

	// ���� ����� ������� �� ���� ��ġ�� �������Ѵ�. => �������� �浹 �ذ��� ���� �� �ִ�.
	vProofDistance.Set(((vNormal * fRadiusOfCir1 + vNormal * fRadiusOfCir2) - vNormal * fDistance) * -0.5f);
	pCir1->Move(vProofDistance);
	pCir2->Move(vProofDistance * -1.0f);

	// ��� �ӵ��� ���Ѵ�.
	vNormalV1 = vNormal * CVectorOperation::C2DDotProduct(&pCir1->GetLinearV(), &vNormal);
	vNormalV2 = vNormal * CVectorOperation::C2DDotProduct(&pCir2->GetLinearV(), &vNormal);

	// ���� ������ ���Ѵ�.
	vRelativeV.Set(pCir1->GetLinearV() - pCir2->GetLinearV());
	C3DVector v3dNormal(vNormal.GetX(), vNormal.GetY(), 0.0f);
	C3DVector v3dRelativeV(vRelativeV.GetX(), vRelativeV.GetY(), 0.0f);
	C3DVector v3dTan;
	CVectorOperation::C3DCross(&v3dTan, &v3dNormal, &v3dRelativeV);
	CVectorOperation::C3DCross(&v3dTan, &v3dTan, &v3dNormal);
	CVectorOperation::C3DNormalize(&v3dTan, &v3dTan);
	vTan.Set(v3dTan.GetX(), v3dTan.GetY());

	// ��ݷ��� ���Ѵ�. (1. ��� �ӵ���... ���� �Ϸ�, 2. ���ӵ� ���䵵...)
	C3DVector v3dRelative1;
	C3DVector v3dRelative2;
	float fCirI1;
	float fCirI2;
	CVectorOperation::C2DTransform((C2DVector*)&v3dRelative1, &pCir1->GetRelative(), &pCir1->m_mtxWorld);
	v3dRelative1.m_fX -= pCir1->m_mtxWorld.m_f31;
	v3dRelative1.m_fY -= pCir1->m_mtxWorld.m_f32;
	v3dRelative1.m_fZ = 0.0f;
	fCirI1 = CVectorOperation::C3DDotProduct(&v3dNormal, &CVectorOperation::C3DCross(&CVectorOperation::C3DCross(&v3dRelative1, &v3dNormal), &v3dRelative1));
	CVectorOperation::C2DTransform((C2DVector*)&v3dRelative2, &pCir2->GetRelative(), &pCir2->m_mtxWorld);
	v3dRelative2.m_fX -= pCir2->m_mtxWorld.m_f31;
	v3dRelative2.m_fY -= pCir2->m_mtxWorld.m_f32;
	v3dRelative2.m_fZ = 0.0f;
	fCirI2 = CVectorOperation::C3DDotProduct(&v3dNormal, &CVectorOperation::C3DCross(&CVectorOperation::C3DCross(&v3dRelative2, &v3dNormal), &v3dRelative2));
	vJn.Set((vNormalV1 - vNormalV2) * (1.0f + fRest) / (1.0f / pCir1->GetMass() + 1.0f / pCir2->GetMass() + fCirI1 / pCir1->GetIzz() + fCirI2 / pCir2->GetIzz()));
	vJn *= -1.0f;

	// �������� ���Ѵ�.
	fJn = CVectorOperation::C2DLength(&vJn);
	vJt.Set(vTan * (fFric * fJn));
	vJt *= -1.0f;

	// �� �ۿ뷮�� ���Ѵ�.
	vJ.Set(vJn + vJt);

	// ���ӵ��� �����Ѵ�.
	pCir1->SetLinearV(pCir1->GetLinearV() + vJ / pCir1->GetMass());
	pCir2->SetLinearV(pCir2->GetLinearV() + (vJ * -1.0f) / pCir2->GetMass());

	// ���ӵ��� �����Ѵ�.
	
	C3DVector v3dJ(vJ.GetX(), vJ.GetY(), 0.0f);
	
	CVectorOperation::C3DCross(&v3dRelative1, &v3dRelative1, &v3dJ);
	pCir1->SetAngularV(pCir1->GetAngularV() + v3dRelative1.GetZ() / pCir1->GetIzz());

	CVectorOperation::C3DCross(&v3dRelative2, &v3dRelative2, &v3dJ);
	pCir2->SetAngularV(pCir2->GetAngularV() + (v3dRelative2.GetZ() * -1.0f) / pCir2->GetIzz());
}

void C2DCollisionSolver::C2DImpurse(C2DColliderCircle *pCir, C2DColliderRect *pRect, float fRest, float fFric, C2DVector* colPos)
{

}

void C2DCollisionSolver::C2DImpurse(C2DColliderRect *pRect, C2DColliderCircle *pCir, float fRest, float fFric, C2DVector* colPos)
{

}

void C2DCollisionSolver::C2DImpurse(C2DColliderRect *pRect1, C2DColliderRect *pRect2, float fRest, float fFric, C2DVector* colPos)
{

}