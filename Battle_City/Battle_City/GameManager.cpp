#include "GameManager.h"



GameManager::GameManager()
{
}

void GameManager::Init(HWND hWnd)
{
	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
	m_hWnd = hWnd;

	GetClientRect(m_hWnd, &m_ClientRect);
	BitMapManager::GetSingleton()->Init(m_hWnd);
	m_backbufferDC = BitMapManager::GetSingleton()->GetBufferDC();

	m_eState = GAMEWAIT;

	m_Map = new Maptool;
	m_Map->Init(hWnd);
	m_Font.Init();

	m_Player.Init();
	m_iHiScore = 20000;
	m_iTitleY = 450;
	m_iSelect = 1;// 다시 타이틀로 돌아올때 초기화 필요
}

void GameManager::Update()
{
	if (m_eState == GAMEWAIT)
	{	
		
		Title();
	}
	else
	{

	}
	Render();
}

void GameManager::Render()
{
	HDC hdc = GetDC(m_hWnd);
	//m_Map->Render(hdc);

	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	//가변비율
	RECT rect;
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, 512, 448, NULL);
	GetClientRect(m_hWnd, &rect);
	SetViewportExtEx(hdc, rect.right, rect.bottom, NULL);

	if (m_eState == GAMEWAIT)
	{
		if (m_iTitleY != 0 && m_fDeltaTime > 0.04f)
		{
			if (m_iTitleY <= 0)
				m_iTitleY = 0;
			else
				m_iTitleY -= 10;
			m_dwLastTime = m_dwCurTime;
		}
		BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc, 0, m_iTitleY, 1);
	}
	else
		BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc);
	ReleaseDC(m_hWnd, hdc);
}

void GameManager:: Title()
{
	TCHAR str[128];
	PatBlt(m_backbufferDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right, m_ClientRect.bottom, BLACKNESS);
	if(m_Player.GetScore() == 0)
		wsprintf(str, TEXT("I-%8d0"), m_Player.GetScore());
	else
		wsprintf(str, TEXT("I-%8d"), m_Player.GetScore());
	m_Font.Text(m_ClientRect.right*0.1, m_ClientRect.bottom*0.1, str, 0x00ffffff);
	wsprintf(str, TEXT("HI-%8d"), m_iHiScore);
	m_Font.Text(m_ClientRect.right*0.4, m_ClientRect.bottom*0.1, str, 0x00ffffff);
	wsprintf(str, TEXT("1  PLAYER"));
	m_Font.Text(m_ClientRect.right*0.4, m_ClientRect.bottom*0.6, str, 0x00ffffff);
	m_Font.Text(m_ClientRect.right*0.4, m_ClientRect.bottom*0.7, L"CONSTRUCTION", 0x00ffffff);

	if (m_iTitleY == 0)
	{
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			if (m_iSelect == 1)
				m_iSelect = 2;
			else
				m_iSelect = 1;
		}
		if(m_iSelect == 1)
			BitMapManager::GetSingleton()->GetImg(T_PLAYER_RT_0)->Draw(m_backbufferDC, m_ClientRect.right*0.3, m_ClientRect.bottom*0.59, 1, 1);
		else
			BitMapManager::GetSingleton()->GetImg(T_PLAYER_RT_0)->Draw(m_backbufferDC, m_ClientRect.right*0.3, m_ClientRect.bottom*0.69, 1, 1);
	}

}


GameManager::~GameManager()
{
	delete m_Map;
}
