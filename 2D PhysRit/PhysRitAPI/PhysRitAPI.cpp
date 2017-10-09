// PhysRitAPI.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "PhysRitAPI.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

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
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PHYSRITAPI));

    MSG msg;

    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

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
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

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
	static C2DColliderRect c2dCar(17500.0f / 9.81f, 1.80f * 100.0f, 4.70f * 100.0f);
	static C2DColliderRect c2dDriver(850.0f / 9.81f, 0.50f * 100.0f, 0.90f * 100.0f);
	static C2DColliderRect c2dFuel(993.0f / 9.81f, 0.90f * 100.0f, 0.50f * 100.0f);

	static C2DColliderCircle c2dCir0(1.0f, 50.0f); 
	static C2DColliderCircle c2dCir1(100.0f, 50.0f); 
	static C2DColliderCircle c2dCir2(1.0f, 50.0f); 
	static C2DColliderCircle c2dCir3(1.0f, 50.0f); 
	static C2DColliderCircle c2dCir4(1.0f, 50.0f);
	static C2DColliderCircle c2dCir5(1.0f, 50.0f); 
	static C2DColliderCircle c2dCir6(1.0f, 50.0f); 
	static C2DColliderCircle c2dCir7(1.0f, 50.0f); 

	static C2DAssembledColliders c2dAssCar;
	static C2DAssembledColliders c2dAssRing;

	static C2DCamera c2dCamera;

	static float fSec = 0.0f;
	static float fTime = 0.0f;
	static int mark = 1;

    switch (message)
    {
	case WM_CREATE:
		SetTimer(hWnd, 1, 1, NULL);

		c2dAssCar.AttachCollider(C2DVector(0.0f, 0.0f), 0.0f, &c2dCar);
		c2dAssCar.AttachCollider(C2DVector(1.0f * 100.0f, 0.5f * 100.0f), 0.0f, &c2dDriver);
		c2dAssCar.AttachCollider(C2DVector(-2.5f * 100.0f, 0.0f), 0.0f, &c2dFuel);

		c2dAssCar.SetPosition(C2DVector(0.0f, 0.5f));
		c2dAssCar.RotateZ(3.14f / 180.0f*34.0f);

		c2dAssRing.AttachCollider(C2DVector(0.0f, 100.0f), 0.0f, &c2dCir0);
		c2dAssRing.AttachCollider(C2DVector(0.0f, -100.0f), 0.0f, &c2dCir1);
		c2dAssRing.AttachCollider(C2DVector(100.0f, 0.0f), 0.0f, &c2dCir2);
		c2dAssRing.AttachCollider(C2DVector(-100.0f, 0.0f), 0.0f, &c2dCir3);
		c2dAssRing.AttachCollider(C2DVector(100.0f, 100.0f), 0.0f, &c2dCir4);
		c2dAssRing.AttachCollider(C2DVector(100.0f, -100.0f), 0.0f, &c2dCir5);
		c2dAssRing.AttachCollider(C2DVector(-100.0f, 100.0f), 0.0f, &c2dCir6);
		c2dAssRing.AttachCollider(C2DVector(-100.0f, -100.0f), 0.0f, &c2dCir7);

		c2dAssRing.SetPosition(C2DVector(0.5f, 0.5f));

		c2dCamera.GenerateView(C2DVector(0.0f, 0.0f), C2DVector(0.0f, 1.0f));
		c2dCamera.GenerateProj(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		c2dCamera.GenerateScreen(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
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
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_UP:
			c2dCamera.Move(C2DVector(0.0f, 40.0f));
			break;
		case VK_DOWN:
			c2dCamera.Move(C2DVector(0.0f, -40.0f));
			break;
		case VK_LEFT:
			c2dCamera.Move(C2DVector(-40.0f, 0.0f));
			break;
		case VK_RIGHT:
			c2dCamera.Move(C2DVector(40.0f, 0.0f));
			break;
		case VK_CONTROL:
			c2dCamera.ZoomInOrOut(2.0f);
			break;
		case VK_SHIFT:
			c2dCamera.ZoomInOrOut(1.0f/2.0f);
			break;
		}
		break;
	case WM_TIMER:
		//c2dAssCar.RotateZ(3.14f / 180.0f*0.1f);
		c2dAssRing.RotateZ(3.14f / 180.0f*1.3f);
		fSec += 0.001f;
		fTime += -5.1f;
		if ((int)fSec % 2 == 0) mark *= -1;
		c2dAssRing.MoveCollider(C2DVector(mark * 0.2f, mark * 0.2f), &c2dCir1);
		//c2dCamera.GenerateView(C2DVector(0.0f, 0.0f), C2DVector(cosf(3.14f / 180.0f * fTime), sinf(3.14f / 180.0f * fTime)));
		InvalidateRgn(hWnd, NULL, TRUE);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
			c2dAssCar.RenderAPI(hdc, &c2dCamera);
			c2dAssRing.RenderAPI(hdc, &c2dCamera);
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
