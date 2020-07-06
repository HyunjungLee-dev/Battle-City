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
	m_iScore = 0;
	m_iHiScore = 20000;

	Font::GetSingleton()->Init();

	ReInit();

}

void GameManager::ReInit()
{
	Maptool::GetSingleton()->Init();

	m_iScrollY = 450;
	m_bGameOver = false;
	
	m_eNowItem = ITEM_NONE;
	m_iSelect = 1;
	if(m_eState != GAMENEXT)
		m_iStage = 1;
	m_iAllEnemyNum = 20;
	m_iCreateEnemyNum = m_iAllEnemyNum;
	m_ikillEnemyNum = 0;

	m_Player = new Player;
	m_Player->Init();


	TCHAR str[128];
	wsprintf(str, TEXT("STAGE0%d"), m_iStage);
	Maptool::GetSingleton()->Load(str);

}


void GameManager::Update()
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;

	if (m_eState == GAMEWAIT)
	{
		Title();
	}
	else if (m_eState == GAMERESTART)
	{
		PatBlt(m_backbufferDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right, m_ClientRect.bottom, BLACKNESS);
		Clear();
		ReInit();
		m_eState = GAMEWAIT;
	}
	else if (m_eState == GAMCONSTRUCTION)
	{
		if (Maptool::GetSingleton()->MapConstruction())
		{
			Maptool::GetSingleton()->Load(TEXT("Construction"));
			m_eState = GAMEWAIT;
		}

	}
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
		m_bGameOver = false;

		if (m_Player->GetLife() < 0 || Maptool::GetSingleton()->FlagCheck())
		{
			m_bGameOver = true;
			if (m_iScrollY == 0)
				m_iScrollY = 450;
		}
		else if (m_iAllEnemyNum == m_ikillEnemyNum)
		{
			m_eState = GAMENEXT;
		}

		ItemUpdate();
		ExsplosionAfter();
		EnemyUpdate();
		m_Player->Update(Maptool::GetSingleton()->GetMap(), m_fDeltaTime);
		TankbulletCollision();

	}
	else if (m_eState == GAMEEND || m_eState == GAMEOVER || m_eState == GAMENEXT)
		GameEnd();
 	Render();
}

void GameManager::Render()
{
 	HDC hdc = GetDC(m_hWnd);

	static float m_fMainTime = 0.0f;
	static float m_fOverTime = 0.0f;

	RECT rect;
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, 512, 448, NULL);
	GetClientRect(m_hWnd, &rect);
	SetViewportExtEx(hdc, rect.right, rect.bottom, NULL);

	if (m_eState == GAMEWAIT)
	{
		m_fMainTime += m_fDeltaTime;

		if (m_iScrollY != 0)
		{
			if (m_fMainTime > 0.04f)
			{
				if (m_iScrollY <= 0)
					m_iScrollY = 0;
				else
					m_iScrollY -= 10;
				m_fMainTime = 0.0f;
			}
		}
		BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc, 0, m_iScrollY, 1);
	}
	else if (m_eState == GAMEPLAY )
	{


		PatBlt(m_backbufferDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right, m_ClientRect.bottom, BLACKNESS);
		MapRender();
		m_Player->Render(m_backbufferDC, m_fDeltaTime);
		EnemyRender(m_backbufferDC);

		Maptool::GetSingleton()->layerRender(m_backbufferDC);
		ItemlistDraw();

		if (m_bGameOver)
		{
			m_fOverTime += m_fDeltaTime;
			if (m_iScrollY != m_ClientRect.bottom*0.5)
			{
				if (m_fOverTime > 0.04f)
				{
					if (m_iScrollY <= m_ClientRect.bottom*0.5)
						m_iScrollY = m_ClientRect.bottom*0.5;
					else
						m_iScrollY -= 10;
					m_fOverTime = 0.0f;
				}
			}
			else
			{
				if (m_fOverTime > 3.0f)
				{
					m_fOverTime = 0.0f;
					m_eState = GAMEEND;
				}
			}

			Font::GetSingleton()->Text(m_ClientRect.right*0.4, m_iScrollY, L"GAME", RGB(255, 187, 0));
			Font::GetSingleton()->Text(m_ClientRect.right*0.4, m_iScrollY + 12, L"OVER", RGB(255, 187, 0));
		}
		BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc, 0, 0, 1);
	}
	else if (m_eState == GAMCONSTRUCTION)
	{
		TCHAR str[128];



		HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(178, 178, 178));
		HBRUSH oldBrush = (HBRUSH)SelectObject(m_backbufferDC, myBrush);

		PatBlt(m_backbufferDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right, m_ClientRect.bottom, PATCOPY);
		Maptool::GetSingleton()->Render(m_backbufferDC);
		Maptool::GetSingleton()->CursorRender(m_backbufferDC);

		wsprintf(str, TEXT("z : Tile Change /  Enter : Save"), (int)m_Player->Getpos().m_iX, (int)m_Player->Getpos().m_iY);
		Font::GetSingleton()->Text(100, 400, str, 0x00000000);

		BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc, 0, 0, 1);
		SelectObject(m_backbufferDC, oldBrush);
		DeleteObject(myBrush);
	}
	else
		BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc,0,0,1);
	ReleaseDC(m_hWnd, hdc);
}

