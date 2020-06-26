#pragma once
#include"BitMapManager.h"

struct POS
{
	float m_iX;
	float m_iY;
};
class Tank
{
protected:
	RECT Rct;
	DIRECTION m_edirection;
	POS m_pos;
	TYPE m_eTankType;
	TANK m_eTankimg;
	TANKSTATE m_eTState;
	int index;
public:
	virtual void Init() = 0;
	virtual void TankImg() = 0;
	virtual void KeyCheck(vector<Tile*> v) = 0;
	bool Movable(vector<Tile*> v, DIRECTION direction);
	void Render(HDC hdc, int StartX, int StartY);

	bool isWallfornt(vector<Tile*> v, int num);

	POS Getpos() { return m_pos; }
	int Getindex() { return index; }
	Tank();
	virtual ~Tank();
};

