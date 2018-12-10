#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "gCamera.h"

#include "ProgramObject.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "TextureObject.h"

/*
struct Vertex
{
	glm::vec3 p; // poz�ci�
	glm::vec3 c; // sz�n
};


class Element {
public:
	Vertex a;
	Vertex b;
	Vertex c;
	static int count;
	Element(glm::vec3 p, glm::vec3 col, double scale, double offsetX, double offsetY, double offsetZ, bool ccw);
	Element(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 col, double scale, bool ccw);
	static void makeIndices(GLushort result[]);
};

*/
class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);
protected:
	// seg�delj�r�sok
	void TextureFromFileAttach(const char* filename, GLuint role) const;

	// shaderekhez sz�ks�ges v�ltoz�k
	ProgramObject		m_program;			// shaderek programja
	ProgramObject		m_programSkybox;	// skybox shaderek

	VertexArrayObject	m_vao;				// VAO objektum
	IndexBuffer			m_gpuBufferIndices;	// indexek
	ArrayBuffer			m_gpuBufferPos;		// poz�ci�k t�mbje

	gCamera				m_camera;

	// nyers OGL azonos�t�k
	GLuint				m_skyboxTexture;
	/*
	Element* vert; // array 8
	glm::vec3* col;  // Color 
	*/
};

