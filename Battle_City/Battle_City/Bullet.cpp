#include "Bullet.h"



Bullet::Bullet()
{
	ImgSize = BitMapManager::GetSingleton()->GetImg(OBJE_BULLET)->GetSize();
	//m_dwLastTime = GetTickCount();
	//m_dwCurTime = GetTickCount();
	//m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
}

void Bullet::Create(POS pos,DIRECTION direction)
{
	POS bulletPos;

	float radiusX = TILESIZEX * 0.5 + ImgSize.cx;
	float radiusY = TILESIZEY * 0.5 + ImgSize.cy;

	CenterX = pos.m_iX + TILESIZEX * 0.5;
	CenterY = pos.m_iY + TILESIZEY * 0.5;

	if (direction == LEFT)
	{
		bulletPos.m_iX = CenterX + (cos(180 * PI / 180)*radiusX);
		bulletPos.m_iY = CenterY + (sin(180 * PI / 180)*radiusY);
	}
	else if (direction == UP)
	{
		bulletPos.m_iX = CenterX + (cos(90 * PI / 180)*radiusX);
		bulletPos.m_iY = CenterY + (sin(90 * PI / 180)*radiusY);
	}
	else if (direction == RIGHT)
	{
		bulletPos.m_iX = CenterX + (cos(360 * PI / 180)*radiusX);
		bulletPos.m_iY = CenterY + (sin(360 * PI / 180)*radiusY);
	}
	else if (direction == DOWN)
	{
		bulletPos.m_iX = CenterX + (cos(270 * PI / 180)*radiusX);
		bulletPos.m_iY = CenterY + (sin(270 * PI / 180)*radiusY);
	}

	m_bullet.push(bulletPos);

}

void Bullet::Shoot(DIRECTION direction, vector<Tile*> v)
{
	//m_dwCurTime = GetTickCount();
	//m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	//m_dwLastTime = m_dwCurTime;

	if (!m_bullet.empty())
	{
		if (Collision(v))
		{
			m_bullet.pop();
		}
		else
		{
			if (direction == LEFT)
			{
				m_bullet.front().m_iX -= 5;
			}
			if (direction == UP)
			{
				m_bullet.front().m_iY -= 5;
			}
			else if (direction == RIGHT)
			{
				m_bullet.front().m_iX += 5;
			}
			else if (direction == DOWN)
			{
				m_bullet.front().m_iY += 5;
			}
		}
	}
}

bool Bullet::Collision(vector<Tile*> v)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i]->eTileID != MAP_NONE)
		{
			if (IsPointInRect(v[i]->Rct))
			{
				return true;
			}
		}

	}
	return false;
}

bool Bullet::IsPointInCircle(POS point) // 탱크 충돌,폭발 5단계
{
	return true;
}

bool Bullet::IsPointInRect(RECT rc) // 벽충돌
{
	int x = m_bullet.front().m_iX + TILESIZEX * 0.5;
	int y = m_bullet.front().m_iY;
	if (!((x < TILESIZEX * 13 && x > 0) && (y < TILESIZEY * 13 && y > 0)))
		return true;

	else if ((rc.left <= x && x <= rc.right) && (rc.top <= y && y <= rc.bottom))
	{
		return true;
	}
	

	return false;
}



void Bullet::Render(int StartX, int StartY)
{
	HDC hdc = BitMapManager::GetSingleton()->GetBufferDC();

	if (!m_bullet.empty())
	{

		BitMapManager::GetSingleton()->GetImg(OBJE_BULLET)->Draw(hdc, StartX + m_bullet.front().m_iX, StartY + m_bullet.front().m_iY, 1, 1);
	}

}

void Bullet::Update()
{

}

void Bullet::Clear()
{
	while (!m_bullet.empty()) m_bullet.pop();
}



Bullet::~Bullet()
{
	Clear();
}
