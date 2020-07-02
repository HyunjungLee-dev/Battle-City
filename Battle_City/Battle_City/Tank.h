#pragma once
#include"Bullet.h"


class Tank 
{
protected:
	RECT Rct;
	DIRECTION m_edirection;
	POS m_pos;
	TYPE m_eTankType;
	TANK m_eTankimg;
	TANKSTATE m_eTState;

	Bullet* m_bullet;

	DWORD	m_dwLastTime;
	DWORD	m_dwCurTime;
	float	m_fDeltaTime;
public:
	virtual void Init() = 0;
	virtual void TankImg() = 0;
	void Move(vector<Tile*> v);
	bool Movable(vector<Tile*> v, DIRECTION direction);
	void Render(HDC hdc);

	bool isWallfornt(vector<Tile*> v, int num);
	bool isTankfornt(RECT rct);
	bool Collision(RECT rct);

	POS Getpos() { return m_pos; }
	Bullet* GetBullet() { return m_bullet; }
//	int Getindex() { return index; }
	RECT GetRect() { return Rct; }
	POS GetCenterPos()
	{
		POS pos;
		pos.m_iX = m_pos.m_iX + TILESIZEX * 0.5;
		pos.m_iY = m_pos.m_iY + TILESIZEY * 0.5;
		return pos;
	}
	Tank();
	virtual ~Tank();
};

