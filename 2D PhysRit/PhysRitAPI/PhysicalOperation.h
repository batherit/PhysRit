#pragma once
#include "VectorOperation.h"

class CPhysicalOperation
{
public:
	CPhysicalOperation();
	~CPhysicalOperation();

	// static void UpdatePos(C2DVector *outPos, C2DVector *inPos, float fElapsedTime);
};

class C2DKinematicProcessUnit
{
public:
	C2DKinematicProcessUnit();
	~C2DKinematicProcessUnit();

protected:
	float m_fAngulardA;			// z�࿡ ���� �����ӵ� ; rad/s^2
	float m_fAngularV;			// z�࿡ ���� ���� ���ӵ� ; rad/s
	C2DVector m_vLinearA;		// xy��鿡 ���� �����ӵ�
	C2DVector m_vLinearV;		// xy��鿡 ���� ���ӵ� ; m/s

	void UpdatePos(C2DVector *outPos, C2DVector *inPos, float fElapsedTime);
};

