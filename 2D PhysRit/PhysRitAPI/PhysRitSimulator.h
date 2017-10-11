#pragma once
#include "Timer.h"

class CPhysRitSimulator
{
private:
	C2DColliderRect *c2dCar;
	C2DColliderRect *c2dDriver;
	C2DColliderRect *c2dFuel;

	C2DColliderCircle *c2dCir0;
	C2DColliderCircle *c2dCir1;
	C2DColliderCircle *c2dCir2;
	C2DColliderCircle *c2dCir3;
	C2DColliderCircle *c2dCir4;
	C2DColliderCircle *c2dCir5;
	C2DColliderCircle *c2dCir6;
	C2DColliderCircle *c2dCir7;

	C2DAssembledColliders *c2dAssCar;
	C2DAssembledColliders *c2dAssRing;

	CTimer m_Timer;
	C2DCamera m_c2dCamera;
	//������ ������ ����Ʈ�� �� �������� ĸ�ǿ� ����ϱ� ���� ���ڿ��̴�.
	//_TCHAR m_pszBuffer[50];

public:
	CPhysRitSimulator();
	~CPhysRitSimulator();

	// �ùķ������� ������Ʈ���� ���� �� �ı��Ѵ�.
	void BuildObjects();
	void DestroyObjects();

	// �浹ü���� ���Ű� ������ ó���Ѵ�.
	void Update();
	void Render();
	void Render(HDC hdc);

	//�������� �޽���(Ű����, ���콺 �Է�)�� ó���ϴ� �Լ��̴�. 
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

