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

bool C2DCollisionDetector::C2DIsCollided_(C2DColliderCircle circle1, C2DColliderCircle circle2, C2DVector* colPos)
{
	float fDistance;

	fDistance = CVectorOperation::C2DLength(&(circle1.GetPosition() - circle2.GetPosition()));

	if (fDistance <= circle1.GetRadius() + circle2.GetRadius())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool C2DCollisionDetector::C2DIsCollided_(C2DVector vPos1, float fR1, C2DVector vPos2, float fR2, C2DVector* colPos)
{
	float fDistance;

	if (fR1 <= 0.0f || fR2 <= 0.0f)
	{
		return false;
	}

	fDistance = CVectorOperation::C2DLength(&(vPos1 - vPos2));

	if (fDistance <= fR1 + fR2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool C2DCollisionDetector::C2DIsCollided_(float fX1, float fY1, float fR1, float fX2, float fY2, float fR2, C2DVector* colPos)
{
	float fDistance;

	if (fR1 <= 0.0f || fR2 <= 0.0f)
	{
		return false;
	}

	fDistance = CVectorOperation::C2DLength(&(C2DVector(fX1, fY1) - C2DVector(fX2, fY2)));

	if (fDistance <= fR1 + fR2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool C2DCollisionDetector::C2DIsCollided_(C2DColliderCircle circle, C2DColliderRect rect, C2DVector* colPos)
{
	// ���� ��ǥ�迡�� rect �������� circle ���������� dir���͸� ���Ѵ�.
	C2DVector vToCircleW;

	C2DVector vProjRightOfRect;
	float fLenOfProjRightOfRect = -1.0f;
	
	C2DVector vProjUpOfRect;
	float fLenOfProjUpOfRect = -1.0f;

	C2DVector vNearest;

	vToCircleW = circle.GetPosition() - rect.GetPosition();
	
	CVectorOperation::C2DScalarProduct(&vProjRightOfRect, CVectorOperation::C2DDotProduct(&vToCircleW, &rect.GetWorldRightV()), &rect.GetWorldRightV());
	fLenOfProjRightOfRect = CVectorOperation::C2DLength(&vProjRightOfRect);
	CVectorOperation::C2DNormalize(&vProjRightOfRect, &vProjRightOfRect);

	CVectorOperation::C2DScalarProduct(&vProjUpOfRect, CVectorOperation::C2DDotProduct(&vToCircleW, &rect.GetWorldUpV()), &rect.GetWorldUpV());
	fLenOfProjUpOfRect = CVectorOperation::C2DLength(&vProjUpOfRect);
	CVectorOperation::C2DNormalize(&vProjUpOfRect, &vProjUpOfRect);

	// rect�� �� ���� �� �� ���� ���� ����� ���� ã�´�. 
	vNearest.Set((vProjRightOfRect * (rect.GetWidth() / 2.0f) + vProjUpOfRect * (rect.GetHeight() / 2.0f)) + rect.GetPosition());
	cout << "[additional information] vNearest : [" << vNearest.m_fX << ", " << vNearest.m_fY << "]"<<endl;
	if (fLenOfProjRightOfRect <= rect.GetWidth() / 2.0f)
	{
		if (fLenOfProjUpOfRect <= circle.GetRadius() + rect.GetHeight() / 2.0f)
		{
			return true;			// case1.���� �浹�� ���
		}
		else
		{
			return false;			// case1.���� �浹���� ���� ���
		}
	}
	else if (fLenOfProjUpOfRect <= rect.GetHeight() / 2.0f)
	{
		if (fLenOfProjRightOfRect <= circle.GetRadius() + rect.GetWidth() / 2.0f)
		{
			return true;			// case2.���� �浹�� ���
		}
		{
			return false;			// case2.���� �浹���� ���� ���
		}
	}
	else
	{
		vToCircleW = circle.GetPosition() - vNearest.GetXY();

		if (CVectorOperation::C2DLength(&vToCircleW) <= circle.GetRadius())
		{
			return true;			// case3. �浹�� ���
		}
		else
		{
			return false;			// case4. �浹���� ���� ���
		}
	}
}

bool C2DCollisionDetector::C2DIsCollided_(C2DColliderRect rect, C2DColliderCircle circle, C2DVector* colPos)
{
	return C2DCollisionDetector::C2DIsCollided_(circle, rect, colPos);
}

bool C2DCollisionDetector::C2DIsCollided_Nearest(C2DColliderRect rect1, C2DColliderRect rect2, C2DVector* colPos)
{
	//rect2���� �������� ���ϱ� ���Ͽ� rect2���� rect1�Ƿ��� ���͸� ���Ѵ�.
	C2DVector vToRectW;

	C2DVector vProjRightOfRect;
	C2DVector vProjUpOfRect;

	C2DVector vNearest;
	C2DVector vToNearest;

	vToRectW = rect1.GetPosition() - rect2.GetPosition();

	CVectorOperation::C2DScalarProduct(&vProjRightOfRect, CVectorOperation::C2DDotProduct(&vToRectW, &rect2.GetWorldRightV()), &rect2.GetWorldRightV());
	CVectorOperation::C2DNormalize(&vProjRightOfRect, &vProjRightOfRect);

	CVectorOperation::C2DScalarProduct(&vProjUpOfRect, CVectorOperation::C2DDotProduct(&vToRectW, &rect2.GetWorldUpV()), &rect2.GetWorldUpV());
	CVectorOperation::C2DNormalize(&vProjUpOfRect, &vProjUpOfRect);

	// rect�� �� ���� �� �� ���� ���� ����� ���� ã�´�. 
	vNearest.Set((vProjRightOfRect * (rect2.GetWidth() / 2.0f) + vProjUpOfRect * (rect2.GetHeight() / 2.0f)) + rect2.GetPosition());
	cout << "[additional information] vNearest of rect2 : [" << vNearest.m_fX << ", " << vNearest.m_fY << "]" << endl;

	// rect1���� rect2�� ������������ ���͸� ���Ѵ�.
	vToNearest = vNearest - rect1.GetPosition();

	// vToNearest�� rect1�� right ���Ϳ� up ���Ϳ� �����Ѵ�. 
	CVectorOperation::C2DScalarProduct(&vProjRightOfRect, CVectorOperation::C2DDotProduct(&vToNearest, &rect1.GetWorldRightV()), &rect1.GetWorldRightV());
	CVectorOperation::C2DScalarProduct(&vProjUpOfRect, CVectorOperation::C2DDotProduct(&vToNearest, &rect1.GetWorldUpV()), &rect1.GetWorldUpV());
	
	if (CVectorOperation::C2DLength(&vProjRightOfRect) <= rect1.GetWidth() / 2.0f && CVectorOperation::C2DLength(&vProjUpOfRect) <= rect1.GetHeight() / 2.0f)
	{
		return true;
	}

	vToRectW = rect2.GetPosition() - rect1.GetPosition();

	CVectorOperation::C2DScalarProduct(&vProjRightOfRect, CVectorOperation::C2DDotProduct(&vToRectW, &rect1.GetWorldRightV()), &rect1.GetWorldRightV());
	CVectorOperation::C2DNormalize(&vProjRightOfRect, &vProjRightOfRect);

	CVectorOperation::C2DScalarProduct(&vProjUpOfRect, CVectorOperation::C2DDotProduct(&vToRectW, &rect1.GetWorldUpV()), &rect1.GetWorldUpV());
	CVectorOperation::C2DNormalize(&vProjUpOfRect, &vProjUpOfRect);

	// rect�� �� ���� �� �� ���� ���� ����� ���� ã�´�. 
	vNearest.Set((vProjRightOfRect * (rect1.GetWidth() / 2.0f) + vProjUpOfRect * (rect1.GetHeight() / 2.0f)) + rect1.GetPosition());
	cout << "[additional information] vNearest of rect1 : [" << vNearest.m_fX << ", " << vNearest.m_fY << "]" << endl;

	// rect2���� rect1�� ������������ ���͸� ���Ѵ�.
	vToNearest = vNearest - rect2.GetPosition();

	// vToNearest�� rect1�� right ���Ϳ� up ���Ϳ� �����Ѵ�. 
	CVectorOperation::C2DScalarProduct(&vProjRightOfRect, CVectorOperation::C2DDotProduct(&vToNearest, &rect2.GetWorldRightV()), &rect2.GetWorldRightV());
	CVectorOperation::C2DScalarProduct(&vProjUpOfRect, CVectorOperation::C2DDotProduct(&vToNearest, &rect2.GetWorldUpV()), &rect2.GetWorldUpV());

	if (CVectorOperation::C2DLength(&vProjRightOfRect) <= rect2.GetWidth() / 2.0f && CVectorOperation::C2DLength(&vProjUpOfRect) <= rect2.GetHeight() / 2.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool C2DCollisionDetector::C2DIsCollided_NearestOBB(C2DColliderRect rect1, C2DColliderRect rect2, C2DVector* colPos)
{
	C2DVector vToRectW;

	C2DVector vProjRightOfRect;
	C2DVector vProjUpOfRect;

	C2DVector vNearestOfRect1;
	C2DVector vNearestOfRect2;

	C2DVector vToNearestOfRect1;
	C2DVector vToNearestOfRect2;

	C2DVector vProjOfNearestOfRect1;
	C2DVector vProjOfNearestOfRect2;

	// rect2�� �������� ���Ѵ�.
	vToRectW = rect1.GetPosition() - rect2.GetPosition();

	CVectorOperation::C2DScalarProduct(&vProjRightOfRect, CVectorOperation::C2DDotProduct(&vToRectW, &rect2.GetWorldRightV()), &rect2.GetWorldRightV());
	CVectorOperation::C2DNormalize(&vProjRightOfRect, &vProjRightOfRect);

	CVectorOperation::C2DScalarProduct(&vProjUpOfRect, CVectorOperation::C2DDotProduct(&vToRectW, &rect2.GetWorldUpV()), &rect2.GetWorldUpV());
	CVectorOperation::C2DNormalize(&vProjUpOfRect, &vProjUpOfRect);

	// rect�� �� ���� �� �� ���� ���� ����� ���� ã�´�. 
	vNearestOfRect2.Set((vProjRightOfRect * (rect2.GetWidth() / 2.0f) + vProjUpOfRect * (rect2.GetHeight() / 2.0f)) + rect2.GetPosition());
	cout << "[additional information] vNearest of rect2 : [" << vNearestOfRect2.m_fX << ", " << vNearestOfRect2.m_fY << "]" << endl;

	// rect1�� �������� ���Ѵ�.
	vToRectW = rect2.GetPosition() - rect1.GetPosition();

	CVectorOperation::C2DScalarProduct(&vProjRightOfRect, CVectorOperation::C2DDotProduct(&vToRectW, &rect1.GetWorldRightV()), &rect1.GetWorldRightV());
	CVectorOperation::C2DNormalize(&vProjRightOfRect, &vProjRightOfRect);

	CVectorOperation::C2DScalarProduct(&vProjUpOfRect, CVectorOperation::C2DDotProduct(&vToRectW, &rect1.GetWorldUpV()), &rect1.GetWorldUpV());
	CVectorOperation::C2DNormalize(&vProjUpOfRect, &vProjUpOfRect);

	// rect�� �� ���� �� �� ���� ���� ����� ���� ã�´�. 
	vNearestOfRect1.Set((vProjRightOfRect * (rect1.GetWidth() / 2.0f) + vProjUpOfRect * (rect1.GetHeight() / 2.0f)) + rect1.GetPosition());
	cout << "[additional information] vNearest of rect1 : [" << vNearestOfRect1.m_fX << ", " << vNearestOfRect1.m_fY << "]" << endl;

	vToNearestOfRect1.Set(vNearestOfRect1 - rect2.GetPosition());
	vToNearestOfRect2.Set(vNearestOfRect2 - rect2.GetPosition());
	cout << "[additional information] vToNearest1 from rect1 : [" << vToNearestOfRect1.m_fX << ", " << vToNearestOfRect1.m_fY << "]" << endl;
	cout << "[additional information] vToNearest2 from rect2 : [" << vToNearestOfRect2.m_fX << ", " << vToNearestOfRect2.m_fY << "]" << endl;

	// rect2�� up ���Ϳ� ������ vNearest.
	CVectorOperation::C2DScalarProduct(&vProjOfNearestOfRect1, CVectorOperation::C2DDotProduct(&vToNearestOfRect1, &rect2.GetWorldUpV()), &rect2.GetWorldUpV());
	CVectorOperation::C2DScalarProduct(&vProjOfNearestOfRect2, CVectorOperation::C2DDotProduct(&vToNearestOfRect2, &rect2.GetWorldUpV()), &rect2.GetWorldUpV());

	if (CVectorOperation::C2DLength(&vProjOfNearestOfRect1) > CVectorOperation::C2DLength(&vProjOfNearestOfRect2))			// �� �������� rect2�� up�࿡ ������ �� �и����� �������?
	{
		CVectorOperation::C2DNormalize(&vProjOfNearestOfRect1, &vProjOfNearestOfRect1);
		CVectorOperation::C2DNormalize(&vProjOfNearestOfRect2, &vProjOfNearestOfRect2);
		if (CVectorOperation::C2DDotProduct(&vProjOfNearestOfRect1, &vProjOfNearestOfRect2) >= 0.999f)
		{
			return false;
		}
	}

	// rect2 right ���Ϳ� ������ vNearest.
	CVectorOperation::C2DScalarProduct(&vProjOfNearestOfRect1, CVectorOperation::C2DDotProduct(&vToNearestOfRect1, &rect2.GetWorldRightV()), &rect2.GetWorldRightV());
	CVectorOperation::C2DScalarProduct(&vProjOfNearestOfRect2, CVectorOperation::C2DDotProduct(&vToNearestOfRect2, &rect2.GetWorldRightV()), &rect2.GetWorldRightV());
	if ( CVectorOperation::C2DLength(&vProjOfNearestOfRect1) > CVectorOperation::C2DLength(&vProjOfNearestOfRect2))
	{
		CVectorOperation::C2DNormalize(&vProjOfNearestOfRect1, &vProjOfNearestOfRect1);
		CVectorOperation::C2DNormalize(&vProjOfNearestOfRect2, &vProjOfNearestOfRect2);
		if (CVectorOperation::C2DDotProduct(&vProjOfNearestOfRect1, &vProjOfNearestOfRect2) >= 0.999f)
		{
			return false;
		}
	}

	vToNearestOfRect1.Set(vNearestOfRect1 - rect1.GetPosition());
	vToNearestOfRect2.Set(vNearestOfRect2 - rect1.GetPosition());

	// rect2�� up ���Ϳ� ������ vNearest.
	CVectorOperation::C2DScalarProduct(&vProjOfNearestOfRect1, CVectorOperation::C2DDotProduct(&vToNearestOfRect1, &rect1.GetWorldUpV()), &rect1.GetWorldUpV());
	CVectorOperation::C2DScalarProduct(&vProjOfNearestOfRect2, CVectorOperation::C2DDotProduct(&vToNearestOfRect2, &rect1.GetWorldUpV()), &rect1.GetWorldUpV());

	if (CVectorOperation::C2DLength(&vProjOfNearestOfRect2) > CVectorOperation::C2DLength(&vProjOfNearestOfRect1))			// �� �������� rect2�� up�࿡ ������ �� �и����� �������?
	{
		CVectorOperation::C2DNormalize(&vProjOfNearestOfRect1, &vProjOfNearestOfRect1);
		CVectorOperation::C2DNormalize(&vProjOfNearestOfRect2, &vProjOfNearestOfRect2);
		if (CVectorOperation::C2DDotProduct(&vProjOfNearestOfRect1, &vProjOfNearestOfRect2) >= 0.999f)
		{
			return false;
		}
	}

	// rect2 right ���Ϳ� ������ vNearest.
	CVectorOperation::C2DScalarProduct(&vProjOfNearestOfRect1, CVectorOperation::C2DDotProduct(&vToNearestOfRect1, &rect1.GetWorldRightV()), &rect1.GetWorldRightV());
	CVectorOperation::C2DScalarProduct(&vProjOfNearestOfRect2, CVectorOperation::C2DDotProduct(&vToNearestOfRect2, &rect1.GetWorldRightV()), &rect1.GetWorldRightV());
	if (CVectorOperation::C2DLength(&vProjOfNearestOfRect2) > CVectorOperation::C2DLength(&vProjOfNearestOfRect1))
	{
		CVectorOperation::C2DNormalize(&vProjOfNearestOfRect1, &vProjOfNearestOfRect1);
		CVectorOperation::C2DNormalize(&vProjOfNearestOfRect2, &vProjOfNearestOfRect2);
		if (CVectorOperation::C2DDotProduct(&vProjOfNearestOfRect1, &vProjOfNearestOfRect2) >= 0.999f)
		{
			return false;
		}
	}

	return true;
}

bool C2DCollisionDetector::C2DIsCollided_OBB(C2DColliderRect rect1, C2DColliderRect rect2, C2DVector* colPos)
{
	C2DVector vProjOfDir;
	float fLenOfProjDir = -1.0f;

	C2DVector vProjOfUp;
	C2DVector vProjOfRight;
	float fLenOfProjAxis = -1.0f;

	// rect1�� Up�࿡ ���͵��� �����Ѵ�.
	vProjOfDir = rect2.GetPosition() - rect1.GetPosition();
	CVectorOperation::C2DScalarProduct(&vProjOfDir, CVectorOperation::C2DDotProduct(&vProjOfDir, &rect1.GetWorldUpV()), &rect1.GetWorldUpV());
	fLenOfProjDir = CVectorOperation::C2DLength(&vProjOfDir);
	CVectorOperation::C2DScalarProduct(&vProjOfUp, CVectorOperation::C2DDotProduct(&(rect2.GetWorldUpV() * rect2.GetHeight()), &rect1.GetWorldUpV()), &rect1.GetWorldUpV());
	CVectorOperation::C2DScalarProduct(&vProjOfRight, CVectorOperation::C2DDotProduct(&(rect2.GetWorldRightV() * rect2.GetWidth()), &rect1.GetWorldUpV()), &rect1.GetWorldUpV());
	fLenOfProjAxis = CVectorOperation::C2DLength(&vProjOfUp) + CVectorOperation::C2DLength(&vProjOfRight);

	cout << "[additional information] fLenOfProjDir : [" << fLenOfProjDir << ", fLenOfProjAxis + rect1.GetHeight() :" << fLenOfProjAxis + rect1.GetHeight() << "]" << endl;
	// �и��� �˻�
	if (fLenOfProjDir + fLenOfProjDir > fLenOfProjAxis + rect1.GetHeight())
	{
		return false;
	}

	// rect1�� Right�࿡ ���͵��� �����Ѵ�.
	vProjOfDir = rect2.GetPosition() - rect1.GetPosition();
	CVectorOperation::C2DScalarProduct(&vProjOfDir, CVectorOperation::C2DDotProduct(&vProjOfDir, &rect1.GetWorldRightV()), &rect1.GetWorldRightV());
	fLenOfProjDir = CVectorOperation::C2DLength(&vProjOfDir);
	CVectorOperation::C2DScalarProduct(&vProjOfUp, CVectorOperation::C2DDotProduct(&(rect2.GetWorldUpV() * rect2.GetHeight()), &rect1.GetWorldRightV()), &rect1.GetWorldRightV());
	CVectorOperation::C2DScalarProduct(&vProjOfRight, CVectorOperation::C2DDotProduct(&(rect2.GetWorldRightV() * rect2.GetWidth()), &rect1.GetWorldRightV()), &rect1.GetWorldRightV());
	fLenOfProjAxis = CVectorOperation::C2DLength(&vProjOfUp) + CVectorOperation::C2DLength(&vProjOfRight);

	cout << "[additional information] fLenOfProjDir : [" << fLenOfProjDir << ", fLenOfProjAxis + rect1.GetWidth() :" << fLenOfProjAxis + rect1.GetWidth() << "]" << endl;
	// �и��� �˻�
	if (fLenOfProjDir + fLenOfProjDir > fLenOfProjAxis + rect1.GetWidth())
	{
		return false;
	}

	// rect2�� Up�࿡ ���͵��� �����Ѵ�.
	vProjOfDir = rect2.GetPosition() - rect1.GetPosition();
	CVectorOperation::C2DScalarProduct(&vProjOfDir, CVectorOperation::C2DDotProduct(&vProjOfDir, &rect2.GetWorldUpV()), &rect2.GetWorldUpV());
	fLenOfProjDir = CVectorOperation::C2DLength(&vProjOfDir);
	CVectorOperation::C2DScalarProduct(&vProjOfUp, CVectorOperation::C2DDotProduct(&(rect1.GetWorldUpV() * rect1.GetHeight()), &rect2.GetWorldUpV()), &rect2.GetWorldUpV());
	CVectorOperation::C2DScalarProduct(&vProjOfRight, CVectorOperation::C2DDotProduct(&(rect1.GetWorldRightV() * rect1.GetWidth()), &rect2.GetWorldUpV()), &rect2.GetWorldUpV());
	fLenOfProjAxis = CVectorOperation::C2DLength(&vProjOfUp) + CVectorOperation::C2DLength(&vProjOfRight);

	cout << "[additional information] fLenOfProjDir : [" << fLenOfProjDir << ", fLenOfProjAxis + rect2.GetHeight() :" << fLenOfProjAxis + rect2.GetHeight() << "]" << endl;
	// �и��� �˻�
	if (fLenOfProjDir + fLenOfProjDir > fLenOfProjAxis + rect2.GetHeight())
	{
		return false;
	}

	// rect2�� Right�࿡ ���͵��� �����Ѵ�.
	vProjOfDir = rect2.GetPosition() - rect1.GetPosition();
	CVectorOperation::C2DScalarProduct(&vProjOfDir, CVectorOperation::C2DDotProduct(&vProjOfDir, &rect2.GetWorldRightV()), &rect2.GetWorldRightV());
	fLenOfProjDir = CVectorOperation::C2DLength(&vProjOfDir);
	CVectorOperation::C2DScalarProduct(&vProjOfUp, CVectorOperation::C2DDotProduct(&(rect1.GetWorldUpV() * rect1.GetHeight()), &rect2.GetWorldRightV()), &rect2.GetWorldRightV());
	CVectorOperation::C2DScalarProduct(&vProjOfRight, CVectorOperation::C2DDotProduct(&(rect1.GetWorldRightV() * rect1.GetWidth()), &rect2.GetWorldRightV()), &rect2.GetWorldRightV());
	fLenOfProjAxis = CVectorOperation::C2DLength(&vProjOfUp) + CVectorOperation::C2DLength(&vProjOfRight);

	cout << "[additional information] fLenOfProjDir : [" << fLenOfProjDir << ", fLenOfProjAxis + rect2.GetWidth() :" << fLenOfProjAxis + rect2.GetWidth() << "]" << endl;
	// �и��� �˻�
	if (fLenOfProjDir + fLenOfProjDir > fLenOfProjAxis + rect2.GetWidth())
	{
		return false;
	}

	return true;
}