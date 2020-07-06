#pragma once
#include"Mecro.h"
#include"BitMapManager.h"

class Maptool : public Singleton< Maptool>
{
private:
	vector<Tile*> m_Map;
	POS m_Cursor;
	bool m_bConstruction;
	int m_iID;
public:
	void Init();
	void SetMap();
	void Create();

	void CursorRender(HDC hdc);
	void Render(HDC hdc);
	void layerRender(HDC hdc);


	void Save();
	void Load(LPCWSTR Flie);
	bool MapConstruction();
	void Collision(int index,DIRECTION direct);
	void Clear();

	vector<Tile*> GetMap() { return m_Map; }
	bool FlagCheck()
	{
		if (m_Map[162]->eTileID == MAP_ENDFALGE)
			return true;
		else
			return false;
	}
	bool GetConstruction() { return m_bConstruction; }
	Maptool();
	~Maptool();
};

