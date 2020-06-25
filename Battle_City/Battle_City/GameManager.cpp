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
	MapStartX = 15;
	MapStartY = 40;

	m_Map = new Maptool;
	m_Map->Init(hWnd);
	m_Font.Init();

	m_Player.Init();
	m_Enemy.Init();

	m_iHiScore = 20000;
	m_iTitleY = 450;
	// 다시 타이틀로 돌아올때 초기화 필요
	m_iSelect = 1;
	m_iStage = 1;


	//스테이지
	TCHAR str[128];
	wsprintf(str, TEXT("STAGE0%d"), m_iStage);
	m_Map->Load(str);
}

void GameManager::Update()
{
	if (m_eState == GAMEWAIT )
		Title();
	else if (m_eState == GAMESTAGE || m_eState == GAMESTART)
	{
		if (m_eState == GAMESTART)
		{
			MapRender();
		}
		Stage();
	}
	else if (m_eState == GAMEPLAY)
	{
		m_Player.Update(m_Map->GetMap());
	}
	else
	{

	}
	Render();
}

void GameManager::Render()
{
	HDC hdc = GetDC(m_hWnd);

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
		if (m_iTitleY != 0)
		{
			if (m_fDeltaTime > 0.04f)
			{
				if (m_iTitleY <= 0)
					m_iTitleY = 0;
				else
					m_iTitleY -= 10;
				m_dwLastTime = m_dwCurTime;
			}
		}
		BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc, 0, m_iTitleY, 1);
	}
	else if (m_eState == GAMEPLAY)
	{
		PatBlt(m_backbufferDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right, m_ClientRect.bottom, BLACKNESS);
		MapRender();
		m_Player.Render(m_backbufferDC, MapStartX, MapStartY);
		BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc, 0, 0, 1);
	}
	else
		BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc,0,0,1);
	ReleaseDC(m_hWnd, hdc);
}

void GameManager:: Title() 
{
	static TANK temp = T_PLAYER_RT_0;
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	if (m_fDeltaTime > 0.05f)
	{
		if (temp == T_PLAYER_RT_0)
			temp = T_PLAYER_RT_1;
		else
			temp = T_PLAYER_RT_0;
		m_dwLastTime = m_dwCurTime;
	}

	TCHAR str[128];
	PatBlt(m_backbufferDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right, m_ClientRect.bottom, BLACKNESS);

	BitMapManager::GetSingleton()->GetImg(OBJE_TITLE)->Draw(m_backbufferDC, m_ClientRect.right*0.11, m_ClientRect.bottom*0.2, 0.7, 0.7);

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
		if (GetAsyncKeyState(VK_SHIFT) & 0x0001)
		{
			if (m_iSelect == 1)
				m_iSelect = 2;
			else
				m_iSelect = 1;
		}

		if (GetAsyncKeyState(VK_RETURN) & 0x0001)
		{
			if (m_iSelect == 1)
				m_eState = GAMESTAGE;
			else
				m_eState = GAMCONSTRUCTION;
			PatBlt(m_backbufferDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right, m_ClientRect.bottom, BLACKNESS);
			return;
		}

		if(m_iSelect == 1)
			BitMapManager::GetSingleton()->GetImg(temp)->Draw(m_backbufferDC, m_ClientRect.right*0.3, m_ClientRect.bottom*0.59, 1, 1);
		else
			BitMapManager::GetSingleton()->GetImg(temp)->Draw(m_backbufferDC, m_ClientRect.right*0.3, m_ClientRect.bottom*0.69, 1, 1);
	}

}

