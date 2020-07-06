#pragma once
#include"Bullet.h"
#include"Font.h"

class Tank 
{
protected:
	RECT Rct;
	DIRECTION m_edirection;
	POS m_pos;
	TYPE m_eTankType;
	TANK m_eTankimg;
	TANKSTATE m_eTState;
	int m_iPoint;

	Bullet* m_bullet;
public:
	virtual void Init() = 0;
	virtual void TankImg() = 0;
	void Move(vector<Tile*> v,float dtime);
	bool Movable(vector<Tile*> v, DIRECTION direction,float dtime);
	void Render(HDC hdc,float Time);

	bool forntObjectCheck(vector<Tile*> v, int num, float dtime);
	bool isTankfornt(RECT rct);
	bool Collision(RECT rct);


	void SetTankState(TANKSTATE state) { m_eTState = state; }
	

	TYPE GetType() { return m_eTankType; }
	TANKSTATE GetState() { return m_eTState; }
	Bullet* GetBullet() { return m_bullet; }
	RECT GetRect() { return Rct; }
	POS Getpos() { return m_pos; }
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

