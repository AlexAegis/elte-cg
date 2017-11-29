#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>
#include <algorithm>

Element::Element(glm::vec3 p, glm::vec3 col, double scale, double offsetX, double offsetY, double offsetZ, bool ccw) {
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

void Element::makeIndices(GLushort result[]) {
		for (int i = 0; i < count * 3; i++) {
			result[i] = i;
		}
	}

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
	double a = sqrt(2) / 2; // az egys�g oldalhosszok�rt

						   // t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)
	glCullFace(GL_BACK); // GL_BACK: a kamer�t�l "elfel�" n�z� lapok, GL_FRONT: a kamera fel� n�z� lapok

						 //
						 // geometria letrehozasa
						 //
	col = new glm::vec3[8];
	col[0] = *new glm::vec3(0.0, 1.0, 1.0);
	col[1] = *new glm::vec3(1.0, 0.5, 1.0);
	col[2] = *new glm::vec3(1.0, 0.0, 0.5);
	col[3] = *new glm::vec3(0.0, 0.2, 1.0);
	col[4] = *new glm::vec3(1.0, 0.8, 0.2);
	col[5] = *new glm::vec3(1.0, 0.0, 1.0);
	col[6] = *new glm::vec3(0.5, 0.0, 1.0);
	col[7] = *new glm::vec3(1.0, 1.0, 0.5);

	vert = new Element[8] {
		//                   x, y, z,     RGB, scale, xOff, yOff, zOff, rotation
		{ Element(glm::vec3( 1, 1, 1), col[0],     a,    0,    0,    0, false) },
		{ Element(glm::vec3(-1, 1, 1), col[1],     a,    0,    0,    0,  true) },
		{ Element(glm::vec3( 1,-1, 1), col[2],     a,    0,    0,    0,  true) },
		{ Element(glm::vec3(-1,-1, 1), col[3],     a,    0,    0,    0, false) },
															   
		{ Element(glm::vec3( 1, 1,-1), col[4],     a,    0,    0,    0,  true) },
		{ Element(glm::vec3(-1, 1,-1), col[5],     a,    0,    0,    0, false) },
		{ Element(glm::vec3( 1,-1,-1), col[6],     a,    0,    0,    0, false) },
		{ Element(glm::vec3(-1,-1,-1), col[7],     a,    0,    0,    0,  true) },
	};

	// indexpuffer adatai
	GLushort indices[24];
	Element::makeIndices(indices);

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen gener�lt VAO beallitasa akt�vnak
	glBindVertexArray(m_vaoID);

	// hozzunk l�tre egy �j VBO er�forr�s nevet
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegy�k "akt�vv�" a l�trehozott VBO-t
											// t�lts�k fel adatokkal az akt�v VBO-t

	glBufferData(GL_ARRAY_BUFFER,	// az akt�v VBO-ba t�lts�nk adatokat
		8 * 24 * 3,		// ennyi b�jt nagys�gban
		vert,	// err�l a rendszermem�riabeli c�mr�l olvasva
		GL_STATIC_DRAW);	// �gy, hogy a VBO-nkba nem tervez�nk ezut�n �rni �s minden kirajzol�skor felhasnz�ljuk a benne l�v� adatokat

							// VAO-ban jegyezz�k fel, hogy a VBO-ban az els� 3 float sizeof(Vertex)-enk�nt lesz az els� attrib�tum (poz�ci�)
	glEnableVertexAttribArray(0); // ez lesz majd a poz�ci�
	glVertexAttribPointer(
		0,				// a VB-ben tal�lhat� adatok k�z�l a 0. "index�" attrib�tumait �ll�tjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. index� attrib�tum hol kezd�dik a sizeof(Vertex)-nyi ter�leten bel�l
	);

	// a m�sodik attrib�tumhoz pedig a VBO-ban sizeof(Vertex) ugr�s ut�n sizeof(glm::vec3)-nyit menve �jabb 3 float adatot tal�lunk (sz�n)
	glEnableVertexAttribArray(1); // ez lesz majd a sz�n
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)));

	// index puffer l�trehoz�sa
	glGenBuffers(1, &m_ibID);
	// a VAO �szreveszi, hogy bind-olunk egy index puffert �s feljegyzi, hogy melyik volt ez!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0); // felt�lt�tt�k a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le

											  //
											  // shaderek bet�lt�se
											  //
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER, "myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, "myFrag.frag");

	// a shadereket t�rol� program l�trehoz�sa
	m_programID = glCreateProgram();

	// adjuk hozz� a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attrib�tumok hozz�rendel�se a shader v�ltoz�khoz
	// FONTOS: linkel�s el�tt kell ezt megtenni!
	glBindAttribLocation(m_programID,	// shader azonos�t�ja, amib�l egy v�ltoz�hoz szeretn�nk hozz�rendel�st csin�lni
		0,				// a VAO-beli azonos�t� index
		"vs_in_pos");	// a shader-beli v�ltoz�n�v
	glBindAttribLocation(m_programID, 1, "vs_in_col");
	glBindAttribLocation(m_programID, 2, "vs_in_tex0");

	// illessz�k �ssze a shadereket (kimen�-bemen� v�ltoz�k �sszerendel�se stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result)
	{
		std::vector<char> ProgramErrorMessage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

		char* aSzoveg = new char[ProgramErrorMessage.size()];
		memcpy(aSzoveg, &ProgramErrorMessage[0], ProgramErrorMessage.size());

		std::cout << "[app.Init()] S�der Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	// mar nincs ezekre szukseg
	glDeleteShader(vs_ID);
	glDeleteShader(fs_ID);

	//
	// egy�b inicializ�l�s
	//

	// vet�t�si m�trix l�trehoz�sa
	m_matProj = glm::perspective(45.0f, 640 / 480.0f, 1.0f, 1000.0f);

	// shader-beli transzform�ci�s m�trixok c�m�nek lek�rdez�se
	m_loc_mvp = glGetUniformLocation(m_programID, "MVP");

	//m_loc_texture = glGetUniformLocation( m_programID, "texImage" );

	//
	// egy�b er�forr�sok bet�lt�se
	//

	// text�ra bet�lt�se
	//m_rndTextureID = GenTexture();
	//m_waterTextureID = TextureFromFile("texture.bmp");

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

// Returns a slope value
double slope(double valStart, double valTarget, double timeTarget) {
	return (valStart - valTarget) / timeTarget;
}

double valAtTime(double valStart, double valTarget, double timeTarget, double elapsedTime) {
	return valStart + elapsedTime * -slope(valStart, valTarget, timeTarget);
}

void setElementsColor(Element *element, double r, double g, double b) {
	element->a.c = glm::vec3(r, g, b);
	element->b.c = glm::vec3(r, g, b);
	element->c.c = glm::vec3(r, g, b);
}

void setElementsColorFromColor(Element *element, glm::vec3 colFrom, glm::vec3 colTo, double timeTarget, double elapsedTime) {
	setElementsColor(element,
		valAtTime(colFrom.r, colTo.r, timeTarget, elapsedTime),
		valAtTime(colFrom.g, colTo.g, timeTarget, elapsedTime),
		valAtTime(colFrom.b, colTo.b, timeTarget, elapsedTime));
}

void backAndForth(Element *element, glm::vec3 colFrom, glm::vec3 colTo, double timeTarget, double elapsedTime) {
	if (elapsedTime <= timeTarget / 2) {
		for (int i = 0; i < 8; i++) {
			setElementsColorFromColor(element, colFrom, colTo, timeTarget / 2, elapsedTime);
		}
	} else {
		for (int i = 0; i < 8; i++) {
			setElementsColorFromColor(element, colTo, colFrom, timeTarget / 2, elapsedTime - timeTarget / 2);
		}
	}
}

void CMyApp::Update() {
	double t = SDL_GetTicks() / 1000.0;
	if (rsecs >= 0.00) {
		double elapsedTime = t - rsecs;
		double time = std::min(elapsedTime, timeLimit);
		for (int i = 0; i < 8; i++) {
			backAndForth(&vert[i], col[i], glm::vec3(1.0, 0.0, 0.0), timeLimit, time);
		}
	}
	if (gsecs >= 0.00) {
		double elapsedTime = t - gsecs;
		double time = std::min(elapsedTime, timeLimit);
		for (int i = 0; i < 8; i++) {
			backAndForth(&vert[i], col[i], glm::vec3(0.0, 1.0, 0.0), timeLimit, time);
		}
	}
	
	if (bsecs >= 0.00) {
		double elapsedTime = t - bsecs;
		double time = std::min(elapsedTime, timeLimit);
		for (int i = 0; i < 8; i++) {
			backAndForth(&vert[i], col[i], glm::vec3(0.0, 0.0, 1.0), timeLimit, time);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegy�k "akt�vv�" a l�trehozott VBO-t
											// t�lts�k fel adatokkal az akt�v VBO-t
	glBufferData(GL_ARRAY_BUFFER,	// az akt�v VBO-ba t�lts�nk adatokat
		8 * 24 * 3,		// ennyi b�jt nagys�gban
		vert,	// err�l a rendszermem�riabeli c�mr�l olvasva
		GL_STATIC_DRAW);	// �gy, hogy a VBO-nkba nem tervez�nk ezut�n �rni �s minden kirajzol�skor felhasnz�ljuk a benne l�v� adatokat

	// n�zeti transzform�ci� be�ll�t�sa
	m_matView = glm::lookAt(glm::vec3(12, 12, 12),		// honnan n�zz�k a sz�nteret
		glm::vec3(0, 0, 0),		// a sz�nt�r melyik pontj�t n�zz�k
		glm::vec3(0, 1, 0));		// felfel� mutat� ir�ny a vil�gban
}

void CMyApp::drawOctahedron(int i, int maxI) {

	// shader bekapcsolasa
	glUseProgram(m_programID);

	// shader parameterek be�ll�t�sa

	double secondsFromStart = SDL_GetTicks() / 1000.;
	
	m_matWorld = glm::rotate<float>((2 * M_PI) / 11 * secondsFromStart, glm::vec3(0, 10, 0)) *
		 glm::translate<float>(glm::vec3(10, 0, 0)) *
		 glm::rotate<float>(2 * M_PI * i / float(maxI), glm::vec3(1, 0, 0)) *
		 glm::translate<float>(glm::vec3(0, 0, 5));
	glm::mat4 mvp = m_matProj * m_matView * m_matWorld;

	// majd k�ldj�k �t a megfelel� m�trixot!
	glUniformMatrix4fv(m_loc_mvp,// erre a helyre t�lts�nk �t adatot
		1,			// egy darab m�trixot
		GL_FALSE,	// NEM transzpon�lva
		&(mvp[0][0])); // innen olvasva a 16 x sizeof(float)-nyi adatot

						// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt)
	glBindVertexArray(m_vaoID);

	// kirajzol�s
	glDrawElements(GL_TRIANGLES,		// primit�v t�pus
		42,					// hany csucspontot hasznalunk a kirajzolashoz
		GL_UNSIGNED_SHORT,	// indexek tipusa
		0);					// indexek cime

							// VAO kikapcsolasa
	glBindVertexArray(0);

	// shader kikapcsolasa
	glUseProgram(0);
}

void CMyApp::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	int maxI = 6;
	for (int i = 0; i <= maxI; i++) {
		drawOctahedron(i, maxI);

	}

}

void CMyApp::saveColors() {
	for (int i = 0; i < 8; i++) {
		col[i] = vert[i].a.c;
	}
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key) {
	if (key.keysym.sym == SDLK_1) {
		std::cout << "1 pressed!";
		saveColors();
		rsecs = SDL_GetTicks() / 1000.0;
		gsecs = -1.0;
		bsecs = -1.0;
	}
	if (key.keysym.sym == SDLK_2) {
		std::cout << "2 pressed!";
		saveColors();
		rsecs = -1.0;
		gsecs = SDL_GetTicks() / 1000.0;
		bsecs = -1.0;
	}
	if (key.keysym.sym == SDLK_3) {
		std::cout << "3 pressed!";
		saveColors();
		rsecs = -1.0;
		gsecs = -1.0;
		bsecs = SDL_GetTicks() / 1000.0;
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
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(45.0f,		// 90 fokos nyilasszog
		_w / (float)_h,	// ablakmereteknek megfelelo nezeti arany
		0.01f,			// kozeli vagosik
		100.0f);		// tavoli vagosik
}