#include "Bullet.h"



Bullet::Bullet()
{
	type = OBJE_BULLET;
	ImgSize = BitMapManager::GetSingleton()->GetImg((OBJECT)type)->GetSize();
	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_bCollision = false;
}

void Bullet::Create(POS pos,DIRECTION direction)
{
	POS bulletPos;

	type = OBJE_BULLET;

	float radiusX = TILESIZEX * 0.4 + ImgSize.cx;
	float radiusY = TILESIZEX * 0.5 + ImgSize.cy;

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
		bulletPos.m_iY = CenterY - (sin(90 * PI / 180)*radiusY);
	}
	else if (direction == RIGHT)
	{
		bulletPos.m_iX = CenterX + (cos(360 * PI / 180)*radiusX);
		bulletPos.m_iY = CenterY + (sin(360 * PI / 180)*radiusY);
	}
	else if (direction == DOWN)
	{
		bulletPos.m_iX = CenterX + (cos(270 * PI / 180)*radiusX);
		bulletPos.m_iY = CenterY - (sin(270 * PI / 180)*radiusY);
	}

	if(m_bullet.empty())
		m_bullet.push(bulletPos);

}

void Bullet::Shoot(vector<Tile*> v)
{
	type = OBJE_BULLET;
	m_bShoot = true;


	if (m_direction == LEFT)
	{
		m_bullet.front().m_iX -= 300* m_fDeltaTime;
	}
	else if (m_direction == UP)
	{
		m_bullet.front().m_iY -= 300 * m_fDeltaTime;
	}
	else if (m_direction == RIGHT)
	{
		m_bullet.front().m_iX += 300 * m_fDeltaTime;
	}
	else if (m_direction == DOWN)
	{
		m_bullet.front().m_iY += 300 * m_fDeltaTime;
	}
}


void  Bullet::Collision(vector<Tile*> v)
{
	int x = m_bullet.front().m_iX + TILESIZEX * 0.5;
	int y = m_bullet.front().m_iY;

	if (y <= 0 || y >= TILESIZEY * 13 - ImgSize.cy || x <= 0 || x >= TILESIZEX * 13 )
	{
		type = OBJE_EXPLOSION00;
		m_bCollision = true;
		return;
	}
	else
	{
		for (int i = 0; i < v.size(); i++)
		{
			if (v[i]->eTileID != MAP_NONE)
			{
				if (IsPointInRect(v[i]->Rct))
				{
					type = OBJE_EXPLOSION00;
					m_bCollision = true;
					return;
				}
			}
		}
	}
	m_bCollision = false;
}

bool Bullet::IsPointInCircle(POS point) // 탱크 충돌,
{
	return true;
}

bool Bullet::IsPointInRect(RECT rc) // 벽충돌
{
	int x = m_bullet.front().m_iX + TILESIZEX * 0.5;
	int y = m_bullet.front().m_iY;


	if ((rc.left <= x && x <= rc.right) && (rc.top <= y && y <= rc.bottom))
	{
		return true;
	}
	else
		return false;
}	

void Bullet::ExsplosionRender(HDC hdc)
{
	int x = STARTX + m_bullet.front().m_iX;
	int y = STARTY + m_bullet.front().m_iY;

	SIZE size = BitMapManager::GetSingleton()->GetImg((OBJECT)type)->GetSize();

	static float Time = 0.0f;
	static int tmptype = type;
	static int RepeatNum = 0;

	
	Time += m_fDeltaTime;

	if (Time > 0.1f)
	{
		tmptype++;
		if (tmptype > OBJE_EXPLOSION01)
		{
			tmptype = OBJE_EXPLOSION00;
			RepeatNum++;
		}
		Time = 0.0f;
	}

	if (m_direction == LEFT)
	{
		BitMapManager::GetSingleton()->GetImg((OBJECT)tmptype)->Draw(hdc, x , y - size.cy* 0.5, 1, 1);
	}
	else if (m_direction == UP)
	{
		BitMapManager::GetSingleton()->GetImg((OBJECT)tmptype)->Draw(hdc, x - size.cx * 0.5, y - size.cy* 0.5, 1, 1);
	}
	else if (m_direction == RIGHT)
	{
		BitMapManager::GetSingleton()->GetImg((OBJECT)tmptype)->Draw(hdc, x , y - size.cy * 0.5, 1, 1);
	}
	else if (m_direction == DOWN)
	{
		BitMapManager::GetSingleton()->GetImg((OBJECT)tmptype)->Draw(hdc, x - size.cx * 0.5, y - size.cy* 0.5, 1, 1);
	}

	if (RepeatNum == 1)
	{
		Clear();
		RepeatNum = 0;
	}

}

void Bullet::Render()
{
	HDC hdc = BitMapManager::GetSingleton()->GetBufferDC();

	if (!m_bullet.empty())
	{
		if(type == OBJE_BULLET)
			BitMapManager::GetSingleton()->GetImg((OBJECT)type)->Draw(hdc, STARTX + m_bullet.front().m_iX, STARTY + m_bullet.front().m_iY, 1, 1);
		else
		{
			ExsplosionRender(hdc);
		}
	}


}

void Bullet::Update(DIRECTION direction, vector<Tile*> v)
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;

	m_direction = direction;

	if (!m_bullet.empty())
	{
		Collision(v);
		if (!m_bCollision)
		{
			Shoot(v);
		}
	}
	else
		m_bShoot = false;
}

void Bullet::Clear()
{
	while (!m_bullet.empty()) m_bullet.pop();
}



Bullet::~Bullet()
{
	Clear();
}
