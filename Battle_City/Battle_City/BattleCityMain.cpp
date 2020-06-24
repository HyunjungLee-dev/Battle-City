#include"Maptool.h"
#include"GameManager.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Battle City");

GameManager g_game;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPervlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	RECT rt = { 0, 0, 512 ,448 };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rt.right - rt.left, rt.bottom - rt.top, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	ShowWindow(hWnd, nCmdShow);

	//Init
	g_game.Init(hWnd);
	while (true)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			g_game.Update();
			//Update
		}

	}
	//Release
	return (int)Message.wParam;
}

static int cur_select = -1;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int cur_select;

	switch (iMessage)
	{
	case WM_CREATE:
		//CreateWindow(L"button", L"Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 400, 100, 30, hWnd, (HMENU)100, g_hInst, NULL);
		//CreateWindow(L"button", L"Load", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 400, 100, 30, hWnd, (HMENU)101, g_hInst, NULL);
		//CreateWindow(L"button", L"LoadJson", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 400, 100, 30, hWnd, (HMENU)102, g_hInst, NULL);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 100: //SAVE
		{
			//g_game.GetMapTool()->Save();
		}
		break;
		case 101://load
		{
			//g_game.GetMapTool()->Load();
		}
		break;
		}
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		/*POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		g_game.GetMapTool()->Create(pt);
		break;*/
	}
	return 0;
	case WM_RBUTTONDOWN:
	{
	}
	return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
