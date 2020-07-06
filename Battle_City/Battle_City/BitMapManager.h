#pragma once
#include"Bitmap.h"


class BitMapManager : public Singleton< BitMapManager>
{
private:
	Bitmap m_BackBuffer;
	vector<Bitmap*> m_MapImglist;
	vector<Bitmap*> m_TankImglist;
	vector<Bitmap*> m_ObjectImglist;
	vector<Bitmap*> m_ItemImglist;
	HDC BufferDC;
public:
	void Init(HWND hWnd);
	void MapImgLoad();
	void TankImgLoad();
	void ObjectImgLoad();
	void ItemImgLoad();
	void Clear();

	HDC GetBufferDC() { return BufferDC; }
	Bitmap GetBackBuffer() { return m_BackBuffer; }
	Bitmap* GetImg(MAP index) { return m_MapImglist.at(index); }
	Bitmap* GetImg(TANK index) { return m_TankImglist.at(index); }
	Bitmap* GetImg(OBJECT index) { return m_ObjectImglist.at(index); }
	Bitmap* GetImg(ITEM index) { return m_ItemImglist.at(index); }
	BitMapManager();
	~BitMapManager();
};