#pragma once
class Player
{
private:
	int m_iScore;
public:
	void Init();
	int GetScore() { return m_iScore; }
	Player();
	~Player();
};

