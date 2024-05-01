#include "pch.h"
#include "Player.h"
#include <dinput.h>
#include "GameData.h"


Player::Player(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	//any special set up for Player goes here
	m_fudge = Matrix::CreateRotationY(XM_PI);

	m_pos.y = 10.0f;

	SetDrag(0.7);
	SetPhysicsOn(true);
	
}

Player::~Player()
{
	//tidy up anything I've created
}


void Player::Tick(GameData* _GD)
{
	switch (_GD->m_GS)
	{
	case GS_PLAY_FIRST_PERSON_CAM:
	{
		//TURN AND FORWARD CONTROL HERE
		Vector3 forwardMove = 40.0f * Vector3::Forward;
		Matrix rotMove = Matrix::CreateRotationY(m_yaw + XMConvertToRadians(180));
		forwardMove = Vector3::Transform(forwardMove, rotMove);
		if (_GD->m_KBS.W)
		{
			m_acc += forwardMove;
		}
		if (_GD->m_KBS.S)
		{
			m_acc -= forwardMove;
		}
		break;
	}
	}

	//change orinetation of player
	float rotSpeed = 1 * _GD->m_dt;

	m_yaw -= rotSpeed * _GD->m_MS.x;

	m_pitch -= -rotSpeed * _GD->m_MS.y;



	if (m_pitch > XMConvertToRadians(60)) m_pitch = XMConvertToRadians(60);
	if (m_pitch < XMConvertToRadians(-60)) m_pitch = XMConvertToRadians(-60);







	;//if (_GD->m_KBS.A)
	{
		//m_yaw += rotSpeed;
	}
	//if (_GD->m_KBS.D)
	{
		//m_yaw -= rotSpeed;
	}

	//move player up and down


	if (_GD->m_KBS.R)
	{
		m_acc.y += 40.0f;
	}

	if (_GD->m_KBS.F)
	{
		m_acc.y -= 40.0f;
	}

	//limit motion of the player
	float length = m_pos.Length();
	float maxLength = 500.0f;
	if (length > maxLength)
	{
		m_pos.Normalize();
		m_pos *= maxLength;
		m_vel *= -0.9; //VERY simple bounce back
	}


	switch (_GD->m_GS)
	{
	case GS_PLAY_FIRST_PERSON_CAM:
	{
		if (_GD->m_MS.leftButton)
		{


			m_vel.y = 20.0f;

		}
		if (_GD->m_KBS_tracker.pressed.Space)
		{
			

			for (size_t i = 0; i < Projectiles.size(); i++)
			{
				if (!Projectiles[i]->isactive() )
				{
					
					
					Projectiles[i]->SetPos(this->GetPos());
					Vector3 forwardMove = 40.0f * Vector3::Forward;
					Matrix rotYaw = Matrix::CreateRotationY(this->GetYaw() + XMConvertToRadians(180));
					Matrix rotPitch = Matrix::CreateRotationX(-this->GetPitch());
					Matrix rotMove = rotPitch * rotYaw;
					forwardMove = Vector3::Transform(forwardMove, rotMove);
					Projectiles[i]->SetActive(true);
					Projectiles[i]->SetYaw(this->GetYaw());
					Projectiles[i]->SetDrag(0.01f);
					Projectiles[i]->SetPhysicsOn(true);
					Projectiles[i]->SetAcceleration(forwardMove * 1000.0f);

					


					break;


				}
			}
			
		}



		m_acc.y -= 15.0f;
		break;

	}
	}

	


   

	

	//apply my base behaviour
	CMOGO::Tick(_GD);
}