#pragma once
#include"Font.h"
#include"Tank.h"
class Player : public Tank
{
private:
	int m_iLife;
public:
	virtual void Init();
	void Update(vector<Tile*> v);
	virtual void TankImg();
	void KeyCheck(vector<Tile*> v);
	void Respon();

	int GetLife() { return m_iLife; }
	void SetLife() { m_iLife--; }
	Player();
	virtual ~Player();
};

