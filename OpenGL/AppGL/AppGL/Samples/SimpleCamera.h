#pragma once
#ifndef SAMPLES_SIMPLECAMERA_H
#define SAMPLES_SIMPLECAMERA_H

#include "Core\Macros.h"
#include "Core\Shader.h"
#include "Core\TargetCamera.h"

#include "Renderables\Grid.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SimpleCamera
{
public:

	SimpleCamera();
	~SimpleCamera();

	void OnRender();

	void OnShutdown();

	void OnResize(int nw, int nh);

	void OnInit();

	void OnIdle();

	void OnMouseDown(int button, int s, int x, int y);
	void OnMouseMove(int x, int y);
	void OnKey(unsigned char key, int x, int y);

private:

	void filterMouseMoves(float dx, float dy);

	static const int VK_W = 0x57;
	static const int VK_S = 0x53;
	static const int VK_A = 0x41;
	static const int VK_D = 0x44;
	static const int VK_Q = 0x51;
	static const int VK_Z = 0x5a;
	static const int MOUSE_HISTORY_BUFFER_SIZE = 10;

	const float MOVE_SPEED = 0.125f;
	const float MOUSE_FILTER_WEIGHT = 0.75f;

	//camera transformation variables
	int _state = 0, _oldX = 0, _oldY = 0;
	float _rX = 0.f, _rY = 135.f, _dist = 0.f;
	float _dt = 0;

	Grid* _grid;
	TargetCamera _camera;

	glm::vec2 _mouseHistory[MOUSE_HISTORY_BUFFER_SIZE];

	float _mouseX = 0, _mouseY = 0; //filtered mouse values
	bool useFiltering = true;

};



#endif // SAMPLES_SIMPLECAMERA_H
