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

	// 충돌을 확인한다.
	float fDistance = CVectorOperation::C2DLength(&(pCir2->GetPosition() - pCir1->GetPosition()));
	float fRadiusOfCir1 = pCir1->GetRadius();
	float fRadiusOfCir2 = pCir2->GetRadius();

	if (fDistance > fRadiusOfCir1 + fRadiusOfCir2) return;

	C2DVector vNormal;					// 접촉 노멀 ; 충돌 후 속도를 구하는데 쓰인다.
	C2DVector vProofDistance;			// 충돌 후 보정 거리
	C2DVector vNormalV1;				// pCir1의 접촉 노멀에 대한 속도
	C2DVector vNormalV2;				// pCir2의 접촉 노멀에 대한 속도
	C2DVector vTan;						// 접촉 접선 ; 마찰량을 구할 때 쓰인다.
	C2DVector vRelativeV;				// 상대속도 V1 - V2
	C2DVector vJn;						// Cir1에 작용하는 충격량
	float fJn;							// vJn의 크기
	C2DVector vJt;						// Cir1에 작용하는 마찰량
	C2DVector vJ;						// 총 작용량

	// 접촉 노멀을 구한다.
	vNormal.Set(pCir2->GetPosition() - pCir1->GetPosition());
	CVectorOperation::C2DNormalize(&vNormal, &vNormal);

	// 접촉 노멀을 기반으로 두 원의 위치를 재조정한다. => 겹쳐지면 충돌 해결이 꼬일 수 있다.
	vProofDistance.Set(((vNormal * fRadiusOfCir1 + vNormal * fRadiusOfCir2) - vNormal * fDistance) * -0.5f);
	pCir1->Move(vProofDistance);
	pCir2->Move(vProofDistance * -1.0f);

	// 노멀 속도를 구한다.
	vNormalV1 = vNormal * CVectorOperation::C2DDotProduct(&pCir1->GetLinearV(), &vNormal);
	vNormalV2 = vNormal * CVectorOperation::C2DDotProduct(&pCir2->GetLinearV(), &vNormal);

	// 접촉 접선을 구한다.
	vRelativeV.Set(pCir1->GetLinearV() - pCir2->GetLinearV());
	C3DVector v3dNormal(vNormal.GetX(), vNormal.GetY(), 0.0f);
	C3DVector v3dRelativeV(vRelativeV.GetX(), vRelativeV.GetY(), 0.0f);
	C3DVector v3dTan;
	CVectorOperation::C3DCross(&v3dTan, &v3dNormal, &v3dRelativeV);
	CVectorOperation::C3DCross(&v3dTan, &v3dTan, &v3dNormal);
	CVectorOperation::C3DNormalize(&v3dTan, &v3dTan);
	vTan.Set(v3dTan.GetX(), v3dTan.GetY());

	// 충격량을 구한다. (1. 노멀 속도로... 수정 완료, 2. 각속도 개념도...)
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

	// 마찰량을 구한다.
	fJn = CVectorOperation::C2DLength(&vJn);
	vJt.Set(vTan * (fFric * fJn));
	vJt *= -1.0f;

	// 총 작용량을 구한다.
	vJ.Set(vJn + vJt);

	// 선속도를 갱신한다.
	pCir1->SetLinearV(pCir1->GetLinearV() + vJ / pCir1->GetMass());
	pCir2->SetLinearV(pCir2->GetLinearV() + (vJ * -1.0f) / pCir2->GetMass());

	// 각속도를 갱신한다.
	
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