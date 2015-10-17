#pragma once

#ifndef RENDERABLES_GRID_H
#define RENDERABLES_GRID_H

#include "Core\Macros.h"
#include "Core\Shader.h"
#include "Core\RenderableObject.h"

namespace Renderables
{

class Grid : public Core::RenderableObject
{
public:
	Grid(int width=10, int depth=10);
	virtual ~Grid();

	void Render(const float* MVP);
};



} // namespace Renderables
#endif // RENDERABLES_GRID_H