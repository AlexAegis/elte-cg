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
	Texture2D			m_texture_ground;	// talaj textúrája

	// wheel - cylinder
	ProgramObject		m_program_wheel_cylinder;	// kerék palástjának programja

	// wheel - circle
	ProgramObject		m_program_wheel_circle;		// kerék oldalának programja
	Texture2D			m_texture_wheel;	// kerék textúrája

	// parametrikus felület
	static const int param_res_n = 100;
	static const int param_res_m = 100;
	static const int param_vert_size = (param_res_n + 1)*(param_res_m + 1);
	static const int param_indices_size = 3 * 2 * (param_res_n)*(param_res_m);

	IndexBuffer			m_gpuBufferIndices_param;	// parametrikus indexek
	ArrayBuffer			m_gpuBufferPos_param;		// parametrikus pozíciók tömbje
	ArrayBuffer			m_gpuBufferNorm_param;		// parametrikus normal tömbje
	ArrayBuffer			m_gpuBufferTex_param;		// talaj textúrakoordináták tömbje

	VertexArrayObject	m_vao_param;				// parametrikus VAO objektum

	// segédeljárások
	void TextureFromFileAttach(const char* filename, GLuint role) const;

	// shaderekhez szükséges változók
	ProgramObject		m_program;			// shaderek programja
	ProgramObject		m_programSkybox;	// skybox shaderek

	VertexArrayObject	m_vao;				// VAO objektum
	IndexBuffer			m_gpuBufferIndices;	// indexek
	ArrayBuffer			m_gpuBufferPos;		// pozíciók tömbje
	ArrayBuffer			m_gpuBufferNorm;	// normálisok tömbje


	gCamera				m_camera;

	// nyers OGL azonosítók
	GLuint				m_skyboxTexture;
};

