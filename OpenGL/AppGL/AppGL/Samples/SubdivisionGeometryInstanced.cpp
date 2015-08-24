#include "SubdivisionGeometryInstanced.h"
#include <algorithm>

using namespace std;
using namespace Core;

namespace Samples
{



const char* subdivision_geometry_instanced_shader_vert = R"(
#version 330 core
  
layout(location=0) in vec3 vVertex;  //object space vertex position

uniform mat4 M[4];		//modeling matrix for each instance
uniform vec4 color[4];	//color for each instance

out VS_OUT {
    vec4 color;
} vs_out;

void main()
{    
	//get the world space position of each instance vertex
	gl_Position =  M[gl_InstanceID]*vec4(vVertex, 1);
	vs_out.color = color[gl_InstanceID];
}
)";

const char* subdivision_geometry_instanced_shader_frag = R"(
#version 330 core
 
layout(location=0) out vec4 vFragColor;	//fragment shader output

//input form the vertex shader
in vec4 vertex_color;		//interpolated colour to fragment shader

void main()
{
   vFragColor = vertex_color;
}
)";

const char* subdivision_geometry_instanced_shader_geom = R"(
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=256) out; 

in VS_OUT {
    vec4 color;
} gs_in[];  

out vec4 vertex_color;
 
//uniforms 
uniform int sub_divisions;	//the number of subdivisions
uniform mat4 PV;			//combined view porjection matrix

void main()
{
	//get the world space vertex positions
	vec4 v0 = gl_in[0].gl_Position;
	vec4 v1 = gl_in[1].gl_Position;
	vec4 v2 = gl_in[2].gl_Position; 

	//determine the size of each sub-division 
	float dx = abs(v0.x-v2.x)/sub_divisions;
	float dz = abs(v0.z-v1.z)/sub_divisions;

	float x=v0.x;
	float z=v0.z;

	//loop through all sub-divisions and emit vertices
	//after mutiplying the world space vertex positions
	// with the view projection matrix. We move first in
	//x axis, once we reach the edge, we reset x to the initial 
	//x value while incrementing the z value.
	for(int j=0;j<sub_divisions*sub_divisions;j++) 
	{
		gl_Position =  PV * vec4(x,0,z,1);
		vertex_color = gs_in[0].color;
		EmitVertex();

		gl_Position =  PV * vec4(x,0,z+dz,1);
		vertex_color = gs_in[0].color;
		EmitVertex();

		gl_Position =  PV * vec4(x+dx,0,z,1);
		vertex_color = gs_in[0].color;
		EmitVertex();

		gl_Position =  PV * vec4(x+dx,0,z+dz,1);
		vertex_color = gs_in[0].color;
		EmitVertex();

		EndPrimitive();

		x+=dx;

			if((j+1) %sub_divisions == 0) {
		   x=v0.x;
		   z+=dz;
		}	
	}	
}
)";


void SubdivisionGeometryInstanced::OnRender(void)
{
	//clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera transformation 
	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, _dist));
	glm::mat4 Rx = glm::rotate(T, _rX, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 V = glm::rotate(Rx, _rY, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 PV = _P*V;

	//bind the shader
	_shader.Use();
	//set the shader uniforms
	glUniformMatrix4fv(_shader("PV"), 1, GL_FALSE, glm::value_ptr(PV));
	glUniform1i(_shader("sub_divisions"), _subDivisions);
	//render instanced geometry
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, 4);
	//unbind shader
	_shader.UnUse();

	//swap front and back buffers to show the rendered result
	glutSwapBuffers();
}

void SubdivisionGeometryInstanced::OnShutdown()
{
	//Destroy shader
	_shader.DeleteShaderProgram();

	//Destroy vao and vbo
	glDeleteBuffers(1, &_vboVerticesID);
	glDeleteBuffers(1, &_vboIndicesID);
	glDeleteVertexArrays(1, &_vaoID);

	cout << "Shutdown successfull" << endl;
}

void SubdivisionGeometryInstanced::OnResize(int w, int h)
{
	//set the viewport size
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	//setup the projection matrix
	_P = glm::perspective(45.0f, (GLfloat)w / h, 0.01f, 1000.f);
}

