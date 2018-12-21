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

	ArrayBuffer			m_gpuBufferNorm;		// kocka normal tömbje
	// shaderekhez szükséges változók
	ProgramObject		m_program_ground;	// talaj programja

	VertexArrayObject	m_vao_ground;				// talaj VAO objektum
	IndexBuffer			m_gpuBufferIndices_ground;	// talaj indexek
	ArrayBuffer			m_gpuBufferPos_ground;		// talaj pozíciók tömbje
	ArrayBuffer			m_gpuBufferNorm_ground;		// talaj normal tömbje
	Texture2D			m_texture_ground;			// talaj textúrája
	ArrayBuffer			m_gpuBufferTex_ground;		// talaj textúrakoordináták tömbje

	ProgramObject		m_program_train;	// mozdony programja

	VertexArrayObject	m_vao_train;				// mozdony VAO objektum
	IndexBuffer			m_gpuBufferIndices_train;	// mozdony indexek
	ArrayBuffer			m_gpuBufferPos_train;		// mozdony pozíciók tömbje
	Texture2D			m_texture_train;			// mozdony textúrája
	ArrayBuffer			m_gpuBufferTex_train;		// mozdony textúrakoordináták tömbje

	ProgramObject		m_program_wheel;	// mozdony kerekének programja
	Texture2D			m_texture_wheel;			// mozdony kerekének textúrája

	int wagon_count = 3;


	// Hó

	ProgramObject		m_particleProgram;
	std::vector<glm::vec3>		m_particlePos{};
	std::vector<glm::vec3>		m_particleVel{};
	VertexArrayObject	m_gpuParticleVAO;
	ArrayBuffer			m_gpuParticleBuffer;
	int		m_particleCount = 100;
	int m_particles_fallen = 0;
	float lap_time = 4;

	ProgramObject		m_program;			// shaderek programja
	ProgramObject		m_programSkybox;	// skybox shaderek

	VertexArrayObject	m_vao;				// VAO objektum
	IndexBuffer			m_gpuBufferIndices;	// indexek
	ArrayBuffer			m_gpuBufferPos;		// pozíciók tömbje

	gCamera				m_camera;

	// nyers OGL azonosítók
	GLuint				m_skyboxTexture;

	bool start = false;


	// Talaj felbontása 20*20as méret 0.2-es felosztás: 20/0.2=100
	static const int ground_res_n = 100;
	static const int ground_res_m = 100;
	static const int ground_vert_size = (ground_res_n + 1)*(ground_res_m + 1);
	static const int ground_indices_size = 3 * 2 * (ground_res_n)*(ground_res_m);
};

