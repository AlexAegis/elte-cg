#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>

#include "imgui\imgui.h"

#include "ObjParser_OGL3.h"

CMyApp::CMyApp(void)
{
}


CMyApp::~CMyApp(void)
{
	
}

bool CMyApp::Init()
{
	// törlési szín legyen fehér
	glClearColor(0.2f, 0.4f, 0.7f, 1);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	//
	// shaderek betöltése
	//

	// a shadereket tároló program létrehozása
	m_program.Init({
		{ GL_VERTEX_SHADER, "myVert.vert" },
		{ GL_FRAGMENT_SHADER, "myFrag.frag" }
	},
	{
		{ 0, "vs_in_pos" },					// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_normal" },				// VAO 1-es csatorna menjen a vs_in_normal-ba
		{ 2, "vs_out_tex0" },				// VAO 2-es csatorna menjen a vs_in_tex0-ba
	});

	// postprocess shader - a geometria a kódban van
	m_programPostprocess.Init({
		{ GL_VERTEX_SHADER,		"postprocess.vert" },
		{ GL_FRAGMENT_SHADER,	"postprocess.frag" }
	});

	// textúra betöltése
	m_textureMetal.FromFile("texture.png");

	// mesh betöltése
	m_mesh = ObjParser::parse("suzanne.obj");

	// kamera
	m_camera.SetProj(45.0f, 640.0f / 480.0f, near, far);

	// FBO - kezdeti
	CreateFrameBuffer(640, 480);

	return true;
}

void CMyApp::Clean()
{
	if (m_frameBufferCreated)
	{
		glDeleteRenderbuffers(1, &m_depthBuffer);
		glDeleteTextures(1, &m_colorBuffer);
		glDeleteFramebuffers(1, &m_frameBuffer);
	}
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
	/*
	
	1. rendereljünk a framebufferbe
	
	*/

	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_program.Use();

	m_program.SetTexture("texImage", 0, m_textureMetal);
	
	// Suzanne fal
	for (int i=-10; i <= 10; ++i)
		for (int j = -10; j <= 10; ++j)
		{
			glm::mat4 suzanneWorld = glm::translate(glm::vec3(4*i, 4*j, 0));
			m_program.SetUniform("world", suzanneWorld);
			m_program.SetUniform("worldIT", glm::transpose(glm::inverse(suzanneWorld)));	// <- ezt most hogyan egyszerűsíthetnénk?
			m_program.SetUniform("MVP", m_camera.GetViewProj()*suzanneWorld);
			m_program.SetUniform("Kd", glm::vec4(1, 0.3, 0.3, 1));

			m_mesh->draw();
		}

	//1.:
	glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, width, height, 0);
	
	/*

	2. rajzoljunk ki egy teljes képernyőt lefedő quad-ot és feszítsük rá az előbb renderelt képet mint textúrát
	
	*/

	// álljunk vissza a default FBO-ra (=frontbuffer)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_programPostprocess.Use();
	m_programPostprocess.SetTexture("frameTex", 0, m_colorBuffer);
	m_programPostprocess.SetUniform("t", m_filterWeight);

	//1.:
	m_programPostprocess.SetTexture("frameDepthTex", 1, m_depthBuffer);
	m_programPostprocess.SetUniform("near", near);
	m_programPostprocess.SetUniform("far", far);

	//2.:
	m_programPostprocess.SetUniform("width", float(width));
	m_programPostprocess.SetUniform("height", float(height));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//
	// UI
	//
	// A következő parancs megnyit egy ImGui tesztablakot és így látszik mit tud az ImGui.
	ImGui::ShowTestWindow();
	// A ImGui::ShowTestWindow implementációja az imgui_demo.cpp-ben található
	// Érdemes még az imgui.h-t böngészni, valamint az imgui.cpp elején a FAQ-ot elolvasni.
	// Rendes dokumentáció nincs, de a fentiek elegendőek kell legyenek.

	ImGui::SetNextWindowPos(ImVec2(300, 400), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Tesztablak");
	ImGui::Text("...");
	ImGui::SliderFloat("t", &m_filterWeight, 0, 1);
	ImGui::End();
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
	width = _w;
	height = _h;
	
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);

	CreateFrameBuffer(_w, _h);
}

void CMyApp::CreateFrameBuffer(int width, int height)
{
	// takarítsunk, ha nem először hívják ezt a függvényt
	if (m_frameBufferCreated)
	{
		glDeleteRenderbuffers(1, &m_depthBuffer);
		glDeleteTextures(1, &m_colorBuffer);
		glDeleteFramebuffers(1, &m_frameBuffer);
	}

	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	glGenTextures(1, &m_colorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_colorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cout << "Error creating color attachment" << std::endl;		
		exit(1);
	}

	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cout << "Error creating depth attachment" << std::endl;
		exit(1);
	}

	// -- Completeness check
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Incomplete framebuffer (";
		switch (status) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout << "GL_FRAMEBUFFER_UNSUPPORTED";
			break;
		}
		std::cout << ")" << std::endl;
		char ch;
		std::cin >> ch;
		exit(1);
	}

	// -- Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_frameBufferCreated = true;
}