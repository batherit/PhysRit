#include "stdafx.h"
#include "PhysRitSimulator.h"


CPhysRitSimulator::CPhysRitSimulator()
{
	c2dCar = nullptr;
	c2dDriver = nullptr;
	c2dFuel = nullptr;

	c2dCir0 = nullptr;
	c2dCir1 = nullptr;
	c2dCir2 = nullptr;
	c2dCir3 = nullptr;
	c2dCir4 = nullptr;
	c2dCir5 = nullptr;
	c2dCir6 = nullptr;
	c2dCir7 = nullptr;
}


CPhysRitSimulator::~CPhysRitSimulator()
{
}

void CPhysRitSimulator::BuildObjects()
{
	c2dCar = new C2DColliderRect(17500.0f / 9.81f, 1.80f * 100.0f, 4.70f * 100.0f);
	c2dDriver = new C2DColliderRect(850.0f / 9.81f, 0.50f * 100.0f, 0.90f * 100.0f);
	c2dFuel = new C2DColliderRect(993.0f / 9.81f, 0.90f * 100.0f, 0.50f * 100.0f);

	c2dCir0 = new C2DColliderCircle(1.0f, 50.0f);
	c2dCir1 = new C2DColliderCircle(300.0f, 50.0f);
	c2dCir2 = new C2DColliderCircle(1.0f, 50.0f);
	c2dCir3 = new C2DColliderCircle(1.0f, 50.0f);
	c2dCir4 = new C2DColliderCircle(1.0f, 50.0f);
	c2dCir5 = new C2DColliderCircle(1.0f, 50.0f);
	c2dCir6 = new C2DColliderCircle(1.0f, 50.0f);
	c2dCir7 = new C2DColliderCircle(1.0f, 50.0f);

	c2dAssCar = new C2DAssembledColliders();
	c2dAssRing = new C2DAssembledColliders();

	c2dAssCar->AttachCollider(C2DVector(0.0f, 0.0f), 0.0f, c2dCar);
	c2dAssCar->AttachCollider(C2DVector(1.0f * 100.0f, 0.5f * 100.0f), 0.0f, c2dDriver);
	c2dAssCar->AttachCollider(C2DVector(-2.5f * 100.0f, 0.0f), 0.0f, c2dFuel);

	c2dAssCar->SetPosition(C2DVector(0.0f, 0.5f));
	c2dAssCar->RotateCMCoordZ(3.14f / 180.0f*34.0f);

	c2dAssRing->AttachCollider(C2DVector(0.0f, 100.0f), 0.0f, c2dCir0);
	c2dAssRing->AttachCollider(C2DVector(0.0f, -300.0f), 0.0f, c2dCir1);
	c2dAssRing->AttachCollider(C2DVector(100.0f, 0.0f), 0.0f, c2dCir2);
	c2dAssRing->AttachCollider(C2DVector(-100.0f, 0.0f), 0.0f, c2dCir3);
	c2dAssRing->AttachCollider(C2DVector(100.0f, 100.0f), 0.0f, c2dCir4);
	c2dAssRing->AttachCollider(C2DVector(100.0f, -100.0f), 0.0f, c2dCir5);
	c2dAssRing->AttachCollider(C2DVector(-100.0f, 100.0f), 0.0f, c2dCir6);
	c2dAssRing->AttachCollider(C2DVector(-100.0f, -100.0f), 0.0f, c2dCir7);

	c2dAssRing->SetPosition(C2DVector(0.f, 0.f));

	m_c2dCamera.GenerateView(C2DVector(0.0f, 0.0f), C2DVector(0.0f, 1.0f));
	m_c2dCamera.GenerateProj(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	m_c2dCamera.GenerateScreen(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
}

void CPhysRitSimulator::DestroyObjects()
{
	if (c2dCar) delete c2dCar; 
	if (c2dDriver) delete c2dDriver;
	if (c2dFuel) delete c2dFuel;

	if (c2dCir0) delete c2dCir0;
	if (c2dCir1) delete c2dCir1;
	if (c2dCir2) delete c2dCir2;
	if (c2dCir3) delete c2dCir3;
	if (c2dCir4) delete c2dCir4;
	if (c2dCir5) delete c2dCir5;
	if (c2dCir6) delete c2dCir6;
	if (c2dCir7) delete c2dCir7;

	if (c2dAssCar) delete c2dAssCar;
	if (c2dAssRing) delete c2dAssRing;
}

// �浹ü���� ���Ű� ������ ó���Ѵ�.
void CPhysRitSimulator::Update(HWND hwnd)
{
	m_Timer.Tick();

	static float fSec = 0.0f;
	static int mark = 1;
	static bool bAttached = true;
	
	c2dAssRing->RotateCMCoordZ(3.14f / 180.0f*50.0f * m_Timer.GetTimeElapsed());
	//c2dAssRing->RotateACCoordZ(3.14f / 180.0f*50.0f * -m_Timer.GetTimeElapsed());
	fSec += m_Timer.GetTimeElapsed();
	if (fSec > (bAttached? 0.5f : 2.0f))   //mark *= -1;
	{
		if (bAttached)
		{
			c2dAssRing->DetachCollider(c2dCir1);
			bAttached = false;
		}
		else
		{
			c2dAssRing->AttachCollider(C2DVector(0.0f, -300.0f), 0.0f, c2dCir1);
			bAttached = true;
		}
		fSec = 0.0f;
	}
	//c2dAssRing->MoveCollider(C2DVector(mark * 0.0002f, mark * 0.0002f), c2dCir1);
	//InvalidateRgn(hwnd, NULL, TRUE);
}

void CPhysRitSimulator::Render()
{

}

void CPhysRitSimulator::Render(HDC hdc)
{
	c2dAssCar->RenderAPI(hdc, &m_c2dCamera);
	c2dAssRing->RenderAPI(hdc, &m_c2dCamera);
}

//�������� �޽���(Ű����, ���콺 �Է�)�� ó���ϴ� �Լ��̴�. 
void CPhysRitSimulator::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	static POINT oldClicked;
	static bool bDrag = false;

	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

	switch (nMessageID)
	{
	case WM_MOUSEWHEEL:
		switch (zDelta)
		{
		case 120:
			m_c2dCamera.ZoomInOrOut(1.0f / 2.0f);
			break;
		case -120:
			m_c2dCamera.ZoomInOrOut(2.0f);
			break;
		}
		break;
	case WM_RBUTTONDOWN:
		oldClicked.x = LOWORD(lParam);       // ���콺�� x, y �� �޾� ����
		oldClicked.y = HIWORD(lParam);
		bDrag = true;
		break;
	case WM_MOUSEMOVE:
		if (bDrag)
		{
			m_c2dCamera.Move(C2DVector(oldClicked.x - LOWORD(lParam), -(oldClicked.y - HIWORD(lParam))) * 1.0f / m_c2dCamera.GetZoomScale());
			oldClicked.x = LOWORD(lParam);
			oldClicked.y = HIWORD(lParam);
		}
		break;
	case WM_RBUTTONUP:
		oldClicked.x = 0;
		oldClicked.y = 0;
		bDrag = false;
		break;
	}
}

void CPhysRitSimulator::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		/*switch (wParam)
		{
		case VK_UP:
			m_c2dCamera.Move(C2DVector(0.0f, 40.0f));
			break;
		case VK_DOWN:
			m_c2dCamera.Move(C2DVector(0.0f, -40.0f));
			break;
		case VK_LEFT:
			m_c2dCamera.Move(C2DVector(-40.0f, 0.0f));
			break;
		case VK_RIGHT:
			m_c2dCamera.Move(C2DVector(40.0f, 0.0f));
			break;
		}*/
		break;
	}
}

LRESULT CALLBACK CPhysRitSimulator::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_MOUSEWHEEL:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}

	return 0;
}