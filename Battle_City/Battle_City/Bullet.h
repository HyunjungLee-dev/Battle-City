#pragma once
#include"Mecro.h"
#include"BitMapManager.h"
class Bullet
{
private:
	queue<POS> m_bullet;
	SIZE ImgSize;

	DWORD	m_dwLastTime;
	DWORD	m_dwCurTime;
	float	m_fDeltaTime;
public:
	Bullet();
	~Bullet();

	void Create(POS pos);
	void Shoot();
	void Render(int StartX, int StartY);
	bool Collision(); 
	bool IsPointInRect(RECT rc);
	bool IsPointInCircle(POS point);
	void Update();
	void Clear();
};

