#include "MyApp.h"
#include <math.h>
#include <vector>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
}


CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)

	//
	// geometria l�trehoz�sa
	//

	std::vector<Vertex> vertices;

	// H�romsz�gek
	// piros
	vertices.push_back({ glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(0, 1, 0), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(-1, 1, 0), glm::vec3(1, 0, 0) });

	// z�ld
	vertices.push_back({ glm::vec3(0, 1, 0), glm::vec3(0, 1, 0) });
	vertices.push_back({ glm::vec3(1, 0, 0), glm::vec3(0, 1, 0) });
	vertices.push_back({ glm::vec3(1, 1, 0), glm::vec3(0, 1, 0) });

	// k�k
	vertices.push_back({ glm::vec3(1, 0, 0), glm::vec3(0, 0, 1) });
	vertices.push_back({ glm::vec3(0, -1, 0), glm::vec3(0, 0, 1) });
	vertices.push_back({ glm::vec3(1, -1, 0), glm::vec3(0, 0, 1) });

	// feh�r
	vertices.push_back({ glm::vec3(0, -1, 0), glm::vec3(1, 1, 1) });
	vertices.push_back({ glm::vec3(-1, 0, 0), glm::vec3(1, 1, 1) });
	vertices.push_back({ glm::vec3(-1, -1, 0), glm::vec3(1, 1, 1) });

	// K�r

	// K�r k�z�ppontja
	vertices.push_back({ glm::vec3(0, 0, 0), glm::vec3(0, 0.7, 0.7) });

	// 0.5 sugar�, orig� k�z�ppont� k�r�v ment�n elhelyezett pontok:
	// N+1 db (a legels�t meg kell ism�telni)
	for (int i = 0; i < N+1; ++i){
		float a = i * 2.0f * 3.14152f / N;
		float x = 0.5 * cosf(a);
		float y = 0.5 * sinf(a);
		vertices.push_back({ glm::vec3(x, y, 0), glm::vec3(0.7, 0, 0.7) });
	}

	/*
	Vertex vert[] =
	{
	{ glm::vec3(-1,  0,  0), glm::vec3(1, 0, 0) },
	{ glm::vec3( 0,  1,  0), glm::vec3(1, 0, 0) },
	{ glm::vec3(-1,  1,  0), glm::vec3(1, 0, 0) },

	{ glm::vec3( 0,  1,  0), glm::vec3(0, 1, 0) },
	{ glm::vec3( 1,  0,  0), glm::vec3(0, 1, 0) },
	{ glm::vec3( 1,  1,  0), glm::vec3(0, 1, 0) },

	{ glm::vec3( 1,  0,  0), glm::vec3(0, 0, 1) },
	{ glm::vec3( 0, -1,  0), glm::vec3(0, 0, 1) },
	{ glm::vec3( 1, -1,  0), glm::vec3(0, 0, 1) },

	{ glm::vec3( 0, -1,  0), glm::vec3(1, 1, 1) },
	{ glm::vec3(-1,  0,  0), glm::vec3(1, 1, 1) },
	{ glm::vec3(-1, -1,  0), glm::vec3(1, 1, 1) },
	};
	*/

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen gener�lt VAO beallitasa akt�vnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk l�tre egy �j VBO er�forr�s nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegy�k "akt�vv�" a l�trehozott VBO-t
	// t�lts�k fel adatokkal az akt�v VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az akt�v VBO-ba t�lts�nk adatokat
				  vertices.size() * sizeof(Vertex),		// ennyi b�jt nagys�gban
				  &vertices[0],	// err�l a rendszermem�riabeli c�mr�l olvasva
				  GL_STATIC_DRAW);	// �gy, hogy a VBO-nkba nem tervez�nk ezut�n �rni �s minden kirajzol�skor felhasnz�ljuk a benne l�v� adatokat
	

	// VAO-ban jegyezz�k fel, hogy a VBO-ban az els� 3 float sizeof(Vertex)-enk�nt lesz az els� attrib�tum (poz�ci�)
	glEnableVertexAttribArray(0); // ez lesz majd a poz�ci�
	glVertexAttribPointer(
		(GLuint)0,				// a VB-ben tal�lhat� adatok k�z�l a 0. "index�" attrib�tumait �ll�tjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. index� attrib�tum hol kezd�dik a sizeof(Vertex)-nyi ter�leten bel�l
	); 

	// a m�sodik attrib�tumhoz pedig a VBO-ban sizeof(Vertex) ugr�s ut�n sizeof(glm::vec3)-nyit menve �jabb 3 float adatot tal�lunk (sz�n)
	glEnableVertexAttribArray(3); // ez lesz majd a sz�n - de mi�rt 3-as attrib�tum?
	glVertexAttribPointer(
		(GLuint)3,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	glBindVertexArray(0); // felt�lt�tt�k a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);
}

void CMyApp::Update()
{
}


void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt)
	glBindVertexArray(m_vaoID);

	// kirajzol�s
	glDrawArrays(GL_TRIANGLES, 0, 3 * tri_num); // h�romsz�gek: 3, 6, 9, vagy 12 db vertex a 0.-t�l kezdve
	glDrawArrays(GL_TRIANGLE_FAN, 12, N + 2); // k�r: N+2 db vertex (k�z�ppont + N+1 a k�r�ven) a 12.-t�l kezdve

	// VAO kikapcsolasa
	glBindVertexArray(0);
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	if (key.keysym.sym == SDLK_1){
		tri_num = 1;
	}
	if (key.keysym.sym == SDLK_2){
		tri_num = 2;
	}
	if (key.keysym.sym == SDLK_3){
		tri_num = 3;
	}
	if (key.keysym.sym == SDLK_4){
		tri_num = 4;
	}
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

// a k�t param�terbe az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );
}