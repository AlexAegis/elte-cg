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
	// ground
	ProgramObject		m_program_ground;	// talaj programja
	Texture2D			m_texture_ground;	// talaj text�r�ja

	// wheel - cylinder
	ProgramObject		m_program_wheel_cylinder;	// ker�k pal�stj�nak programja

	// wheel - circle
	ProgramObject		m_program_wheel_circle;		// ker�k oldal�nak programja
	Texture2D			m_texture_wheel;	// ker�k text�r�ja

	// parametrikus fel�let
	static const int param_res_n = 100;
	static const int param_res_m = 100;
	static const int param_vert_size = (param_res_n + 1)*(param_res_m + 1);
	static const int param_indices_size = 3 * 2 * (param_res_n)*(param_res_m);

	IndexBuffer			m_gpuBufferIndices_param;	// parametrikus indexek
	ArrayBuffer			m_gpuBufferPos_param;		// parametrikus poz�ci�k t�mbje
	ArrayBuffer			m_gpuBufferNorm_param;		// parametrikus normal t�mbje
	ArrayBuffer			m_gpuBufferTex_param;		// talaj text�rakoordin�t�k t�mbje

	VertexArrayObject	m_vao_param;				// parametrikus VAO objektum

	// seg�delj�r�sok
	void TextureFromFileAttach(const char* filename, GLuint role) const;

	// shaderekhez sz�ks�ges v�ltoz�k
	ProgramObject		m_program;			// shaderek programja
	ProgramObject		m_programSkybox;	// skybox shaderek

	VertexArrayObject	m_vao;				// VAO objektum
	IndexBuffer			m_gpuBufferIndices;	// indexek
	ArrayBuffer			m_gpuBufferPos;		// poz�ci�k t�mbje
	ArrayBuffer			m_gpuBufferNorm;	// norm�lisok t�mbje


	gCamera				m_camera;

	// nyers OGL azonos�t�k
	GLuint				m_skyboxTexture;
};

