#include "Player.h"



Player::Player()
{
}

void Player::Init()
{
	//m_dwLastTime = GetTickCount();
	//m_dwCurTime = GetTickCount();
	//m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	m_iScore = 0;
	m_iLife = 3;

	//Tank
	Respon();

	Rct = { (long)m_pos.m_iX + 3 ,(long)m_pos.m_iY + 3,(long)m_pos.m_iX + TILESIZEX - 3  ,(long)m_pos.m_iY + TILESIZEY - 3 };
}

void Player::Respon()
{
	m_edirection = UP;
	m_eTankimg = T_PLAYER_UP_0;
	m_pos.m_iX = 4 * TILESIZEX;
	m_pos.m_iY = 12 * TILESIZEY;
	m_eTankType = TYPEPLAYER;
	m_eTState = TANKAPPEAR;
}

void Player::Update(vector<Tile*> v)
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;

	if (m_eTState != TANKAPPEAR)
	{
		m_bullet->Update(v);
		KeyCheck(v);
	}
}


void Player::KeyCheck(vector<Tile*> v)
{
		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			m_edirection = LEFT;
			TankImg();
			Move(v);
		}
		else if (GetKeyState(VK_UP) & 0x8000)
		{
			m_edirection = UP;
			TankImg();
			Move(v);
		}
		else if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			m_edirection = RIGHT;
			TankImg();
			Move(v);
		}
		else if (GetKeyState(VK_DOWN) & 0x8000)
		{
			m_edirection = DOWN;
			TankImg();
			Move(v);
		}

		if (GetAsyncKeyState(VK_SPACE) & 0x0001)
		{
			m_bullet->Create(m_pos, m_edirection);
		}
}

void Player::TankImg()
{

		switch (m_edirection)
		{
		case UP:
			if (m_eTankimg == T_PLAYER_UP_0)
				m_eTankimg = T_PLAYER_UP_1;
			else
				m_eTankimg = T_PLAYER_UP_0;
			break;
		case DOWN:
			if (m_eTankimg == T_PLAYER_DN_0)
				m_eTankimg = T_PLAYER_DN_1;
			else
				m_eTankimg = T_PLAYER_DN_0;
			break;
		case LEFT:
			if (m_eTankimg == T_PLAYER_LT_0)
				m_eTankimg = T_PLAYER_LT_1;
			else
				m_eTankimg = T_PLAYER_LT_0;
			break;
		case RIGHT:
			if (m_eTankimg == T_PLAYER_RT_0)
				m_eTankimg = T_PLAYER_RT_1;
			else
				m_eTankimg = T_PLAYER_RT_0;
			break;
		default:
			break;
		}
	
}



Player::~Player()
{
}
