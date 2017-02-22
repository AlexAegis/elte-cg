#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_programID = 0;
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
	glCullFace(GL_BACK); // GL_BACK: a kamer�t�l "elfel�" n�z� lapok, GL_FRONT: a kamera fel� n�z� lapok

	// ha szeretn�nk a h�tlapokat dr�tv�zasan, akkor kommentelj�k ki a
	//      glEnable(GL_CULL_FACE);
	// sort (hogy legyenek h�tlapok),
	// �s adjuk ki az al�bbi utas�t�st (a h�tlapok megjelen�t�s�t vonalasan szeretn�nk)
	//      glPolygonMode(GL_BACK, GL_LINE);

	//
	// geometria letrehozasa
	//

	Vertex vert[] =
	{ 
		// 1. h�romsz�g
		//          x,  y, z             R, G, B
		{glm::vec3(-1, 0, -1), glm::vec3(1, 0, 0)},
		{glm::vec3( 1, 0, -1), glm::vec3(0, 1, 0)},
		{glm::vec3(-1, 0,  1), glm::vec3(0, 0, 1)},

		// 2. h�romsz�g
		{glm::vec3(-1, 0,  1), glm::vec3(0, 0, 1)},
		{glm::vec3( 1, 0, -1), glm::vec3(0, 1, 0)},
		{glm::vec3( 1, 0,  1), glm::vec3(1, 1, 1)},

		// szemk�zti lap
		{ glm::vec3(0, 2, 0),  glm::vec3(1, 0, 0) },
		{ glm::vec3(-1, 0, 1), glm::vec3(1, 0, 0) },
		{ glm::vec3(1, 0, 1),  glm::vec3(1, 0, 0) },

		// jobb oldali lap
		{ glm::vec3(0, 2, 0),  glm::vec3(0, 1, 0) },
		{ glm::vec3(1, 0, 1),  glm::vec3(0, 1, 0) },
		{ glm::vec3(1, 0, -1), glm::vec3(0, 1, 0) },

		// h�tuls� lap
		{ glm::vec3(0, 2, 0),   glm::vec3(0, 0, 1) },
		{ glm::vec3(1, 0, -1),  glm::vec3(0, 0, 1) },
		{ glm::vec3(-1, 0, -1), glm::vec3(0, 0, 1) },

		// bal oldali lap
		{ glm::vec3(0, 2, 0),   glm::vec3(1, 1, 1) },
		{ glm::vec3(-1, 0, -1), glm::vec3(1, 1, 1) },
		{ glm::vec3(-1, 0, 1),  glm::vec3(1, 1, 1) },

	};

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen gener�lt VAO beallitasa akt�vnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk l�tre egy �j VBO er�forr�s nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegy�k "akt�vv�" a l�trehozott VBO-t
	// t�lts�k fel adatokkal az akt�v VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az akt�v VBO-ba t�lts�nk adatokat
				  sizeof(vert),		// ennyi b�jt nagys�gban
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
		(void*)(sizeof(glm::vec3)) );

	glBindVertexArray(0); // felt�lt�tt�k a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le

	//
	// shaderek bet�lt�se
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket t�rol� program l�trehoz�sa
	m_programID = glCreateProgram();

	// adjuk hozz� a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attrib�tumok hozz�rendel�se a shader v�ltoz�khoz
	// FONTOS: linkel�s el�tt kell ezt megtenni!
	glBindAttribLocation(	m_programID,	// shader azonos�t�ja, amib�l egy v�ltoz�hoz szeretn�nk hozz�rendel�st csin�lni
							0,				// a VAO-beli azonos�t� index
							"vs_in_pos");	// a shader-beli v�ltoz�n�v
	glBindAttribLocation( m_programID, 1, "vs_in_col");

	// illessz�k �ssze a shadereket (kimen�-bemen� v�ltoz�k �sszerendel�se stb.)
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

		std::cout << "[app.Init()] S�der Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	// mar nincs ezekre szukseg
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	//
	// egy�b inicializ�l�s
	//

	// vet�t�si m�trix l�trehoz�sa
	m_matProj = glm::perspective( 45.0f, 640/480.0f, 1.0f, 1000.0f );

	// shader-beli transzform�ci�s m�trixok c�m�nek lek�rdez�se
	m_loc_world = glGetUniformLocation( m_programID, "world");
	m_loc_view  = glGetUniformLocation( m_programID, "view" );
	m_loc_proj  = glGetUniformLocation( m_programID, "proj" );

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	// n�zeti transzform�ci� be�ll�t�sa
	m_matView = glm::lookAt(glm::vec3( 0,  5,  8),		// honnan n�zz�k a sz�nteret
							glm::vec3( 0,  0,  0),		// a sz�nt�r melyik pontj�t n�zz�k
							glm::vec3( 0,  1,  0));		// felfel� mutat� ir�ny a vil�gban
}


