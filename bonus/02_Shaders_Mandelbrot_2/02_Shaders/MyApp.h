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
	// shaderekhez szükséges változók
	GLuint m_programID_float; // shaderek programja
	GLuint m_programID_double; // shaderek programja

	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object erõforrás azonosító
	GLuint m_vboID; // vertex buffer object erõforrás azonosító

	struct Vertex
	{
		glm::vec3 p;

	};

	// float vagy double pontosságot használjunk a koordinátákhoz
	bool precision_is_float = true;

	// a shaderben a Mandelbrot meghatározásához használt iterációszám;
	// változtathatjuk billentyûk hatására
	// (rajzolási pontosság VS rajzolási sebesség)
	int max_iteration = 500;

	float scale = 1.0f; // a nagyításhoz használt szorzó

	// a képernyõ közepének koordinátája (hol vagyunk a halmazban)
	double x = 0;
	double y = 0;

	// nyomva tartunk-e egy billentyût?
	// mozgás
	bool go_right = false;
	bool go_left = false;
	bool go_up = false;
	bool go_down = false;
	// nagyítás
	bool scale_in = false;
	bool scale_out = false;

};

