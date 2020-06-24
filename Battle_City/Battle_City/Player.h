#pragma once
#include"Font.h"
#include"Tank.h"
class Player : public Tank
{
private:
	int m_iScore;
	int m_iLife;

	DWORD	m_dwLastTime;
	DWORD	m_dwCurTime;
	float	m_fDeltaTime;

public:
	virtual void Init();
	void Update(vector<Tile*> v);
	virtual void TankImg();
	virtual void KeyCheck(vector<Tile*> v);
	void Move(vector<Tile*> v);
	int GetScore() { return m_iScore; }
	int GetLife() { return m_iLife; }
	Player();
	virtual ~Player();
};

