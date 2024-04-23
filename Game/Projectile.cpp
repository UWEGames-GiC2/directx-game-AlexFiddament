#include "pch.h"
#include "Projectile.h"
#include <dinput.h>
#include "GameData.h"

Projectiles::Projectiles(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	
	m_fudge = Matrix::CreateRotationY(XM_PI);

	m_pos.y = 10.0f;

	SetDrag(0.7);
	SetPhysicsOn(true);
}

Projectiles::~Projectiles()
{
	
}


void Projectiles::Tick(GameData* _GD)
{
	

	
	CMOGO::Tick(_GD);
}