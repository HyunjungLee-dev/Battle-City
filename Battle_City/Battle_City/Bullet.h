#pragma once
#include"Mecro.h"
#include"BitMapManager.h"
class Bullet
{
private:
	queue<POS> m_bullet;
	SIZE ImgSize;

	float CenterX, CenterY;

	DWORD	m_dwLastTime;
	DWORD	m_dwCurTime;
	float	m_fDeltaTime;
public:
	Bullet();
	~Bullet();

	void Create(POS pos, DIRECTION direction);
	void Shoot(DIRECTION direction, vector<Tile*> v);
	void Render(int StartX, int StartY);
	bool Collision(vector<Tile*> v);
	bool IsPointInRect(RECT rc);
	bool IsPointInCircle(POS point);
	void Update();
	void Clear();
};

