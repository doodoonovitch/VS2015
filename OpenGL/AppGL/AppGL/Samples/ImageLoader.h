#pragma once
#ifndef SAMPLES_IMAGELOADER_H
#define SAMPLES_IMAGELOADER_H

#include "Core\Macros.h"
#include "Core\Shader.h"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Samples
{



class ImageLoader
{
public:

	ImageLoader();
	~ImageLoader();

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

	GLuint _vaoID;
	GLuint _vboVerticesID;
	GLuint _vboIndicesID;

	//texture ID
	GLuint _textureID;

	//mesh vertices and indices
	glm::vec2 _vertices[4];
	GLushort _indices[6];

	Core::Shader _shader;

	//texture image filename
	const std::string _filename = "medias/Lenna.png";
};




} // namespace Samples
#endif // SAMPLES_IMAGELOADER_H
