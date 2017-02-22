#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_programID_float = 0;
	m_programID_double = 0;
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

	//
	// geometria letrehozasa
	//

	Vertex vert[] =
	{ 
		{glm::vec3(-1, -1, 0)},
		{glm::vec3( 1, -1, 0)},
		{glm::vec3(-1,  1, 0)},
		{glm::vec3( 1,  1, 0)},
	};

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk létre egy új VBO erõforrás nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
				  sizeof(vert),		// ennyi bájt nagyságban
				  vert,	// errõl a rendszermemóriabeli címrõl olvasva
				  GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévõ adatokat
	

	// VAO-ban jegyezzük fel, hogy a VBO-ban az elsõ 3 float sizeof(Vertex)-enként lesz az elsõ attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		(GLuint)0,				// a VB-ben található adatok közül a 0. "indexû" attribútumait állítjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. indexû attribútum hol kezdõdik a sizeof(Vertex)-nyi területen belül
	); 

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le

	//
	// shaderek betöltése
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID_float = loadShader(GL_FRAGMENT_SHADER,	"myFrag_float.frag");
	GLuint fs_ID_double = loadShader(GL_FRAGMENT_SHADER, "myFrag_double.frag");

	// a shadereket tároló program létrehozása
	m_programID_float = glCreateProgram();
	m_programID_double = glCreateProgram();

	// adjuk hozzá a programhoz a shadereket
	glAttachShader(m_programID_float, vs_ID);
	glAttachShader(m_programID_float, fs_ID_float);
	glAttachShader(m_programID_double, vs_ID);
	glAttachShader(m_programID_double, fs_ID_double);

	// attributomok osszerendelese a VAO es shader kozt
	glBindAttribLocation( m_programID_float, 0, "vs_in_pos");
	glBindAttribLocation(m_programID_double, 0, "vs_in_pos");

	// illesszük össze a shadereket (kimenõ-bemenõ változók összerendelése stb.)
	glLinkProgram(m_programID_float);
	glLinkProgram(m_programID_double);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID_float, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID_float, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 1)
	{
		std::vector<char> ProgramErrorMessage( infoLogLength );
		glGetProgramInfoLog(m_programID_float, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
		
		char* aSzoveg = new char[ProgramErrorMessage.size()];
		memcpy( aSzoveg, &ProgramErrorMessage[0], ProgramErrorMessage.size());

		std::cout << "[app.Init()] Sáder Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}
	glGetProgramiv(m_programID_double, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID_double, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1)
	{
		std::vector<char> ProgramErrorMessage(infoLogLength);
		glGetProgramInfoLog(m_programID_double, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

		char* aSzoveg = new char[ProgramErrorMessage.size()];
		memcpy(aSzoveg, &ProgramErrorMessage[0], ProgramErrorMessage.size());

		std::cout << "[app.Init()] Sáder Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	// mar nincs ezekre szukseg
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID_float );
	glDeleteShader( fs_ID_double );

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram( m_programID_float );
	glDeleteProgram( m_programID_double );
}

void CMyApp::Update()
{
	if (go_left){
		x -= 0.01f * scale;
	}
	if (go_right){
		x += 0.01f * scale;
	}
	if (go_up){
		y += 0.01f * scale;
	}
	if (go_down){
		y -= 0.01f * scale;
	}
	if (scale_in){
		scale /= 1.03f;
	}
	if (scale_out){
		scale *= 1.03f;
	}
}


void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolasa
	GLuint m_programID = precision_is_float ? m_programID_float : m_programID_double;
	glUseProgram( m_programID );

	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
	glBindVertexArray(m_vaoID);

	// átadjuk a shadernek a számoláshoz használt értékeket
	GLuint loc_z = glGetUniformLocation(m_programID, "zoom");
	GLuint loc_x = glGetUniformLocation(m_programID, "x");
	GLuint loc_y = glGetUniformLocation(m_programID, "y");
	GLuint loc_mit = glGetUniformLocation(m_programID, "max_iteration");

	glUniform1f(loc_z, scale);
	glUniform1d(loc_x, x);
	glUniform1d(loc_y, y);
	glUniform1i(loc_mit, max_iteration);

	// kirajzolás
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// VAO kikapcsolasa
	glBindVertexArray(0);

	// shader kikapcsolasa
	glUseProgram( 0 );
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	if (key.keysym.sym == SDLK_LEFT){
		go_left = true;
	}
	if (key.keysym.sym == SDLK_RIGHT){
		go_right = true;
	}
	if (key.keysym.sym == SDLK_UP){
		go_up = true;
	}
	if (key.keysym.sym == SDLK_DOWN){
		go_down = true;
	}
	if (key.keysym.sym == SDLK_w){
		scale_in = true;
	}
	if (key.keysym.sym == SDLK_s){
		scale_out = true;
	}
	if (key.keysym.sym == SDLK_a){
		precision_is_float = true;
	}
	if (key.keysym.sym == SDLK_d){
		precision_is_float = false;
	}
	if (key.keysym.sym == SDLK_r){
		max_iteration += 10;
	}
	if (key.keysym.sym == SDLK_f){
		max_iteration -= 10;
	}

	// kiírjuk az aktuális értékeket
	std::cout.precision(18);
	std::cout << "Using " << (precision_is_float ? "float " : "double ") << "precision.\n";
	std::cout << "Max iteration number is " << max_iteration << "\n";
	std::cout << "Center value is (" << x << " + " << y << "i)\n";
	std::cout << "Factor for scaling is " << scale << "\n";
	std::cout << "Estimated magnification: " << round(1.0f / scale) << " X\n\n";

}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	if (key.keysym.sym == SDLK_LEFT){
		go_left = false;
	}
	if (key.keysym.sym == SDLK_RIGHT){
		go_right = false;
	}
	if (key.keysym.sym == SDLK_UP){
		go_up = false;
	}
	if (key.keysym.sym == SDLK_DOWN){
		go_down = false;
	}
	if (key.keysym.sym == SDLK_w){
		scale_in = false;
	}
	if (key.keysym.sym == SDLK_s){
		scale_out = false;
	}

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
	glViewport(0, 0, _w, _h );
}