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
	void Update(vector<Tile*> v);
	void Changedirection();
	int GetPoint() { return m_iPoint; }
	Enemy();
	virtual ~Enemy();
};

