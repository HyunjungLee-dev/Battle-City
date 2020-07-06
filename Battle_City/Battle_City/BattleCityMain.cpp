#include"Maptool.h"
#include"GameManager.h"
#include"resource.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK StageDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Battle City");

GameManager g_game;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPervlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	srand((unsigned int)time(NULL));
	g_hInst = hInstance;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	RECT rt = { 0, 0, 512 ,448 };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rt.right - rt.left, rt.bottom - rt.top, NULL, (HMENU)NULL, hInstance, NULL);
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
	g_game.~GameManager();
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
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_STAGE:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, StageDlgProc);
			break;
		case ID_EXIT:
			PostQuitMessage(0);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}


INT_PTR CALLBACK StageDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int Editnum;

	Editnum = GetDlgItemInt(hDlg, IDC_EDIT, NULL, NULL);

	switch (uMsg)
	{
	case WM_COMMAND:
		switch (wParam)
		{
			case IDOK:
				if (Editnum > 3 || Editnum < 1)
					return (INT_PTR)TRUE;
				g_game.SetStage(Editnum);
				EndDialog(hDlg, 0);
				return (INT_PTR)TRUE;
			case IDCANCEL:
				EndDialog(hDlg, 0);
				return (INT_PTR)TRUE;
		}
	}
	return(INT_PTR)FALSE;
}
