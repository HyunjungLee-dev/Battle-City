#include "Bullet.h"



Bullet::Bullet()
{
	type = OBJE_BULLETL;
	ImgSize = BitMapManager::GetSingleton()->GetImg((OBJECT)type)->GetSize();
	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_bCollision = false;
}

void Bullet::Create(POS pos,DIRECTION direction)
{
	POS bulletPos;


	float radiusX = TILESIZEX * 0.3 ;
	float radiusY = TILESIZEX * 0.3 ;

	CenterX = pos.m_iX + TILESIZEX * 0.5;
	CenterY = pos.m_iY + TILESIZEY * 0.5;

	if (m_bullet.empty())
	{
		m_direction = direction;
		if (m_direction == LEFT)
		{
			type = OBJE_BULLETL;
			bulletPos.m_iX = CenterX + (cos(180 * PI / 180)*radiusX);
			bulletPos.m_iY = CenterY + (sin(180 * PI / 180)*radiusY);
		}
		else if (m_direction == UP)
		{
			type = OBJE_BULLETU;
			bulletPos.m_iX = CenterX + (cos(90 * PI / 180)*radiusX);
			bulletPos.m_iY = CenterY - (sin(90 * PI / 180)*radiusY);
		}
		else if (m_direction == RIGHT)
		{
			type = OBJE_BULLETR;
			bulletPos.m_iX = CenterX + (cos(360 * PI / 180)*radiusX);
			bulletPos.m_iY = CenterY + (sin(360 * PI / 180)*radiusY);
		}
		else if (m_direction == DOWN)
		{
			type = OBJE_BULLETD;
			bulletPos.m_iX = CenterX + (cos(270 * PI / 180)*radiusX);
			bulletPos.m_iY = CenterY - (sin(270 * PI / 180)*radiusY);
		}


		m_bullet.push(bulletPos);
	}
}

void Bullet::Shoot(vector<Tile*> v)
{

	if (m_direction == LEFT)
	{
		type = OBJE_BULLETL;
		m_bullet.front().m_iX -= 200* m_fDeltaTime;
	}
	else if (m_direction == UP)
	{
		type = OBJE_BULLETU;
		m_bullet.front().m_iY -= 200 * m_fDeltaTime;
	}
	else if (m_direction == RIGHT)
	{
		type = OBJE_BULLETR;
		m_bullet.front().m_iX += 200 * m_fDeltaTime;
	}
	else if (m_direction == DOWN)
	{
		type = OBJE_BULLETD;
		m_bullet.front().m_iY += 200 * m_fDeltaTime;
	}
}


void  Bullet::Collision(vector<Tile*> v)
{
	ImgSize = BitMapManager::GetSingleton()->GetImg((OBJECT)type)->GetSize();
	int x, y;

	switch ((OBJECT)type)
	{
	case OBJE_BULLETL:
		x = m_bullet.front().m_iX ;
		y = m_bullet.front().m_iY;
		break;
	case OBJE_BULLETU:
		x = m_bullet.front().m_iX ;
		y = m_bullet.front().m_iY;
		break;
	case OBJE_BULLETR:
		x = m_bullet.front().m_iX + ImgSize.cx;
		y = m_bullet.front().m_iY;
		break;
	case OBJE_BULLETD:
		x = m_bullet.front().m_iX ;
		y = m_bullet.front().m_iY+ ImgSize.cy;
		break;
	default:
		return;
		break;
	}

	if (y <= 0 || y >= TILESIZEY * 13 - ImgSize.cy || x <= 0 || x >= TILESIZEX * 13 )
	{
		type = OBJE_EXPLOSION00;
		m_eCollType = TYPEWALL;
		m_bCollision = true;
		return;
	}
	else
	{
		for (int i = 0; i < v.size(); i++) // 벽
		{
			if (v[i]->eTileID != MAP_NONE)
			{
				if (IsPointInRect(v[i]->Rct,x,y))
				{
					m_eCollType = TYPEWALL;
					type = OBJE_EXPLOSION00;
					//충돌했을때 벽 변화 넣기
					Maptool::GetSingleton()->Collision(i, m_direction);
					if (v[i]->eTileID == MAP_ENDFALGE)
					{
						m_eCollType = TYPETANK;;
					}
					m_bCollision = true;
					return;
				}
			}
		}
	}
	m_bCollision = false;
}

bool Bullet::IsPointInCircle(float cx, float cy) // 탱크 충돌
{
	ImgSize = BitMapManager::GetSingleton()->GetImg((OBJECT)type)->GetSize();
	int x, y;
	float cr;
	cr = TILESIZEX * 0.5;

	if (!m_bullet.empty())
	{
		x = m_bullet.front().m_iX;
		y = m_bullet.front().m_iY;

		//내 총알이  상대 탱크에 맞았는가
		float deltaX = cx - x; //centerx
		float deltaY = cy - y; //centery

		float length = sqrt(deltaX * deltaX + deltaY * deltaY);

		if (length > cr) //반지름
			return false;
		else
		{
			type = OBJE_EXPLOSION00;
			m_eCollType = TYPETANK;
			m_bCollision = true;
			return true;
		}
	}
	return false;
}

bool Bullet::IsPointInRect(RECT rc, int x, int y) // 벽충돌
{
	//내 총알이 벽에 맞았는가
	
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
	static bool check = false;
	
	Time += m_fDeltaTime;

	if (Time > 0.1f)
	{
		if (m_eCollType == TYPEWALL)
		{
			tmptype++;
			if (tmptype > OBJE_EXPLOSION01)
			{
				tmptype = OBJE_EXPLOSION00;
				RepeatNum++;
			}
		}
		else if (m_eCollType == TYPETANK)
		{
			tmptype++;
			if (tmptype > OBJE_EXPLOSION04)
			{
				tmptype = OBJE_EXPLOSION00;
				RepeatNum++;
			}
		}
		Time = 0.0f;
	}

	if (m_direction == LEFT)
	{
		BitMapManager::GetSingleton()->GetImg((OBJECT)tmptype)->Draw(hdc, x - size.cx * 0.5, y - size.cy* 0.5, 1, 1);
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
		if(type >= OBJE_BULLETL  && type <= OBJE_BULLETR)
			BitMapManager::GetSingleton()->GetImg((OBJECT)type)->Draw(hdc, STARTX + m_bullet.front().m_iX, STARTY + m_bullet.front().m_iY, 1, 1);
		else
		{
			ExsplosionRender(hdc);
		}
	}


}

void Bullet::Update(vector<Tile*> v)
{
	m_dwCurTime = GetTickCount();
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;

	if (!m_bullet.empty())
	{
		Collision(v);
		if (!m_bCollision)
		{
			Shoot(v);
		}
	}

}

void Bullet::Clear()
{
	while (!m_bullet.empty()) m_bullet.pop();
}



Bullet::~Bullet()
{
	Clear();
}