void GameManager::GameEnd()
{
	static float m_fNumTime = 0.0f;

	m_fNumTime += m_fDeltaTime;

	PatBlt(m_backbufferDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right, m_ClientRect.bottom, BLACKNESS);
	if (m_eState == GAMEOVER)
	{
		PatBlt(m_backbufferDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right, m_ClientRect.bottom, BLACKNESS);
		BitMapManager::GetSingleton()->GetImg(OBJE_GAMEOVER)->Draw(m_backbufferDC, m_ClientRect.right*0.2, m_ClientRect.bottom*0.2, 2, 2);
		if (m_fNumTime > 2.0f)
		{
			if (m_iScore > m_iHiScore)
			{
				m_iHiScore = m_iScore;
			}
			m_eState = GAMERESTART;
			m_fNumTime = 0.0f;
		}
	}
	else
		ScoreCheck(&m_fNumTime);
	

}

void GameManager::ScoreCheck(float *Time)
{
	TCHAR str[128];
	static int TankNum = 0;
	static int TankScore = 0;
	int TotalNum = 0;

	

	wsprintf(str, TEXT("%8d"), m_iHiScore);
	Font::GetSingleton()->Text(m_ClientRect.right*0.3, m_ClientRect.bottom*0.2, TEXT("HI-SCORE"), RGB(255, 94, 0));
	Font::GetSingleton()->Text(m_ClientRect.right*0.53, m_ClientRect.bottom*0.2, str, RGB(255, 187, 0));

	wsprintf(str, TEXT("STAGE  %d"), m_iStage);
	Font::GetSingleton()->Text(m_ClientRect.right*0.43, m_ClientRect.bottom*0.3, str, 0x00FFFFFF);

	Font::GetSingleton()->Text(m_ClientRect.right*0.2, m_ClientRect.bottom*0.4, TEXT("I-PLAYER"), RGB(255, 94, 0));
	wsprintf(str, TEXT(" %8d"), m_iScore);
	Font::GetSingleton()->Text(m_ClientRect.right*0.175, m_ClientRect.bottom*0.45, str, RGB(255, 187, 0));

	Font::GetSingleton()->Text(m_ClientRect.right*0.31, m_ClientRect.bottom*0.6, TEXT("PTS"), 0x00FFFFFF);

	BitMapManager::GetSingleton()->GetImg(OBJE_SCORE)->Draw(m_backbufferDC, m_ClientRect.right*0.4, m_ClientRect.bottom*0.57, 0.8, 0.8);

	Font::GetSingleton()->Text(m_ClientRect.right*0.26, m_ClientRect.bottom*0.68, TEXT("TOTAL"), 0x00FFFFFF);

	wsprintf(str, TEXT("%8d"), TankScore);
	Font::GetSingleton()->Text(m_ClientRect.right*0.1, m_ClientRect.bottom*0.6, str, 0x00FFFFFF);

	wsprintf(str, TEXT("%2d"), TankNum);
	Font::GetSingleton()->Text(m_ClientRect.right*0.4, m_ClientRect.bottom*0.6, str, 0x00FFFFFF);

	if (TankNum != m_ikillEnemyNum)
	{
		if (*Time > 0.2f)
		{
			TankScore += 100;
			TankNum += 1;

			*Time = 0.0f;
		}
	}
	else
	{
		TotalNum = TankNum;
		wsprintf(str, TEXT("%2d"), TotalNum);
		Font::GetSingleton()->Text(m_ClientRect.right*0.4, m_ClientRect.bottom*0.68, str, 0x00FFFFFF);
		if (*Time > 2.0f)
		{
			m_iScore += TankScore;
			if (m_bGameOver)
			{
				m_eState = GAMEOVER;
			}
			else if (m_eState == GAMENEXT)
			{
				m_iStage++;
				if (m_iStage > MAXSTAGE)
				{
					m_iStage = 1;
				}
				ReInit();
				m_eState = GAMESTAGE;
			}
			else
				m_eState = GAMERESTART;

			TankNum = 0;
			TankScore = 0;
			*Time = 0.0f;
			return;

		}


	}



}

