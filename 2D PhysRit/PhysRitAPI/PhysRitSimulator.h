#pragma once
#include "Timer.h"

class CPhysRitSimulator
{
private:
	std::vector<C2DCollider*> m_fvtColliderList;
	CTimer m_Timer;
	C2DCamera m_c2dCamera;
	//������ ������ ����Ʈ�� �� �������� ĸ�ǿ� ����ϱ� ���� ���ڿ��̴�.
	//_TCHAR m_pszBuffer[50];

private:
	void CollisionProcessor(void);

public:
	CPhysRitSimulator();
	~CPhysRitSimulator();

	// �ùķ������� ������Ʈ���� ���� �� �ı��Ѵ�.
	void BuildObjects();
	void DestroyObjects();

	// �浹ü���� �浹ü ����Ʈ�� ����Ѵ�.
	void PushToColList(C2DCollider* pCol = nullptr);

	// �浹ü�� �浹ü ����Ʈ���� �����Ѵ�.
	void PopFromColList(C2DCollider* pCol = nullptr);

	// �浹ü���� ���Ű� ������ ó���Ѵ�.
	void Update(HWND hwnd);
	void Render();
	void RenderAPI(HDC hdc);

	//�������� �޽���(Ű����, ���콺 �Է�)�� ó���ϴ� �Լ��̴�. 
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

