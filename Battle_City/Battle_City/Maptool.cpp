#include "Maptool.h"



Maptool::Maptool()
{
}

void Maptool::Init(HWND hwnd)
{
	m_hWnd = hwnd;
	memset(g_map, -1, sizeof(int) * 13 * 13); // √ ±‚»≠
}

void Maptool::Create(POINT pt)
{
	if ((pt.x <  WIDTH * 13 && pt.x > 0) && (pt.y <   HEIGHT * 13 && pt.y > 0))
	{
		if (g_map[pt.y / HEIGHT][pt.x / WIDTH] >= MAP_ENDFALGE)
			g_map[pt.y / HEIGHT][pt.x / WIDTH] = MAP_NONE;
		else
			g_map[pt.y / HEIGHT][pt.x / WIDTH]++;
		InvalidateRect(m_hWnd, NULL, true);
	}
}

void Maptool::Render(HDC hdc)
{

	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			if (g_map[i][j] == MAP_NONE)
			{
				//Rectangle(hdc, j * WIDTH, i * HEIGHT, (j + 1) * WIDTH, (i + 1) * HEIGHT);
			}
			else
				BitMapManager::GetSingleton()->GetImg((MAP)g_map[i][j])->Draw(hdc, j * WIDTH, i * HEIGHT, 1, 1);
			
		}
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
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			DWORD writeB;
			WriteFile(hFile, &g_map[i][j], sizeof(int), &writeB, NULL);
		}
	}
	CloseHandle(hFile);
	InvalidateRect(m_hWnd, NULL, false);
}

void Maptool::Load()
{
	HANDLE hFile = CreateFile(L"save.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			DWORD readB;
			ReadFile(hFile, &g_map[i][j], sizeof(int), &readB, NULL);
		}
	}
	CloseHandle(hFile);
	InvalidateRect(m_hWnd, NULL, false);
}

Maptool::~Maptool()
{

}
