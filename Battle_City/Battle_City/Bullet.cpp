#include "Bullet.h"



Bullet::Bullet()
{
	ImgSize = BitMapManager::GetSingleton()->GetImg(OBJE_BULLET)->GetSize();
	//m_dwLastTime = GetTickCount();
	//m_dwCurTime = GetTickCount();
	//m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
}

void Bullet::Create(POS pos)
{
	POS bulletPos;

	bulletPos.m_iX = pos.m_iX + TILESIZEX * 0.35;
	bulletPos.m_iY = pos.m_iY ;

	m_bullet.push(bulletPos);

}

void Bullet::Shoot()
{
	//m_dwCurTime = GetTickCount();
	//m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	//m_dwLastTime = m_dwCurTime;

	if (Collision())
	{
		m_bullet.pop();
	}
	else if(!m_bullet.empty())
		m_bullet.front().m_iY -= 5;
}

bool Bullet::Collision()
{
	return true;
}

bool Bullet::IsPointInCircle(POS point) // 탱크 충돌,폭발 5단계
{
	return true;
}

bool Bullet::IsPointInRect(RECT rc) // 벽충돌
{
	return true;
}

void Bullet::Render(int StartX, int StartY)
{
	HDC hdc = BitMapManager::GetSingleton()->GetBufferDC();

	if (!m_bullet.empty())
		BitMapManager::GetSingleton()->GetImg(OBJE_BULLET)->Draw(hdc, StartX + m_bullet.front().m_iX, StartY + m_bullet.front().m_iY, 1, 1);

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
