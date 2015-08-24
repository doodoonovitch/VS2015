#include "SubdivisionGeometry.h"
#include <algorithm>

using namespace std;
using namespace Core;


namespace Samples
{




const char* subdivision_geometry_shader_vert = R"(
#version 330 core
  
layout(location=0) in vec3 vVertex;		 //object space vertex position
 
void main()
{    
	//set the object space position 
	gl_Position =  vec4(vVertex, 1);			
}
)";

const char* subdivision_geometry_shader_frag = R"(
#version 330 core
 
layout(location=0) out vec4 vFragColor;	//fragment shader output

void main()
{
	//output a constant white colour vec4(1,1,1,1)
   vFragColor = vec4(1,1,1,1);
}
)";

const char* subdivision_geometry_shader_geom = R"(
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=256) out; 
 
//uniforms
uniform int sub_divisions;		 //the number of subdivisions
uniform mat4 MVP;				 //combined view porjection matrix

void main()
{
	//get the object space vertex positions
	vec4 v0 = gl_in[0].gl_Position;
	vec4 v1 = gl_in[1].gl_Position;
	vec4 v2 = gl_in[2].gl_Position; 

		//determine the size of each sub-division 
	float dx = abs(v0.x-v2.x)/sub_divisions;
	float dz = abs(v0.z-v1.z)/sub_divisions;

		float x=v0.x;
	float z=v0.z;

		//loop through all sub-divisions and emit vertices
	//after mutiplying the object space vertex positions
	//with the combined modelview projection matrix. We 
	//move first in x axis, once we reach the edge, we 
	//reset x to the initial x value while incrementing 
	//the z value.
	for(int j=0;j<sub_divisions*sub_divisions;j++) { 		 
		gl_Position =  MVP * vec4(x,0,z,1);        EmitVertex();		
		gl_Position =  MVP * vec4(x,0,z+dz,1);     EmitVertex();				  
		gl_Position =  MVP * vec4(x+dx,0,z,1);     EmitVertex(); 
		gl_Position =  MVP * vec4(x+dx,0,z+dz,1);  EmitVertex();
		EndPrimitive();	 
		x+=dx;

			if((j+1) %sub_divisions == 0) {
		   x=v0.x;
		   z+=dz;
		}	
	}	
}
)";


void SubdivisionGeometry::OnRender(void)
{
	//clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set teh camera viewing transformation
	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, _dist));
	glm::mat4 Rx = glm::rotate(T, _rX, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 MV = glm::rotate(Rx, _rY, glm::vec3(0.0f, 1.0f, 0.0f));
	MV = glm::translate(MV, glm::vec3(-5, 0, -5));

	//bind the shader
	_shader.Use();
	//set the shader uniforms
	glUniform1i(_shader("sub_divisions"), _subDivisions);
	glUniformMatrix4fv(_shader("MVP"), 1, GL_FALSE, glm::value_ptr(_P * MV));
	//draw the first submesh
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	MV = glm::translate(MV, glm::vec3(10, 0, 0));
	glUniformMatrix4fv(_shader("MVP"), 1, GL_FALSE, glm::value_ptr(_P * MV));
	//draw the second submesh
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	MV = glm::translate(MV, glm::vec3(0, 0, 10));
	glUniformMatrix4fv(_shader("MVP"), 1, GL_FALSE, glm::value_ptr(_P * MV));
	//draw the third submesh
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	MV = glm::translate(MV, glm::vec3(-10, 0, 0));
	glUniformMatrix4fv(_shader("MVP"), 1, GL_FALSE, glm::value_ptr(_P * MV));
	//draw the fourth submesh
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	//unbind the shader
	_shader.UnUse();

	//swap front and back buffers to show the rendered result
	glutSwapBuffers();
}

void SubdivisionGeometry::OnShutdown()
{
	//Destroy shader
	_shader.DeleteShaderProgram();

	//Destroy vao and vbo
	glDeleteBuffers(1, &_vboVerticesID);
	glDeleteBuffers(1, &_vboIndicesID);
	glDeleteVertexArrays(1, &_vaoID);

	cout << "Shutdown successfull" << endl;
}

void SubdivisionGeometry::OnResize(int w, int h)
{
	//set the viewport size
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	//setup the projection matrix
	_P = glm::perspective(45.0f, (GLfloat)w / h, 0.01f, 1000.f);
}

void SubdivisionGeometry::OnInit()
{
	GL_CHECK_ERRORS;
	//load the shader
	_shader.LoadFromString(GL_VERTEX_SHADER, subdivision_geometry_shader_vert);
	_shader.LoadFromString(GL_GEOMETRY_SHADER, subdivision_geometry_shader_geom);
	_shader.LoadFromString(GL_FRAGMENT_SHADER, subdivision_geometry_shader_frag);
	//create and link shader
	_shader.CreateAndLinkProgram();
	_shader.Use();
	//add attribute and uniform
	_shader.AddAttribute("vVertex");
	_shader.AddUniform("MVP");
	_shader.AddUniform("sub_divisions");
	//set values of constant uniforms at initialization
	glUniform1i(_shader("sub_divisions"), _subDivisions);
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

SubdivisionGeometry::SubdivisionGeometry()
{
	_P = glm::mat4(1);
}

SubdivisionGeometry::~SubdivisionGeometry()
{
}

//mosue click handler
void SubdivisionGeometry::OnMouseDown(int button, int s, int x, int y)
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
void SubdivisionGeometry::OnMouseMove(int x, int y)
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
void SubdivisionGeometry::OnKey(unsigned char key, int x, int y) 
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