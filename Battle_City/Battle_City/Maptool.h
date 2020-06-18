#pragma once
#include"Mecro.h"
#include"BitMapManager.h"

class Maptool
{
private:
	HWND m_hWnd;
	int g_map[13][13];
public:
	void Init(HWND hwnd);
	void Create(POINT pt);
	void Render(HDC hdc);
	void Save();
	void Load();
	Maptool();
	~Maptool();
};

