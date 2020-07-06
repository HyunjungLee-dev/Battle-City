#pragma once
#include"Font.h"
#include"Tank.h"
class Player : public Tank
{
private:
	int m_iLife;
	bool m_bKeypush;


public:
	virtual void Init();
	void Update(vector<Tile*> v,float dtime);
	virtual void TankImg();
	void KeyCheck(vector<Tile*> v, float dtime);
	void Respon();

	int GetLife() { return m_iLife; }
	void SetLife(int life) { m_iLife = life; }
	Player();
	virtual ~Player();
};

