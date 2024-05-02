#include "pch.h"
#include "firstpersoncam.h"
#include "GameData.h"

firstpersoncam::firstpersoncam(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, std::shared_ptr<GameObject> _target, Vector3 _up, Vector3 _dpos)
	:Camera(_fieldOfView, _aspectRatio, _nearPlaneDistance, _farPlaneDistance, _up)
{
	m_targetObject = _target;
	m_dpos = _dpos;
	
	m_camYaw = m_targetObject->GetYaw();
	m_camPitch = m_targetObject->GetPitch();
	
}

firstpersoncam::~firstpersoncam()
{

}

void firstpersoncam::Tick(GameData* _GD)
{
	
	

	m_pos = m_targetObject->GetPos();
	m_camYaw = m_targetObject->GetYaw();
	m_camPitch = m_targetObject->GetPitch();

	m_pos += m_dpos;

	
		float speed = 0.0007f;
		
		

		if (m_camPitch > XMConvertToRadians(60)) m_camPitch = XMConvertToRadians(60);
		if (m_camPitch < XMConvertToRadians(-60)) m_camPitch = XMConvertToRadians(-60);
		
	

	camrotatey = Matrix::CreateRotationX(m_camPitch);
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


