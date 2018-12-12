#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}


CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}

float CMyApp::Random(float min, float max) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}

//
// egy parametrikus felület (u,v) paraméterértékekhez tartozó pontjának
// kiszámítását végzõ függvény
//P(u, v, d) = (1 + d)*[(R + r * cos(2pi*u))*cos(2pi*v), r*sin(2pi*u), (R + r * cos(2pi*u))*sin(2pi*v)]
//
glm::vec3 CMyApp::ParamTor(float u, float v, float d)
{
	// P(u, v, d) = (1 + d)*[(R + r * cos(2pi*u))*cos(2pi*v), r*sin(2pi*u), (R + r * cos(2pi*u))*sin(2pi*v)]
	float pi = 3.14159265359f;
	u *= 2 * pi;
	//P(u, v, d) = (1 + d)*[(R + r * cos(u))*cos(2pi*v), r*sin(u), (R + r * cos(u))*sin(2pi*v)]
	v *= 2 * pi;
	//P(u, v, d) = (1 + d)*[(R + r * cos(u))*cos(v), r*sin(u), (R + r * cos(u))*sin(v)]
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	//P(u, v, d) = (1 + d)*[(R + r * cu)*cv, r*su, (R + r * cu)*sv]
	float R = 2;
	float r = 1;

	return (1.0f + d) * glm::vec3((R + r * cu) * cv, r * su, (R + r * cu) * sv);
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

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

	//
	// geometria definiálása (std::vector<...>) és GPU pufferekbe (m_buffer*) való feltöltése BufferData-val
	//x

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

	//2.:  P(u,v,d) = (1+d)*[ (R+r*cos(2pi*u))*cos(2pi*v), r*sin(2pi*u), (R+r*cos(2pi*u))*sin(2pi*v)]
	std::vector<glm::vec3> bufferPos_sphere((N + 1)*(M + 1));
	for (int i = 0; i <= N; ++i) {
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;
			float d = Random(-0.05f, 0.05f);
			bufferPos_sphere[i + j * (N + 1)] = ParamTor(u, v, d);
		}
	}

	std::vector<int> bufferIndices_sphere(3 * 2 * N*M);
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j)
		{
			bufferIndices_sphere[6 * i + j * 3 * 2 * (N)+0] = (i)     + (j)     * (N + 1);
			bufferIndices_sphere[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j)     * (N + 1);
			bufferIndices_sphere[6 * i + j * 3 * 2 * (N)+2] = (i)     + (j + 1) * (N + 1);
			bufferIndices_sphere[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j)     * (N + 1);
			bufferIndices_sphere[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1) * (N + 1);
			bufferIndices_sphere[6 * i + j * 3 * 2 * (N)+5] = (i)     + (j + 1) * (N + 1);
		}
	}

	m_gpuBufferPos_sphere.BufferData( bufferPos_sphere );
	m_gpuBufferIndices_sphere.BufferData( bufferIndices_sphere );

	m_vao_sphere.Init(
	{
		{ CreateAttribute< 0, glm::vec3, 0, sizeof(glm::vec3) >, m_gpuBufferPos_sphere },
	},
		m_gpuBufferIndices_sphere
	);

	// skybox
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glGenTextures(1, &m_skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//KOMMENT: itt a textúra-koordináták nevei: "S", "T" és "R"
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//KOMMENT: alapból "GL_REPEAT"-re lenne állítva
	//KOMMENT: "GL_CLAMP_TO_EDGE" ==> mivel két lap találkozásánál a "pixel" lehet, hogy olyan helyen van, ahol
	//								  nem tudnánk eldönteni az interpoláció miatt, hogy melyik oldalához tartozik
	//								  a "cube"-nak... (mivel pl. az "x" és "y" komponensek értéke megegyezhet...)
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

	glTexImage2D(		//KOMMENT:
		role,				// cél
		0,					// LOD level
		GL_RGBA,			// szín formátum
		loaded_img->w,		// szélesség
		loaded_img->h,		// magasság
		0,					// border --> nulla kell, hogy legyen (dokumentáció...)
		GL_RGBA,			// a pixelek formátuma
		GL_UNSIGNED_BYTE,	// pixelek adat típusa
		loaded_img->pixels	// honnan olvassuk az adatokat
	);

	SDL_FreeSurface(loaded_img);	//KOMMENT: lefoglalt memória felszabadítása
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

	// fõ kocka
	glm::mat4 cubeWorld = glm::scale(glm::vec3(-1,-1,-1));	// kifordítjuk, mert egyébként "kívül a belül"
	m_program.SetUniform("MVP", m_camera.GetViewProj()*cubeWorld);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	// kicsi kockák
	for (int i = 0; i < 10; ++i)
	{
		cubeWorld = 
			glm::rotate( SDL_GetTicks()/1000.0f + 2 * glm::pi<float>()/10*i, glm::vec3(0,1,0) )*
			glm::translate(glm::vec3(10 + 5*sinf(SDL_GetTicks()/1000.0f),0,0))*
			glm::rotate( (i+1)*SDL_GetTicks() / 1000.0f, glm::vec3(0, 1, 0))*
			glm::scale(glm::vec3(-0.5, -0.5, -0.5));	// kifordítjuk, mert egyébként "kívül a belül"
		m_program.SetUniform("MVP", m_camera.GetViewProj()*cubeWorld);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	}

	// skybox

	// mentsük el az elõzõ Z-test eredményt, azaz azt a relációt, ami alapján update-eljük a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);	//KOMMENT: azért így csináljuk, hogy semmit ne takarjon ki a skybox...
	// most kisebb-egyenlõt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);	//KOMMENT: , azaz: ami még pont a vágósík távolságában van, az is látszik
							//					--> pont itt lesz a skybox felülete...

	m_vao.Bind();

	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", m_camera.GetViewProj() * glm::translate( m_camera.GetEye()) );	//KOMMENT: velünk együtt mozog a skybox
	
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
	m_programAxes.Use();
	m_programAxes.SetUniform("MVP", m_camera.GetViewProj()/* * glm::translate(m_camera.GetEye()) */ );
	glDrawArrays(GL_LINES, 0, 6);

	// 2. feladat: rajzoljunk ki egy gömböt a középsõ kocka fölé, amin a skybox tükrözõdik
	m_vao_sphere.Bind();
	
	m_programReflection.Use();
	
	glm::mat4 sphereWorld = glm::scale(glm::vec3(1, 1, 1));
	m_programReflection.SetUniform("MVP", m_camera.GetViewProj()*sphereWorld);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);

	glm::vec3 camera_pos = m_camera.GetEye();
	glUniform3f(m_programReflection.GetLocation("camera_pos"), camera_pos.x, camera_pos.y, camera_pos.z);

	glDrawElements(GL_TRIANGLES, 3*2*N*M, GL_UNSIGNED_INT, nullptr);
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