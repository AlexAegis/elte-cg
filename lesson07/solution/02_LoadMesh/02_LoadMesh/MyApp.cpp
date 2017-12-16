#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_programID = 0;
	m_waterTextureID = 0;

	m_mesh = 0;
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

	Vertex vert[] =
	{ 
		//          x,  y, z               nx,ny,nz			 s, t
		{glm::vec3(-10, 0, -10), glm::vec3( 0, 1, 0), glm::vec2(0, 0)},
		{glm::vec3(-10, 0,  10), glm::vec3( 0, 1, 0), glm::vec2(0, 1)},
		{glm::vec3( 10, 0, -10), glm::vec3( 0, 1, 0), glm::vec2(1, 0)},
		{glm::vec3( 10, 0,  10), glm::vec3( 0, 1, 0), glm::vec2(1, 1)},
	};

	// indexpuffer adatai
    GLushort indices[]=
    {
		// 1. háromszög
        0,1,2,
		// 2. háromszög
        2,1,3,
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

	// textúrakoordináták bekapcsolása a 2-es azonosítójú attribútom csatornán
	glEnableVertexAttribArray(2); 
	glVertexAttribPointer(
		2,
		2, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2*sizeof(glm::vec3)) );

	// index puffer létrehozása
	glGenBuffers(1, &m_ibID);
	// a VAO észreveszi, hogy bind-olunk egy index puffert és feljegyzi, hogy melyik volt ez!
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
	glBindAttribLocation( m_programID, 2, "vs_in_tex0");

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

	m_loc_texture = glGetUniformLocation( m_programID, "texture" );

	//
	// egyéb erõforrások betöltése
	//

	// textúra betöltése
	m_waterTextureID = TextureFromFile("texture.bmp");

	// mesh betoltese
	m_mesh = ObjParser::parse("Suzanne.obj");
	m_mesh->initBuffers();	//KOMMENT: "./Utils/MeshOGL3.cpp"-ben...


	//1.a.:
	theta = glm::clamp(theta, 0.1f, 3.0f);
	camera_fw = toDescartes(theta, phi);
	//1.b.:
	t = SDL_GetTicks();


	return true;
}

void CMyApp::Clean()
{
	delete m_mesh;
	glDeleteTextures(1, &m_waterTextureID);

	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	//1.:
	// nézeti transzformáció beállítása
	m_matView = glm::lookAt(camera_pos,		// honnan nézzük a színteret
							camera_pos + camera_fw,		// a színtér melyik pontját nézzük
							camera_up);		// felfelé mutató irány a világban

	t = SDL_GetTicks();
}


void CMyApp::DrawGround()
{
	// a talaj kirajzolasahoz szukseges shader beallitasa
	glUseProgram( m_programID );

	// shader parameterek beállítása
	/*
	GLM transzformációs mátrixokra példák:
		glm::rotate<float>( szög, glm::vec3(tengely_x, tengely_y, tengely_z) ) <- tengely_{xyz} körüli elforgatás
		glm::translate<float>( glm::vec3(eltol_x, eltol_y, eltol_z) ) <- eltolás
		glm::scale<float>( glm::vec3(s_x, s_y, s_z) ) <- léptékezés
	*/
	m_matWorld = glm::mat4(1.0f);
	glm::mat4 mvp = m_matProj * m_matView * m_matWorld;
	// majd küldjük át a megfelelõ mátrixokat!
	glUniformMatrix4fv( m_loc_mvp,// erre a helyre töltsünk át adatot
						1,			// egy darab mátrixot
						GL_FALSE,	// NEM transzponálva
						&(mvp[0][0]) ); // innen olvasva a 16 x sizeof(float)-nyi adatot
	
	// aktiváljuk a 0-és textúra mintavételezõ egységet
	glActiveTexture(GL_TEXTURE0);
	// aktiváljuk a generált textúránkat 
	glBindTexture(GL_TEXTURE_2D, m_waterTextureID);
	// textúra mintavételezõ és shader-beli sampler2D összerendelése
	glUniform1i(	m_loc_texture,	// ezen azonosítójú sampler 2D
					0);				// olvassa az ezen indexû mintavételezõt

	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
	glBindVertexArray(m_vaoID);

	// kirajzolás
	glDrawElements(	GL_TRIANGLES,		// primitív típus
					6,					// hany csucspontot hasznalunk a kirajzolashoz
					GL_UNSIGNED_SHORT,	// indexek tipusa
					0);					// indexek cime

	// VAO kikapcsolasa
	glBindVertexArray(0);

	// textúra kikapcsolása
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram( 0 );
}

