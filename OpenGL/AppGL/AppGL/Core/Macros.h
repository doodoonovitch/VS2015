#pragma once
#ifndef CORE_MACROS_H
#define CORE_MACROS_H

#include <GL/glew.h>
#include <GL/freeglut.h>


#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR)


#endif // CORE_MACROS_H
