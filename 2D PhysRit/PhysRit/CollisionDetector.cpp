#include <iostream>
#include "CollisionDetector.h"
#include "VectorOperation.h"

using namespace std;


CCollisionDetector::CCollisionDetector()
{
}


CCollisionDetector::~CCollisionDetector()
{
}

C2DCollisionDetector::C2DCollisionDetector()
{

}

C2DCollisionDetector::~C2DCollisionDetector()
{

}

bool C2DCollisionDetector::C2DIsCollided(C2DColliderCircle circle, C2DColliderRect rect, C2DVector* colPos)
{
	// 월드 좌표계에서 rect 원점에서 circle 원점으로의 dir벡터를 구한다.
	C2DVector vToCircleW;

	C2DVector vProjRightOfRect;
	float fLenOfProjRightOfRect = 0.0f;
	
	C2DVector vProjUpOfRect;
	float fLenOfProjUpOfRect = 0.0f;

	C2DVector vNearest;

	vToCircleW = circle.GetPosition() - rect.GetPosition();
	
	CVectorOperation::C2DScalarProduct(&vProjRightOfRect, CVectorOperation::C2DDotProduct(&vToCircleW, &rect.GetWorldRightV()), &rect.GetWorldRightV());
	fLenOfProjRightOfRect = CVectorOperation::C2DLength(&vProjRightOfRect);
	CVectorOperation::C2DNormalize(&vProjRightOfRect, &vProjRightOfRect);

	CVectorOperation::C2DScalarProduct(&vProjUpOfRect, CVectorOperation::C2DDotProduct(&vToCircleW, &rect.GetWorldUpV()), &rect.GetWorldUpV());
	fLenOfProjUpOfRect = CVectorOperation::C2DLength(&vProjUpOfRect);
	CVectorOperation::C2DNormalize(&vProjUpOfRect, &vProjUpOfRect);

	// rect의 네 개의 점 중 원에 가장 가까운 점을 찾는다. 
	vNearest.Set((vProjRightOfRect * (rect.GetWidth() / 2.0f) + vProjUpOfRect * (rect.GetHeight() / 2.0f)) + rect.GetPosition());
	cout << "[additional information] vNearest : [" << vNearest.m_fX << ", " << vNearest.m_fY << "]"<<endl;
	if (fLenOfProjRightOfRect <= rect.GetWidth() / 2.0f)
	{
		if (fLenOfProjUpOfRect <= circle.GetRadius() + rect.GetHeight() / 2.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (fLenOfProjUpOfRect <= rect.GetHeight() / 2.0f)
	{
		if (fLenOfProjRightOfRect <= circle.GetRadius() + rect.GetWidth() / 2.0f)
		{
			return true;
		}
		{
			return false;
		}
	}
	else
	{
		vToCircleW = circle.GetPosition() - vNearest.GetXY();

		if (CVectorOperation::C2DLength(&vToCircleW) <= circle.GetRadius())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool C2DCollisionDetector::C2DIsCollided(C2DColliderRect rect, C2DColliderCircle circle, C2DVector* colPos)
{
	return C2DCollisionDetector::C2DIsCollided(circle, rect, colPos);
}