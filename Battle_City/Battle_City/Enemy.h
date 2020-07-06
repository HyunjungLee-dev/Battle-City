#pragma once
#include"BitMapManager.h"
#include "Tank.h"
class Enemy : public Tank
{
private:
	int m_iPoint;

public:
	virtual void TankImg();
	virtual void Init();

	void SetPos();
	void SetTankType(TYPE type) { m_eTankType = type; };
	void Update(vector<Tile*> v, float dtime);
	void Changedirection();
	int GetPoint() { return m_iPoint; }
	Enemy();
	virtual ~Enemy();
};

