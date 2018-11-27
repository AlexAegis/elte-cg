#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>
#include <imgui/imgui.h>

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

Element::Element(glm::vec3 _a, glm::vec3 _b, glm::vec3 _c, glm::vec3 col, double scale, bool ccw) {
	if (ccw == false) {
		a = { glm::vec3(_a.x * scale, _a.y * scale, _a.z * scale), col };
		b = { glm::vec3(_b.x * scale, _b.y * scale, _b.z * scale), col };
		c = { glm::vec3(_c.x * scale, _c.y * scale, _c.z * scale), col };
	}
	else {
		b = { glm::vec3(_a.x * scale, _a.y * scale, _a.z * scale), col };
		a = { glm::vec3(_b.x * scale, _b.y * scale, _b.z * scale), col };
		c = { glm::vec3(_c.x * scale, _c.y * scale, _c.z * scale), col };
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
	m_ibID = 0;

	m_programID = 0;
}


CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" nézõ lapok, GL_FRONT: a kamera felé nézõ lapok

	//
	// geometria letrehozasa
	//
	double a = 5; // az egység oldalhosszokért

	col = new glm::vec3[12];
	col[0] = *new glm::vec3(1.0, 0.0, 0.0);
	col[1] = *new glm::vec3(0.0, 1.0, 0.0);
	col[2] = *new glm::vec3(0.0, 0.0, 1.0);
	col[3] = *new glm::vec3(1.0, 1.0, 0.0);
	col[4] = *new glm::vec3(0.0, 1.0, 1.0);
	col[5] = *new glm::vec3(1.0, 0.0, 1.0);
	col[6] = *new glm::vec3(0.0, 0.0, 0.0);
	col[7] = *new glm::vec3(1.0, 1.0, 1.0);
	col[8] = *new glm::vec3(1.0, 1.0, 0.5);
	col[9] = *new glm::vec3(1.0, 0.5, 1.0);
	col[10] = *new glm::vec3(0.5, 1.0, 1.0);
	col[11] = *new glm::vec3(0.5, 1.0, 0.5);



	vert = new Element[12]{
		//                   x, y, z,     RGB, rotation
		{ Element(glm::vec3(-1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, -1, 1), col[0], 0.5, true) },
		{ Element(glm::vec3(-1, 1, 1), glm::vec3(-1, -1, 1), glm::vec3(1, -1, 1), col[1],  0.5, false) }, // UPPER ROOF - Z FIXED 1

		{ Element(glm::vec3(-1, 1, 1), glm::vec3(-1, -1, 1), glm::vec3(-1, -1, -1), col[2], 0.5,  true) },
		{ Element(glm::vec3(-1, 1, -1), glm::vec3(-1, 1, 1), glm::vec3(-1, -1, -1), col[3], 0.5,  true) }, // LEFT SIDE - X FIXED -1

		{ Element(glm::vec3(-1, -1, 1), glm::vec3(1, -1, 1), glm::vec3(1, -1, -1), col[4], 0.5,  true) },
		{ Element(glm::vec3(-1, -1, 1), glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1), col[5], 0.5,  true) }, // LOWER SIDE - Y FIXED -1

		{ Element(glm::vec3(1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(1, -1, -1), col[6],  0.5, false) },
		{ Element(glm::vec3(1, 1, -1), glm::vec3(1, 1, 1), glm::vec3(1, -1, -1), col[7], 0.5,  false) }, // RIGHT SIDE - X FIXED 1

		{ Element(glm::vec3(-1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, -1), col[8], 0.5,  false) },
		{ Element(glm::vec3(-1, 1, 1), glm::vec3(1, 1, -1), glm::vec3(-1, 1, -1), col[9], 0.5,  false) }, // UPPER SIDE - Y FIXED 1

		{ Element(glm::vec3(-1, 1, -1), glm::vec3(1, 1, -1), glm::vec3(1, -1, -1), col[10], 0.5, false) },
		{ Element(glm::vec3(-1, 1, -1), glm::vec3(-1, -1, -1), glm::vec3(1, -1, -1), col[11], 0.5,  true) } // UPPER ROOF - Z FIXED -1

	};
	// indexpuffer adatai: NxM négyszög = 2xNxM háromszög = háromszöglista esetén 3x2xNxM index
    GLushort indices[3*12];
	Element::makeIndices(indices);

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk létre egy új VBO erõforrás nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
		4096,		// ennyi bájt nagyságban
				  vert,	// errõl a rendszermemóriabeli címrõl olvasva
				  GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévõ adatokat
	

	// VAO-ban jegyezzük fel, hogy a VBO-ban az elsõ 3 float sizeof(Vertex)-enként lesz az elsõ attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		0,				// a VB-ben található adatok közül a 0. "indexû" attribútumait állítjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. indexû attribútum hol kezdõdik a sizeof(Vertex)-nyi területen belül
	); 

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a szín
	glVertexAttribPointer(
		1,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	// index puffer létrehozása
	glGenBuffers(1, &m_ibID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le

	//
	// shaderek betöltése
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket tároló program létrehozása
	m_programID = glCreateProgram();

	// adjuk hozzá a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attribútumok hozzárendelése a shader változókhoz
	// FONTOS: linkelés elõtt kell ezt megtenni!
	glBindAttribLocation(	m_programID,	// shader azonosítója, amibõl egy változóhoz szeretnénk hozzárendelést csinálni
							0,				// a VAO-beli azonosító index
							"vs_in_pos");	// a shader-beli változónév
	glBindAttribLocation( m_programID, 1, "vs_in_col");

	// illesszük össze a shadereket (kimenõ-bemenõ változók összerendelése stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( GL_FALSE == result )
	{
		std::vector<char> ProgramErrorMessage( infoLogLength );
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
		
		char* aSzoveg = new char[ProgramErrorMessage.size()];
		memcpy( aSzoveg, &ProgramErrorMessage[0], ProgramErrorMessage.size());

		std::cout << "[app.Init()] Sáder Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	// mar nincs ezekre szukseg
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	//
	// egyéb inicializálás
	//

	// vetítési mátrix létrehozása
	m_matProj = glm::perspective( 45.0f, 640/480.0f, 1.0f, 1000.0f );

	// shader-beli transzformációs mátrixok címének lekérdezése
	m_loc_mvp = glGetUniformLocation( m_programID, "MVP");

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_ibID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	// nézeti transzformáció beállítása
	float t = SDL_GetTicks()/1000.0f;
	m_matView = glm::lookAt(glm::vec3( 15/**cosf(t)*/, 15 /** sinf(t)*/, 15  ),		// honnan nézzük a színteret
							glm::vec3( 0,  0,  0),		// a színtér melyik pontját nézzük
							glm::vec3( 0,  0,  1));		// felfelé mutató irány a világban
}


void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (int i = 0; i < 5; i++) { // 5 kocka
		for (int j = 0; j < 4; j++) { // 4 helyen

		
			// shader bekapcsolasa
			glUseProgram(m_programID);

			float halfSecondsFromStart = SDL_GetTicks() / 1000.;
			float x = std::fmod(halfSecondsFromStart, 10.);
			x *= 4;
			if (x > 20) {
				x = 40 - x;
			}
			x -= 10;


			glm::vec3 translate;

			switch (i) {
			case 0: //center
				translate = glm::vec3(0, 0, 0);
				break;
			case 1: //up
				translate = glm::vec3(0, 0, 1);
				break;		
			case 2: //right
				translate = glm::vec3(1, 0, 0);
				break;
			case 3: //down
				translate = glm::vec3(0, 0, -1);
				break;
			case 4: //left
				translate = glm::vec3(-1, 0, 0);
				break;
			}

			m_matWorld = glm::mat4(1.0f)
				* glm::translate<float>(glm::vec3(x, 0.05*(std::pow(x, 2)), 0))
				* glm::rotate<float>(2 * M_PI * j / float(4), glm::vec3(0, 1, 0))
				* glm::translate<float>(translate)
				* glm::translate<float>(glm::vec3(0, 0, 5));

			glm::mat4 mvp = m_matProj * m_matView * m_matWorld;

			// majd küldjük át a megfelelõ mátrixot!
			glUniformMatrix4fv(m_loc_mvp,// erre a helyre töltsünk át adatot
				1,			// egy darab mátrixot
				GL_FALSE,	// NEM transzponálva
				&(mvp[0][0])); // innen olvasva a 16 x sizeof(float)-nyi adatot

	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
			glBindVertexArray(m_vaoID);

			// kirajzolás
			glDrawElements(GL_TRIANGLES,		// primitív típus
				4096,		// hany csucspontot hasznalunk a kirajzolashoz
				GL_UNSIGNED_SHORT,	// indexek tipusa
				0);					// indexek cime

	// VAO kikapcsolasa
			glBindVertexArray(0);

			// shader kikapcsolasa
			glUseProgram(0);
		}
	}


	ImGui::ShowTestWindow();
	ImGui::Text("Hello world!");
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

	m_matProj = glm::perspective(  45.0f,		// 90 fokos nyilasszog
									_w/(float)_h,	// ablakmereteknek megfelelo nezeti arany
									0.01f,			// kozeli vagosik
									100.0f);		// tavoli vagosik
}