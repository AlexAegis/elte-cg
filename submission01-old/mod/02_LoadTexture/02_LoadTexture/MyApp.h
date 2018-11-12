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

struct Vertex
{
	glm::vec3 p; // poz�ci�
	glm::vec3 c; // sz�n
};


class Element {
public:
	Vertex a;
	Vertex b;
	Vertex c;
	static int count;
	Element(glm::vec3 p, glm::vec3 col, double scale, double offsetX, double offsetY, double offsetZ, bool ccw);
	static void makeIndices(GLushort result[]);
};
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
	void drawOctahedron(int i, int maxI);

	// shaderekhez sz�ks�ges v�ltoz�k
	GLuint m_programID; // shaderek programja
	Element* vert; // array 8
	// transzform�ci�s m�trixok
	glm::mat4 m_matWorld;
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	glm::vec3* col;  // array 8 

	void saveColors();
	double rsecs = -1.0;
	double gsecs = -1.0;
	double bsecs = -1.0;
	double timeLimit = 9.0;

	// m�trixok helye a shaderekben
	GLuint	m_loc_mvp;
	GLuint	m_loc_texture;

	// OpenGL-es dolgok
	GLuint m_vaoID; // vertex array object er�forr�s azonos�t�
	GLuint m_vboID; // vertex buffer object er�forr�s azonos�t�
	GLuint m_ibID;  // index buffer object er�forr�s azonos�t�
	GLuint m_rndTextureID; // text�ra er�forr�s azonos�t�
	GLuint m_waterTextureID; // f�jlb�l bet�lt�tt text�ra azonos�t�ja

};