void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader bekapcsolasa
	glUseProgram( m_programID );

	// shader parameterek be�ll�t�sa
	/*

	GLM transzform�ci�s m�trixokra p�ld�k:
		glm::rotate<float>( sz�g, tengely_x, tengely_y, tengely_z) <- tengely_{xyz} k�r�li elforgat�s
		glm::translate<float>( eltol_x, eltol_y, eltol_z) <- eltol�s
		glm::scale<float>( s_x, s_y, s_z ) <- l�pt�kez�s

	*/

	// **************************************************
	// EGY FORG� PIRAMIS KIRAJZOL�SA K�Z�PEN
	// **************************************************

	// rot legyen olyan �rt�k, hogy 3 mp-k�nt �rje el 360 eg�sz sz�m� t�bbsz�r�s�t
	// rot = SDL_GetTicks();					rot mp-k�nt 1000-rel n�vekszik
	// rot = SDL_GetTicks() / 3000.0f;			rot 3 mp-k�nt 1-gyel n�vekszik (�s nem kerek�t�nk eg�szre!)
	// rot = SDL_GetTicks() / 3000.0f * 360;	rot 3 mp-k�nt 360-gyel n�vekszik (�s nem kerek�t�nk eg�szre!)
	float rot = SDL_GetTicks() / 3000.0f * 360;
		
	// vil�gtranszform�ci�: forgatunk az y tengely k�r�l rot sz�ggel
	m_matWorld = glm::rotate<float>(rot, 0, 1, 0);

	// majd k�ldj�k �t a megfelel� m�trixokat!
	glUniformMatrix4fv( m_loc_world,// erre a helyre t�lts�nk �t adatot
						1,			// egy darab m�trixot
						GL_FALSE,	// NEM transzpon�lva
						&(m_matWorld[0][0]) ); // innen olvasva a 16 x sizeof(float)-nyi adatot
	glUniformMatrix4fv( m_loc_view,  1, GL_FALSE, &( m_matView[0][0]) );
	glUniformMatrix4fv( m_loc_proj,  1, GL_FALSE, &( m_matProj[0][0]) );

	// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt)
	glBindVertexArray(m_vaoID);

	// kirajzol�s
	glDrawArrays(	GL_TRIANGLES,	// rajzoljunk ki h�romsz�glista primit�vet
					0,				// a VB els� eleme legyen az els� kiolvasott vertex
					18);				// �s 6db cs�cspont seg�ts�g�vel rajzoljunk h�romsz�glist�t


	// **************************************************
	// PIRAMIS-CSOPORT KIRAJZOL�SA
	// **************************************************

	// Rajzoljunk ki N darab piramist! Minden piramisnak m�s lesz a vil�gtranszform�ci�ja.
	for (int i = 0; i < N; ++i){

		// Hat�rozzuk meg az �sszetett transzform�ci� elemeit:
		
		// 1. l�p�s: minden piramist zsugor�tsunk fel�re x �s z tengely ment�n, de a magass�ga maradjon.
		glm::mat4 kicsinyit = glm::scale<float>(0.5, 1, 0.5);

		// 2. l�p�s: minden piramis forogjon helyben: z tengely k�r�l tegyen meg egy teljes fordulatot 5 mp alatt.
		glm::mat4 helyben_forog = glm::rotate<float>(SDL_GetTicks() / 5000.0f * 360, 0, 0, 1);

		// 3. l�p�s: a (z tengely k�r�l) forg� piramosokat toljuk el x tengely ment�n 2 egys�ggel.
		glm::mat4 eltol = glm::translate<float>(2, 0, 0);

		// 4. l�p�s: a piramisok most mind a (2, 0, 0) pont k�r�l forognak, helyben (a saj�t z tengely�k k�r�l).
		// Ha most y tengely ment�n m�s �s m�s fokkal forgatjuk el az eltolt piramisokat, k�rbeveszik a k�z�pen �ll� piramist.
		// Ha egyenletesen akarjuk sz�tsz�rni a piramisokat, akkor minden ciklus lefut�sakor �jabb 360/N fokokkal kell forgatnunk.
		glm::mat4 korbeszor = glm::rotate<float>(i * 360.0f / N, 0, 1, 0);

		// Ha mondjuk m�g azt is szeretn�nk, hogy az �gy el��ll�tott k�rgy�r�-szer� forg� alakzat
		// a k�z�pen �ll� nagy piramis cs�cs�nak a magass�g�ban legyen (y = 2 magass�g),
		// akkor vegy�k be a list�ba �t�dik transzform�ci�nak az al�bbit,
		// amivel mindenkit eltolunk 2 egys�ggel az y tengely ment�n.
		//glm::mat4 magasabbra = glm::translate<float>(0, 2, 0);
		// Gyakorl� feladat: ezt a magass�got gombnyom�sra lehessen �ll�tani!

		// j�het a vil�gtranszform�ci�: mivel vektorokra balr�l szorzunk (l�sd: Vertex shader),
		// �gy az �j transzform�ci� mindig az el�tte l�v� bal oldal�n helyezkedik el: 
		// a szorz�s sorrendje a logikai sorrendhez k�pest FORD�TOTT!
		m_matWorld =
			//magasabbra *	// 5. TRANSZFORM�CI�
			korbeszor *		// 4. TRANSZFORM�CI�
			eltol *			// 3. TRANSZFORM�CI�
			helyben_forog * // 2. TRANSZFORM�CI�
			kicsinyit;		// 1. TRANSZFORM�CI�
		// Pr�b�ljuk ki mi t�rt�nik, ha v�ltoztatunk a sorrenden!
		// Gondoljuk v�gig, mi�rt azt l�tjuk a k�perny�n, amit!

		// Az �jrasz�molt vil�gtranszform�ci�t �jra �t kell adni a shadernek!
		glUniformMatrix4fv(m_loc_world,1,GL_FALSE,&(m_matWorld[0][0]));
		// Most m�r rajzolhatunk: egy piramis kirajzol�sa a shadernek �tadott transzform�ci�k szerint.
		glDrawArrays(GL_TRIANGLES,0,18);

	}


	// VAO kikapcsolasa
	glBindVertexArray(0);

	// shader kikapcsolasa
	glUseProgram( 0 );
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	// w gomb hat�s�ra N-t n�velj�k, s gombra cs�kkentj�k,
	// N �rt�ke legal�bb 1 �s legfeljebb 50 lehet
	if (key.keysym.sym == SDLK_w && N < 50){ ++N; }
	if (key.keysym.sym == SDLK_s && N > 1 ){ --N; }
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

	m_matProj = glm::perspective(  45.0f,		// 90 fokos nyilasszog
									_w/(float)_h,	// ablakmereteknek megfelelo nezeti arany
									0.01f,			// kozeli vagosik
									100.0f);		// tavoli vagosik
}