void GameManager::EnemyIconRender(HDC hdc)
{
	int X = m_ClientRect.right*0.9;
	int Y = m_ClientRect.bottom*0.15;

	SIZE Size = BitMapManager::GetSingleton()->GetImg(OBJE_ENEMY)->GetSize();

	for (int i = 1; i <= m_iCreateEnemyNum; i++)
	{
		if (i % 2 == 0)
		{
			BitMapManager::GetSingleton()->GetImg(OBJE_ENEMY)->Draw(hdc, X + Size.cx, Y, 1, 1);
			Y += Size.cy;
		}
		else
			BitMapManager::GetSingleton()->GetImg(OBJE_ENEMY)->Draw(hdc, X, Y, 1, 1);
	}
}

void GameManager::EnemyCreate()
{
	static float m_fCreateTime = 0.0f;
	
		if ( m_iCreateEnemyNum < m_iAllEnemyNum && m_iCreateEnemyNum != 0)
		{
			if (!m_Enemylist.empty() && m_Enemylist.front()->GetItemFlage())
			{
				return;
			}
				m_fCreateTime += m_fDeltaTime;

				if (m_fCreateTime > 5.0f)
				{
					m_Enemylist.push_back(new Enemy);
					m_Enemylist.back()->Init();
					while (1)
					{
						if (!CreatePosCheck(m_Enemylist.back()->Getpos()))
						{
							m_Enemylist.back()->SetPos();
						}
						else
							break;
					}
					if (m_iCreateEnemyNum == 3 || m_iCreateEnemyNum == 10 || m_iCreateEnemyNum == 17)
						m_Enemylist.back()->SetTankType(TYPEITEM_ENEMY);
					m_iCreateEnemyNum--;
					if (m_iCreateEnemyNum < 0)
						m_iCreateEnemyNum = 0;
					m_fCreateTime = 0.0f;
				}
			
		}
		else if(m_iCreateEnemyNum == m_iAllEnemyNum)
		{
			m_Enemylist.push_back(new Enemy);
			m_Enemylist.back()->Init();		
			m_iCreateEnemyNum--;
		}
	
}

void GameManager::EnemyUpdate()
{
	EnemyCreate();
	TankCollision();
	for (list<Enemy*>::iterator it = m_Enemylist.begin(); it != m_Enemylist.end(); it++)
	{
		(*it)->Update(Maptool::GetSingleton()->GetMap(), m_fDeltaTime);
	}


}

void GameManager::ExsplosionAfter()
{
	if (m_Player->GetState() == TANKNONE)
	{
		m_Player->SetLife(m_Player->GetLife() - 1);
		if(m_Player->GetLife() >= 0)
			m_Player->Respon();
	}

	for (list<Enemy*>::iterator it = m_Enemylist.begin(); it != m_Enemylist.end(); it++)
	{

		if ((*it)->GetState() == TANKNONE)
		{
			m_ikillEnemyNum++;
			delete (*it);
			m_Enemylist.erase(it);
			break;
		}
	}
}

void GameManager::EnemyRender(HDC hdc)
{
	for (list<Enemy*>::iterator it = m_Enemylist.begin(); it != m_Enemylist.end(); it++)
	{
			(*it)->Render(hdc, m_fDeltaTime);
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
				if ((*iter)->GetState() != TANKEXSPLOSION )
				{
					if (iter != it)
					{
						if ((*iter)->isTankfornt((*it)->GetRect()))
						{
							(*iter)->Changedirection();
							(*it)->Changedirection();
							break;
						}
					}
					if ((*iter)->isTankfornt(m_Player->GetRect()))
					{
						(*iter)->Changedirection();
						break;
					}
				}
			}
			iter++;
		}
	}
}

