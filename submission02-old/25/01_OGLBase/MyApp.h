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
	glm::vec3 ParamTor(float u, float v, float d);
	float Random(float min, float max);
	// shaderekhez szükséges változók
	ProgramObject		m_program;			// shaderek programja
	ProgramObject		m_programSkybox;	// skybox shaderek
	//1.:
	ProgramObject		m_programAxes;		// tengelyek shaderei
	//2.:
	ProgramObject		m_programReflection;		// tükrözõdés shaderei

	VertexArrayObject	m_vao;				// VAO objektum
	IndexBuffer			m_gpuBufferIndices;	// indexek
	ArrayBuffer			m_gpuBufferPos;		// pozíciók tömbje

	//2.:
	VertexArrayObject	m_vao_sphere;
	IndexBuffer			m_gpuBufferIndices_sphere;
	ArrayBuffer			m_gpuBufferPos_sphere;
	const int N = 80;
	const int M = 80;	
	gCamera				m_camera;

	// nyers OGL azonosítók
	GLuint				m_skyboxTexture;

};

