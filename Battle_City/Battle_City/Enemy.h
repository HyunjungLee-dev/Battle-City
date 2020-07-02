#pragma once
#include"BitMapManager.h"
#include "Tank.h"
class Enemy : public Tank
{
private:
public:
	virtual void Init();
	virtual void TankImg();
	void Create();
	void Update(vector<Tile*> v);
	void Changedirection();
	Enemy();
	virtual ~Enemy();
};