void GameManager::TankbulletCollision()
{

		for (list<Enemy*>::iterator it = m_Enemylist.begin(); it != m_Enemylist.end(); it++)
		{
			float cx = (*it)->GetCenterPos().m_iX;
			float cy = (*it)->GetCenterPos().m_iY;

			if ((*it)->GetState() != TANKEXSPLOSION && (*it) ->GetState() != TANKAPPEAR && m_Player->GetBullet()->IsPointInCircle(cx, cy) )
			{
				if ((*it)->GetType() == TYPEITEM_ENEMY)
				{
					CreateItem();
				}
				(*it)->SetTankState(TANKEXSPLOSION);
				break;
			}

			float pcx = m_Player->GetCenterPos().m_iX;
			float pcy = m_Player->GetCenterPos().m_iY;

			if (m_Player->GetState() != TANKEXSPLOSION &&  m_Player->GetState() != TANKAPPEAR  &&m_Player->GetState() != TANKSHIELD && (*it)->GetBullet()->IsPointInCircle(pcx, pcy) )
			{	
				m_Player->SetTankState(TANKEXSPLOSION);

			}
			
		}
}

void GameManager::CreateItem()
{
//	ITEM random = ITEM (rand() % ITEM_TANK + 1);
	ITEM random = ITEM_TANK;

	switch (random)
	{
	case ITEM_BOMB:
		m_Itemlist.push_back(new Bomb);
		break;
	case ITEM_CLOCK:
		m_Itemlist.push_back(new Clock);
		break;
	case ITEM_HELMET:
		m_Itemlist.push_back(new Helmet);
		break;
	case ITEM_SHOVEL:
		m_Itemlist.push_back(new Shovel);
		break;
	case ITEM_TANK:
		m_Itemlist.push_back(new ItemTank);
		break;
	default:
		break;
	}

	m_Itemlist.back()->Init();
}

void GameManager::ItemlistDraw()
{
	for (list<Item*>::iterator it = m_Itemlist.begin(); it != m_Itemlist.end(); it++)
	{
		(*it)->Draw();
	}
}

void GameManager::ItemUpdate()
{
	static float m_fTime = 0.0f;
	static float m_fEnemyItemTime = 0.0f;

	if (Maptool::GetSingleton()->GetItemFlag())
	{
		m_fTime += m_fDeltaTime;
		if (m_fTime > 5.0f)
		{
			Maptool::GetSingleton()->SetItemFlag(false);
			Maptool::GetSingleton()->ItemTileChange();
			m_fTime = 0.0f;
		}
	}

	if (!m_Enemylist.empty() && m_Enemylist.front()->GetItemFlage())
	{
		m_fEnemyItemTime += m_fDeltaTime;
		if (m_fEnemyItemTime > 5.0f)
		{
			for (list<Enemy*>::iterator it = m_Enemylist.begin(); it != m_Enemylist.end(); it++)
			{
				(*it)->SetItemFlag(false);
			}
			m_fEnemyItemTime = 0.0f;
		}
	}

	if (m_eNowItem == ITEM_NONE)
	{
		for (list<Item*>::iterator it = m_Itemlist.begin(); it != m_Itemlist.end(); it++)
		{
			if ((*it)->Collision(m_Player->GetRect()))
			{
				switch ((*it)->GetItemType())
				{
				case ITEM_BOMB:
					m_eNowItem = ITEM_BOMB;
					break;
				case ITEM_CLOCK:
					m_eNowItem = ITEM_CLOCK;
					break;
				case ITEM_HELMET:
					m_eNowItem = ITEM_HELMET;
					break;
				case ITEM_SHOVEL:
					m_eNowItem = ITEM_SHOVEL;
					break;
				case ITEM_TANK:
					m_eNowItem = ITEM_TANK;
					break;
				default:
					break;
				}
				break;

			}
		}
	}
	else if (m_eNowItem == ITEM_CHECK)
	{
		for (list<Item*>::iterator it = m_Itemlist.begin(); it != m_Itemlist.end(); it++)
		{

			if ((*it)->GetItemFlage() && (*it)->GetTime() == 2.0f)
			{
				m_eNowItem = ITEM_NONE;
				m_iScore += (*it)->GetPoint();
				delete (*it);
				m_Itemlist.erase(it);
				break;
			}
		}
	}
	else
	{
			switch (m_eNowItem)
			{
			case ITEM_BOMB: //화면상의 적 탱크를 전멸시킨다.
				m_ikillEnemyNum += m_Enemylist.size();
				EnemyListBomb();
				break;
			case ITEM_CLOCK://일정시간동안 적 택크의 움직임이 멈춘다.
				for (list<Enemy*>::iterator it = m_Enemylist.begin(); it != m_Enemylist.end(); it++)
				{
					(*it)->SetItemFlag(true);
				}
				break;
			case ITEM_HELMET:// 일정시간동안 실드
				m_Player->SetTankState(TANKSHIELD);
				break;
			case ITEM_SHOVEL://일정시간동안 사령부 주변 벽을 회색 블럭으로 변경
				Maptool::GetSingleton()->SetItemFlag(true);
				Maptool::GetSingleton()->ItemTileChange();
				break;
			case ITEM_TANK://라이프 1
				m_Player->SetLife(m_Player->GetLife() + 1);
				break;
			default:
				break;
			}

			m_eNowItem = ITEM_CHECK;
	}
}

