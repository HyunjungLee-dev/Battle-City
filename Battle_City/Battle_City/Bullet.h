#pragma once
#include"Mecro.h"
#include"BitMapManager.h"
class Bullet
{
private:
	queue<POS> m_bullet;
	SIZE ImgSize;
	DIRECTION m_direction;
	bool m_bShoot, m_bCollision;
	int type;

	float CenterX, CenterY;

	DWORD	m_dwLastTime;
	DWORD	m_dwCurTime;
	float	m_fDeltaTime;
public:
	Bullet();
	~Bullet();

	void Create(POS pos, DIRECTION direction);
	void Shoot(vector<Tile*> v);
	void ExsplosionRender(HDC hdc);
	void Render();
	void Collision(vector<Tile*> v);
	bool IsPointInRect(RECT rc);
	bool IsPointInCircle(POS point);

	bool GetShoot() { return m_bShoot; }
	void Update(DIRECTION direction, vector<Tile*> v);
	void Clear();
};

