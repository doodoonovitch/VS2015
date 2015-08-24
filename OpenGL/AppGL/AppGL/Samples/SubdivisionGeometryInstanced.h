#pragma once
#ifndef SAMPLES_SUBDIVISIONGEOMETRYINSTANCED_H
#define SAMPLES_SUBDIVISIONGEOMETRYINSTANCED_H

#include "Core\Macros.h"
#include "Core\Shader.h"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Samples
{



class SubdivisionGeometryInstanced
{
public:

	SubdivisionGeometryInstanced();
	~SubdivisionGeometryInstanced();

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
	void OnKey(unsigned char key, int x, int y);

private:

	//camera transformation variables
	int _state = 0, _oldX = 0, _oldY = 0;
	float _rX = 25.f, _rY = -40.f, _dist = -35.f;

	//number of sub-divisions
	int _subDivisions = 1;

	GLuint _vaoID;
	GLuint _vboVerticesID;
	GLuint _vboIndicesID;

	//mesh vertices and indices
	glm::vec3 _vertices[4];
	GLushort _indices[6];

	//projection and modelview matrices
	glm::mat4 _P;
	glm::mat4 _M[4];
	glm::vec4 _color[4];

	Core::Shader _shader;
};


} // namespace Samples
#endif // SAMPLES_SUBDIVISIONGEOMETRYINSTANCED_H
