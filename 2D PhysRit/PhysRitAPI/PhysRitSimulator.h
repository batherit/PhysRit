#pragma once
#include "Timer.h"

class CPhysRitSimulator
{
private:
	std::vector<C2DCollider*> m_fvtColliderList;
	CTimer m_Timer;
	C2DCamera m_c2dCamera;
	//다음은 프레임 레이트를 주 윈도우의 캡션에 출력하기 위한 문자열이다.
	//_TCHAR m_pszBuffer[50];

private:
	void CollisionProcessor(void);

public:
	CPhysRitSimulator();
	~CPhysRitSimulator();

	// 시뮬레이터할 오브젝트들을 생성 및 파괴한다.
	void BuildObjects();
	void DestroyObjects();

	// 충돌체들을 충돌체 리스트에 등록한다.
	void PushToColList(C2DCollider* pCol = nullptr);

	// 충돌체를 충돌체 리스트에서 제거한다.
	void PopFromColList(C2DCollider* pCol = nullptr);

	// 충돌체들의 갱신과 렌더를 처리한다.
	void Update(HWND hwnd);
	void Render();
	void RenderAPI(HDC hdc);

	//윈도우의 메시지(키보드, 마우스 입력)를 처리하는 함수이다. 
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

