#include<iostream>
#include"PhysRit.h"
using namespace std;

int main(void)
{
	C2DColliderRect c2dCar(17500.0f / 9.81f, 1.80f, 4.70f);
	C2DColliderRect c2dDriver(850.0f / 9.81f, 0.50f, 0.90f);
	C2DColliderRect c2dFuel(993.0f / 9.81f, 0.90f, 0.50f);

	C2DAssembledColliders c2dAssCar;

	c2dAssCar.AttachCollider(C2DVector(0.0f, 0.0f), 0.0f, &c2dCar);
	c2dAssCar.AttachCollider(C2DVector(1.0f, 0.5f), 0.0f, &c2dDriver);
	c2dAssCar.AttachCollider(C2DVector(-2.5f, 0.0f), 0.0f, &c2dFuel);

	c2dAssCar.SetPosition(C2DVector(30.5f, 30.5f));

	cout << "TotalMass : " << c2dAssCar.GetMass() << endl;
	cout << "Center Of Mass : [" << c2dAssCar.GetCenterOfMass().GetX() << ", " << c2dAssCar.GetCenterOfMass().GetY() << "]" << endl;
	cout << "Izz : " << c2dAssCar.GetIzz() << endl;

	c2dAssCar.DetachCollider(&c2dCar);
	c2dAssCar.DetachCollider(&c2dDriver);
	c2dAssCar.DetachCollider(&c2dFuel);

	cout << "TotalMass : " << c2dAssCar.GetMass() << endl;
	cout << "Center Of Mass : [" << c2dAssCar.GetCenterOfMass().GetX() << ", " << c2dAssCar.GetCenterOfMass().GetY() << "]" << endl;
	cout << "Izz : " << c2dAssCar.GetIzz() << endl;

	return 0;
}