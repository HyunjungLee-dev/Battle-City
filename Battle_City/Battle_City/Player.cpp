#include "Player.h"



Player::Player()
{
}

void Player::Init()
{

	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	m_iScore = 0;
	m_iLife = 3;

	//Tank
	m_edirection = UP;
	m_eTankimg = T_PLAYER_UP_0;
	m_pos.m_iX =  4 * TILESIZEX;
	m_pos.m_iY = 12 * TILESIZEY;
	m_eTankType = PLAYER;
	m_eTState = TANKAPPEAR;
	Rct = { (long)m_pos.m_iX + 3 ,(long)m_pos.m_iY +3 ,(long)m_pos.m_iX + TILESIZEX - 3 ,(long)m_pos.m_iY+ TILESIZEY  -3};
}

void Player::Update(vector<Tile*> v)
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;

	if (m_eTState != TANKAPPEAR)
	{
		m_bullet->Shoot();
		KeyCheck(v);
	}
}

void Player::Move(vector<Tile*> v)
{
	if (Movable(v, m_edirection))
	{
		switch (m_edirection)
		{
		case UP:
			m_pos.m_iY -= 100 * m_fDeltaTime;
			break;
		case DOWN:
			m_pos.m_iY += 100 * m_fDeltaTime;
			break;
		case LEFT:
			m_pos.m_iX -= 100 * m_fDeltaTime;
			break;
		case RIGHT:
			m_pos.m_iX += 100 * m_fDeltaTime;
			break;
		default:
			break;
		}
	}

	Rct = { (long)m_pos.m_iX + 3 ,(long)m_pos.m_iY + 3 ,(long)m_pos.m_iX + TILESIZEX - 3 ,(long)m_pos.m_iY + TILESIZEY - 3 };
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

	if (GetAsyncKeyState(VK_SPACE) & 0x0001) // 장애물에 부딪히지 않으며 생성 안함, 큐로 해도 괜찮을듯,,,..한데
	{
		m_bullet->Create(m_pos);
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
