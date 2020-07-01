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
	int index;

public:
	virtual void Init() = 0;
	virtual void TankImg() = 0;
	void Move(vector<Tile*> v);
	bool Movable(vector<Tile*> v, DIRECTION direction);
	void Render(HDC hdc);

	bool isWallfornt(vector<Tile*> v, int num);
	void SetNotIntersect(RECT Rect, const RECT Hold);

	POS Getpos() { return m_pos; }
	int Getindex() { return index; }
	RECT GetRect() { return Rct; }
	Tank();
	virtual ~Tank();
};

