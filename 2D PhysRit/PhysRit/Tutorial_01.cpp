#include<iostream>
#include "PhysRit.h"
using namespace std;

int main(void)
{
	C2DColliderCircle circle(1);
	C2DColliderRect rect(2, 2);

	float cirX = 0.0f;
	float cirY = 0.0f;
	float rotRadian = 0.0f;
	
	while (1)
	{
		cout << "--<<collision test>>--" << endl;
		cout << "input [cirX, cirY, rotRadian] : ";
		cin >> cirX >> cirY >> rotRadian;

		circle.SetPosition(C2DVector(cirX, cirY));

		cout <<endl<< "[circle information] Radius : " << circle.GetRadius() << ", " << "Position : [x : " << circle.GetPosition().m_fX << ", y : " << circle.GetPosition().m_fY << "]" << endl;

		rect.RotateZ(3.14f / 180.0f * rotRadian);
		
		cout << "[rect information] Width : " << rect.GetWidth() << ", " << "Height : " << rect.GetHeight() << ", " << "Position : [x : " << rect.GetPosition().m_fX << ", y : " << rect.GetPosition().m_fY << "]" << endl;
		cout << "[rect information] Right Vector : [" << rect.GetWorldRightV().m_fX << ", " << rect.GetWorldRightV().m_fY << "], " << "Up Vector : [" << rect.GetWorldUpV().m_fX << ", " << rect.GetWorldUpV().m_fY << "]" <<endl;

		if (C2DCollisionDetector::C2DIsCollided(rect, circle, nullptr))
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