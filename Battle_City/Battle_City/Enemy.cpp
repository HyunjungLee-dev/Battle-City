#include "Enemy.h"



Enemy::Enemy()
{
}

void Enemy::Init()
{
	m_iTankNum = 20;
}

void Enemy::EnemyIconRender(HDC hdc,int Startx ,int StartY)
{
	
	BitMapManager::GetSingleton()->GetImg(OBJE_ENEMY)->Draw(hdc, Startx , StartY, 1, 1);
}

Enemy::~Enemy()
{
}