void SubdivisionGeometryInstanced::OnInit()
{
	//set the instance modeling matrix
	_M[0] = glm::translate(glm::mat4(1), glm::vec3(-5, 0, -5));
	_M[1] = glm::translate(_M[0], glm::vec3(10, 0, 0));
	_M[2] = glm::translate(_M[1], glm::vec3(0, 0, 10));
	_M[3] = glm::translate(_M[2], glm::vec3(-10, 0, 0));

	_color[0] = glm::vec4(1.f, 0.f, 0.f, 1.f);
	_color[1] = glm::vec4(0.f, 1.f, 0.f, 1.f);
	_color[2] = glm::vec4(0.f, 0.f, 1.f, 1.f);
	_color[3] = glm::vec4(1.f, 0.f, 1.f, 1.f);

	GL_CHECK_ERRORS;
	//load the shader
	_shader.LoadFromString(GL_VERTEX_SHADER, subdivision_geometry_instanced_shader_vert);
	_shader.LoadFromString(GL_GEOMETRY_SHADER, subdivision_geometry_instanced_shader_geom);
	_shader.LoadFromString(GL_FRAGMENT_SHADER, subdivision_geometry_instanced_shader_frag);
	//create and link shader
	_shader.CreateAndLinkProgram();
	_shader.Use();
	//add attribute and uniform
	_shader.AddAttribute("vVertex");
	_shader.AddUniform("PV");
	_shader.AddUniform("M");
	_shader.AddUniform("color");
	_shader.AddUniform("sub_divisions");
	//set values of constant uniforms at initialization
	glUniform1i(_shader("sub_divisions"), _subDivisions);
	glUniformMatrix4fv(_shader("M"), 4, GL_FALSE, glm::value_ptr(_M[0]));
	glUniform4fv(_shader("color"), 4, glm::value_ptr(_color[0]));
	_shader.UnUse();

	GL_CHECK_ERRORS;

	//setup quad geometry
	//setup quad vertices
	_vertices[0] = glm::vec3(-5, 0, -5);
	_vertices[1] = glm::vec3(-5, 0, 5);
	_vertices[2] = glm::vec3(5, 0, 5);
	_vertices[3] = glm::vec3(5, 0, -5);

	//setup quad indices
	GLushort* id = &_indices[0];
	*id++ = 0;
	*id++ = 1;
	*id++ = 2;

	*id++ = 0;
	*id++ = 2;
	*id++ = 3;

	GL_CHECK_ERRORS;

		//setup quad vao and vbo stuff
	glGenVertexArrays(1, &_vaoID);
	glGenBuffers(1, &_vboVerticesID);
	glGenBuffers(1, &_vboIndicesID);

	glBindVertexArray(_vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, _vboVerticesID);
	//pass the quad vertices to buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), &_vertices[0], GL_STATIC_DRAW);
	GL_CHECK_ERRORS;
	//enable vertex attribute array for position
	glEnableVertexAttribArray(_shader["vVertex"]);
	glVertexAttribPointer(_shader["vVertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);
	GL_CHECK_ERRORS;
	//pass the quad indices to element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), &_indices[0], GL_STATIC_DRAW);
	GL_CHECK_ERRORS;

		//set the polygon mode to render lines
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GL_CHECK_ERRORS;

	cout << "Initialization successfull" << endl;
}

SubdivisionGeometryInstanced::SubdivisionGeometryInstanced()
{
	_P = glm::mat4(1);
}

SubdivisionGeometryInstanced::~SubdivisionGeometryInstanced()
{
}

//mosue click handler
void SubdivisionGeometryInstanced::OnMouseDown(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN)
	{
		_oldX = x;
		_oldY = y;
	}

	if (button == GLUT_MIDDLE_BUTTON)
		_state = 0;
	else
		_state = 1;
}

//mosue move handler
void SubdivisionGeometryInstanced::OnMouseMove(int x, int y)
{
	if (_state == 0)
		_dist *= (1 + (y - _oldY) / 60.0f);
	else
	{
		_rY += (x - _oldX) / 5.0f;
		_rX += (y - _oldY) / 5.0f;
	}
	_oldX = x;
	_oldY = y;

	glutPostRedisplay();
}

//key event handler to increase/decrease number of sub-divisions
void SubdivisionGeometryInstanced::OnKey(unsigned char key, int x, int y)
{
	switch (key) 
	{
	case '-':	_subDivisions--; break;
	case '+':	_subDivisions++; break;
	}

	_subDivisions = std::max(1, std::min(8, _subDivisions));

	glutPostRedisplay();
}



} // namespace Samples