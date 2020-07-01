#pragma once
#include"Mecro.h"
#include"BitMapManager.h"


class Maptool
{
private:
	HWND m_hWnd;
	vector<Tile*> m_Map;
public:
	void Init(HWND hwnd);
	void SetMap();
	void Create(POINT pt);
	void Render(HDC hdc);
	void Save();
	void Load(LPCWSTR Flie);
	void Clear();

	vector<Tile*> GetMap() { return m_Map; }
	//int(*arr(void))[13] { return g_map; }
	Maptool();
	~Maptool();
};

