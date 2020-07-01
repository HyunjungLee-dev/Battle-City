#pragma once
#include"Maptool.h"
#include"Font.h"
#include"Player.h"
#include"Enemy.h"
class GameManager
{
private:
	HWND m_hWnd;
	HDC m_backbufferDC;
	RECT m_ClientRect;

	Maptool* m_Map;
	GAMESTATE m_eState;

	DWORD	m_dwLastTime;
	DWORD	m_dwCurTime;
	float	m_fDeltaTime;

	Font m_Font;
	Player* m_Player;
	list<Enemy*> m_Enemylist;
	void EnemyUpdate();
	int m_iEnemyNum;

	int m_iTitleY;
	int m_iSelect;
	int m_iHiScore;
	int m_iStage;
public:
	void Init(HWND hWnd);
	void Update();
	void Render();
	void EnemyIconRender(HDC hdc);
	void EnemyCreate();
	void EnemyRender(HDC hdc);
	void Title();
	void Stage();
	void MapRender();

	Maptool* GetMapTool() {return m_Map;}
	GameManager();
	~GameManager();
};