void GameManager::Stage()
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	static float NextTime = 0;

	static int UpY = 0;
	static int DownY = m_ClientRect.bottom;
	static bool bClose = true;

	RECT UpRct, DownRct;

	UpRct = { m_ClientRect.left, m_ClientRect.top,m_ClientRect.right, UpY};
	DownRct = { m_ClientRect.left, DownY,m_ClientRect.right,  m_ClientRect.bottom };

	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(178, 178, 178));
	HBRUSH oldBrush = (HBRUSH)SelectObject(m_backbufferDC, myBrush);
	SelectObject(m_backbufferDC, GetStockObject(NULL_PEN));

	Rectangle(m_backbufferDC, UpRct.left, UpRct.top, UpRct.right, UpRct.bottom);
	Rectangle(m_backbufferDC, DownRct.left, DownRct.top, DownRct.right, DownRct.bottom);

	if (NextTime > 0.5f)
	{
		bClose = false;
	}
	else
	{
		TCHAR str[128];
		wsprintf(str, TEXT("STAGE  %d"), m_iStage);
		m_Font.Text(m_ClientRect.right*0.4, m_ClientRect.bottom*0.5, str, 0x00000000);
	}

	if (bClose)
	{
		if (m_fDeltaTime > 0.04f)
		{
			if (DownY < m_ClientRect.bottom*0.5 && UpY > m_ClientRect.bottom*0.5)
			{
				NextTime += m_fDeltaTime;
				DownY = m_ClientRect.bottom*0.5;
				UpY = m_ClientRect.bottom*0.5;
			}
			else
			{
				UpY += 10;
				DownY -= 10;
			}
			m_dwLastTime = m_dwCurTime;
		}
	}
	else
	{
		m_eState = GAMESTART;

		if (m_fDeltaTime > 0.04f)
		{

			if (DownY > m_ClientRect.bottom && UpY < 0)
			{
				DownY = m_ClientRect.bottom;
				UpY = 0;
				bClose = true;
				NextTime = 0;
				m_eState = GAMEPLAY;
			}
			else
			{
				UpY -= 10;
				DownY += 10;
			}
			m_dwLastTime = m_dwCurTime;
		}
	}

	
	SelectObject(m_backbufferDC, oldBrush);
	DeleteObject(myBrush);
}

void GameManager::MapRender()
{
	TCHAR str[128];

	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(178, 178, 178));
	HBRUSH oldBrush = (HBRUSH)SelectObject(m_backbufferDC, myBrush);

	//배경
	PatBlt(m_backbufferDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right, m_ClientRect.bottom, PATCOPY);

	//적
	m_Enemy.EnemyIconRender(m_backbufferDC, m_ClientRect.right*0.9, m_ClientRect.bottom*0.15);


	//플레이어
	m_Font.Text(m_ClientRect.right*0.9, m_ClientRect.bottom*0.5, L"IP", 0x00000000);
	BitMapManager::GetSingleton()->GetImg(OBJE_PLAYER)->Draw(m_backbufferDC, m_ClientRect.right*0.9, m_ClientRect.bottom*0.55, 1, 1);
	wsprintf(str, TEXT("%d"),m_Player.GetLife());
	m_Font.Text(m_ClientRect.right*0.9 + BitMapManager::GetSingleton()->GetImg(OBJE_PLAYER)->GetSize().cx, m_ClientRect.bottom*0.55, str, 0x00000000);

	//스테이지
	BitMapManager::GetSingleton()->GetImg(OBJE_STAGE)->Draw(m_backbufferDC, m_ClientRect.right*0.9, m_ClientRect.bottom*0.7, 1, 1);
	wsprintf(str, TEXT("%d"),m_iStage);
	m_Font.Text(m_ClientRect.right*0.9 + 20 , m_ClientRect.bottom*0.7 + 20, str, 0x00000000);

	//맵
	m_Map->Render(m_backbufferDC, MapStartX, MapStartY);


	wsprintf(str, TEXT("Tank pos x : %d  y : %d"), m_Player.Getpos().m_iX, m_Player.Getpos().m_iY);
	m_Font.Text(100, 400, str, 0x00000000);
	wsprintf(str, TEXT("Tank index %d"), m_Player.Getindex());
	m_Font.Text(95, 380, str, 0x00000000);

	SelectObject(m_backbufferDC, oldBrush);
	DeleteObject(myBrush);
}


GameManager::~GameManager()
{
	delete m_Map;
}
