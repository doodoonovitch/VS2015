#include "Ripple.h"
#include <iostream>

using namespace std;

const char* ripple_shader_vert = R"(
#version 330 core
 
layout(location=0) in vec3 vVertex;		//object space vertex position

//uniforms
uniform mat4 MVP;		//combined modelview projection matrix
uniform float time;		//elapsed time 

//shader constants
const float amplitude = 0.125;
const float frequency = 4;
const float PI = 3.14159;

void main()
{ 
	//get the Euclidean distance of the current vertex from the center of the mesh
	float distance = length(vVertex);  
	//create a sin function using the distance, multiply frequency and add the elapsed time
	float y = amplitude*sin(-PI*distance*frequency+time);		
	//multiply the MVP matrix with the new position to get the clipspace position
	gl_Position = MVP*vec4(vVertex.x, y, vVertex.z,1);
}
)";

const char* ripple_shader_frag = R"(
#version 330 core

layout(location=0) out vec4 vFragColor;	//fragment shader output

void main()
{
	//output a constant white colour vec4(1,1,1,1)
	vFragColor = vec4(1,1,1,1);
})";


void Ripple::OnRender(void)
{
	//get the elapse time
	_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f * SPEED;

	//clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set teh camera viewing transformation
	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, _dist));
	glm::mat4 Rx = glm::rotate(T, _rX, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 MV = glm::rotate(Rx, _rY, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 MVP = _P * MV;

	//bind the shader
	_shader.Use();
	//pass the shader uniform
	glUniformMatrix4fv(_shader("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform1f(_shader("time"), _time);
	//drwa triangle
	glDrawElements(GL_TRIANGLES, TOTAL_INDICES, GL_UNSIGNED_SHORT, 0);
	//unbind the shader
	_shader.UnUse();

	//swap front and back buffers to show the rendered result
	glutSwapBuffers();
}

void Ripple::OnShutdown()
{
	//Destroy shader
	_shader.DeleteShaderProgram();

	//Destroy vao and vbo
	glDeleteBuffers(1, &_vboVerticesID);
	glDeleteBuffers(1, &_vboIndicesID);
	glDeleteVertexArrays(1, &_vaoID);

	cout << "Shutdown successfull" << endl;
}

void Ripple::OnResize(int w, int h)
{
	//set the viewport size
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	//setup the projection matrix
	_P = glm::perspective(45.0f, (GLfloat)w / h, 1.f, 1000.f);
}

void Ripple::OnInit()
{
	//set the polygon mode to render lines
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GL_CHECK_ERRORS;

	//load the shader
	_shader.LoadFromString(GL_VERTEX_SHADER, ripple_shader_vert);
	_shader.LoadFromString(GL_FRAGMENT_SHADER, ripple_shader_frag);
	//compile and link shader
	_shader.CreateAndLinkProgram();
	_shader.Use();
	//add shader attribute and uniforms
	_shader.AddAttribute("vVertex");
	_shader.AddUniform("MVP");
	_shader.AddUniform("time");
	_shader.UnUse();

	GL_CHECK_ERRORS;

	//setup plane geometry
	//setup plane vertices
	int count = 0;
	int i = 0, j = 0;
	for (j = 0; j <= NUM_Z; j++) 
	{
		for (i = 0; i <= NUM_X; i++) 
		{
			_vertices[count++] = glm::vec3(((float(i) / (NUM_X - 1)) * 2 - 1)* HALF_SIZE_X, 0.f, ((float(j) / (NUM_Z - 1)) * 2 - 1)*HALF_SIZE_Z);
		}
	}

	//fill plane indices array
	GLushort* id = &_indices[0];
	for (i = 0; i < NUM_Z; i++) 
	{
		for (j = 0; j < NUM_X; j++) 
		{
			int i0 = i * (NUM_X + 1) + j;
			int i1 = i0 + 1;
			int i2 = i0 + (NUM_X + 1);
			int i3 = i2 + 1;
			if ((j + i) % 2) 
			{
				*id++ = i0; *id++ = i2; *id++ = i1;
				*id++ = i1; *id++ = i2; *id++ = i3;
			}
			else 
			{
				*id++ = i0; *id++ = i2; *id++ = i3;
				*id++ = i0; *id++ = i3; *id++ = i1;
			}
		}
	}


	GL_CHECK_ERRORS;

	//setup triangle vao and vbo stuff
	glGenVertexArrays(1, &_vaoID);
	glGenBuffers(1, &_vboVerticesID);
	glGenBuffers(1, &_vboIndicesID);

	glBindVertexArray(_vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, _vboVerticesID);
	//pass triangle verteices to buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * TOTAL_VERTICES, &_vertices[0], GL_STATIC_DRAW);
	GL_CHECK_ERRORS;

	//enable vertex attribute array for position
	glEnableVertexAttribArray(_shader["vVertex"]);
	glVertexAttribPointer(_shader["vVertex"], 3, GL_FLOAT, GL_FALSE, 0, 0);
	GL_CHECK_ERRORS;

	//pass indices to element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * TOTAL_INDICES, &_indices[0], GL_STATIC_DRAW);
	GL_CHECK_ERRORS;

	cout << "Initialization successfull" << endl;
}

Ripple::Ripple()
{
	_vertices = new glm::vec3[TOTAL_VERTICES];
	_indices = new GLushort[TOTAL_INDICES];
	_P = glm::mat4(1);
}

Ripple::~Ripple()
{
	delete [] _vertices;
	delete [] _indices;
}

//mosue click handler
void Ripple::OnMouseDown(int button, int s, int x, int y)
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
void Ripple::OnMouseMove(int x, int y)
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
