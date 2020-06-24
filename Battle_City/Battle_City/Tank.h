#pragma once
#include"BitMapManager.h"
class Tank
{

protected:
	RECT Rct;
	DIRECTION m_edirection;
	float m_iX, m_iY;
	TYPE m_eTankType;
	TANK m_eTankimg;
	TANKSTATE m_eTState;
public:
	virtual void Init() = 0;
	virtual void TankImg() = 0;
	virtual void KeyCheck(vector<Tile*> v) = 0;
	bool Movable(vector<Tile*> v, DIRECTION direction);
	void Render(HDC hdc, int StartX, int StartY);
	Tank();
	virtual ~Tank();
};

