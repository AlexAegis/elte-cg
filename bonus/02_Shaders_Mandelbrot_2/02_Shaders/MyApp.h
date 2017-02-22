#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>

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
	// shaderekhez sz�ks�ges v�ltoz�k
	GLuint m_programID_float; // shaderek programja
	GLuint m_programID_double; // shaderek programja

	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object er�forr�s azonos�t�
	GLuint m_vboID; // vertex buffer object er�forr�s azonos�t�

	struct Vertex
	{
		glm::vec3 p;

	};

	// float vagy double pontoss�got haszn�ljunk a koordin�t�khoz
	bool precision_is_float = true;

	// a shaderben a Mandelbrot meghat�roz�s�hoz haszn�lt iter�ci�sz�m;
	// v�ltoztathatjuk billenty�k hat�s�ra
	// (rajzol�si pontoss�g VS rajzol�si sebess�g)
	int max_iteration = 500;

	float scale = 1.0f; // a nagy�t�shoz haszn�lt szorz�

	// a k�perny� k�zep�nek koordin�t�ja (hol vagyunk a halmazban)
	double x = 0;
	double y = 0;

	// nyomva tartunk-e egy billenty�t?
	// mozg�s
	bool go_right = false;
	bool go_left = false;
	bool go_up = false;
	bool go_down = false;
	// nagy�t�s
	bool scale_in = false;
	bool scale_out = false;

};

