#include "Maptool.h"



Maptool::Maptool()
{
}

void Maptool::Init(HWND hwnd)
{
	m_hWnd = hwnd;
	SetMap();
}

void Maptool::SetMap()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			m_Map.push_back(new Tile);
			m_Map.back()->eTileType = MAP_NONE;
			m_Map.back()->fX = j * TILESIZEX;
			m_Map.back()->fY = i * TILESIZEY;
		}
	}
}

void Maptool::Create(POINT pt)
{
	int index = (pt.y / TILESIZEY) * TILEX + (pt.x / TILESIZEX);

	if ((pt.x < TILESIZEX * TILEX && pt.x > 0) && (pt.y < TILESIZEY * TILEY && pt.y > 0))
	{
		if (m_Map[index]->eTileType >= MAP_ENDFALGE)
			m_Map[index]->eTileType = MAP_NONE;
		else
		{

			m_Map[index]->eTileType++;
		}
		InvalidateRect(m_hWnd, NULL, true);
	}
}

void Maptool::Render(HDC hdc, int startX, int StartY)
{
	RECT ret = { startX ,StartY, TILESIZEX * TILEX , TILESIZEY * TILEY };
	PatBlt(hdc, ret.left, ret.top, ret.right, ret.bottom, BLACKNESS);

	for (int i = 0; i < m_Map.size(); i++)
	{
		if (m_Map[i]->eTileType == MAP_NONE)
		{
			Rectangle(hdc, startX + m_Map[i]->fX, StartY + m_Map[i]->fY, startX + m_Map[i]->fX + TILESIZEX, StartY + m_Map[i]->fY + TILESIZEY);

		}
		else
			BitMapManager::GetSingleton()->GetImg((MAP)m_Map[i]->eTileType)->Draw(hdc, startX + m_Map[i]->fX, StartY + m_Map[i]->fY, 1, 1);
	}
}

void Maptool::Save()
{
	OPENFILENAME OFN;
	char str[300];
	char lpstrFile[MAX_PATH] = "";
	char lpstrPath[MAX_PATH] = "";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = m_hWnd;
	OFN.lpstrFilter = L"Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
	OFN.lpstrFile = (LPWSTR)lpstrFile;
	OFN.nMaxFile = 256;
	GetCurrentDirectory(MAX_PATH, (LPWSTR)lpstrPath);
	OFN.lpstrInitialDir = (LPWSTR)lpstrPath;
	if (GetSaveFileName(&OFN) == 0)
	{
		DWORD err = CommDlgExtendedError();
		return;
	}

	HANDLE hFile = CreateFile(OFN.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	for (int i = 0; i < m_Map.size(); i++)
	{

		DWORD writeB;
		WriteFile(hFile, &m_Map[i]->fX, sizeof(float), &writeB, NULL);
		WriteFile(hFile, &m_Map[i]->fY, sizeof(float), &writeB, NULL);
		WriteFile(hFile, &m_Map[i]->eTileType, sizeof(int), &writeB, NULL);

	}
	CloseHandle(hFile);
	InvalidateRect(m_hWnd, NULL, false);
}

void Maptool::Load(LPCWSTR Flie)
{
	Clear();
	SetMap();
	HANDLE hFile = CreateFile(Flie, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	for (int i = 0; i < m_Map.size(); i++)
	{
		DWORD readB;
		ReadFile(hFile, &m_Map[i]->fX, sizeof(float), &readB, NULL);
		ReadFile(hFile, &m_Map[i]->fY, sizeof(float), &readB, NULL);
		ReadFile(hFile, &m_Map[i]->eTileType, sizeof(int), &readB, NULL);
	}
	CloseHandle(hFile);
	InvalidateRect(m_hWnd, NULL, false);
}

void Maptool::Clear()
{
	for (vector<Tile*>::iterator it = m_Map.begin(); it != m_Map.end(); it++)

	{
		delete (*it);
	}
	m_Map.clear();
}

Maptool::~Maptool()
{

}
