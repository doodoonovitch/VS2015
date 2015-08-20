#pragma once
#ifndef SAMPLES_RIPPLE_H
#define SAMPLES_RIPPLE_H

#include "Core\Macros.h"
#include "Core\Shader.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Ripple
{
public:

	Ripple();
	~Ripple();

	void OnRender();

	void OnShutdown();

	void OnResize(int nw, int nh);

	void OnInit();

	void OnIdle()
	{
		glutPostRedisplay();
	}

	void OnMouseDown(int button, int s, int x, int y);
	void OnMouseMove(int x, int y);

private:

	const int NUM_X = 40; //total quads on X axis
	const int NUM_Z = 40; //total quads on Z axis

	const float SIZE_X = 4.f; //size of plane in world space
	const float SIZE_Z = 4.f;
	const float HALF_SIZE_X = SIZE_X / 2.0f;
	const float HALF_SIZE_Z = SIZE_Z / 2.0f;

	//ripple displacement speed
	const float SPEED = 2.f;
	const int TOTAL_VERTICES = (NUM_X + 1)*(NUM_Z + 1);
	const int TOTAL_INDICES = NUM_X*NUM_Z * 2 * 3;

	//camera transformation variables
	int _state = 0, _oldX = 0, _oldY = 0;
	float _rX = 25.f, _rY = -40.f, _dist = -7.f;

	//current time
	float _time = 0.f;

	GLuint _vaoID;
	GLuint _vboVerticesID;
	GLuint _vboIndicesID;

	//triangle vertices and indices
	glm::vec3* _vertices;
	GLushort* _indices;

	//projection and modelview matrices
	glm::mat4 _P;
	glm::mat4 _MV;

	Shader _shader;
};



#endif // SAMPLES_SIMPLETRIANGLE_H
