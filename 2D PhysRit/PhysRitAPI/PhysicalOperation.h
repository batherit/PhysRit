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
	float m_fAngulardA;			// z축에 대한 각가속도 ; rad/s^2
	float m_fAngularV;			// z축에 대한 갱신 각속도 ; rad/s
	C2DVector m_vLinearA;		// xy평면에 대한 선가속도
	C2DVector m_vLinearV;		// xy평면에 대한 선속도 ; m/s

	void UpdatePos(C2DVector *outPos, C2DVector *inPos, float fElapsedTime);
};

