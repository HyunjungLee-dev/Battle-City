#pragma once
#include"BitMapManager.h"
#include "Tank.h"
class Enemy : public Tank
{
private:
	bool m_bItemFlag;
public:
	virtual void TankImg();
	virtual void Init();

	bool GetItemFlage() { return m_bItemFlag; }
	void SetItemFlag(bool b) { m_bItemFlag = b; }
	void SetPos();
	void SetTankType(TYPE type) { m_eTankType = type; };
	void Update(vector<Tile*> v, float dtime);
	void Changedirection();
	int GetPoint() { return m_iPoint; }
	Enemy();
	virtual ~Enemy();
};

