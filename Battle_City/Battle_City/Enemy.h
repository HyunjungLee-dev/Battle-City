#pragma once
#include"BitMapManager.h"
class Enemy
{
private:
	int m_iTankNum;
public:
	void Init();
	void EnemyIconRender(HDC hdc,int StartX, int StrtY);
	Enemy();
	~Enemy();
};

