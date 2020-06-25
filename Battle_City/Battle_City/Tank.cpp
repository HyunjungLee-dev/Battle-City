#include "Tank.h"



Tank::Tank()
{

}


void Tank::Render(HDC hdc, int StartX, int StartY)
{

	BitMapManager::GetSingleton()->GetImg(m_eTankimg)->Draw(hdc, StartX+m_pos.m_iX , StartY+ m_pos.m_iY , 1, 1);
}

bool Tank::Movable(vector<Tile*> v, DIRECTION direction)
{
	//index의 변화를 주는 곳을 
	index = ((m_pos.m_iY + TILESIZEY) / TILESIZEY) * TILEX + ((m_pos.m_iX + TILESIZEX * 0.5) / TILESIZEX);

	switch (direction)
	{
	case UP:
		if (m_pos.m_iY <= 0)
			return false;
		else if (v[index- TILEX]->eTileType != (int)MAP_NONE)
			return false;
		else
			return true;
		break;
	case DOWN:
		if (m_pos.m_iY >= TILESIZEY * 12)
			return false;
		else if (v[index + TILEX]->eTileType != (int)MAP_NONE)
			return false;
		else
			return true;
		break;
	case LEFT:
		if (m_pos.m_iX < 0)
			return false;
		else
			return true;
		break;
	case RIGHT:
		if (m_pos.m_iX > TILESIZEX * 12)
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
