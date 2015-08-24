#include "GL\glew.h"
#include "GL\freeglut.h"
#include <iostream>

#include "Core\TRunSample.h"
#include "Samples\SimpleTriangle.h"
#include "Samples\Ripple.h"
#include "Samples\SubdivisionGeometry.h"
#include "Samples\SubdivisionGeometryInstanced.h"
#include "Samples\ImageLoader.h"
#include "Samples\SimpleCamera.h"

using namespace std;
using namespace Core;
using namespace Samples;


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion(4, 5);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
	glutInitWindowSize(1280, 960);
	glutCreateWindow("OpenGL First Window");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cerr << "Error: " << glewGetErrorString(err) << endl;
		return (int)err;
	}

	cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	if (glewIsSupported("GL_VERSION_4_5"))
	{
		std::cout << "\tGLEW Version is 4.5\n ";
	}
	else
	{
		std::cout << "\tGLEW 4.5 not supported\n ";
	}

	glEnable(GL_DEPTH_TEST);

	TRunSample<SimpleTriangle> simpleTriangleSample;
	TRunSample<Ripple> rippleSample;
	TRunSample<SubdivisionGeometry> subdivisionGeometrySample;
	TRunSample<SubdivisionGeometryInstanced> subdivisionGeometryInstancedSample;
	TRunSample<ImageLoader> imageLoaderSample;
	TRunSample<SimpleCamera> simpleCameraSample;

	//simpleTriangleSample.Activate();
	//rippleSample.Activate();
	//subdivisionGeometrySample.Activate();
	//subdivisionGeometryInstancedSample.Activate();
	//imageLoaderSample.Activate();
	simpleCameraSample.Activate();

	glutMainLoop();

	return 0;
}