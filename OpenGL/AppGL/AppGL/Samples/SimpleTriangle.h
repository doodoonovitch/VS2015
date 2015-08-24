#pragma once
#ifndef SAMPLES_SIMPLETRIANGLE_H
#define SAMPLES_SIMPLETRIANGLE_H

#include "Core\Macros.h"
#include "Core\Shader.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Samples
{



class SimpleTriangle
{
public:

	SimpleTriangle();
	~SimpleTriangle();

	void OnRender();

	void OnShutdown();

	void OnResize(int nw, int nh);

	void OnInit();

	void OnIdle() {}
	void OnMouseDown(int button, int s, int x, int y) {}
	void OnMouseMove(int x, int y) {}
	void OnKey(unsigned char key, int x, int y) {}

private:

	//out vertex struct for interleaved attributes
	struct Vertex 
	{
		glm::vec3 position;
		glm::vec3 color;
	};

	GLuint _vaoID;
	GLuint _vboVerticesID;
	GLuint _vboIndicesID;

	//triangle vertices and indices
	Vertex _vertices[3];
	GLushort _indices[3];

	//projection and modelview matrices
	glm::mat4 _P;
	glm::mat4 _MV;

	Core::Shader _shader;
};



} // namespace Samples
#endif // SAMPLES_SIMPLETRIANGLE_H
