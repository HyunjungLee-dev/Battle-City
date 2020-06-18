#include "GameManager.h"



GameManager::GameManager()
{
}

void GameManager::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	BitMapManager::GetSingleton()->Init(m_hWnd);
	m_backbufferDC = BitMapManager::GetSingleton()->GetBufferDC();
	m_Map = new Maptool;
	m_Map->Init(hWnd);
}

void GameManager::Render()
{
	HDC hdc = GetDC(m_hWnd);
	m_Map->Render(hdc);
//	BitMapManager::GetSingleton()->GetBackBuffer().Draw(hdc);
	ReleaseDC(m_hWnd, hdc);
}


GameManager::~GameManager()
{
	delete m_Map;
}
