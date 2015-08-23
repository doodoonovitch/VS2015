#pragma once
#include "CameraBase.h"

#ifndef SAMPLES_TARGETCAMERA_H
#define SAMPLES_TARGETCAMERA_H

class TargetCamera :
	public CameraBase
{
public:
	TargetCamera();
	~TargetCamera();

	void Update();
	void Rotate(const float yaw, const float pitch, const float roll);
	 
	void SetTarget(const glm::vec3 tgt);
	const glm::vec3 GetTarget() const;

	void Pan(const float dx, const float dy);
	void Zoom(const float amount );
	void Move(const float dx, const float dz);

protected:
	glm::vec3 _target;  
	float _yaw, _pitch, _roll;
	 
	float _minRy, _maxRy;
	float _distance;
	float _minDistance, _maxDistance;

};

#endif // SAMPLES_TARGETCAMERA_H