#include<iostream>
#include "PhysRit.h"
using namespace std;

int main(void)
{
	C2DColliderRect rect1(2, 2);
	C2DColliderRect rect2(4, 2);

	float rect2X = 0.0f;
	float rect2Y = 0.0f;
	float rect2Radian = 0.0f;

	while (1)
	{
		cout << "--<<collision test>>--" << endl;
		cout << "input [rect2X, rect2Y, rect2Radian] : ";
		cin >> rect2X >> rect2Y >> rect2Radian;

		rect2.SetPosition(C2DVector(rect2X, rect2Y));
		rect2.RotateZ(3.14f / 180.0f * rect2Radian);

		cout << "[rect1 information] Width : " << rect1.GetWidth() << ", " << "Height : " << rect1.GetHeight() << ", " << "Position : [x : " << rect1.GetPosition().m_fX << ", y : " << rect1.GetPosition().m_fY << "]" << endl;
		cout << "[rect1 information] Right Vector : [" << rect1.GetWorldRightV().m_fX << ", " << rect1.GetWorldRightV().m_fY << "], " << "Up Vector : [" << rect1.GetWorldUpV().m_fX << ", " << rect1.GetWorldUpV().m_fY << "]" << endl;

		cout << "[rect2 information] Width : " << rect2.GetWidth() << ", " << "Height : " << rect2.GetHeight() << ", " << "Position : [x : " << rect2.GetPosition().m_fX << ", y : " << rect2.GetPosition().m_fY << "]" << endl;
		cout << "[rect2 information] Right Vector : [" << rect2.GetWorldRightV().m_fX << ", " << rect2.GetWorldRightV().m_fY << "], " << "Up Vector : [" << rect2.GetWorldUpV().m_fX << ", " << rect2.GetWorldUpV().m_fY << "]" << endl;

		if (C2DCollisionDetector::C2DIsCollided_OBB(rect1, rect2, nullptr))
		{
			cout << "[collision result] true" << endl << endl << endl;
		}
		else
		{
			cout << "[collision result] false" << endl << endl << endl;
		}
	}


	return 0;
}