//2.:
void CMyApp::DrawMesh(int &i, int &max_i)
{
	// a mesh kirajzolasahoz hasznalt shader bekapcsolasa
	glUseProgram( m_programID );

	float angle = SDL_GetTicks() / 1000.0f;
	float scale = abs(cos( SDL_GetTicks() / 1000.0 + (i*2*3.1415/max_i) )) + 0.5;
	m_matWorld =
		glm::rotate<float>(angle + 2 * 3.1415 * i / float(max_i), glm::vec3(0, 1, 0))
		*glm::translate<float>(glm::vec3(8, 1, 0))
		*glm::rotate<float>(angle * 3.0, glm::vec3(0, 1, 0))
		*glm::scale<float>(glm::vec3(scale, scale, scale));
	
	glm::mat4 mvp = m_matProj * m_matView * m_matWorld;
	// majd küldjük át a megfelelõ mátrixokat!
	glUniformMatrix4fv( m_loc_mvp,// erre a helyre töltsünk át adatot
						1,			// egy darab mátrixot
						GL_FALSE,	// NEM transzponálva
						&(mvp[0][0]) ); // innen olvasva a 16 x sizeof(float)-nyi adatot

	// aktiváljuk a 0-és textúra mintavételezõ egységet
	glActiveTexture(GL_TEXTURE0);
	// aktiváljuk a generált textúránkat 
	glBindTexture(GL_TEXTURE_2D, m_waterTextureID);
	// textúra mintavételezõ és shader-beli sampler2D összerendelése
	glUniform1i(	m_loc_texture,	// ezen azonosítójú sampler 2D
					0);				// olvassa az ezen indexû mintavételezõt


	m_mesh->draw();

	glUseProgram( 0 );
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//2.:
	//DrawGround();
	int max_i = 7;
	for (int i = 0; i < max_i; ++i) {
		DrawMesh(i,max_i);
	}
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	//1.b.:
	glm::vec3 movement = glm::vec3(0, 0, 0);
	glm::vec3 left_dir = glm::cross(camera_up, camera_fw);

	float lambda = (SDL_GetTicks() - t) / 500.0f;

	if (key.keysym.sym == SDLK_RIGHT) {
		movement -= left_dir * lambda;
	}
	if (key.keysym.sym == SDLK_LEFT) {
		movement += left_dir * lambda;
	}
	if (key.keysym.sym == SDLK_UP) {
		movement += camera_fw * lambda;
	}
	if (key.keysym.sym == SDLK_DOWN) {
		movement -= camera_fw * lambda;
	}

	camera_pos += movement;
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	if (mouse_left_down) {
		theta += mouse.yrel / 100.0f;
		phi+= mouse.xrel / 100.0f;

		theta = glm::clamp(theta, 0.1f, 3.0f);	//KOMMENT: hogy ne tudjunk se felfelé, se lefelé sem átfordulni "bukfencezni"

		camera_fw = toDescartes(theta, phi);
	}
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
	//1.a.:
	if (mouse.button == SDL_BUTTON_LEFT) {
		mouse_left_down = true;
	}
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
	//1.a.:
	if (mouse.button == SDL_BUTTON_LEFT) {
		mouse_left_down = false;
	}
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

glm::vec3 CMyApp::toDescartes(float theta, float phi) {
	return glm::vec3(sinf(theta)*cosf(phi), cosf(theta), sinf(theta)*sinf(phi));
}