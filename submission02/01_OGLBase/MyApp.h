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
	// segédeljárások
	void TextureFromFileAttach(const char* filename, GLuint role) const;

	// shaderekhez szükséges változók
	ProgramObject		m_program;			// shaderek programja
	ProgramObject		m_programSkybox;	// skybox shaderek

	VertexArrayObject	m_vao;				// VAO objektum
	IndexBuffer			m_gpuBufferIndices;	// indexek
	ArrayBuffer			m_gpuBufferPos;		// pozíciók tömbje

	VertexArrayObject	m_sky_vao;				// VAO objektum
	IndexBuffer			m_sky_gpuBufferIndices;	// indexek
	ArrayBuffer			m_sky_gpuBufferPos;		// pozíciók tömbje

	gCamera				m_camera;

	Texture2D			m_texture_a;
	Texture2D			m_texture_b;

	glm::vec3 GetUV(float u, float v, float d); // u, v, coordinates, d, height of the parametric surface
	static const int N = 600;
	static const int M = 600;
	static const int vertSize = (N + M * (N + 1) + 1);
	static const int indicesSize = (6 * N + M * 6 * (N)+5 + 1);
	// nyers OGL azonosítók
	GLuint				m_skyboxTexture;

	// Imgui stuff
	float m_x_offset = 1.5f;
	float m_y_offset = 1.2f;
	float m_x_scale = 0.4f;
	float m_y_scale = 0.4f;
	float m_treshhold = 2;
	bool m_task = false;
};

