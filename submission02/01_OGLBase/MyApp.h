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

	glm::vec3 GetUV(float u, float v, float d); // u, v, coordinates, d, height of the parametric surface
	static const int N = 20;
	static const int M = 10;
	static const int vertSize = N + M * (N + 1) + 1;
	static const int indicesSize = 6 * N + M * 6 * (N)+5 + 1;
	// nyers OGL azonos�t�k
	GLuint				m_skyboxTexture;
};

