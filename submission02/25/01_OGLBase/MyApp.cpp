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
// egy parametrikus fel�let (u,v) param�ter�rt�kekhez tartoz� pontj�nak
// kisz�m�t�s�t v�gz� f�ggv�ny
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
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)

	//
	// shaderek bet�lt�se
	//

	// a shadereket t�rol� program l�trehoz�sa az OpenGL-hez hasonl� m�don:
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
	// geometria defini�l�sa (std::vector<...>) �s GPU pufferekbe (m_buffer*) val� felt�lt�se BufferData-val
	//x

	// geometria VAO-ban val� regisztr�l�sa
	m_vao.Init(
	{
		// 0-�s attrib�tum "l�nyeg�ben" glm::vec3-ak sorozata �s az adatok az m_gpuBufferPos GPU pufferben vannak
		{ CreateAttribute<		0,						// csatorna: 0
								glm::vec3,				// CPU oldali adatt�pus amit a 0-�s csatorna attrib�tumainak meghat�roz�s�ra haszn�ltunk <- az elj�r�s a glm::vec3-b�l kik�vetkezteti, hogy 3 darab float-b�l �ll a 0-�s attrib�tum
								0,						// offset: az attrib�tum t�rol� elej�t�l vett offset-je, byte-ban
								sizeof(glm::vec3)		// stride: a k�vetkez� cs�cspont ezen attrib�tuma h�ny byte-ra van az aktu�list�l
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
	//KOMMENT: itt a text�ra-koordin�t�k nevei: "S", "T" �s "R"
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//KOMMENT: alapb�l "GL_REPEAT"-re lenne �ll�tva
	//KOMMENT: "GL_CLAMP_TO_EDGE" ==> mivel k�t lap tal�lkoz�s�n�l a "pixel" lehet, hogy olyan helyen van, ahol
	//								  nem tudn�nk eld�nteni az interpol�ci� miatt, hogy melyik oldal�hoz tartozik
	//								  a "cube"-nak... (mivel pl. az "x" �s "y" komponensek �rt�ke megegyezhet...)
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
		std::cout << "[TextureFromFile] Hiba a k�p bet�lt�se k�zben: " << filename << std::endl;
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
		role,				// c�l
		0,					// LOD level
		GL_RGBA,			// sz�n form�tum
		loaded_img->w,		// sz�less�g
		loaded_img->h,		// magass�g
		0,					// border --> nulla kell, hogy legyen (dokument�ci�...)
		GL_RGBA,			// a pixelek form�tuma
		GL_UNSIGNED_BYTE,	// pixelek adat t�pusa
		loaded_img->pixels	// honnan olvassuk az adatokat
	);

	SDL_FreeSurface(loaded_img);	//KOMMENT: lefoglalt mem�ria felszabad�t�sa
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
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// kocka
	m_vao.Bind();

	m_program.Use();

	// f� kocka
	glm::mat4 cubeWorld = glm::scale(glm::vec3(-1,-1,-1));	// kiford�tjuk, mert egy�bk�nt "k�v�l a bel�l"
	m_program.SetUniform("MVP", m_camera.GetViewProj()*cubeWorld);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	// kicsi kock�k
	for (int i = 0; i < 10; ++i)
	{
		cubeWorld = 
			glm::rotate( SDL_GetTicks()/1000.0f + 2 * glm::pi<float>()/10*i, glm::vec3(0,1,0) )*
			glm::translate(glm::vec3(10 + 5*sinf(SDL_GetTicks()/1000.0f),0,0))*
			glm::rotate( (i+1)*SDL_GetTicks() / 1000.0f, glm::vec3(0, 1, 0))*
			glm::scale(glm::vec3(-0.5, -0.5, -0.5));	// kiford�tjuk, mert egy�bk�nt "k�v�l a bel�l"
		m_program.SetUniform("MVP", m_camera.GetViewProj()*cubeWorld);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	}

	// skybox

	// ments�k el az el�z� Z-test eredm�nyt, azaz azt a rel�ci�t, ami alapj�n update-elj�k a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);	//KOMMENT: az�rt �gy csin�ljuk, hogy semmit ne takarjon ki a skybox...
	// most kisebb-egyenl�t haszn�ljunk, mert mindent kitolunk a t�voli v�g�s�kokra
	glDepthFunc(GL_LEQUAL);	//KOMMENT: , azaz: ami m�g pont a v�g�s�k t�vols�g�ban van, az is l�tszik
							//					--> pont itt lesz a skybox fel�lete...

	m_vao.Bind();

	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", m_camera.GetViewProj() * glm::translate( m_camera.GetEye()) );	//KOMMENT: vel�nk egy�tt mozog a skybox
	
	// cube map text�ra be�ll�t�sa 0-�s mintav�telez�re �s annak a shaderre be�ll�t�sa
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);

	// az el�z� h�rom sor <=> m_programSkybox.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	// v�g�l �ll�tsuk vissza
	glDepthFunc(prevDepthFnc);

	// 1. feladat: k�sz�ts egy vertex shader-fragment shader p�rt, ami t�rolt geometria _n�lk�l_ kirajzol egy tetsz�leges poz�ci�ba egy XYZ tengely-h�rmast,
	//			   ahol az X piros, az Y z�ld a Z pedig k�k!
	m_programAxes.Use();
	m_programAxes.SetUniform("MVP", m_camera.GetViewProj()/* * glm::translate(m_camera.GetEye()) */ );
	glDrawArrays(GL_LINES, 0, 6);

	// 2. feladat: rajzoljunk ki egy g�mb�t a k�z�ps� kocka f�l�, amin a skybox t�kr�z�dik
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

// a k�t param�terbe az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}