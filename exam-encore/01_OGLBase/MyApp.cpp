#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>

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

	// glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	//
	// shaderek betöltése
	//


	// parametrikusok
	// ground
	m_program_ground.Init(
		{
			{ GL_VERTEX_SHADER, "ground.vert"},
			{ GL_FRAGMENT_SHADER, "ground.frag"}
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
			{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
			{ 2, "vs_in_tex" }				// VAO 2-es csatorna menjen a vs_in_tex-ba
		}
	);

	m_texture_ground.FromFile("trail.jpg");

	// wheel
	m_texture_wheel.FromFile("wheel.jpg");

	// wheel cylinder

	m_program_wheel_cylinder.Init(
		{
			{ GL_VERTEX_SHADER, "wheel_cylinder.vert"},
			{ GL_FRAGMENT_SHADER, "wheel_cylinder.frag"}
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
			{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
			{ 2, "vs_in_tex" }				// VAO 2-es csatorna menjen a vs_in_tex-ba
		}
	);

	// wheel circle

	m_program_wheel_circle.Init(
		{
			{ GL_VERTEX_SHADER, "wheel_circle.vert"},
			{ GL_FRAGMENT_SHADER, "wheel_circle.frag"}
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
			{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
			{ 2, "vs_in_tex" }				// VAO 2-es csatorna menjen a vs_in_tex-ba
		}
	);
	

	// kockák
	m_program.Init(
		{
			{ GL_VERTEX_SHADER, "myVert.vert"},
			{ GL_FRAGMENT_SHADER, "myFrag.frag"}
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
			{ 1, "vs_in_norm" }				// VAO 1-es csatorna menjen a vs_in_norm-ba
		}
	);


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
	// Parametrikus geometria letrehozasa
	//

	// NxM darab négyszöggel közelítjük a parametrikus felületünket => (N+1)x(M+1) pontban kell kiértékelni
	glm::vec3 param_vert_arr[param_vert_size];
	glm::vec3 param_vert_norm_arr[param_vert_size];
	glm::vec2 param_vert_text_arr[param_vert_size];
	for (int i = 0; i <= param_res_n; ++i) {
		for (int j = 0; j <= param_res_m; ++j)
		{
			float u = i / (float)param_res_n;
			float v = j / (float)param_res_m;
			param_vert_arr[i + j * (param_res_n + 1)] = glm::vec3(u, 0, v);
			param_vert_norm_arr[i + j * (param_res_n + 1)] = glm::normalize(param_vert_arr[i + j * (param_res_n + 1)]);
			param_vert_text_arr[i + j * (param_res_n + 1)] = glm::vec2(u, 1 - v);
		}
	}

	int param_indices_arr[param_indices_size];
	for (int i = 0; i < param_res_n; ++i) {
		for (int j = 0; j < param_res_m; ++j)
		{
			param_indices_arr[6 * i + j * 3 * 2 * (param_res_n)+0] = (i)+(j) * (param_res_n + 1);
			param_indices_arr[6 * i + j * 3 * 2 * (param_res_n)+1] = (i + 1) + (j)*	(param_res_n + 1);
			param_indices_arr[6 * i + j * 3 * 2 * (param_res_n)+2] = (i)+(j + 1)*(param_res_n + 1);
			param_indices_arr[6 * i + j * 3 * 2 * (param_res_n)+3] = (i + 1) + (j)*	(param_res_n + 1);
			param_indices_arr[6 * i + j * 3 * 2 * (param_res_n)+4] = (i + 1) + (j + 1)*(param_res_n + 1);
			param_indices_arr[6 * i + j * 3 * 2 * (param_res_n)+5] = (i)+(j + 1)*(param_res_n + 1);
		}
	}

	// bufferelés az előbb elkészített ponthalmazokból (tömbökből)

	m_gpuBufferPos_param.BufferData(param_vert_arr);
	m_gpuBufferNorm_param.BufferData(param_vert_norm_arr);
	m_gpuBufferIndices_param.BufferData(param_indices_arr);
	m_gpuBufferTex_param.BufferData(param_vert_text_arr);




	// bufferek felhasználása, parametrikus felület VAO-ja. geometria VAO-ban való regisztrálása
	m_vao_param.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_gpuBufferPos GPU pufferben vannak
			{ CreateAttribute<	0,						// csatorna: 0
								glm::vec3,				// CPU oldali adattípus amit a 0-ás csatorna attribútumainak meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
								0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
								sizeof(glm::vec3)		// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_gpuBufferPos_param },
			{ CreateAttribute<	1,						// csatorna: 1
								glm::vec3,				// CPU oldali adattípus amit a 0-ás csatorna attribútumainak meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
								0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
								sizeof(glm::vec3)
								>,  m_gpuBufferNorm_param},
			{ CreateAttribute<	2, 
								glm::vec2, 
								0, 
								sizeof(glm::vec2)
								>, m_gpuBufferTex_param },		// 2-es attribútum "lényegében" glm::vec2-õk sorozata és az adatok az m_gpuBufferTex GPU pufferben vannak
		},
		m_gpuBufferIndices_param
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
			{ CreateAttribute<		1,						// csatorna: 1
									glm::vec3,				// CPU oldali adattípus amit a 0-ás csatorna attribútumainak meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
									sizeof(glm::vec3)		// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_gpuBufferNorm },
		},
		m_gpuBufferIndices
	);

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

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	m_vao_param.Bind();
	m_program_ground.Use();
	glm::mat4 groundWorld = glm::translate(glm::vec3(10, 0, -10)) * glm::scale(glm::vec3(-20, 1, 20));
	m_program_ground.SetUniform("MVP", m_camera.GetViewProj() * groundWorld);
	m_program_ground.SetTexture("m_texture_ground", 0, m_texture_ground);
	glDrawElements(GL_TRIANGLES, param_indices_size * 2, GL_UNSIGNED_INT, nullptr);
	
	
	// mozdony
	// kocka program
	m_vao.Bind();
	m_program.Use();

	glm::mat4 train_raise = glm::translate(glm::vec3(0, 1, 0));
	glm::mat4 train = train_raise * glm::scale(glm::vec3(1.5, 0.5, 0.5)) * glm::scale(glm::vec3(-0.5, -0.5, -0.5));	// kifordítjuk, mert egyébként "kívül a belül" és egység oldalú lesz
	m_program.SetUniform("MVP", m_camera.GetViewProj() * train);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	glm::mat4 train_upper = train_raise * glm::translate(glm::vec3(0.5, 0.5 - (0.25 / 2), 0)) * glm::scale(glm::vec3(.5, 0.25, 0.5)) * glm::scale(glm::vec3(-0.5, -0.5, -0.5));	// kifordítjuk, mert egyébként "kívül a belül" és egység oldalú lesz
	m_program.SetUniform("MVP", m_camera.GetViewProj() * train_upper);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	// kerék - param program
	m_vao_param.Bind();
	
	glm::mat4 wheelWorld = glm::translate(glm::vec3(0, -0.25, -0.05f)) 
		* train_raise 
		* glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0)) 
		* glm::scale(glm::vec3(0.3, 0.1, 0.3));

	for (int wheels_other_side = -1; wheels_other_side <= 1; wheels_other_side += 2) {

		glm::mat4 wheels_shift_other_side = glm::translate(glm::vec3(0, 3 * wheels_other_side, 0 ));

		for (int wheels_side = -1; wheels_side <= 1; wheels_side++) {
			glm::mat4 wheels_shift_single_side = glm::translate(glm::vec3(wheels_side * 2, 0, 0));

			for (int wheelsides = 0; wheelsides < 2; wheelsides++) {
				glm::mat4 second_wheel = glm::translate(glm::vec3(0, wheelsides, 0));
				m_program_wheel_circle.Use();
				m_program_wheel_circle.SetTexture("m_texture_wheel", 0, m_texture_wheel);
				m_program_wheel_circle.SetUniform("MVP", m_camera.GetViewProj() * wheelWorld  * wheels_shift_other_side * wheels_shift_single_side * second_wheel);
				glDrawElements(GL_TRIANGLE_FAN, param_indices_size * 2, GL_UNSIGNED_INT, nullptr);
			}
	
			m_program_wheel_cylinder.Use();
			m_program_wheel_cylinder.SetTexture("m_texture_wheel", 0, m_texture_wheel);
			m_program_wheel_cylinder.SetUniform("MVP", m_camera.GetViewProj() * wheelWorld * wheels_shift_other_side * wheels_shift_single_side );
			glDrawElements(GL_TRIANGLES, param_indices_size * 2, GL_UNSIGNED_INT, nullptr);
		}
	}

	// vagon

	m_vao.Bind();
	m_program.Use();
	glm::mat4 wagon_raise = glm::translate(glm::vec3(0, 0.125, 0));
	glm::mat4 wagon = train_raise * wagon_raise * glm::scale(glm::vec3(2, 0.75, 0.6)) * glm::scale(glm::vec3(-0.5, -0.5, -0.5));	// kifordítjuk, mert egyébként "kívül a belül" és egység oldalú lesz
	m_program.SetUniform("MVP", m_camera.GetViewProj() * glm::translate(glm::vec3(3, 0, 0)) * wagon);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);



	// kicsi kockák
	/*
	for (int i = 0; i < 10; ++i)
	{
		cubeWorld =
			glm::rotate(SDL_GetTicks() / 1000.0f + 2 * glm::pi<float>() / 10 * i, glm::vec3(0, 1, 0))*
			glm::translate(glm::vec3(10 + 5 * sinf(SDL_GetTicks() / 1000.0f), 0, 0))*
			glm::rotate((i + 1)*SDL_GetTicks() / 1000.0f, glm::vec3(0, 1, 0))*
			glm::scale(glm::vec3(-0.5, -0.5, -0.5));	// kifordítjuk, mert egyébként "kívül a belül"
		m_program.SetUniform("MVP", m_camera.GetViewProj()*cubeWorld);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	}
	*/
	// skybox

	// mentsük el az elõzõ Z-test eredményt, azaz azt a relációt, ami alapján update-eljük a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenlõt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);

	m_vao.Bind();

	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", m_camera.GetViewProj() * glm::translate(m_camera.GetEye()));

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
		//ImGui::SliderInt("Wagon Count", &wagon_count, 0, 8);
		//ImGui::SliderFloat("Lap time", &lap_time, 3, 20); // 4 kéne alsó határnak de az eredeti feladat 3 at ír.
		//ImGui::Text("Particles fallen down: %i", m_particles_fallen);
		//ImGui::Checkbox("Start", &start);
	}
	ImGui::End();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
	if (key.keysym.sym == SDLK_SPACE) {
		// space lenyomva
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
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);
}