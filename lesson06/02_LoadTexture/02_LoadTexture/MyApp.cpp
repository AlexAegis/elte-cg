#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

struct Vertex
{
	glm::vec3 p; // pozíció
	glm::vec3 c; // szín
};

class Element {
public:
	Vertex a;
	Vertex b;
	Vertex c;
	static int count;
	Element(glm::vec3 p, glm::vec3 col, double scale, double offsetX, double offsetY, double offsetZ, bool ccw) {
		if (ccw == false) {
			a = { glm::vec3(p.x * scale + offsetX,                offsetY,               offsetZ), col };
			b = { glm::vec3(offsetX,                p.y * scale + offsetY,               offsetZ), col };
			c = { glm::vec3(offsetX,                              offsetY, p.z * scale + offsetZ), col };
		}
		else {
			b = { glm::vec3(p.x * scale + offsetX,                offsetY,               offsetZ), col };
			a = { glm::vec3(offsetX,                p.y * scale + offsetY,               offsetZ), col };
			c = { glm::vec3(offsetX,                              offsetY, p.z * scale + offsetZ), col };
		}

		count++;

		std::cout << count << " ";
	}

	static void makeIndices(GLushort result[]) {
		for (int i = 0; i < count * 3; i++) {
			result[i] = i;
		}
	}
};

int Element::count = 0;

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_programID = 0;
	m_rndTextureID = 0;
	m_waterTextureID = 0;
}


CMyApp::~CMyApp(void)
{
}



bool CMyApp::Init()
{
	float a = sqrt(2) / 2; // az egység oldalhosszokért

						   // törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" nézõ lapok, GL_FRONT: a kamera felé nézõ lapok

						 //
						 // geometria letrehozasa
						 //


	std::vector<Element> elements;
	//									    x, y, z               R,   G,   B   scale, xOff, yOff, zOff, rotation
	elements.push_back({ Element(glm::vec3( 1, 1, 1), glm::vec3(0.8, 0.5, 0.3),     a,    0,    1,    0, false) });
	elements.push_back({ Element(glm::vec3(-1, 1, 1), glm::vec3(0.7, 0.1, 0.1),     a,    0,    1,    0,  true) });
	elements.push_back({ Element(glm::vec3( 1,-1, 1), glm::vec3(0.2, 0.6, 0.2),     a,    0,    1,    0,  true) });
	elements.push_back({ Element(glm::vec3(-1,-1, 1), glm::vec3(0.6, 0.9, 0.7),     a,    0,    1,    0, false) });

	elements.push_back({ Element(glm::vec3( 1, 1,-1), glm::vec3(0.9, 0.4, 0.8),     a,    0,    1,    0,  true) });
	elements.push_back({ Element(glm::vec3(-1, 1,-1), glm::vec3(0.3, 0.4, 0.1),     a,    0,    1,    0, false) });
	elements.push_back({ Element(glm::vec3( 1,-1,-1), glm::vec3(0.7, 0.8, 0.9),     a,    0,    1,    0, false) });
	elements.push_back({ Element(glm::vec3(-1,-1,-1), glm::vec3(0.9, 0.2, 0.6),     a,    0,    1,    0,  true) });


	// indexpuffer adatai
	GLushort indices[24];
	Element::makeIndices(indices);
	
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(Element), &elements[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Element), 0);


	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Element), (void*) (sizeof(glm::vec3)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);




	return true;
}

void CMyApp::Clean()
{
	//glDeleteTextures(1, &m_rndTextureID);
	//glDeleteTextures(1, &m_waterTextureID);

	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram(m_programID);
}

void CMyApp::Update()
{
	float t = SDL_GetTicks() / 1000.0f;
	// nézeti transzformáció beállítása
	m_matView = glm::lookAt(glm::vec3(5 * cosf(t), 3, 5 * sinf(t)),		// honnan nézzük a színteret
		glm::vec3(0, 0, 0),		// a színtér melyik pontját nézzük
		glm::vec3(0, 1, 0));		// felfelé mutató irány a világban
}


void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(m_vaoID);
	glDrawArrays(GL_TRIANGLES, 0, 43);
	glBindVertexArray(0);


	// shader kikapcsolasa
	glUseProgram(0);

}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{

}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(45.0f,		// 90 fokos nyilasszog
		_w / (float)_h,	// ablakmereteknek megfelelo nezeti arany
		0.01f,			// kozeli vagosik
		100.0f);		// tavoli vagosik
}