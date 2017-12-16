#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

// mesh
#include "ObjParser_OGL3.h"

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
	// belsõ eljárások
	void DrawGround();
	void DrawMesh();

	// shaderekhez szükséges változók
	GLuint m_programID; // shaderek programja

	// transzformációs mátrixok
	glm::mat4 m_matWorld;
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	// mátrixok helye a shaderekben
	GLuint	m_loc_mvp;
	GLuint	m_loc_texture;

	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object erõforrás azonosító
	GLuint m_vboID; // vertex buffer object erõforrás azonosító
	GLuint m_ibID;  // index buffer object erõforrás azonosító
	GLuint m_waterTextureID; // fájlból betöltött textúra azonosítója

	struct Vertex
	{
		glm::vec3 p; // pozíció
		glm::vec3 c; // szín
		glm::vec2 t; // textúra koordináták
	};

	// mesh adatok
	Mesh *m_mesh;	//KOMMENT: "Mesh" osztály a "./Utils/Mesh_OGL3.h"-ban és "./Utils/Mesh_OGL3.cpp"-ben

	//1.:
	glm::vec3 camera_pos = glm::vec3(10, 10, 10);
	glm::vec3 camera_fw;
	glm::vec3 camera_up = glm::vec3(0, 1, 0);
	//1.a.:
	glm::vec3 toDescartes(float theta, float phi);
	float theta = 3.1415f - 3.1415f / 4.0f;
	float phi = 3.1415f + 3.1415f / 4.0f;
	bool mouse_left_down = false;
	//1.b.:
	float t;
};

