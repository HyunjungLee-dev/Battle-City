#include "Enemy.h"



Enemy::Enemy()
{
}

void Enemy::Update(vector<Tile*> v,float dtime)
{

	if (m_eTState != TANKAPPEAR && m_eTState != TANKEXSPLOSION)
	{
		m_bullet->Init(m_pos, m_edirection);
		m_bullet->Update(v);
		TankImg();
		Move(v, dtime);
		if (!Movable(v, m_edirection, dtime))
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
		{
			if (m_eTankType == TYPEITEM_ENEMY)
			{
				m_eTankimg = T_ENUMY_RED_UP;
			}
			else
				m_eTankimg = T_ENUMY_UP_1;
		}
		else
			m_eTankimg = T_ENUMY_UP_0;

		break;
	case DOWN:
		if (m_eTankimg == T_ENUMY_DN_0)
		{
			if (m_eTankType == TYPEITEM_ENEMY)
			{
				m_eTankimg = T_ENUMY_RED_DN;
			}
			else
				m_eTankimg = T_ENUMY_DN_1;
		}
		else
			m_eTankimg = T_ENUMY_DN_0;
		break;
	case LEFT:
		if (m_eTankimg == T_ENUMY_LT_0)
		{
			if (m_eTankType == TYPEITEM_ENEMY)
			{
				m_eTankimg = T_ENUMY_RED_LT;
			}
			else
				m_eTankimg = T_ENUMY_LT_1;
		}
		else
			m_eTankimg = T_ENUMY_LT_0;
		break;
	case RIGHT:
		if (m_eTankimg == T_ENUMY_RT_0)
		{
			if (m_eTankType == TYPEITEM_ENEMY)
			{
				m_eTankimg = T_ENUMY_RED_RT;
			}
			else
				m_eTankimg = T_ENUMY_RT_1;
		}
		else
			m_eTankimg = T_ENUMY_RT_0;
		break;
	default:
		break;
	}
}



void Enemy::Init()
{
	m_edirection = LEFT;
	m_eTankimg = T_ENUMY_LT_0;
	SetPos();
	m_eTankType = TYPEENEMY;
	m_eTState = TANKAPPEAR;
	m_iPoint = 100;
}

void Enemy::SetPos()
{
	int random = rand() % 12;
	m_pos.m_iX = random * TILESIZEX;
	m_pos.m_iY = 0;

	Rct = { (long)m_pos.m_iX + 3 ,(long)m_pos.m_iY + 3,(long)m_pos.m_iX + TILESIZEX - 3  ,(long)m_pos.m_iY + TILESIZEY - 3 };
}

Enemy::~Enemy()
{
}
