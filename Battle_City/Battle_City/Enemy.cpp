#include "Enemy.h"



Enemy::Enemy()
{
}

void Enemy::Update(vector<Tile*> v)
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;

	if (m_eTState != TANKAPPEAR && m_eTState != TANKEXSPLOSION)
	{
		m_bullet->Init(m_pos, m_edirection);
		m_bullet->Update(v);
		TankImg();
		Move(v);
		if (!Movable(v, m_edirection))
		{
			int random = rand() % 4;
			switch ((DIRECTION)random)
			{
			case UP:
				m_edirection = LEFT;
				m_eTankimg = T_ENUMY_LT_0;
				break;
			case DOWN:
				m_edirection = RIGHT;
				m_eTankimg = T_ENUMY_RT_0;
				break;
			case LEFT:
				m_edirection = DOWN;
				m_eTankimg = T_ENUMY_DN_0;
				break;
			case RIGHT:
				m_edirection = UP;
				m_eTankimg = T_ENUMY_UP_0;
				break;
			default:
				break;
			}
		}
	}
}

void Enemy::Changedirection()
{

		switch (m_edirection)
		{
		case UP:
			m_edirection = LEFT;
			m_eTankimg = T_ENUMY_LT_0;
			break;
		case DOWN:
			m_edirection = RIGHT;
			m_eTankimg = T_ENUMY_RT_0;
			break;
		case LEFT:
			m_edirection = DOWN;
			m_eTankimg = T_ENUMY_DN_0;
			break;
		case RIGHT:
			m_edirection = UP;
			m_eTankimg = T_ENUMY_UP_0;
			break;
		default:
			break;
		}
}

void Enemy::TankImg()
{
	switch (m_edirection)
	{
	case UP:
		if (m_eTankimg == T_ENUMY_UP_0)
			m_eTankimg = T_ENUMY_UP_1;
		else
			m_eTankimg = T_ENUMY_UP_0;
		break;
	case DOWN:
		if (m_eTankimg == T_ENUMY_DN_0)
			m_eTankimg = T_ENUMY_DN_1;
		else
			m_eTankimg = T_ENUMY_DN_0;
		break;
	case LEFT:
		if (m_eTankimg == T_ENUMY_LT_0)
			m_eTankimg = T_ENUMY_LT_1;
		else
			m_eTankimg = T_ENUMY_LT_0;
		break;
	case RIGHT:
		if (m_eTankimg == T_ENUMY_RT_0)
			m_eTankimg = T_ENUMY_RT_1;
		else
			m_eTankimg = T_ENUMY_RT_0;
		break;
	default:
		break;
	}
}



void Enemy::Init()
{
	int random = rand() % 12;

	m_edirection = LEFT;
	m_eTankimg = T_ENUMY_LT_0;
	m_pos.m_iX = random * TILESIZEX;
	m_pos.m_iY =  0;
	m_eTankType = TYPEENEMY;
	m_eTState = TANKAPPEAR;
	m_iPoint = 100;

	Rct = { (long)m_pos.m_iX + 3 ,(long)m_pos.m_iY + 3,(long)m_pos.m_iX + TILESIZEX - 3  ,(long)m_pos.m_iY + TILESIZEY - 3 };

}

Enemy::~Enemy()
{
}
