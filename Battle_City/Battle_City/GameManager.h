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

	GAMESTATE m_eState;

	DWORD	m_dwLastTime;
	DWORD	m_dwCurTime;
	float	m_fDeltaTime;

	Font m_Font;
	Player* m_Player;
	list<Enemy*> m_Enemylist;

	int m_iAllEnemyNum;
	int m_ikillEnemyNum;
	int m_iCreateEnemyNum;


	int m_iScrollY;
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
	void EnemyUpdate();

	void TankCollision();
	void TankbulletCollision();


	void Title();
	void Stage();
	void GameOver();
	void MapRender();

	GameManager();
	~GameManager();
};

