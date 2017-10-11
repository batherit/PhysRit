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
	//다음은 프레임 레이트를 주 윈도우의 캡션에 출력하기 위한 문자열이다.
	//_TCHAR m_pszBuffer[50];

public:
	CPhysRitSimulator();
	~CPhysRitSimulator();

	// 시뮬레이터할 오브젝트들을 생성 및 파괴한다.
	void BuildObjects();
	void DestroyObjects();

	// 충돌체들의 갱신과 렌더를 처리한다.
	void Update();
	void Render();
	void Render(HDC hdc);

	//윈도우의 메시지(키보드, 마우스 입력)를 처리하는 함수이다. 
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

