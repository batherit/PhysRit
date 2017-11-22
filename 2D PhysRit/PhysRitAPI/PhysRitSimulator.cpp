#include "stdafx.h"
#include "PhysRitSimulator.h"


CPhysRitSimulator::CPhysRitSimulator()
{
}


CPhysRitSimulator::~CPhysRitSimulator()
{
}

void CPhysRitSimulator::BuildObjects()
{
	m_c2dCamera.GenerateView(C2DVector(0.0f, 0.0f), C2DVector(0.0f, 1.0f));
	m_c2dCamera.GenerateProj(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	m_c2dCamera.GenerateScreen(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
}

void CPhysRitSimulator::DestroyObjects()
{
}

// �浹ü���� �浹ü ����Ʈ�� ����Ѵ�.
void CPhysRitSimulator::PushToColList(C2DCollider* pCol)
{
	if (!pCol) return;
	m_fvtColliderList.push_back(pCol);
}

// �浹ü�� �浹ü ����Ʈ���� �����Ѵ�.
void CPhysRitSimulator::PopFromColList(C2DCollider* pCol)
{
	if (!pCol) return;

	std::vector<C2DCollider*>::iterator iter;

	if ((iter = std::find(m_fvtColliderList.begin(), m_fvtColliderList.end(), pCol)) != m_fvtColliderList.end())
	{
		m_fvtColliderList.erase(iter);
	}
}

void CPhysRitSimulator::CollisionProcessor(void)
{
	int iNumOfCollider = m_fvtColliderList.size();

	if (iNumOfCollider <= 1) return;

	for (int i = 0; i < iNumOfCollider - 1; i++)
	{
		for (int j = 1; j < iNumOfCollider; j++)
		{
			switch (m_fvtColliderList[j]->GetType())
			{
			case COLLIDER_TYPE_CIRCLE:
				if (m_fvtColliderList[i]->IsCollided_(dynamic_cast<C2DColliderCircle*> (m_fvtColliderList[j]), nullptr))
				{
					// ��ݷ� ��� �ذ�
					m_fvtColliderList[i]->Impurse(dynamic_cast<C2DColliderCircle*> (m_fvtColliderList[j]), 0.6f, 0.f, nullptr);
				}
				break;
			case COLLIDER_TYPE_RECT:
				if (m_fvtColliderList[i]->IsCollided_(dynamic_cast<C2DColliderRect*> (m_fvtColliderList[j]), nullptr))
				{
					// ��ݷ� ��� �ذ�
					m_fvtColliderList[i]->Impurse(dynamic_cast<C2DColliderRect*> (m_fvtColliderList[j]), 0.7f, 0.0f, nullptr);
				}
				break;
			case COLLIDER_TYPE_COLLIDERS:
				if (m_fvtColliderList[i]->IsCollided_(dynamic_cast<C2DColliders*> (m_fvtColliderList[j]), nullptr))
				{
					// ��ݷ� ��� �ذ�
					m_fvtColliderList[i]->Impurse(dynamic_cast<C2DColliders*> (m_fvtColliderList[j]), 0.6f, 0.f, nullptr);
				}
				break;
			default:
				break;
			}
		}
	}
}

// �浹ü���� ���Ű� ������ ó���Ѵ�.
void CPhysRitSimulator::Update(HWND hwnd)
{
	m_Timer.Tick();

	for (auto &collider : m_fvtColliderList)
	{
		collider->Update(m_Timer.GetTimeElapsed()/2.5f);
	}
	CollisionProcessor();
}

void CPhysRitSimulator::Render()
{

}

void CPhysRitSimulator::RenderAPI(HDC hdc)
{
	for (auto &collider : m_fvtColliderList)
	{
		collider->RenderAPI(hdc, nullptr, &m_c2dCamera);
	}
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