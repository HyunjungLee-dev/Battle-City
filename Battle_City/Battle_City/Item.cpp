#include "Item.h"



Item::Item()
{
	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
}

void Item::SetItem()
{
	int randX = rand() % (TILEX -1);
	int randY = rand() % (TILEY-1);

	m_pos.m_iX = TILESIZEX * randX;
	m_pos.m_iY = TILESIZEY * randY;

	m_bItemFlag = false;
	m_fItemTime = 0.0f;
	m_iPoint = 500;
	SIZE ImgSize = BitMapManager::GetSingleton()->GetImg(m_eItemType)->GetSize();
	m_Rct = { long(m_pos.m_iX),long(m_pos.m_iY),long(m_pos.m_iX + ImgSize.cx), long(m_pos.m_iY + ImgSize.cy )};
	
}

void Item::Draw()
{


	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	TCHAR str[128];
	HDC backbuf = BitMapManager::GetSingleton()->GetBufferDC();
	SIZE imgsize = BitMapManager::GetSingleton()->GetImg(m_eItemType)->GetSize();

	if (m_bItemFlag == true)
	{
		m_fItemTime += m_fDeltaTime;
		if (m_fItemTime < 2.0f)
		{
			wsprintf(str, TEXT("%d"), m_iPoint);
			Font::GetSingleton()->Text(m_pos.m_iX + STARTX, m_pos.m_iY + STARTY, str, 0x00ffffff);
		}
		else
			m_fItemTime = 2.0f;

	}
	else
	{
		if (m_fDeltaTime > 0.2f)
		{
			BitMapManager::GetSingleton()->GetImg(m_eItemType)->Draw(backbuf, STARTX + m_pos.m_iX, STARTY + m_pos.m_iY, 0.8, 0.8);
		}
		if (m_fDeltaTime > 0.4f)
		{
			m_dwLastTime = m_dwCurTime;
		}
	}
}


bool Item::Collision(RECT rct)
{
	RECT tmp;

	if (IntersectRect(&tmp, &rct, &m_Rct))
	{
		m_bItemFlag = true;
		return true;
	}
	return false;
}

Item::~Item()
{
}



void Bomb::Init()
{

	m_eItemType = ITEM_BOMB;
	SetItem();
	
}





void Clock::Init()
{

	m_eItemType = ITEM_CLOCK;
	SetItem();

}


void Helmet::Init()
{

	m_eItemType = ITEM_HELMET;
	SetItem();

}



void Shovel::Init()
{

	m_eItemType = ITEM_SHOVEL;
	SetItem();

}




void ItemTank::Init()
{

	m_eItemType = ITEM_TANK;
	SetItem();

}


