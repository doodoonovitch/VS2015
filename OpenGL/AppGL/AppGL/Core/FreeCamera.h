#pragma once
#include "CameraBase.h"

#ifndef SAMPLES_FREECAMERA_H
#define SAMPLES_FREECAMERA_H

namespace Core
{



class FreeCamera :	public CameraBase
{
public:
	FreeCamera();
	~FreeCamera();

	void Update();
	void Rotate(const float yaw, const float pitch, const float roll);
	void Walk(const float amount);
	void Strafe(const float amount);
	void Lift(const float amount);
	 

protected:

	float _yaw, _pitch, _roll;
	
	glm::vec3 _translation;
};




} // namespace Core
#endif // SAMPLES_FREECAMERA_H