bool GameManager::CreatePosCheck(POS pos)
{
	int index = (pos.m_iY / TILESIZEY) * TILEX + (pos.m_iX / TILESIZEX);

	if (Maptool::GetSingleton()->GetMap().at(index)->eTileID != MAP_NONE)
		return false;

	RECT tmp;
	RECT rct = { pos.m_iX ,pos.m_iY ,pos.m_iX + TILESIZEX ,pos.m_iY + TILESIZEY };

	if (m_Player != NULL)
	{
		if (IntersectRect(&tmp, &m_Player->GetRect(), &rct))
			return false;
	}

	for (list<Enemy*>::iterator it = m_Enemylist.begin(); it != m_Enemylist.end(); it++)
	{
		if ((*it)->Getpos().m_iX != pos.m_iX && (*it)->Getpos().m_iY != pos.m_iY)
		{
			if (IntersectRect(&tmp, &(*it)->GetRect(), &rct))
				return false;
		}
	}

	return true;

}

void GameManager:: Title() 
{
	static TANK temp = T_PLAYER_RT_0;
	static float m_fMoveTime = 0.0f;

	m_fMoveTime += m_fDeltaTime;

	if (m_fMoveTime > 0.05f)
	{
		if (temp == T_PLAYER_RT_0)
			temp = T_PLAYER_RT_1;
		else
			temp = T_PLAYER_RT_0;
		m_fMoveTime = 0.0f;
	}

	TCHAR str[128];
	PatBlt(m_backbufferDC, m_ClientRect.left, m_ClientRect.top, m_ClientRect.right, m_ClientRect.bottom, BLACKNESS);

	BitMapManager::GetSingleton()->GetImg(OBJE_TITLE)->Draw(m_backbufferDC, m_ClientRect.right*0.11, m_ClientRect.bottom*0.2, 0.7, 0.7);

	if(m_iScore == 0)
		wsprintf(str, TEXT("I-%8d0"), m_iScore);
	else
		wsprintf(str, TEXT("I-%8d"), m_iScore);
	Font::GetSingleton()->Text(m_ClientRect.right*0.1, m_ClientRect.bottom*0.1, str, 0x00ffffff);
	wsprintf(str, TEXT("HI-%8d"), m_iHiScore);
	Font::GetSingleton()->Text(m_ClientRect.right*0.4, m_ClientRect.bottom*0.1, str, 0x00ffffff);



	wsprintf(str, TEXT("1  PLAYER"));
	Font::GetSingleton()->Text(m_ClientRect.right*0.4, m_ClientRect.bottom*0.6, str, 0x00ffffff);
	Font::GetSingleton()->Text(m_ClientRect.right*0.4, m_ClientRect.bottom*0.7, L"CONSTRUCTION", 0x00ffffff);

	wsprintf(str, TEXT("Shift | Enter "));
	Font::GetSingleton()->Text(m_ClientRect.right*0.6, m_ClientRect.bottom*0.9, str, 0x00ffffff);

	if (m_iScrollY == 0)
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
			{
				m_eState = GAMESTAGE;
				m_iScore = 0;
			}
			else
			{
				m_eState = GAMCONSTRUCTION;
				Maptool::GetSingleton()->Clear();
				Maptool::GetSingleton()->SetMap();
			}
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
	for (list<Item*>::iterator it = m_Itemlist.begin(); it != m_Itemlist.end(); it++)
	{
		delete (*it);
	}
	m_Itemlist.clear();
	m_eNowItem = ITEM_NONE;

	static float NextTime = 0.0f;

	static int UpY = 0;
	static int DownY = m_ClientRect.bottom;
	static bool bClose = true;
	static float m_fStageTime = 0.0f;

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
		Font::GetSingleton()->Text(m_ClientRect.right*0.4, m_ClientRect.bottom*0.5, str, 0x00000000);
	}

	if (bClose)
	{
		m_fStageTime += m_fDeltaTime;
		if (m_fStageTime > 0.04f)
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
			m_fStageTime = 0.0f;
		}
	}
	else
	{
		m_eState = GAMESTART;
		m_fStageTime += m_fDeltaTime;
		if (m_fStageTime > 0.04f)
		{

			if (DownY > m_ClientRect.bottom && UpY < 0)
			{
				DownY = m_ClientRect.bottom;
				UpY = 0;
				bClose = true;
				NextTime = 0;
				m_eState = GAMEPLAY;
				m_Player->SetLife(m_Player->GetLife() - 1);
			}
			else
			{
				UpY -= 10;
				DownY += 10;
			}
			m_fStageTime = 0.0f;
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
	EnemyIconRender(m_backbufferDC);


	//플레이어
	Font::GetSingleton()->Text(m_ClientRect.right*0.9, m_ClientRect.bottom*0.5, L"IP", 0x00000000);
	BitMapManager::GetSingleton()->GetImg(OBJE_PLAYER)->Draw(m_backbufferDC, m_ClientRect.right*0.9, m_ClientRect.bottom*0.55, 1, 1);
	if(m_Player->GetLife() < 0)
		wsprintf(str, TEXT("%d"), 0);
	else
		wsprintf(str, TEXT("%d"),m_Player->GetLife());
	Font::GetSingleton()->Text(m_ClientRect.right*0.9 + BitMapManager::GetSingleton()->GetImg(OBJE_PLAYER)->GetSize().cx, m_ClientRect.bottom*0.55, str, 0x00000000);

	//스테이지
	BitMapManager::GetSingleton()->GetImg(OBJE_STAGE)->Draw(m_backbufferDC, m_ClientRect.right*0.9, m_ClientRect.bottom*0.7, 1, 1);
	wsprintf(str, TEXT("%d"),m_iStage);
	Font::GetSingleton()->Text(m_ClientRect.right*0.9 + 20 , m_ClientRect.bottom*0.7 + 20, str, 0x00000000);

	//맵
	Maptool::GetSingleton()->Render(m_backbufferDC);


	wsprintf(str, TEXT("z : Bullet "));
	Font::GetSingleton()->Text(m_ClientRect.right*0.6, m_ClientRect.bottom*0.9, str, 0x00000000);


	SelectObject(m_backbufferDC, oldBrush);
	DeleteObject(myBrush);
}

void GameManager::EnemyListBomb()
{
	for (list<Enemy*>::iterator it = m_Enemylist.begin(); it != m_Enemylist.end(); it++)
	{
		(*it)->SetTankState(TANKEXSPLOSION);
	}

}


void GameManager::Clear()
{
	if (m_Player != NULL)
	{
		delete m_Player;
		m_Player = NULL;
	}

	for (list<Enemy*>::iterator it = m_Enemylist.begin(); it != m_Enemylist.end(); it++)
	{
		delete (*it);
	}
	m_Enemylist.clear();

	for (list<Item*>::iterator it = m_Itemlist.begin(); it != m_Itemlist.end(); it++)
	{
		delete (*it);
	}
	m_Itemlist.clear();
}

GameManager::~GameManager()
{
	Clear();
	Maptool::GetSingleton()->Clear();
	Maptool::Release();

	BitMapManager::GetSingleton()->Clear();
	BitMapManager::Release();
}
