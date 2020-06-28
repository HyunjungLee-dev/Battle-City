#include "Tank.h"



Tank::Tank()
{
	m_bullet = new Bullet;
}


void Tank::Render(HDC hdc, int StartX, int StartY)
{
	static int appeartype = OBJE_APPEAR0;
	static int Shieldtype = OBJE_SHIELD00;
	static float Time = 0.0f;
	static int RepeatNum = 0;

	if (m_eTState == TANKAPPEAR)
	{
		Time += m_fDeltaTime;
		BitMapManager::GetSingleton()->GetImg((OBJECT)appeartype)->Draw(hdc, StartX + m_pos.m_iX, StartY + m_pos.m_iY - 6, 2, 2);
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
				m_eTState = TANKSHIELD;
				RepeatNum = 0;
			}
			Time = 0.0f;

		}
	}
	else if (m_eTState == TANKSHIELD)
	{
		Time += m_fDeltaTime;

		BitMapManager::GetSingleton()->GetImg(m_eTankimg)->Draw(hdc, StartX + m_pos.m_iX, StartY + m_pos.m_iY, 1, 1);
		BitMapManager::GetSingleton()->GetImg((OBJECT)Shieldtype)->Draw(hdc, StartX + m_pos.m_iX, StartY + m_pos.m_iY, 1, 1);

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
		BitMapManager::GetSingleton()->GetImg(m_eTankimg)->Draw(hdc, StartX + m_pos.m_iX, StartY + m_pos.m_iY, 1, 1);
		m_bullet->Render(StartX, StartY);
	}

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
	y[5] = y[6] = y[7] = y[0] + TILESIZEY * 0.7;
	
		int index = (int)(y[num] / TILESIZEY) * TILEX + (int)(x[num] / TILESIZEX);
		if (v[index]->eTileID != (int)MAP_NONE)
		{
			if(IntersectRect(&tmp, &v[index]->Rct, &Rct))
				return false;
		}
		

	return true;
}

bool Tank::Movable(vector<Tile*> v, DIRECTION direction)
{

		switch (direction)
		{
		case UP:
			if (m_pos.m_iY <= 0)
				return false;
			if (!isWallfornt(v, 1)|| !isWallfornt(v, 0) || !isWallfornt(v, 2))
				return false;
			return true;
			break;
		case DOWN:
			if (m_pos.m_iY >= TILESIZEY * 12)
				return false;
			if (!isWallfornt(v, 6) || !isWallfornt(v, 5) || !isWallfornt(v, 7))
				return false;
			else
				return true;
			break;
		case LEFT:
			if (m_pos.m_iX < 0)
				return false;
			if (!isWallfornt(v, 3) || !isWallfornt(v, 0) || !isWallfornt(v, 5))
				return false;
			else
				return true;
			break;
		case RIGHT:
			if (m_pos.m_iX > TILESIZEX * 12)
				return false;
			if (!isWallfornt(v, 4) || !isWallfornt(v, 2) || !isWallfornt(v, 7))
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
