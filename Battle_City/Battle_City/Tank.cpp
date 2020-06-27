#include "Tank.h"



Tank::Tank()
{
}


void Tank::Render(HDC hdc, int StartX, int StartY)
{

	BitMapManager::GetSingleton()->GetImg(m_eTankimg)->Draw(hdc, StartX+m_pos.m_iX , StartY+ m_pos.m_iY , 1, 1);

}


bool Tank::isWallfornt(vector<Tile*> v, int num)
{
	int x[8];
	int y[8];
	RECT tmp;

	x[0] = x[3] = x[5] = m_pos.m_iX +5;
	x[1] = x[6] = x[0] + TILESIZEX * 0.5 ;
	x[2] = x[4] = x[7] = x[0] + TILESIZEX * 0.7;

	y[0] = y[1] = y[2] = m_pos.m_iY + 5 ;
	y[3] = y[4] = y[0] + TILESIZEY * 0.5;
	y[5] = y[6] = y[7] = y[0] + TILESIZEY * 0.7;

	
		index = (int)(y[num] / TILESIZEY) * TILEX + (int)(x[num] / TILESIZEX);
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
}
