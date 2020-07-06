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
	bool m_bGameOver;


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
	int m_iScore;
	int m_iStage;
public:
	void Init(HWND hWnd);
	void ReInit();
	void Update();
	void Render();


	//EnemyManagerClass
	void EnemyIconRender(HDC hdc);
	void EnemyCreate();
	void EnemyRender(HDC hdc);
	void EnemyUpdate();
	void EnemyPointRender();

	void TankCollision();
	void TankbulletCollision();
	bool CreatePosCheck(POS pos);


	void Title();
	void Stage();
	void GameEnd();
	void MapRender();


	void Clear();

	void  ScoreCheck(float *Time);


	GameManager();
	~GameManager();
};

