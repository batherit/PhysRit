// PhysRitAPI.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "PhysRitAPI.h"

#define MAX_LOADSTRING 100

#define MAX_OBSTACLES 50
#define MAX_OBSTACLES1 15

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
CPhysRitSimulator gPhysRitSimulator;
C2DColliderCircle* c2dCir[MAX_OBSTACLES];
C2DColliderCircle* c2dCir1[MAX_OBSTACLES1];

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PHYSRITAPI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PHYSRITAPI));

	MSG msg;

	// 기본 메시지 루프입니다.
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
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
            // 메뉴 선택을 구문 분석합니다.
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
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

// 정보 대화 상자의 메시지 처리기입니다.
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
