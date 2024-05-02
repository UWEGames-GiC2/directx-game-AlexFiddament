#ifndef _firstpersoncam_
#define _firstpersoncam_
#include "camera.h"

//=================================================================
//TPS style camera which will follow a given GameObject around _target
//=================================================================

class firstpersoncam : public Camera
{
public:
	firstpersoncam(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, std::shared_ptr<GameObject> _target, Vector3 _up, Vector3 _dpos);
	virtual ~firstpersoncam();

	virtual void Tick(GameData* _GD) override;

	float m_camYaw;
	float m_camPitch;
	Matrix camrotatey;
	Matrix camrotatex;
protected:
	std::shared_ptr<GameObject> m_targetObject;
	
	Vector3	m_dpos; //I'll lurk this far behind and away from it
	
};

#endif