#pragma once

#ifndef SAMPLES_GRID_H
#define SAMPLES_GRID_H

#include "Core\Macros.h"
#include "Core\Shader.h"
#include "Core\RenderableObject.h"

class Grid : public RenderableObject
{
public:
	Grid(int width=10, int depth=10);
	virtual ~Grid();

	void Render(const float* MVP);
};

#endif // SAMPLES_GRID_H