#include "Player.h"



Player::Player()
{
}

void Player::Init()
{

	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();

	m_iScore = 0;
	m_iLife = 3;

	//Tank
	m_edirection = UP;
	m_eTankimg = T_PLAYER_UP_0;
	m_iX =  4 * TILESIZEX;
	m_iY = 12 * TILESIZEY;
	m_eTankType = PLAYER;
	m_eTState = TANKAPPEAR;
}

void Player::Update(vector<Tile*> v)
{
	KeyCheck(v);
}

void Player::Move(vector<Tile*> v)
{
	if (Movable(v, m_edirection))
	{
		switch (m_edirection)
		{
		case UP:
			m_iY -= 100 * m_fDeltaTime;
			break;
		case DOWN:
			m_iY += 100 * m_fDeltaTime;
			break;
		case LEFT:
			m_iX -= 100 * m_fDeltaTime;
			break;
		case RIGHT:
			m_iX += 100 * m_fDeltaTime;
			break;
		default:
			break;
		}
	}

}

void Player::KeyCheck(vector<Tile*> v)
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;

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
