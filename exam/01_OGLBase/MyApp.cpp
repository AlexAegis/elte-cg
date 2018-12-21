#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>

#include <random>


CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}


CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	//
	// shaderek betöltése
	//

	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program_ground.AttachShaders({
		{ GL_VERTEX_SHADER, "ground.vert"},
		{ GL_FRAGMENT_SHADER, "ground.frag"}
	});

	// attributomok osszerendelese a VAO es shader kozt
	m_program_ground.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
			
		{1, "vs_in_col" },				// VAO 1-es csatorna menjen a vs_in_col-ba
			{ 2, "vs_out_ground_tex" },				// VAO 2-es csatorna menjen a vs_out_ground_tex-ba
					{ 3, "vs_in_norm" }				// VAO 3, normal
	});

	m_program_ground.LinkProgram();


	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program_wheel.AttachShaders({
		{ GL_VERTEX_SHADER, "wheel.vert"},
		{ GL_FRAGMENT_SHADER, "wheel.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	m_program_wheel.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{1, "vs_in_col" },				// VAO 1-es csatorna menjen a vs_in_col-ba
		{ 2, "vs_in_norm" }				// VAO 2, normal
		});

	m_program_wheel.LinkProgram();

	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program_train.AttachShaders({
		{ GL_VERTEX_SHADER, "train.vert"},
		{ GL_FRAGMENT_SHADER, "train.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	m_program_train.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{1, "vs_in_col" },				// VAO 1-es csatorna menjen a vs_in_col-ba
		{ 2, "vs_in_norm" }				// VAO 2, normal
	});

	m_program_train.LinkProgram();


	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_col" },				// VAO 1-es csatorna menjen a vs_in_col-ba
		{ 2, "vs_in_norm" }				// VAO 2, normal
		});

	m_program.LinkProgram();

	// shader program rövid létrehozása, egyetlen függvényhívással a fenti három:
	m_programSkybox.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
	{
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
	}
	);
	



	//
	// TALAJ geometria letrehozasa
	//

	// NxM darab négyszöggel közelítjük a parametrikus felületünket => (N+1)x(M+1) pontban kell kiértékelni
	glm::vec3 ground_vert_arr[ground_vert_size];
	glm::vec3 ground_vert_norm[ground_vert_size];
	for (int i = 0; i <= ground_res_n; ++i) {
		for (int j = 0; j <= ground_res_m; ++j)
		{
			float u = i / (float)ground_res_n;
			float v = j / (float)ground_res_m;
			ground_vert_arr[i + j * (ground_res_n + 1)] = glm::vec3(u, 1, v);
			ground_vert_norm[i + j * (ground_res_n + 1)] = glm::normalize(ground_vert_arr[i + j * (ground_res_n + 1)]);
		}
	}
	std::vector<glm::vec3> ground_vert_vec(ground_vert_arr, ground_vert_arr + ground_vert_size);
	std::vector<glm::vec3> ground_vert_norm_vec(ground_vert_norm, ground_vert_norm + ground_vert_size);

	// indexpuffer adatai: NxM négyszög = 2xNxM háromszög = háromszöglista esetén 3x2xNxM index
	int ground_indices_arr[ground_indices_size];
	for (int i = 0; i < ground_res_n; ++i) {
		for (int j = 0; j < ground_res_m; ++j)
		{
			ground_indices_arr[6 * i + j * 3 * 2 * (ground_res_n)+0] = (i)+(j)*	(ground_res_n + 1);
			ground_indices_arr[6 * i + j * 3 * 2 * (ground_res_n)+1] = (i + 1) + (j)*	(ground_res_n + 1);
			ground_indices_arr[6 * i + j * 3 * 2 * (ground_res_n)+2] = (i)+(j + 1)*(ground_res_n + 1);
			ground_indices_arr[6 * i + j * 3 * 2 * (ground_res_n)+3] = (i + 1) + (j)*	(ground_res_n + 1);
			ground_indices_arr[6 * i + j * 3 * 2 * (ground_res_n)+4] = (i + 1) + (j + 1)*(ground_res_n + 1);
			ground_indices_arr[6 * i + j * 3 * 2 * (ground_res_n)+5] = (i)+(j + 1)*(ground_res_n + 1);
		}
	}

	std::vector<int> ground_indices_vec (ground_indices_arr, ground_indices_arr + ground_indices_size);

	m_gpuBufferPos_ground.BufferData(ground_vert_vec);

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	m_gpuBufferIndices_ground.BufferData(ground_indices_vec);

	m_gpuBufferTex_ground.BufferData(
		std::vector<glm::vec2>{
		//        u, v
		glm::vec2(0, 0), // 0-ás csúcspont
			glm::vec2(10, 0), // 1-es
			glm::vec2(0, 10), // 2-es
			glm::vec2(10, 10)	 // 3-as
	}
	);

	m_gpuBufferNorm_ground.BufferData(ground_vert_norm_vec);
	m_texture_wheel.FromFile("wheel.jpg");
	m_texture_ground.FromFile("trail.jpg");
	// geometria VAO-ban való regisztrálása
	m_vao_ground.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_gpuBufferPos GPU pufferben vannak
			{ CreateAttribute<		0,						// csatorna: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás csatorna attribútumainak meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
									sizeof(glm::vec3)		// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_gpuBufferPos_ground },
			{ CreateAttribute<2, glm::vec2, 0, sizeof(glm::vec2)>, m_gpuBufferTex_ground },		// 2-es attribútum "lényegében" glm::vec2-õk sorozata és az adatok az m_gpuBufferTex GPU pufferben vannak
			{ CreateAttribute<3, glm::vec2, 0, sizeof(glm::vec2)>,  m_gpuBufferNorm_ground}
		},
		m_gpuBufferIndices_ground
	);


	//
	// geometria definiálása (std::vector<...>) és GPU pufferekbe (m_buffer*) való feltöltése BufferData-val
	//

	// vertexek pozíciói:
	/*
	Az m_gpuBufferPos konstruktora már létrehozott egy GPU puffer azonosítót és a most következõ BufferData hívás ezt
	1. bind-olni fogja GL_ARRAY_BUFFER target-re (hiszen m_gpuBufferPos típusa ArrayBuffer) és
	2. glBufferData segítségével áttölti a GPU-ra az argumentumban adott tároló értékeit

	*/
	m_gpuBufferPos.BufferData(
		std::vector<glm::vec3>{
		// hátsó lap
		glm::vec3(-1, -1, -1),
			glm::vec3(1, -1, -1),
			glm::vec3(1, 1, -1),
			glm::vec3(-1, 1, -1),
			// elülsõ lap
			glm::vec3(-1, -1, 1),
			glm::vec3(1, -1, 1),
			glm::vec3(1, 1, 1),
			glm::vec3(-1, 1, 1),

	}
	);

	m_gpuBufferNorm.BufferData(
		std::vector<glm::vec3>{
		// hátsó lap
		glm::normalize(glm::vec3(-1, -1, -1)),
			glm::normalize(glm::vec3(1, -1, -1)),
				glm::normalize(glm::vec3(1, 1, -1)),
					glm::normalize(glm::vec3(-1, 1, -1)),
			// elülsõ lap
						glm::normalize(glm::vec3(-1, -1, 1)),
							glm::normalize(glm::vec3(1, -1, 1)),
								glm::normalize(glm::vec3(1, 1, 1)),
									glm::normalize(glm::vec3(-1, 1, 1)),

	}
	);

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	m_gpuBufferIndices.BufferData(
		std::vector<int>{
		// hátsó lap
		0, 1, 2,
			2, 3, 0,
			// elülsõ lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felsõ
			3, 2, 6,
			3, 6, 7,
	}
	);

	// geometria VAO-ban való regisztrálása
	m_vao.Init(
	{
		// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_gpuBufferPos GPU pufferben vannak
		{ CreateAttribute<		0,						// csatorna: 0
								glm::vec3,				// CPU oldali adattípus amit a 0-ás csatorna attribútumainak meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
								0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
								sizeof(glm::vec3)		// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
							>, m_gpuBufferPos },	
			{ CreateAttribute<		1,						// csatorna: 0
								glm::vec3,				// CPU oldali adattípus amit a 0-ás csatorna attribútumainak meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
								0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
								sizeof(glm::vec3)		// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
							>, m_gpuBufferNorm }
	},
	m_gpuBufferIndices
	);

	// kockából indulok ki
	m_vao_train.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_gpuBufferPos GPU pufferben vannak
			{ CreateAttribute<		0,						// csatorna: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás csatorna attribútumainak meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
									sizeof(glm::vec3)		// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_gpuBufferPos },
				{ CreateAttribute<		1,						// csatorna: 0
								glm::vec3,				// CPU oldali adattípus amit a 0-ás csatorna attribútumainak meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
								0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
								sizeof(glm::vec3)		// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
							>, m_gpuBufferNorm }
		},
		m_gpuBufferIndices
	);


	// részecskék inicializálása
	m_particlePos.reserve(m_particleCount);
	m_particleVel.reserve(m_particleCount);

	// véletlenszám generátor inicializálása
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rnd(-1, 1);

	// CPU oldali részecsketömbök feltöltése
	for (int i = 0; i < m_particleCount; ++i)
	{
		m_particlePos.push_back(glm::vec3(rnd(gen), rnd(gen), rnd(gen)));
		m_particleVel.push_back(glm::vec3(2 * rnd(gen), 2 * rnd(gen), 2 * rnd(gen)));
	}

	// GPU-ra áttölteni a részecskék pozícióit
	m_gpuParticleBuffer.BufferData(m_particlePos);	// <=>	m_gpuParticleBuffer = m_particlePos;

	// és végül a VAO-t inicializálni
	m_gpuParticleVAO.Init({
		{CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_gpuParticleBuffer}
		});




	// skybox
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glGenTextures(1, &m_skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	TextureFromFileAttach("xpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	TextureFromFileAttach("xneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	TextureFromFileAttach("ypos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	TextureFromFileAttach("yneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	TextureFromFileAttach("zpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	TextureFromFileAttach("zneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// kamera
	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);

	return true;
}

void CMyApp::TextureFromFileAttach(const char* filename, GLuint role) const
{
	SDL_Surface* loaded_img = IMG_Load(filename);

	int img_mode = 0;

	if (loaded_img == 0)
	{
		std::cout << "[TextureFromFile] Hiba a kép betöltése közben: " << filename << std::endl;
		return;
	}

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	if (loaded_img->format->BytesPerPixel == 4)
		img_mode = GL_BGRA;
	else
		img_mode = GL_BGR;
#else
	if (loaded_img->format->BytesPerPixel == 4)
		img_mode = GL_RGBA;
	else
		img_mode = GL_RGB;
#endif

	glTexImage2D(role, 0, GL_RGBA, loaded_img->w, loaded_img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, loaded_img->pixels);

	SDL_FreeSurface(loaded_img);
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_skyboxTexture);
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;



	// frissítsük a pozíciókat
	static const float energyRemaining = 1;	// tökéletesen rugalmas ütközés
	for (int i = 0; i < m_particleCount; ++i)
	{
		m_particlePos[i] += m_particleVel[i] * delta_time;

		if ((m_particlePos[i].x >= 1 && m_particleVel[i].x > 0) || (m_particlePos[i].x <= -1 && m_particleVel[i].x < 0))
			m_particleVel[i].x *= -energyRemaining;
		if ((m_particlePos[i].y >= 1 && m_particleVel[i].y > 0) || (m_particlePos[i].y <= -1 && m_particleVel[i].y < 0))
			m_particleVel[i].y *= -energyRemaining;
		if ((m_particlePos[i].z >= 1 && m_particleVel[i].z > 0) || (m_particlePos[i].z <= -1 && m_particleVel[i].z < 0))
			m_particleVel[i].z *= -energyRemaining;
	}

	// frissítsük a puffert
	glBindBuffer(GL_ARRAY_BUFFER, m_gpuParticleBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3)*m_particlePos.size(), &(m_particlePos[0][0]));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	if (start) {
		//delta_time = last_time;
	}
	else {
		last_time = SDL_GetTicks();
		delta_time = 0;
	}
	

	// vonat
	// 
	glm::mat4 pos = glm::rotate<float>((delta_time * (20 / lap_time)  )/ ( 3.1415f), glm::vec3(0, 1, 0));

	glm::mat4 wheel_rot = glm::rotate<float>((delta_time * (20 / 2)) / (3.1415f), glm::vec3(0, 0, 1));

	float train_r = 8.5;
	glm::mat4 railpos = glm::translate(glm::vec3(0, -0.5, -train_r));

	m_vao_train.Bind();
	m_program.Use();
	glm::mat4 trainWorld = glm::scale(glm::vec3(-1.5, -0.5, -0.5));	// kifordítjuk, mert egyébként "kívül a belül"
	m_program.SetUniform("MVP", m_camera.GetViewProj() * pos *railpos*trainWorld);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	trainWorld = glm::translate(glm::vec3(1, 0.75, 0)) * glm::scale(glm::vec3(-0.5, -0.25, -0.5));	// kifordítjuk, mert egyébként "kívül a belül"
	m_program.SetUniform("MVP", m_camera.GetViewProj() * pos * railpos *trainWorld);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	// kerekek
	m_program_wheel.Use();
	m_program_wheel.SetTexture("m_texture_wheel", 0, m_texture_wheel);
	m_vao_ground.Bind();

	// mozdony
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j = j + 2) {

			glm::mat4 wheelWorld = glm::translate(glm::vec3(i, -0.5, -0.51 * j)) * glm::scale(glm::vec3(-0.2, -0.2, 0)) * wheel_rot;
			m_program_wheel.SetUniform("MVP", m_camera.GetViewProj()  * pos * railpos *wheelWorld);
			glDrawElements(GL_TRIANGLES, ground_indices_size, GL_UNSIGNED_INT, nullptr);
		}

	}

	for (int v = 1; v < wagon_count + 1; v++) {
		m_vao_train.Bind();
		m_program.Use();
		// fok -(v / 3.1415f) * 1.6
		// vagonok  glm::translate(glm::vec3(4/* * v + 0.2 * v*/, 0.25, 0))     -  * glm::rotate<float>(-v / 3.1415f, glm::vec3(0, 1, 0));
		//glm::mat4 wagonTrans = glm::translate(glm::vec3(cosf(v) / train_r, 0, sinf(v) / train_r)) * glm::translate(glm::vec3(4 * v + 0.2 * v, 0.25, 0));
		glm::mat4 wagonTrans = glm::rotate<float>((-(v / 3.1415f) * 1.7), glm::vec3(0, 1, 0))  * pos * railpos ;
	
		trainWorld = wagonTrans * glm::scale(glm::vec3(-2, -0.75, -0.6));	// kifordítjuk, mert egyébként "kívül a belül"
		m_program.SetUniform("MVP", m_camera.GetViewProj() *trainWorld );
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		m_program_wheel.Use();
		m_program_wheel.SetTexture("m_texture_wheel", 0, m_texture_wheel);
		m_vao_ground.Bind();

		// vagon
		for (int i = -1; i <= 1; i = i + 2) {
			for (int j = -1; j <= 1; j = j + 2) {

				glm::mat4 wheelWorld = glm::translate(glm::vec3(i, -0.75, -0.76 * j)) * glm::scale(glm::vec3(-0.2, -0.2, 0))  * wheel_rot;
				m_program_wheel.SetUniform("MVP", m_camera.GetViewProj()* wagonTrans*wheelWorld);
				glDrawElements(GL_TRIANGLES, ground_indices_size, GL_UNSIGNED_INT, nullptr);
			}

		}


	}
	

	// Talaj:
	m_vao_ground.Bind();

	m_program_ground.Use();
	m_program_ground.SetTexture("m_texture_ground", 0, m_texture_ground);
	glm::mat4 groundWorld = glm::translate(glm::vec3(10, 0, -10)) * glm::scale(glm::vec3(-20, -1, 20));	// kifordítjuk, mert egyébként "kívül a belül"
	m_program.SetUniform("MVP", m_camera.GetViewProj()*groundWorld);
	glDrawElements(GL_TRIANGLES, ground_indices_size, GL_UNSIGNED_INT, nullptr);


	// kocka
	m_vao.Bind();

	m_program.Use();

	// fõ kocka
	/*
	glm::mat4 cubeWorld = glm::scale(glm::vec3(-1,-1,-1));	// kifordítjuk, mert egyébként "kívül a belül"
	m_program.SetUniform("MVP", m_camera.GetViewProj()*cubeWorld);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	*/
	// kicsi kockák
	/*
	for (int i = 0; i < 10; ++i)
	{
		cubeWorld = 
			glm::rotate( SDL_GetTicks()/1000.0f + 2 * glm::pi<float>()/10*i, glm::vec3(0,1,0) )*
			glm::translate(glm::vec3(10 + 5*sinf(SDL_GetTicks()/1000.0f),0,0))*
			glm::rotate( (i+1)*SDL_GetTicks() / 1000.0f, glm::vec3(0, 1, 0))*
			glm::scale(glm::vec3(-0.5, -0.5, -0.5));	// kifordítjuk, mert egyébként "kívül a belül"
		m_program.SetUniform("MVP", m_camera.GetViewProj()*cubeWorld);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	}*/

	glEnable(GL_PROGRAM_POINT_SIZE);
	m_gpuParticleVAO.Bind();
	m_particleProgram.Use();
	m_particleProgram.SetUniform("mvp", m_camera.GetViewProj());

	glDrawArrays(GL_POINTS, 0, m_particleCount);

	glDisable(GL_PROGRAM_POINT_SIZE);


	// skybox

	// mentsük el az elõzõ Z-test eredményt, azaz azt a relációt, ami alapján update-eljük a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenlõt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);

	m_vao.Bind();

	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", m_camera.GetViewProj() * glm::translate( m_camera.GetEye()) );
	
	// cube map textúra beállítása 0-ás mintavételezõre és annak a shaderre beállítása
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);

	// az elõzõ három sor <=> m_programSkybox.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	// végül állítsuk vissza
	glDepthFunc(prevDepthFnc);

	// 1. feladat: készíts egy vertex shader-fragment shader párt, ami tárolt geometria _nélkül_ kirajzol egy tetszõleges pozícióba egy XYZ tengely-hármast,
	//			   ahol az X piros, az Y zöld a Z pedig kék!

	//ImGui Testwindow
	//ImGui::ShowTestWindow();


	ImGui::SetNextWindowPos(ImVec2(300, 400), ImGuiSetCond_FirstUseEver);
	if (ImGui::Begin("Train"))
	{
		ImGui::SliderInt("Wagon Count", &wagon_count, 0, 8);
		ImGui::SliderFloat("Lap time", &lap_time, 3, 20); // 4 kéne alsó határnak de az eredeti feladat 3 at ír.
		ImGui::Text("Particles fallen down: %i", m_particles_fallen);
		ImGui::Checkbox("Start", &start);
	}
	ImGui::End();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
	if (key.keysym.sym == SDLK_SPACE) {
		start = !start;
	}

}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);

}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
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

	m_camera.Resize(_w, _h);
}