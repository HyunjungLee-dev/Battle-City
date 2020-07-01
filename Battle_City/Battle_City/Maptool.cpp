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
			m_Map.back()->eTileID = MAP_NONE;
			m_Map.back()->fX = j * TILESIZEX;
			m_Map.back()->fY = i * TILESIZEY;
			m_Map.back()->fSizeX = TILESIZEX;
			m_Map.back()->fSizeY = TILESIZEY;
			m_Map.back()->Rct = {
				long(m_Map.back()->fX),long(m_Map.back()->fY),long(m_Map.back()->fX + TILESIZEX),long(m_Map.back()->fY + TILESIZEY)
			};
		}
	}
}


void Maptool::Create(POINT pt)
{
	int index = (pt.y / TILESIZEY) * TILEX + (pt.x / TILESIZEX);

	if ((pt.x < TILESIZEX * 13 && pt.x > 0) && (pt.y < TILESIZEY * 13 && pt.y > 0))
	{
		if (m_Map[index]->eTileID >= MAP_ENDFALGE)
			m_Map[index]->eTileID = MAP_NONE;
		else
		{
			m_Map[index]->eTileID++;
			m_Map[index]->Rct = {long(m_Map[index]->fX),long(m_Map[index]->fY),long(m_Map[index]->fX + TILESIZEX),long(m_Map[index]->fY + TILESIZEY) };

			if (m_Map[index]->eTileID == MAP_BLOCKT || m_Map[index]->eTileID == MAP_GBLOCKT)
				m_Map[index]->Rct.bottom = long(m_Map[index]->fY + TILESIZEY * 0.5);
			else if (m_Map[index]->eTileID == MAP_BLOCKL || m_Map[index]->eTileID == MAP_GBLOCKL)
			{
				m_Map[index]->Rct.right = long(m_Map[index]->fX + TILESIZEX * 0.5);
			}
			else if (m_Map[index]->eTileID == MAP_BLOCKB || m_Map[index]->eTileID == MAP_GBLOCKB)
			{
				m_Map[index]->Rct.top = long(m_Map[index]->fY + TILESIZEY * 0.5);
			}
			else if (m_Map[index]->eTileID == MAP_BLOCKR || m_Map[index]->eTileID == MAP_GBLOCKR)
			{
				m_Map[index]->Rct.left = long(m_Map[index]->fX + TILESIZEX * 0.5);

			}
		}
		InvalidateRect(m_hWnd, NULL, true);
	}
}

void Maptool::Render(HDC hdc)
{
	RECT ret = { STARTX ,STARTY, TILESIZEX * TILEX , TILESIZEY * TILEY };
	PatBlt(hdc, ret.left, ret.top, ret.right, ret.bottom, BLACKNESS);

	for (int i = 0; i < m_Map.size(); i++)
	{
		if (m_Map[i]->eTileID == MAP_NONE)
		{
			//Rectangle(hdc, STARTX + m_Map[i]->fX, STARTY + m_Map[i]->fY, STARTX + m_Map[i]->fX + TILESIZEX, STARTY + m_Map[i]->fY + TILESIZEY);

		}
		else
			BitMapManager::GetSingleton()->GetImg((MAP)m_Map[i]->eTileID)->Draw(hdc, STARTX + m_Map[i]->fX, STARTY + m_Map[i]->fY, 1, 1);
		//Rectangle(hdc, STARTX + m_Map[i]->Rct.left, STARTY + m_Map[i]->Rct.top, STARTX + m_Map[i]->Rct.right, STARTY + m_Map[i]->Rct.bottom);
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
		WriteFile(hFile, &m_Map[i]->eTileID, sizeof(int), &writeB, NULL);
		WriteFile(hFile, &m_Map[i]->fSizeX, sizeof(float), &writeB, NULL);
		WriteFile(hFile, &m_Map[i]->fSizeY, sizeof(float), &writeB, NULL);
		WriteFile(hFile, &m_Map[i]->Rct, sizeof(RECT), &writeB, NULL);

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
		ReadFile(hFile, &m_Map[i]->eTileID, sizeof(int), &readB, NULL);
		ReadFile(hFile, &m_Map[i]->fSizeX, sizeof(float), &readB, NULL);
		ReadFile(hFile, &m_Map[i]->fSizeY, sizeof(float), &readB, NULL);
		ReadFile(hFile, &m_Map[i]->Rct, sizeof(RECT), &readB, NULL);
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
