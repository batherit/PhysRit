// PhysRitAPI.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "PhysRitAPI.h"

#define MAX_LOADSTRING 100

#define MAX_OBSTACLES 50
#define MAX_OBSTACLES1 15

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
CPhysRitSimulator gPhysRitSimulator;
C2DColliderCircle* c2dCir[MAX_OBSTACLES];
C2DColliderCircle* c2dCir1[MAX_OBSTACLES1];

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PHYSRITAPI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PHYSRITAPI));

	MSG msg;

	// �⺻ �޽��� �����Դϴ�.
	/*while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}*/
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// test if this is a quit
			if (msg.message == WM_QUIT)
				break;

			// translate any accelerator keys
			TranslateMessage(&msg);
			// send the message to the window proc
			DispatchMessage(&msg);
		}

		gPhysRitSimulator.Update(hWnd);
		gPhysRitSimulator.Render();
	}

	gPhysRitSimulator.DestroyObjects();

    return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PHYSRITAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PHYSRITAPI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   RECT rc = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };
   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   gPhysRitSimulator.BuildObjects();

   for (int i = 0; i < MAX_OBSTACLES; i++)
   {
	   c2dCir[i] = new C2DColliderCircle(100.0f, 20.0f, 0);
	   c2dCir[i]->SetPosition(C2DVector(400.0f  * cosf(3.14f / 180.0f * (360.0f / MAX_OBSTACLES) * i) , 400.0f * sinf(3.14f / 180.0f * (360.0f / MAX_OBSTACLES) * i)));
	   gPhysRitSimulator.PushToColList(c2dCir[i]);
   }

   for (int i = 0; i < MAX_OBSTACLES1; i++)
   {
	   c2dCir1[i] = new C2DColliderCircle(100.0f, 15.0f, 0);
	   c2dCir1[i]->SetPosition(C2DVector(200.0f  * cosf(3.14f / 180.0f * (360.0f / MAX_OBSTACLES1) * i), 180.0f * sinf(3.14f / 200.0f * (360.0f / MAX_OBSTACLES1) * i)));
	   gPhysRitSimulator.PushToColList(c2dCir1[i]);
   }

   C2DColliderCircle* c2dColCir1 = new C2DColliderCircle(5.0f, 30.0f, EF_IMPURSE | EF_GRAVITY);
   C2DColliderCircle* c2dColCir2 = new C2DColliderCircle(5.0f, 30.0f, EF_IMPURSE | EF_GRAVITY);
   C2DColliderCircle* c2dColCir3 = new C2DColliderCircle(5.0f, 40.0f, EF_IMPURSE | EF_GRAVITY);
   C2DColliderCircle* c2dColCir4 = new C2DColliderCircle(5.0f, 40.0f, EF_IMPURSE | EF_GRAVITY);
   C2DColliderCircle* c2dColCir5 = new C2DColliderCircle(15.0f, 60.0f, EF_IMPURSE | EF_GRAVITY);
   C2DColliderCircle* c2dColCir6 = new C2DColliderCircle(20.0f, 80.0f, EF_IMPURSE | EF_GRAVITY);
   C2DColliders* c2dColCirs = new C2DColliders(EF_IMPURSE | EF_GRAVITY);
   c2dColCirs->AttachCollider(C2DVector(0.0f, -50.0f), 0.0f, c2dColCir1);
   c2dColCirs->AttachCollider(C2DVector(0.0f, 50.0f), 0.0f, c2dColCir2);
   c2dColCirs->SetPosition(C2DVector(-52.0f, 200.0f));
   c2dColCirs->RotateACCoordZ(3.14f / 180.0f * -45.0f);
   c2dColCir3->SetPosition(C2DVector(52.0f, 180.0f));
   gPhysRitSimulator.PushToColList(c2dColCirs);
   gPhysRitSimulator.PushToColList(c2dColCir3);
   gPhysRitSimulator.PushToColList(c2dColCir4);
   gPhysRitSimulator.PushToColList(c2dColCir5);
   gPhysRitSimulator.PushToColList(c2dColCir6);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static float fTimeStamp = 0.0f;
    switch (message)
    {
	case WM_CREATE:
		SetTimer(hWnd, 1, 1, NULL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_TIMER:
		fTimeStamp += 0.007f;
		for (int i = 0; i < MAX_OBSTACLES; i++)
		{
			c2dCir[i]->SetPosition(C2DVector(400.0f * cosf(3.14f / 180.0f * (360.0f / MAX_OBSTACLES) * i + fTimeStamp), 400.0f * sinf(3.14f / 180.0f * (360.0f / MAX_OBSTACLES) * i + fTimeStamp)));
		
			
		}

		for (int i = 0; i < MAX_OBSTACLES1; i++)
		{
			c2dCir1[i]->SetPosition(C2DVector(200.0f  * cosf(3.14f / 180.0f * (360.0f / MAX_OBSTACLES1) * i - fTimeStamp), 200.0f * sinf(3.14f / 180.0f * (360.0f / MAX_OBSTACLES1) * i - fTimeStamp)));
		}
		InvalidateRgn(hWnd, NULL, TRUE);
		break;
	case WM_SIZE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_MOUSEWHEEL:
		//gGameFramework.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
		gPhysRitSimulator.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
			gPhysRitSimulator.RenderAPI(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
