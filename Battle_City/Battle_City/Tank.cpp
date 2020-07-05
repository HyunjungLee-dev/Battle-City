#include "Tank.h"



Tank::Tank()
{
	m_bullet = new Bullet;
	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
}


void Tank::Render(HDC hdc)
{
	static int appeartype = OBJE_APPEAR0;
	static int Shieldtype = OBJE_SHIELD00;
	static float Time = 0.0f;
	static int RepeatNum = 0;

	if (m_eTState == TANKAPPEAR)
	{
		Time += m_fDeltaTime;
		
		BitMapManager::GetSingleton()->GetImg((OBJECT)appeartype)->Draw(hdc, STARTX + m_pos.m_iX + 5, STARTY + m_pos.m_iY + 4, 1.4, 1.4);
		if (Time > 0.1f)
		{
			appeartype++;
			if (appeartype > OBJE_APPEAR3)
			{
				appeartype = OBJE_APPEAR0;
				RepeatNum++;
			}
			if (RepeatNum == 4)
			{
				if (m_eTankType == TYPEPLAYER)
					m_eTState = TANKSHIELD;
				else
					m_eTState = TANKMOVE;
				RepeatNum = 0;
			}
			Time = 0.0f;

		}
	}
	else if (m_eTState == TANKSHIELD)
	{
		Time += m_fDeltaTime;

		BitMapManager::GetSingleton()->GetImg(m_eTankimg)->Draw(hdc, STARTX + m_pos.m_iX, STARTY + m_pos.m_iY, 1, 1);
		BitMapManager::GetSingleton()->GetImg((OBJECT)Shieldtype)->Draw(hdc, STARTX + m_pos.m_iX, STARTY + m_pos.m_iY, 1, 1);

		if (Time > 0.1f)
		{
			Shieldtype++;
			if (Shieldtype > OBJE_SHIELD01)
			{
				Shieldtype = OBJE_SHIELD00;
				RepeatNum++;
			}
			if (RepeatNum == 10)
			{
				m_eTState = TANKMOVE;
				RepeatNum = 0;
			}
			Time = 0.0f;

		}
	}
	else
	{
		if (m_eTState != TANKNONE || m_eTState != TANKEXSPLOSION)
		{
			m_bullet->Render();
			BitMapManager::GetSingleton()->GetImg(m_eTankimg)->Draw(hdc, STARTX + m_pos.m_iX, STARTY + m_pos.m_iY, 1, 1);
		}
	}

}

bool Tank::Collision(RECT rct)
{
	RECT tmp;

	if (IntersectRect(&tmp, &rct, &Rct))
	{
		int InterW = tmp.right - tmp.left;
		int InterH = tmp.bottom - tmp.top;

		if (InterW > InterH)
		{
			if (tmp.top == rct.top)
			{
				if (m_pos.m_iY - InterH > 0 && m_eTState != TANKAPPEAR)
				{
					m_pos.m_iY -= InterH;
					Rct.top -= InterH;
					Rct.bottom -= InterH;
				}
			}
			else if (tmp.bottom == rct.bottom)
			{
				if (m_pos.m_iY - InterH < TILESIZEY * TILESIZEY &&  m_eTState != TANKAPPEAR)
				{
					m_pos.m_iY += InterH;
					Rct.top += InterH;
					Rct.bottom += InterH;
				}
			}
		}
		else
		{
			if (tmp.left == rct.left)
			{
				if (m_pos.m_iY - InterW > 0 &&  m_eTState != TANKAPPEAR)
				{
					m_pos.m_iX -= InterW;
					Rct.left -= InterW;
					Rct.right -= InterW;
				}
			}
			else if (tmp.right == rct.right)
			{
				if (m_pos.m_iX - InterW < TILESIZEX * TILESIZEX && m_eTState != TANKAPPEAR)
				{
					m_pos.m_iX += InterW;
					Rct.left += InterW;
					Rct.right += InterW;
				}
			}
		}
		Rct = { (long)m_pos.m_iX + 3 ,(long)m_pos.m_iY + 3,(long)m_pos.m_iX + TILESIZEX - 3  ,(long)m_pos.m_iY + TILESIZEY - 3 };
		return true;
	}
	return false;
}

bool Tank::isTankfornt(RECT rct)
{
	if (Collision(rct))
	{
		return true;
	}
	return false;
}


bool Tank::isWallfornt(vector<Tile*> v, int num)
{
	int x[8];
	int y[8];
	RECT tmp;

	x[0] = x[3] = x[5] = m_pos.m_iX + 5;
	x[1] = x[6] = x[0] + TILESIZEX * 0.5;
	x[2] = x[4] = x[7] = x[0] + TILESIZEX * 0.7;

	y[0] = y[1] = y[2] = m_pos.m_iY + 5;
	y[3] = y[4] = y[0] + TILESIZEY * 0.5;
	y[5] = y[6] = y[7] = y[0] + TILESIZEY  * 0.7;
	
	int	index = (int)(y[num] / TILESIZEY) * TILEX + (int)(x[num] / TILESIZEX);
	if (index <0 || index >= TILEX * TILEY)
		return false;
	else if (v[index]->eTileID != (int)MAP_NONE)
	{
		if (Collision(v[index]->Rct))
			return true;
	}
		

	return false;
}

void Tank::Move(vector<Tile*> v)
{

	if (Movable(v, m_edirection))
	{
		switch (m_edirection)
		{
		case UP:
			m_pos.m_iY -= 80 * m_fDeltaTime;
			break;
		case DOWN:
			m_pos.m_iY += 80 * m_fDeltaTime;
			break;
		case LEFT:
			m_pos.m_iX -= 80 * m_fDeltaTime;
			break;
		case RIGHT:
			m_pos.m_iX += 80 * m_fDeltaTime;
			break;
		default:
			break;
		}
	}

	Rct = { (long)m_pos.m_iX + 3 ,(long)m_pos.m_iY  + 3,(long)m_pos.m_iX + TILESIZEX -3  ,(long)m_pos.m_iY + TILESIZEY  - 3};
}

bool Tank::Movable(vector<Tile*> v, DIRECTION direction)
{

		switch (direction)
		{
		case UP:
			if (m_pos.m_iY <= 0)
				return false;
			else if (isWallfornt(v, 1) || isWallfornt(v, 0) || isWallfornt(v, 2))
				return false;
			return true;
			break;
		case DOWN:
			if (m_pos.m_iY >= TILESIZEY * 12)
				return false;
			else if (isWallfornt(v, 6)|| isWallfornt(v, 5) || isWallfornt(v, 7))
				return false;
			else
				return true;
			break;
		case LEFT:
			if (m_pos.m_iX < 0)
				return false;
			else if (isWallfornt(v, 3) || isWallfornt(v, 0) || isWallfornt(v, 5))
				return false;
			else
				return true;
			break;
		case RIGHT:
			if (m_pos.m_iX > TILESIZEX * 12)
				return false;
			else if (isWallfornt(v, 4) || isWallfornt(v, 2) || isWallfornt(v, 7))
				return false;
			else
				return true;
			break;
		default:
			break;
		}

	
	
}



Tank::~Tank()
{
	delete m_bullet;
}
