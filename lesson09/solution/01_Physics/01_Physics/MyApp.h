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

#include "ProgramObject.h"
#include "BufferObject.h"
#include "VertexArrayObject.h"
#include "TextureObject.h"

#include "Mesh_OGL3.h"
#include "gCamera.h"

#include <vector>

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
	// shaderekhez szükséges változók
	ProgramObject		m_passthroughProgram;
	ProgramObject		m_axesProgram;
	ProgramObject		m_particleProgram;
	//3.:
	ProgramObject		m_velocityProgram;

	VertexArrayObject	m_vao;				// VAO objektum
	IndexBuffer			m_gpuBufferIndices;	// indexek
	ArrayBuffer			m_gpuBufferPos;		// pozíciók tömbje

	gCamera				m_camera;

	int		m_particleCount = 10;

	std::vector<glm::vec3>		m_particlePos{};
	std::vector<glm::vec3>		m_particleVel{};

	VertexArrayObject	m_gpuParticleVAO;
	ArrayBuffer			m_gpuParticleBuffer;

	//0.:
	float energyRemaining = 1.0;

	//1.:
	float particle_weight = 1.0;
	float gravity = 9.80665;

	//2.:
	float bounce_threshold = 0.1;

	//3.:
	std::vector<glm::vec3>		m_particleVelArrows{};
	VertexArrayObject	m_gpuVelArrowVAO;
	ArrayBuffer			m_gpuVelArrowBuffer;

	//4.:
	void generate_rand(std::vector<glm::vec3> &vec, float mult);
};

