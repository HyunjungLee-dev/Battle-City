#pragma once
#include"Mecro.h"
#include"BitMapManager.h"
#include"Maptool.h"
class Bullet
{
private:
	queue<POS> m_bullet;
	SIZE ImgSize;
	DIRECTION m_direction;
	TYPE m_eCollType;
	bool m_bCollision;


	int type;

	float CenterX, CenterY;

	DWORD	m_dwLastTime;
	DWORD	m_dwCurTime;
	float	m_fDeltaTime;
public:
	Bullet();
	~Bullet();

	void Init(POS pos, DIRECTION direction);
	void Shoot(vector<Tile*> v);
	void ExsplosionRender(HDC hdc);
	void Render();
	void Collision(vector<Tile*> v);
	bool IsPointInRect(RECT rc,int x, int y);
	bool IsPointInCircle(float cx, float cy);
	void Update(vector<Tile*> v);
	void Clear();


	bool CheckEmpty() { return m_bullet.empty(); }
};

