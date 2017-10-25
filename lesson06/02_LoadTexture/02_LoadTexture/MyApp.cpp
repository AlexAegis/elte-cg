#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

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

GLuint CMyApp::GenTexture()
{
    unsigned char tex[256][256][3];
 
    for (int i=0; i<256; ++i)
        for (int j=0; j<256; ++j)
        {
			tex[i][j][0] = rand()%256;
			tex[i][j][1] = rand()%256;
			tex[i][j][2] = rand()%256;
        }
 
	GLuint tmpID;

	// generáljunk egy textúra erõforrás nevet
    glGenTextures(1, &tmpID);
	// aktiváljuk a most generált nevû textúrát
    glBindTexture(GL_TEXTURE_2D, tmpID);
	// töltsük fel adatokkal
	glTexImage2D(	GL_TEXTURE_2D,		// melyik binding point-on van a textúra erõforrás, amihez tárolást rendelünk
					0,					// melyik részletességi szint adatait határozzuk meg
					GL_RGB,				// textúra belsõ tárolási formátuma (GPU-n)
					256, 256,			// szélesség, magasság
					0,					// nulla kell, hogy legyen ( https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml )
					GL_RGB,				// forrás (=CPU-n) formátuma
					GL_UNSIGNED_BYTE,	// forrás egy pixelének egy csatornáját hogyan tároljuk
					tex);				// forráshoz pointer
	glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// bilineáris szûrés kicsinyítéskor
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// és nagyításkor is

	glBindTexture(GL_TEXTURE_2D, 0);

	return tmpID;
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
		//          x,  y, z             R, G, B			 s, t
		//{glm::vec3(-1, -1, 1), glm::vec3(1, 0, 0), glm::vec2(0, 0)}, // 0
		//{glm::vec3( 1, -1, 1), glm::vec3(0, 1, 0), glm::vec2(1, 0)}, // 1
		//{glm::vec3(-1,  1, 1), glm::vec3(0, 0, 1), glm::vec2(0, 1)}, // 4
		//{glm::vec3( 1,  1, 1), glm::vec3(1, 1, 1), glm::vec2(1, 1)}, // 5


		{ glm::vec3(-1, -1, 1), glm::vec3(1, 0, 0), glm::vec2(0, 0) }, // 0
		{ glm::vec3(1, -1, 1), glm::vec3(0, 1, 0), glm::vec2(1, 0) }, // 1
		{ glm::vec3(1, -1, -1), glm::vec3(1, 1, 1), glm::vec2(0, 1) }, // 2
		{ glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec2(1, 1) }, // 3
		{ glm::vec3(-1,  1, 1), glm::vec3(0, 0, 1), glm::vec2(0, 0) }, // 4
		{ glm::vec3(1,  1, 1), glm::vec3(1, 1, 1), glm::vec2(1, 0) }, // 5
		{ glm::vec3(1,  1, -1), glm::vec3(1, 1, 1), glm::vec2(0, 1) }, // 6
		{ glm::vec3(-1,  1, -1), glm::vec3(1, 1, 1), glm::vec2(0, 1) }, // 7
		{ glm::vec3(0,  2, 0), glm::vec3(1, 1, 1), glm::vec2(1, 1) }, // 8
	};


	// indexpuffer adatai
    GLushort indices[]=
    {
        0,1,2,
        0,2,3,

		0,1,5,
		0,5,4,

		7,3,0,
		4,7,0,

		2,6,1,
		6,5,1,

		7,6,2,
		3,7,2,

		6,8,5,

		5,8,4,

		7,4,8,

		8,6,7,



	


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


	m_loc_texture = glGetUniformLocation( m_programID, "texImage" );

	//
	// egyéb erõforrások betöltése
	//

	// textúra betöltése
	m_rndTextureID = GenTexture();
	m_waterTextureID = TextureFromFile("texture.bmp");

	return true;
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_rndTextureID);
	glDeleteTextures(1, &m_waterTextureID);

	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	float t = SDL_GetTicks() / 1000.0f;
		// nézeti transzformáció beállítása
	m_matView = glm::lookAt(glm::vec3( 5*cosf(t),  3,  5*sinf(t)),		// honnan nézzük a színteret
							glm::vec3( 0,  0,  0),		// a színtér melyik pontját nézzük
							glm::vec3( 0,  1,  0));		// felfelé mutató irány a világban
}


void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolasa
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

	// majd küldjük át a megfelelõ mátrixot!
	glUniformMatrix4fv( m_loc_mvp,// erre a helyre töltsünk át adatot
						1,			// egy darab mátrixot
						GL_FALSE,	// NEM transzponálva
						&(mvp[0][0]) ); // innen olvasva a 16 x sizeof(float)-nyi adatot
	
	// aktiváljuk a 0-és textúra mintavételezõ egységet
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_waterTextureID);
	// aktiváljuk a generált textúránkat 
	// textúra mintavételezõ és shader-beli sampler2D összerendelése
	glUniform1i(	m_loc_texture,	// ezen azonosítójú sampler 2D
					0);				// olvassa az ezen indexû mintavételezõt

	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
	glBindVertexArray(m_vaoID);

	// kirajzolás
	glDrawElements(	GL_TRIANGLES,		// primitív típus
					42,					// hany csucspontot hasznalunk a kirajzolashoz
					GL_UNSIGNED_SHORT,	// indexek tipusa
					0);					// indexek cime

	// VAO kikapcsolasa
	glBindVertexArray(0);

	// textúra kikapcsolása
	glBindTexture(GL_TEXTURE_2D, 0);

	// shader kikapcsolasa
	glUseProgram( 0 );

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