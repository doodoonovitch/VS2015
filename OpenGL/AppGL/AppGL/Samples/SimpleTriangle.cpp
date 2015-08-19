#include "SimpleTriangle.h"
#include <iostream>

using namespace std;

const char* shader_vert = R"(
#version 330 core
 
layout(location = 0) in vec3 vVertex;	//object space vertex position
layout(location = 1) in vec3 vColor;	//per-vertex colour

//output from the vertex shader
smooth out vec4 vSmoothColor;		//smooth colour to fragment shader

//uniform
uniform mat4 MVP;	//combined modelview projection matrix

void main()
{
	//assign the per-vertex colour to vSmoothColor varying
   vSmoothColor = vec4(vColor,1);

	   //get the clip space position by multiplying the combined MVP matrix with the object space 
   //vertex position
   gl_Position = MVP*vec4(vVertex,1);
}
)";

const char* shader_frag = R"(
#version 330 core

layout(location=0) out vec4 vFragColor;	//fragment shader output

//input form the vertex shader
smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader

void main()
{
	//set the interpolated colour as the shader output
	vFragColor = vSmoothColor;
}
)";

void SimpleTriangle::OnRender(void)
{
	//clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//bind the shader
	_shader.Use();
	//pass the shader uniform
	glUniformMatrix4fv(_shader("MVP"), 1, GL_FALSE, glm::value_ptr(_P *_MV));
	//drwa triangle
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
	//unbind the shader
	_shader.UnUse();

	//swap front and back buffers to show the rendered result
	glutSwapBuffers();
}

void SimpleTriangle::OnShutdown()
{
	//Destroy shader
	_shader.DeleteShaderProgram();

	//Destroy vao and vbo
	glDeleteBuffers(1, &_vboVerticesID);
	glDeleteBuffers(1, &_vboIndicesID);
	glDeleteVertexArrays(1, &_vaoID);

	cout << "Shutdown successfull" << endl;
}

void SimpleTriangle::OnResize(int w, int h)
{
	//set the viewport size
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	//setup the projection matrix
	_P = glm::ortho(-1, 1, -1, 1);
}

void SimpleTriangle::OnInit()
{
	GL_CHECK_ERRORS;

	//load the shader
	//_shader.LoadFromFile(GL_VERTEX_SHADER, "shaders\\SimpleTriangle_shader.vert");
	//_shader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders\\SimpleTriangle_shader.frag");
	_shader.LoadFromString(GL_VERTEX_SHADER, shader_vert);
	_shader.LoadFromString(GL_FRAGMENT_SHADER, shader_frag);
	//compile and link shader
	_shader.CreateAndLinkProgram();
	_shader.Use();
	//add attributes and uniforms
	_shader.AddAttribute("vVertex");
	_shader.AddAttribute("vColor");
	_shader.AddUniform("MVP");
	_shader.UnUse();

	GL_CHECK_ERRORS;

	//setup triangle geometry
	//setup triangle vertices
	_vertices[0].color = glm::vec3(1, 0, 0);
	_vertices[1].color = glm::vec3(0, 1, 0);
	_vertices[2].color = glm::vec3(0, 0, 1);

	_vertices[0].position = glm::vec3(-1, -1, 0);
	_vertices[1].position = glm::vec3(0, 1, 0);
	_vertices[2].position = glm::vec3(1, -1, 0);

	//setup triangle indices
	_indices[0] = 0;
	_indices[1] = 1;
	_indices[2] = 2;

	GL_CHECK_ERRORS;

	//setup triangle vao and vbo stuff
	glGenVertexArrays(1, &_vaoID);
	glGenBuffers(1, &_vboVerticesID);
	glGenBuffers(1, &_vboIndicesID);
	GLsizei stride = sizeof(Vertex);

	glBindVertexArray(_vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, _vboVerticesID);
	//pass triangle verteices to buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), &_vertices[0], GL_STATIC_DRAW);
	GL_CHECK_ERRORS;

	//enable vertex attribute array for position
	glEnableVertexAttribArray(_shader["vVertex"]);
	glVertexAttribPointer(_shader["vVertex"], 3, GL_FLOAT, GL_FALSE, stride, 0);
	GL_CHECK_ERRORS;

	//enable vertex attribute array for colour
	glEnableVertexAttribArray(_shader["vColor"]);
	glVertexAttribPointer(_shader["vColor"], 3, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)offsetof(Vertex, color));
	GL_CHECK_ERRORS;

	//pass indices to element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), &_indices[0], GL_STATIC_DRAW);
	GL_CHECK_ERRORS;

	cout << "Initialization successfull" << endl;
}

SimpleTriangle::SimpleTriangle()
{
	_P = glm::mat4(1);
	_MV = glm::mat4(1);
}

SimpleTriangle::~SimpleTriangle()
{
}
