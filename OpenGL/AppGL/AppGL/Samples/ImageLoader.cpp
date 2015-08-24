#include "ImageLoader.h"
#include <algorithm>

using namespace std;

namespace Samples
{

const char* image_loader_shader_vert = R"(
#version 330 core
  
layout(location=0) in vec2 vVertex; //object space vertex

//vertex shader output
smooth out vec2 vUV;	//texture coordinates for texture lookup in the fragment shader

void main()
{    
	//output the clipspace position
	gl_Position = vec4(vVertex*2.0-1,0,1);	 

		//set the input object space vertex position as texture coordinate
	vUV = vVertex;
}
)";

const char* image_loader_shader_frag = R"(
#version 330 core
 
layout (location=0) out vec4 vFragColor;	//fragment shader output

//input from the vertex shader
smooth in vec2 vUV;		//2D texture coordinates

//shader uniform
uniform sampler2D textureMap;		//the image to display

void main()
{
	//sample the textureMap at the given 2D texture coodinates to obntain the colour
	vFragColor =   texture(textureMap, vUV);
})";


void ImageLoader::OnRender(void)
{
	//clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//bind the shader
	_shader.Use();
	//render instanced geometry
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, 4);
	//unbind shader
	_shader.UnUse();

	//swap front and back buffers to show the rendered result
	glutSwapBuffers();
}

void ImageLoader::OnShutdown()
{
	//Destroy shader
	_shader.DeleteShaderProgram();

	//Destroy vao and vbo
	glDeleteBuffers(1, &_vboVerticesID);
	glDeleteBuffers(1, &_vboIndicesID);
	glDeleteVertexArrays(1, &_vaoID);

	//Delete textures
	glDeleteTextures(1, &_textureID);

	cout << "Shutdown successfull" << endl;
}

void ImageLoader::OnResize(int w, int h)
{
	//set the viewport size
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void ImageLoader::OnInit()
{
	GL_CHECK_ERRORS;
	//load the shader
	_shader.LoadFromString(GL_VERTEX_SHADER, image_loader_shader_vert);
	_shader.LoadFromString(GL_FRAGMENT_SHADER, image_loader_shader_frag);
	//create and link shader
	_shader.CreateAndLinkProgram();
	_shader.Use();
	//add attributes and uniforms
	_shader.AddAttribute("vVertex");
	_shader.AddUniform("textureMap");
	//pass values of constant uniforms at initialization
	glUniform1i(_shader("textureMap"), 0);
	_shader.UnUse();

	GL_CHECK_ERRORS;

	//setup quad geometry
	//setup quad vertices
	_vertices[0] = glm::vec2(0.0, 0.0);
	_vertices[1] = glm::vec2(1.0, 0.0);
	_vertices[2] = glm::vec2(1.0, 1.0);
	_vertices[3] = glm::vec2(0.0, 1.0);

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
	glVertexAttribPointer(_shader["vVertex"], 2, GL_FLOAT, GL_FALSE, 0, 0);
	GL_CHECK_ERRORS;
	//pass the quad indices to element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), &_indices[0], GL_STATIC_DRAW);
	GL_CHECK_ERRORS;

	//load the image using SOIL
	int texture_width = 0, texture_height = 0, channels = 0;
	GLubyte* pData = SOIL_load_image(_filename.c_str(), &texture_width, &texture_height, &channels, SOIL_LOAD_AUTO);
	if (pData == NULL) {
		cerr << "Cannot load image: " << _filename.c_str() << endl;
		exit(EXIT_FAILURE);
	}
	//vertically flip the image on Y axis since it is inverted
	int i, j;
	for (j = 0; j * 2 < texture_height; ++j)
	{
		int index1 = j * texture_width * channels;
		int index2 = (texture_height - 1 - j) * texture_width * channels;
		for (i = texture_width * channels; i > 0; --i)
		{
			GLubyte temp = pData[index1];
			pData[index1] = pData[index2];
			pData[index2] = temp;
			++index1;
			++index2;
		}
	}

	//setup OpenGL texture and bind to texture unit 0
	glGenTextures(1, &_textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	//set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//allocate texture 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);

	//free SOIL image data
	SOIL_free_image_data(pData);

	GL_CHECK_ERRORS;

	cout << "Initialization successfull" << endl;
}

ImageLoader::ImageLoader()
{
}

ImageLoader::~ImageLoader()
{
}

//mosue click handler
void ImageLoader::OnMouseDown(int button, int s, int x, int y)
{
}

//mosue move handler
void ImageLoader::OnMouseMove(int x, int y)
{
}

//key event handler to increase/decrease number of sub-divisions
void ImageLoader::OnKey(unsigned char key, int x, int y)
{
}



} // namespace Samples