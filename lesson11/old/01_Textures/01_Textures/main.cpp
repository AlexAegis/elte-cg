#include <osg/Texture2D>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

#ifdef _DEBUG
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgViewerd.lib")
#else
#pragma comment(lib, "osg.lib")
#pragma comment(lib, "osgDB.lib")
#pragma comment(lib, "osgViewer.lib")
#endif

#pragma warning(disable : 4482 )

int main(int argc, char** argv)
{
	// pozíciók
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back(osg::Vec3(-1.0f, 0.0f, -1.0f));
	vertices->push_back(osg::Vec3(1.0f, 0.0f, -1.0f));
	vertices->push_back(osg::Vec3(-1.0f, 0.0f, 1.0f));
	vertices->push_back(osg::Vec3(1.0f, 0.0f, 1.0f));

	// normálisok
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	// textúrakoordináták
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
	texcoords->push_back(osg::Vec2(0.0f, 0.0f));
	texcoords->push_back(osg::Vec2(3.0f, 0.0f));
	texcoords->push_back(osg::Vec2(0.0f, 3.0f));
	texcoords->push_back(osg::Vec2(3.0f, 3.0f));

	// négyszög geometria
	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	quad->setUseVertexBufferObjects(true);

	// állítsuk be, hogy a VBO-ba milyen adatok kerüljenek
	quad->setVertexArray(vertices.get());
	quad->setNormalArray(normals.get());
	quad->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	quad->setTexCoordArray(0, texcoords.get());
	// kirajzolandó primitív meghatározása
	quad->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLE_STRIP, 0, 4));

	// textúra betöltése
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/moon256128.TGA");
	texture->setImage(image.get());
	texture->setFilter(osg::Texture::FilterParameter::MIN_FILTER, osg::Texture::FilterMode::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::FilterParameter::MAG_FILTER, osg::Texture::FilterMode::LINEAR);
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::WrapMode::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::REPEAT);

	// rakjuk be egy geode-ba a quad-ot, mint kirajzolandó elemet!
	osg::ref_ptr<osg::Geode> root = new osg::Geode;
	root->addDrawable(quad.get());

	// 0-ás mintavételezõre rakjuk rá a textúrát
	root->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
	osg::StateSet* state = root->getOrCreateStateSet();
	state->setMode(GL_LIGHTING,
		osg::StateAttribute::OFF |
		osg::StateAttribute::PROTECTED);

	// hozzuk létre a viewer-t és állítsuk be a gyökeret megjelenítendõ adatnak
	osgViewer::Viewer viewer;
	viewer.setSceneData(root.get());

	// a (20,20) kezdeti pozícióba hozzunk létre egy 640x480-as ablakot
	viewer.setUpViewInWindow(20, 20, 640, 480);
	viewer.realize();

	// adjuk át a vezérlést a viewer-nek
	return viewer.run();
}