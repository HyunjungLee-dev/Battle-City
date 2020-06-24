#include "Tank.h"



Tank::Tank()
{

}


void Tank::Render(HDC hdc, int StartX, int StartY)
{
		BitMapManager::GetSingleton()->GetImg(m_eTankimg)->Draw(hdc, StartX+m_iX , StartY+ m_iY , 1, 1);
}

bool Tank::Movable(vector<Tile*> v, DIRECTION direction)// 반개일경우 예외
{
	int y = (int)m_iY + TILESIZEY ;
	int x = (int)m_iX ;

	int index = (y / TILESIZEY) * TILEX + (x / TILESIZEX);

	switch (direction)
	{
	case UP:
		if (m_iY < 0)
			return false;
		else if (v[index- TILEX]->eTileType == (int)MAP_NONE)
			return true;
		else
			return false;
		break;
	case DOWN:
		if (m_iY > TILESIZEY * 12)
			return false;
		else
			return true;
		break;
	case LEFT:
		if (m_iX < 0)
			return false;
		else
			return true;
		break;
	case RIGHT:
		if (m_iX > TILESIZEX * 12)
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
