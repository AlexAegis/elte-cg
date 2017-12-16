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
	glClearColor(1,1,1, 1);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	//glDepthMask(GL_FALSE); // kikapcsolja a z-pufferbe történő írást - https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glDepthMask.xml

	glLineWidth(4.0f); // a vonalprimitívek vastagsága: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glLineWidth.xhtml
	//glEnable(GL_LINE_SMOOTH);

	// átlátszóság engedélyezése
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // meghatározza, hogy az átlátszó objektum az adott pixelben hogyan módosítsa a korábbi fragmentekből oda lerakott színt: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBlendFunc.xhtml

	// a raszterizált pontprimitívek mérete
	glPointSize(15.0f);

	//
	// shaderek betöltése
	//

	// így is létre lehetne hozni:

	// a shadereket tároló program létrehozása
	m_program.Init({ 
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
	},
	{ 
		{ 0, "vs_in_pos" },					// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_normal" },				// VAO 1-es csatorna menjen a vs_in_normal-ba
		{ 2, "vs_out_tex0" },				// VAO 2-es csatorna menjen a vs_in_tex0-ba
	});

	m_program.LinkProgram();

	// tengelyeket kirajzoló shader rövid inicializálása
	m_axesProgram.Init({
		{GL_VERTEX_SHADER, "axes.vert"},
		{GL_FRAGMENT_SHADER, "axes.frag"}
	});
	// haladóknak, még rövidebb: 
	//m_axesProgram.Init({"axes.vert"_vs, "axes.frag"_fs});

	// kontrollpontokat kirajzoló program
	m_pointProgram.Init({
		{ GL_VERTEX_SHADER, "pointDrawer.vert" },
		{ GL_FRAGMENT_SHADER, "pointDrawer.frag" }
	});

	//3.:
	m_splineProgram.Init({ "pointDrawer.vert"_vs, "pointDrawer.frag"_fs});

	//
	// geometria definiálása (std::vector<...>) és GPU pufferekbe (m_buffer*) való feltöltése BufferData-val
	//

	// vertexek pozíciói:
	/*
	Az m_gpuBufferPos konstruktora már létrehozott egy GPU puffer azonosítót és a most következő BufferData hívás ezt 
		1. bind-olni fogja GL_ARRAY_BUFFER target-re (hiszen m_gpuBufferPos típusa ArrayBuffer) és
		2. glBufferData segítségével áttölti a GPU-ra az argumentumban adott tároló értékeit

	*/
	m_gpuBufferPos.BufferData( 
		std::vector<glm::vec3>{
			//		  X, Y, Z
			glm::vec3(-10,-1, -10),
			glm::vec3(-10,-1,  10), 
			glm::vec3( 10,-1, -10),
			glm::vec3( 10,-1,  10) 
		}
	);

	// normálisai
	m_gpuBufferNormal.BufferData(
		std::vector<glm::vec3>{
			// normal.X,.Y,.Z
			glm::vec3(0, 1, 0), // 0-ás csúcspont
			glm::vec3(0, 1, 0), // 1-es
			glm::vec3(0, 1, 0), // 2-es
			glm::vec3(0, 1, 0)  // 3-as
		}
	);

	// textúrakoordinátái
	m_gpuBufferTex.BufferData(
		std::vector<glm::vec2>{
			//        u, v
			glm::vec2(0, 0), // 0-ás csúcspont
			glm::vec2(1, 0), // 1-es
			glm::vec2(0, 1), // 2-es
			glm::vec2(1, 1)	 // 3-as
		}
	);

	// és a primitíveket alkotó csúcspontok indexei (az előző tömbökből) - triangle list-el való kirajzolásra felkészülve
	m_gpuBufferIndices.BufferData(
		std::vector<int>{
			0,1,2,	// 1. háromszög: 0-1-2 csúcspontokból
			2,1,3	// 2. háromszög: 2-1-3 csúcspontokból
		}
	);

	// geometria VAO-ban való regisztrálása
	m_vao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_gpuBufferPos },	// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_gpuBufferPos GPU pufferben vannak
			{ CreateAttribute<1, glm::vec3, 0, sizeof(glm::vec3)>, m_gpuBufferNormal },	// 1-es attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_gpuBufferNormal GPU pufferben vannak
			{ CreateAttribute<2, glm::vec2, 0, sizeof(glm::vec2)>, m_gpuBufferTex }		// 2-es attribútum "lényegében" glm::vec2-ők sorozata és az adatok az m_gpuBufferTex GPU pufferben vannak
		},
		m_gpuBufferIndices
	);

	// textúra betöltése
	m_textureMetal.FromFile("texture.png");

	// mesh betöltése
	m_mesh = ObjParser::parse("suzanne.obj");

	// kamera
	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();

	//3.:
	const int drawing_point_count = (m_pointName.size() - 3);
	m_spline.resize(drawing_point_count * spline_segment_count + 1);
	std::cout << "size: " << m_spline.size() << "\n";
	for (int i = 0; i < drawing_point_count; ++i) {
		//
		glm::vec3 P0 = m_controlPoints[i];
		glm::vec3 P1 = m_controlPoints[i + 1];
		glm::vec3 P2 = m_controlPoints[i + 2];
		glm::vec3 P3 = m_controlPoints[i + 3];
		//
		for (int j = i*spline_segment_count; j <= (i+1)*spline_segment_count; ++j) {
			//
			if (i != 0 && j == i*spline_segment_count)
				++j;
			//
			float t = (j - i*spline_segment_count) / float(spline_segment_count);
			//std::cout <<"t: "<< t << "\n";
			//
			glm::vec3 a = 2.0f * P1;
			glm::vec3 b = P2 - P0;
			glm::vec3 c = 2.0f * P0 - 5.0f * P1 + 4.0f * P2 - P3;
			glm::vec3 d = -P0 + 3.0f * P1 - 3.0f * P2 + P3;
			glm::vec3 P = 0.5f * (a + (b * t) + (c * t * t) + (d * t * t * t));
			m_spline[j] = P;
			std::cout << j << ":   ( " << P.x << ", " << P.y << ", " << P.z << " )\n";
		}
		std::cout << "\n";
	}
	//
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// tengelyek
	m_axesProgram.Use();
	m_axesProgram.SetUniform("mvp", m_camera.GetViewProj()*glm::translate(glm::vec3(0,0.1f,0)));

	glDrawArrays(GL_LINES, 0, 6);

	// kontrollpontok
	m_pointProgram.Use();
	m_pointProgram.SetUniform("mvp", m_camera.GetViewProj());
	m_pointProgram.SetUniform("points", m_controlPoints);
	m_pointProgram.SetUniform("color", glm::vec4(0.5, 0.5, 0.5, 1));
	glDrawArrays(GL_POINTS, 0, (GLsizei)m_controlPoints.size());
	// kontrollpontokat összekötő szakaszok
	glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)m_controlPoints.size());


	//3.:
	m_splineProgram.Use();
	m_splineProgram.SetUniform("mvp", m_camera.GetViewProj());
	m_splineProgram.SetUniform("points", m_spline);
	m_pointProgram.SetUniform("color", glm::vec4(1, 0, 1, 1));
	//glDrawArrays(GL_POINTS, 0, (GLsizei)m_spline.size());
	glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)m_spline.size());
	//FONTOS:
	//		a "pointDrawer.vert" fájlban a "uniform vec3 points[1000];"-ben az 1000 miatt legfeljebb 1000 vertexet tudunk kirajzolni!


	// mindegyik geometria ugyanazt a shader programot hasznalja: ne kapcsolgassuk most ki-be
	m_program.Use();

	m_program.SetTexture("texImage", 0, m_textureMetal);

	// talaj
	glm::mat4 wallWorld = glm::translate(glm::vec3(0, 0, 0));
	m_program.SetUniform("world", wallWorld);
	m_program.SetUniform("worldIT", glm::transpose(glm::inverse(wallWorld)));
	m_program.SetUniform("MVP", m_camera.GetViewProj()*wallWorld);
	m_program.SetUniform("Kd", glm::vec4(1,1,1, 1));

	m_vao.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	m_vao.Unbind(); // nem feltétlen szükséges: a m_mesh->draw beállítja a saját VAO-ját

	// Suzanne 1
	//1.:
	// a UI-ban nincs megcsinálva, hogy a "Delete" esetén a jó helyre álljon az "m_currentParam"
	if (m_currentParam > m_pointName.size()-1) {
		m_currentParam = m_pointName.size()-1;
	}
	//
	glm::vec3 suzanne_from;
	glm::vec3 suzanne_to;
	if (m_currentParam == 0) {
		suzanne_from = m_controlPoints[0];
		suzanne_to = m_controlPoints[1];
	}
	else {
		int i = ceil(m_currentParam);
		suzanne_from = m_controlPoints[i-1];
		suzanne_to = m_controlPoints[i];
	}
	//
	glm::vec3 suzanne_dir = suzanne_to - suzanne_from;
	//
	// a "(0,0,0),(x,0,z),(x,y,z)" háromszög a síkba vetítve: "(0,0),(a,0),(a,b)"
	float a = sqrt(pow(suzanne_dir.x, 2) + pow(suzanne_dir.z, 2));
	float b = suzanne_dir.y;
	float angleXY = atan2(b, a);
	//
	float angleXZ = -atan2(suzanne_dir.z, suzanne_dir.x);
	//
	glm::mat4 suzanne1World =
		glm::translate( Eval(m_currentParam) )*
		glm::rotate(angleXZ, glm::vec3(0,1,0))*		// forgatás a vízszintes mentén
		glm::rotate(angleXY, glm::vec3(0,0,1))*		// forgatás a függőleges mentén
		glm::rotate(3.1415f / 2.0f, glm::vec3(0,1,0));	// alapból rossz irányultságú a suzanne fej...

	//
	m_program.SetUniform("world", suzanne1World);
	m_program.SetUniform("worldIT", glm::transpose(glm::inverse(suzanne1World)));
	m_program.SetUniform("MVP", m_camera.GetViewProj()*suzanne1World);
	m_program.SetUniform("Kd", glm::vec4(1, 0.3, 0.3, 1));
	
	m_mesh->draw();

	// végetért a 3D színtér rajzolása
	m_program.Unuse();

	//
	// UI
	//
	// A következő parancs megnyit egy ImGui tesztablakot és így látszik mit tud az ImGui.
	ImGui::ShowTestWindow();
	// A ImGui::ShowTestWindow implementációja az imgui_demo.cpp-ben található
	// Érdemes még az imgui.h-t böngészni, valamint az imgui.cpp elején a FAQ-ot elolvasni.
	// Rendes dokumentáció nincs, de a fentiek elegendőek kell legyenek.
	
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_FirstUseEver);
	// csak akkor lépjünk be, hogy ha az ablak nincs csíkká lekicsinyítve...
	if (ImGui::Begin("Curve designer"))	
	{
		static size_t currentItem = 0;

		ImGui::ListBoxHeader("List", 4);
		for (size_t i = 0; i < m_pointName.size(); ++i)
		{
			if (ImGui::Selectable(m_pointName[i].c_str(), (i==currentItem))  )
				currentItem = i;
		}
		ImGui::ListBoxFooter();

		if (ImGui::Button("Add") && m_pointName.size() < kMaxPointCount)
		{
			m_pointName.push_back("Point " + std::to_string(m_pointName.size()+1));
			m_controlPoints.push_back({ 0,0,0 });
			currentItem = m_pointName.size() - 1;
		}

		ImGui::SameLine();

		//if (ImGui::Button("Delete") && m_pointName.size() > 0 && currentItem < m_pointName.size())
		//3.:
		if (ImGui::Button("Delete") && m_pointName.size() > kMinPointCount && currentItem < m_pointName.size())
		{
			m_pointName.erase(m_pointName.begin() + currentItem);
			m_controlPoints.erase(m_controlPoints.begin() + currentItem);

			size_t number = currentItem+1;
			for (auto& it = m_pointName.begin()+ currentItem; it != m_pointName.end(); ++it)
			{
				*it = "Point " + std::to_string(number);
				++number;
			}
			if (m_pointName.size() == 0)
				currentItem = 0;
			else
				if (currentItem >= m_pointName.size())
					currentItem = m_pointName.size() - 1;
		}

		if (m_controlPoints.size() > 0)
			ImGui::SliderFloat3("Coordinates", &(m_controlPoints[currentItem][0]), -10, 10);

		//ImGui::SliderFloat("Parameter", &m_currentParam, 0, (float)(m_controlPoints.size()-1));
		//3.:
		ImGui::SliderFloat("Parameter", &m_currentParam, 1, (float)(m_controlPoints.size() - 2));

		// 1. feladat: Suzanne feje mindig forduljon a menetirány felé! Először ezt elég síkban (=XZ) megoldani!
		
		// 2. feladat: valósíts meg egy Animate gombot! Amíg nyomva van az m_currentParameter periodikusan változzon a [0,m_controlPoints.size()-1] intervallumon belül!
		
		if (ImGui::Button("Animate")) 
			animating = !animating;
		if (animating)
			m_currentParam = (sin(3.1415 * SDL_GetTicks() / 1000.0) + 1.0) / 2.0 * (m_controlPoints.size() - 1);

		// 3. feladat: egyenközű Catmull-Rom spline-nal valósítsd meg Suzanne görbéjét a mostani törött vonal helyett!

	}
	ImGui::End(); // ...de még ha le is volt, End()-et hívnunk kell
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

glm::vec3 CMyApp::Eval(float t)
{
	if (m_controlPoints.size() == 0)
		return glm::vec3(0);

	int interval = (int)t;

	if (interval < 0)
		return m_controlPoints[0];
		
	if (interval >= m_controlPoints.size()-1)
		return m_controlPoints[m_controlPoints.size()-1];

	float localT = t - interval;
	return (1- localT)*m_controlPoints[interval] + localT*m_controlPoints[interval+1];
}
