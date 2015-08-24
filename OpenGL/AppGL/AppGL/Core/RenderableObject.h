#pragma once

#ifndef SAMPLES_RENDERABLEOBJECT_H
#define SAMPLES_RENDERABLEOBJECT_H

#include "Macros.h"
#include "Shader.h"


namespace Core
{



class RenderableObject
{
public:
	RenderableObject(void);
	virtual ~RenderableObject(void);
	virtual void Render(const float* MVP)=0;

protected:
	GLuint _vaoID;
	GLuint _vboVerticesID;
	GLuint _vboIndicesID;
	int _totalIndices; 
	Shader _shader;
};




} // namespace Core

#endif // SAMPLES_RENDERABLEOBJECT_H