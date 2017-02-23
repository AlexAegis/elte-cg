#include <osg/ShapeDrawable>
#include <osg/Geode>
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

int main( int argc, char** argv )
{
	// hozzunk létre egy ShapeDrawable-t
    osg::ref_ptr<osg::ShapeDrawable> shape1 = new osg::ShapeDrawable;
	// amibe rakjunk bele egy...
    shape1->setShape( 
				new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f),	// origó középpontú...
				4.0f, 3.0f, 2.0f) );						// 4x3x2-es téglatestet

	// a színtérgráfunk gyökere legyen egy Geode
	osg::ref_ptr<osg::Geode> root = new osg::Geode;
	// aminek kirajzolandó elemként adjuk hozzá a fenti téglatestet
    root->addDrawable( shape1.get() );

	//	1. feladat: jelenítsünk meg más geometriai alakzatokat!
	//		a) gömb: osg::Sphere(<középpont>, <sugár>) - http://trac.openscenegraph.org/documentation/OpenSceneGraphReferenceDocs/a00775.html 
	//		b) kocka: osg::Box(<középpont>, <oldalhossz>) - http://trac.openscenegraph.org/documentation/OpenSceneGraphReferenceDocs/a00072.html
	//		c) henger: osg::Cylinder(<középpont>, <sugár>, <magasság>) - http://trac.openscenegraph.org/documentation/OpenSceneGraphReferenceDocs/a00186.html
	//
	//	2. feladat: transzformációk
	//		a) transzformációs node-ok header fájlja:
	//			#include <osg/MatrixTransform>
	//		b) transzformációs node létrehozása:
	//			osg::ref_ptr<osg::MatrixTransform> trafo1 = new osg::MatrixTransform;
	//		c) transzformáció megadása:
	//			trafo1->setMatrix( osg::Matrix::scale(osg::Vec3(0.1f, 2, 0.1f) ));
	//		   (transzformációk: 
	//				- osg::Matrix::translate - http://trac.openscenegraph.org/documentation/OpenSceneGraphReferenceDocs/a00490.html#a5f4e4ab467143942c2f580eafca84bfc
	//				- osg::Matrix::rotate - http://trac.openscenegraph.org/documentation/OpenSceneGraphReferenceDocs/a00490.html#aff5e82f94d3fcb3e9dad58d1f65d138b
	//				- osg::Matrix::scale - http://trac.openscenegraph.org/documentation/OpenSceneGraphReferenceDocs/a00490.html#a39c72138fd6d8b7528d25176d1c498a5 )
	//		d) a transzformációs node egy group-node, a gyermekeire hat a trafó node transzformációja;
	//			adjuk tehát hozza a transzformálandó objektumot gyermekként a trafo1-nek:
	//
	//			trafo1->addChild( root.get() );
	//
	//		e) figyelj arra, hogy mostantól a színtér gyökere a trafo1 kell legyen! (de: próbáljuk ki nélküle)
	//
	//			viewer.setSceneData( trafo1.get() );
	//
	//	3. feladat: rajzoljunk ki két 4x3x2-es téglatestet, egyet a (-2,0,0)-ba, egyet pedig a (2,0,0).
	//			ehhez: osg::Group típusú legyen a gyökere a színtérnek és csak ez alá jöjjenek az eltolások!
	//
	//	4. feladat: jelenítsünk meg egy tehenet! Ehhez kell:
	//		a) fájlbeolvasó: 
	//			#include <osgDB/ReadFile>
	//
	//		b) fájl beolvasása: 
	//			osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( "cow.osg" );
	//
	//		c) rúttá tétel: 
	//			 viewer.setSceneData( model.get() );
    
	// hozzuk létre a viewer-t és állítsuk be a gyökeret megjelenítendõ adatnak
    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );
	
	// a (20,20) kezdeti pozícióba hozzunk létre egy 640x480-as ablakot
    viewer.setUpViewInWindow(20, 20, 640, 480); 
    viewer.realize(); 

	// adjuk át a vezérlést a viewer-nek
    return viewer.run();
}