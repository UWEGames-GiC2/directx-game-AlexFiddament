#include "pch.h"
#include "firstpersoncam.h"
#include "GameData.h"

firstpersoncam::firstpersoncam(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, GameObject* _target, Vector3 _up, Vector3 _dpos)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _up)
{
	m_targetObject = _target;
	m_dpos = _dpos;
	

	
}

firstpersoncam::~firstpersoncam()
{

}

void firstpersoncam::Tick(GameData* _GD)
{
	
	

	m_pos = m_targetObject->GetPos();

	
	
	
	
		float speed = 0.0007f;
		m_camYaw +=  + sin(speed * _GD->m_MS.x) * m_dpos.z;
		m_camPitch += sin(speed * _GD->m_MS.y) * m_dpos.z;

		if (m_camPitch > XMConvertToRadians(60)) m_camPitch = XMConvertToRadians(60);
		if (m_camPitch < XMConvertToRadians(-60)) m_camPitch = XMConvertToRadians(-60);
		
	

	camrotatey = Matrix::CreateRotationX(-m_camPitch);
	camrotatex = Matrix::CreateRotationY(m_camYaw);

	Matrix rotation = camrotatey * camrotatex;

	Vector3 forwardDirection = Vector3::TransformNormal(Vector3::UnitZ, rotation);

	Vector3 cameraPosition = m_pos;

	Vector3 targetPoint = cameraPosition + forwardDirection;

	m_viewMat = Matrix::CreateLookAt(cameraPosition, targetPoint, m_up);

	m_projMat = Matrix::CreatePerspectiveFieldOfView(m_fieldOfView, m_aspectRatio, m_nearPlaneDistance, m_farPlaneDistance);

	

	


	//and then set up proj and view matrices
	Camera::Tick(_GD);
}


