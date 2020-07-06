#pragma once
#include"Mecro.h"
#include"Maptool.h"
#include"Font.h"

class Item
{
private:
	DWORD	m_dwLastTime;
	DWORD	m_dwCurTime;
	float	m_fDeltaTime;
	float	 m_fItemTime;
protected :
	ITEM m_eItemType;
	int m_iPoint;
	POS m_pos;
	RECT m_Rct;
	bool m_bItemFlag;
public:
	void SetItem();
	virtual void Init() = 0;
	void Draw();
	bool Collision(RECT ret);

	float GetTime() { return m_fItemTime; }
	int GetPoint() { return m_iPoint; }
	bool GetItemFlage() { return m_bItemFlag; }
	ITEM GetItemType() { return m_eItemType; }
	Item();
	virtual ~Item();
};

class Bomb :public Item
{
public:
	virtual void Init();
	Bomb() {};
	virtual ~Bomb() {};
};

class Clock :public Item
{
public:
	virtual void Init();
	Clock() {};
	virtual ~Clock() {};
};

class Helmet :public Item
{
public:
	virtual void Init();
	Helmet() {};
	virtual ~Helmet() {};
};

class Shovel : public Item
{
public:
	virtual void Init();
	Shovel() {};
	virtual ~Shovel() {};

};

class ItemTank : public Item
{
	public:
		virtual void Init();
		ItemTank() {};
		virtual ~ItemTank() {};
};