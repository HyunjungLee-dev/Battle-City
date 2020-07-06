#pragma once
#include"Maptool.h"
#include"Font.h"
#include"Player.h"
#include"Enemy.h"
#include"Item.h"
class GameManager
{
private:


	HWND m_hWnd;
	HDC m_backbufferDC;
	RECT m_ClientRect;

	GAMESTATE m_eState;
	bool m_bGameOver;
	ITEM m_eNowItem;

	DWORD	m_dwLastTime;
	DWORD	m_dwCurTime;
	float	m_fDeltaTime;

	Player* m_Player;
	list<Enemy*> m_Enemylist;
	list<Item*> m_Itemlist;

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


	//Enemy
	void EnemyIconRender(HDC hdc);
	void EnemyCreate();
	void EnemyRender(HDC hdc);
	void EnemyUpdate();


	void TankCollision();
	void TankbulletCollision();

	bool CreatePosCheck(POS pos);
	void ExsplosionAfter();
	
	//Item
	void CreateItem();
	void ItemlistDraw();
	void ItemUpdate();	
	void EnemyListBomb();


	void Title();
	void Stage();
	void GameEnd();
	void MapRender();
	void ScoreCheck(float *Time);

	void Clear();


	void SetStage(int stage);


	GameManager();
	~GameManager();
};

