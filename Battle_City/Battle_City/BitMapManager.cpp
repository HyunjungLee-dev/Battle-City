#include "BitmapManager.h"

BitMapManager* BitMapManager::_Singleton = NULL;

BitMapManager::BitMapManager()
{
}

void BitMapManager::Init(HWND hWnd)
{

	HDC hdc = GetDC(hWnd);
	m_BackBuffer.Init(hdc);
	BufferDC = m_BackBuffer.GetMemDC();
	MapImgLoad();
	TankImgLoad();
	ObjectImgLoad();
	ReleaseDC(hWnd, hdc);
}

void BitMapManager::MapImgLoad()
{
	TCHAR str[128];

	for (int i = MAP_BLOCK; i < MAP_END; i++)
	{
		m_MapImglist.push_back(new BitMap);
		if(i<10)
			wsprintf(str, TEXT("Res//block0%d.bmp"),i);
		else
			wsprintf(str, TEXT("Res//block%d.bmp"), i);
		m_MapImglist.at(i)->ImgLoad(BufferDC,str);
	}

}

void BitMapManager::TankImgLoad()
{
	TCHAR str[128];

	for (int i = TANK_START; i < TANK_END; i++)
	{
		m_TankImglist.push_back(new BitMap);
		if(i<= T_ENUMY_RED_UP)
			wsprintf(str, TEXT("Res//e_up_0%d.bmp"), i);
		else if(i>= T_ENUMY_LT_0 && i <= T_ENUMY_RED_LT)
			wsprintf(str, TEXT("Res//e_right_0%d.bmp"), i-3);
		else if (i >= T_ENUMY_DN_0 && i <= T_ENUMY_RED_DN)
			wsprintf(str, TEXT("Res//e_down_0%d.bmp"), i - 6);
		else if (i >= T_ENUMY_RT_0 && i <= T_ENUMY_RED_RT)
			wsprintf(str, TEXT("Res//e_left_0%d.bmp"), i - 9);
		else if (i >= T_PLAYER_UP_0 && i <= T_PLAYER_UP_1)
			wsprintf(str, TEXT("Res//tank_up_0%d.bmp"), i - 12);
		else if (i >= T_PLAYER_LT_0 && i <= T_PLAYER_LT_1)
			wsprintf(str, TEXT("Res//tank_right_0%d.bmp"), i - 14);
		else if (i >= T_PLAYER_DN_0 && i <= T_PLAYER_DN_1)
			wsprintf(str, TEXT("Res//tank_down_0%d.bmp"), i - 16);
		else
			wsprintf(str, TEXT("Res//tank_left_0%d.bmp"), i - 18);
		m_TankImglist.at(i)->ImgLoad(BufferDC, str);
	}
}

void BitMapManager::ObjectImgLoad()
{
	CHAR str[128];

	for (int i = OBJE_START; i < OBJE_END; i++)
	{
		m_ObjectImglist.push_back(new BitMap);
	}
	m_ObjectImglist.at(OBJE_ENEMY)->ImgLoad(BufferDC, L"Res//enemy_icon.bmp");
	m_ObjectImglist.at(OBJE_EXPLOSION00)->ImgLoad(BufferDC, L"Res//explosion_00.bmp");
	m_ObjectImglist.at(OBJE_EXPLOSION01)->ImgLoad(BufferDC, L"Res//explosion_01.bmp");
	m_ObjectImglist.at(OBJE_EXPLOSION02)->ImgLoad(BufferDC, L"Res//explosion_02.bmp");
	m_ObjectImglist.at(OBJE_EXPLOSION03)->ImgLoad(BufferDC, L"Res//explosion_03.bmp");
	m_ObjectImglist.at(OBJE_EXPLOSION04)->ImgLoad(BufferDC, L"Res//explosion_04.bmp");
	m_ObjectImglist.at(OBJE_MISSILE)->ImgLoad(BufferDC, L"Res//missile_00.bmp");
	m_ObjectImglist.at(OBJE_PLAYER)->ImgLoad(BufferDC, L"Res//player_icon.bmp");
	m_ObjectImglist.at(OBJE_SHIELD00)->ImgLoad(BufferDC, L"Res//shield_00.bmp");
	m_ObjectImglist.at(OBJE_SHIELD01)->ImgLoad(BufferDC, L"Res//shield_01.bmp");
	m_ObjectImglist.at(OBJE_STAGE)->ImgLoad(BufferDC, L"Res//stage_icon.bmp");
}

void BitMapManager::Clear()
{
	for (vector<BitMap*>::iterator it = m_MapImglist.begin(); it != m_MapImglist.end(); it++)
	{
		(*it)->Release();
		delete (*it);

	}
	m_MapImglist.clear();

	for (vector<BitMap*>::iterator it = m_ObjectImglist.begin(); it != m_ObjectImglist.end(); it++)
	{
		(*it)->Release();
		delete (*it);

	}
	m_ObjectImglist.clear();

	for (vector<BitMap*>::iterator it = m_TankImglist.begin(); it != m_TankImglist.end(); it++)
	{
		(*it)->Release();
		delete (*it);

	}
	m_TankImglist.clear();
}

BitMapManager::~BitMapManager()
{
}
