#pragma once
#include"BitMap.h"
#include"Singleton.h"


class BitMapManager : public Singleton< BitMapManager>
{
private:
	BitMap m_BackBuffer;
	vector<BitMap*> m_MapImglist;
	vector<BitMap*> m_TankImglist;
	vector<BitMap*> m_ObjectImglist;
	HDC BufferDC;

public:
	void Init(HWND hWnd);
	void MapImgLoad();
	void TankImgLoad();
	void ObjectImgLoad();
	void Clear();

	HDC GetBufferDC() { return BufferDC; }
	BitMap GetBackBuffer() { return m_BackBuffer; }
	BitMap* GetImg(MAP index) { return m_MapImglist.at(index); }
	BitMap* GetImg(TANK index) { return m_TankImglist.at(index); }
	BitMap* GetImg(OBJECT index) { return m_ObjectImglist.at(index); }
	BitMapManager();
	~BitMapManager();
};