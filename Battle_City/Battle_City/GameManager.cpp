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
	m_iEnemyNum = 20;

	m_Map = new Maptool;
	m_Map->Init(hWnd);
	m_Font.Init();

	m_Player = new Player; // ��ũ���丮?

	m_Player->Init();

	m_iHiScore = 20000;
	m_iTitleY = 450;
	// �ٽ� Ÿ��Ʋ�� ���ƿö� �ʱ�ȭ �ʿ�
	m_iSelect = 1;
	m_iStage = 1;

	//�׽�Ʈ
	testnum = 0;

	//��������
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
		//Enemy
			EnemyUpdate();

		//Player
			m_Player->Update(m_Map->GetMap());
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


	//��������
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
//		Rectangle(m_backbufferDC, MapStartX + m_Player.GetRect().left, MapStartY + m_Player.GetRect().top, MapStartX + m_Player.GetRect().right, MapStartY + m_Player.GetRect().bottom);
		m_Player->Render(m_backbufferDC);
		EnemyRender(m_backbufferDC);
		BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc, 0, 0, 1);
	}
	else
		BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc,0,0,1);
	ReleaseDC(m_hWnd, hdc);
}

void GameManager::EnemyIconRender(HDC hdc)
{
	int X = m_ClientRect.right*0.9;
	int Y = m_ClientRect.bottom*0.15;

	SIZE Size = BitMapManager::GetSingleton()->GetImg(OBJE_ENEMY)->GetSize();

	for (int i = 0; i < m_iEnemyNum*0.5; i++)
	{
		BitMapManager::GetSingleton()->GetImg(OBJE_ENEMY)->Draw(hdc, X, Y, 1, 1);
		BitMapManager::GetSingleton()->GetImg(OBJE_ENEMY)->Draw(hdc, X + Size.cx, Y, 1, 1);
		Y += Size.cy;
	}
}

void GameManager::EnemyCreate()//���� �ð��� ������ ����
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	if (m_Enemylist.empty())
	{
		if (m_fDeltaTime > 1.0f)
		{
			m_Enemylist.push_back(new Enemy);
			m_Enemylist.back()->Create();
			m_dwLastTime = m_dwCurTime;
			testnum++;
		}
	}
	else
	{
		if (m_Enemylist.size() != m_iEnemyNum)
		{
			if (m_fDeltaTime > 8.0f)
			{
				m_Enemylist.push_back(new Enemy);
				m_Enemylist.back()->Create();
				m_dwLastTime = m_dwCurTime;
				testnum++;
			}

		}
	}
}

void GameManager::EnemyUpdate()
{
	EnemyCreate();
	TankCollision();
	for (list<Enemy*>::iterator it = m_Enemylist.begin(); it != m_Enemylist.end(); it++)
	{
		(*it)->Update(m_Map->GetMap());
	}

}

void GameManager::EnemyRender(HDC hdc)
{
	for (list<Enemy*>::iterator it = m_Enemylist.begin(); it != m_Enemylist.end(); it++)
	{
		(*it)->Render(hdc);
	}
}

void GameManager::TankCollision()
{
	if (!m_Enemylist.empty())
	{
		list<Enemy*>::iterator iter = m_Enemylist.begin();

		while (iter != m_Enemylist.end())
		{
			for (list<Enemy*>::iterator it = m_Enemylist.begin(); it != m_Enemylist.end(); it++)
			{
				if (iter != it)
				{
					if ((*iter)->isTankfornt((*it)->GetRect()))
					{
						(*iter)->Changedirection();
						(*it)-> Changedirection();
						break;
					}
				}
			}
			iter++;
		}
	}
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

	if(m_Player->GetScore() == 0)
		wsprintf(str, TEXT("I-%8d0"), m_Player->GetScore());
	else
		wsprintf(str, TEXT("I-%8d"), m_Player->GetScore());
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

	//���
	PatBlt(m_backbufferDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right, m_ClientRect.bottom, PATCOPY);

	//��
	EnemyIconRender(m_backbufferDC);


	//�÷��̾�
	m_Font.Text(m_ClientRect.right*0.9, m_ClientRect.bottom*0.5, L"IP", 0x00000000);
	BitMapManager::GetSingleton()->GetImg(OBJE_PLAYER)->Draw(m_backbufferDC, m_ClientRect.right*0.9, m_ClientRect.bottom*0.55, 1, 1);
	wsprintf(str, TEXT("%d"),m_Player->GetLife());
	m_Font.Text(m_ClientRect.right*0.9 + BitMapManager::GetSingleton()->GetImg(OBJE_PLAYER)->GetSize().cx, m_ClientRect.bottom*0.55, str, 0x00000000);

	//��������
	BitMapManager::GetSingleton()->GetImg(OBJE_STAGE)->Draw(m_backbufferDC, m_ClientRect.right*0.9, m_ClientRect.bottom*0.7, 1, 1);
	wsprintf(str, TEXT("%d"),m_iStage);
	m_Font.Text(m_ClientRect.right*0.9 + 20 , m_ClientRect.bottom*0.7 + 20, str, 0x00000000);

	//��
	m_Map->Render(m_backbufferDC);


	wsprintf(str, TEXT("Tank pos x : %d  y : %d"), (int)m_Player->Getpos().m_iX, (int)m_Player->Getpos().m_iY);
	m_Font.Text(100, 400, str, 0x00000000);
	wsprintf(str, TEXT("enemy num %d"), testnum);
	m_Font.Text(95, 380, str, 0x00000000);

	SelectObject(m_backbufferDC, oldBrush);
	DeleteObject(myBrush);
}


GameManager::~GameManager()
{
	delete m_Map;
	delete m_Player;
	for (list<Enemy*>::iterator it = m_Enemylist.begin(); it != m_Enemylist.end(); it++)
	{
		delete (*it);
	}
	m_Enemylist.clear();
}
