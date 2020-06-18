#pragma once
#include"Maptool.h"
class GameManager
{
private:
	HWND m_hWnd;
	HDC m_backbufferDC;
	Maptool* m_Map;
public:
	void Init(HWND hWnd);
	void Render();


	Maptool* GetMapTool() {return m_Map;}
	GameManager();
	~GameManager();
};

