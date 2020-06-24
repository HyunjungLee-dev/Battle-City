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
	int X = Startx;
	int Y = StartY;
	SIZE Size = BitMapManager::GetSingleton()->GetImg(OBJE_ENEMY)->GetSize();

	for (int i = 0; i < m_iTankNum*0.5; i++)
	{
		BitMapManager::GetSingleton()->GetImg(OBJE_ENEMY)->Draw(hdc, X, Y, 1, 1);
		BitMapManager::GetSingleton()->GetImg(OBJE_ENEMY)->Draw(hdc, X + Size.cx, Y, 1, 1);
		Y += Size.cy;
	}
}

Enemy::~Enemy()
{
}
