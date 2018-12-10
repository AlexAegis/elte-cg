#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
/*

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
*/
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

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	/*

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
	GLushort indices[3 * 12];
	Element::makeIndices(indices);
	*/

	//
	// shaderek betöltése
	//

	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
	});

	// attributomok osszerendelese a VAO es shader kozt
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_col" }				// VAO 1-es csatorna menjen a vs_in_col-ba
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

	// kocka
	m_vao.Bind();

	m_program.Use();

	float secondsFromStart = SDL_GetTicks() / 1000.;
	float x = std::fmod(secondsFromStart, 10.);
	x *= 4;
	if (x > 20) {
		x = 40 - x;
	}
	x -= 10;


	for (int i = 0; i < 5; i++) { // 5 kocka

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

		for (int j = 0; j < 4; j++) { // 4 helyen

			glm::mat4 cubeWorld =  glm::scale(glm::vec3(-1, -1, -1)) * glm::scale(glm::vec3(0.5, 0.5, 0.5));	// kifordítjuk, mert egyébként "kívül a belül"
			m_program.SetUniform("MVP", m_camera.GetViewProj() * glm::translate<float>(glm::vec3(x, 0.05*(std::pow(x, 2)), 0))
				* glm::rotate<float>(2 * M_PI * j / float(4), glm::vec3(0, 1, 0))
				* glm::translate<float>(translate)
				* glm::translate<float>(glm::vec3(0, 0, 5)) * cubeWorld
			);
			glDrawElements(GL_TRIANGLES, 36 * 40, GL_UNSIGNED_INT, nullptr);

		}
	}

	// kicsi kockák
	/*for (int i = 0; i < 10; ++i)
	{
		cubeWorld = 
			glm::rotate( SDL_GetTicks()/1000.0f + 2 * glm::pi<float>()/10*i, glm::vec3(0,1,0) )*
			glm::translate(glm::vec3(10 + 5*sinf(SDL_GetTicks()/1000.0f),0,0))*
			glm::rotate( (i+1)*SDL_GetTicks() / 1000.0f, glm::vec3(0, 1, 0))*
			glm::scale(glm::vec3(-0.5, -0.5, -0.5));	// kifordítjuk, mert egyébként "kívül a belül"
		m_program.SetUniform("MVP", m_camera.GetViewProj()*cubeWorld);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	}*/

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
	ImGui::ShowTestWindow();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
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