#include "Player.h"



Player::Player()
{

}

void Player::Init()
{
	m_iLife = 2;
	Respon();
}

void Player::Respon()
{
	m_edirection = UP;
	m_eTankimg = T_PLAYER_UP_0;
	m_pos.m_iX = 4 * TILESIZEX;
	m_pos.m_iY = 12 * TILESIZEY;
	m_eTankType = TYPEPLAYER;
	m_eTState = TANKAPPEAR;
	m_bullet = new Bullet;

	Rct = { (long)m_pos.m_iX + 3 ,(long)m_pos.m_iY + 3,(long)m_pos.m_iX + TILESIZEX - 3  ,(long)m_pos.m_iY + TILESIZEY - 3 };
}

void Player::Update(vector<Tile*> v, float dtime)
{

	if (m_eTState != TANKAPPEAR && m_eTState != TANKNONE && m_eTState != TANKEXSPLOSION)
	{
		m_bullet->Update(v);
		KeyCheck(v, dtime);
	}
}


void Player::KeyCheck(vector<Tile*> v, float dtime)
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_edirection = LEFT;
		TankImg();
		m_bKeypush = true;
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_edirection = UP;
		TankImg();
		m_bKeypush = true;
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_edirection = RIGHT;
		TankImg();
		m_bKeypush = true;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_edirection = DOWN;
		TankImg();
		m_bKeypush = true;
	}
	else
		m_bKeypush = false;

	if(m_bKeypush)
		Move(v, dtime);

		if (GetAsyncKeyState(0x5A) & 0x0001)
		{
			m_bullet->Init(m_pos, m_edirection);
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
