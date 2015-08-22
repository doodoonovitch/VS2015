#pragma once
#ifndef CORE_MACROS_H
#define CORE_MACROS_H

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>

#define GL_CHECK_ERRORS \
{\
	GLenum err = glGetError();\
	if (err != GL_NO_ERROR) {\
	std::cerr << "Error " << err << " : " << gluErrorString(err) << endl;\
	assert(false);}\
}



#endif // CORE_MACROS_H
