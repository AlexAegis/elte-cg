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
	// bels� elj�r�sok
	void DrawGround();
	void DrawMesh();

	// shaderekhez sz�ks�ges v�ltoz�k
	GLuint m_programID; // shaderek programja

	// transzform�ci�s m�trixok
	glm::mat4 m_matWorld;
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	// m�trixok helye a shaderekben
	GLuint	m_loc_mvp;
	GLuint	m_loc_texture;

	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object er�forr�s azonos�t�
	GLuint m_vboID; // vertex buffer object er�forr�s azonos�t�
	GLuint m_ibID;  // index buffer object er�forr�s azonos�t�
	GLuint m_waterTextureID; // f�jlb�l bet�lt�tt text�ra azonos�t�ja

	struct Vertex
	{
		glm::vec3 p; // poz�ci�
		glm::vec3 c; // sz�n
		glm::vec2 t; // text�ra koordin�t�k
	};

	// mesh adatok
	Mesh *m_mesh;	//KOMMENT: "Mesh" oszt�ly a "./Utils/Mesh_OGL3.h"-ban �s "./Utils/Mesh_OGL3.cpp"-ben

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

