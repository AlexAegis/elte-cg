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
	// hozzunk l�tre egy ShapeDrawable-t
    osg::ref_ptr<osg::ShapeDrawable> shape1 = new osg::ShapeDrawable;
	// amibe rakjunk bele egy...
    shape1->setShape( 
				new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f),	// orig� k�z�ppont�...
				4.0f, 3.0f, 2.0f) );						// 4x3x2-es t�glatestet

	// a sz�nt�rgr�funk gy�kere legyen egy Geode
	osg::ref_ptr<osg::Geode> root = new osg::Geode;
	// aminek kirajzoland� elemk�nt adjuk hozz� a fenti t�glatestet
    root->addDrawable( shape1.get() );

	//	1. feladat: jelen�ts�nk meg m�s geometriai alakzatokat!
	//		a) g�mb: osg::Sphere(<k�z�ppont>, <sug�r>) - http://trac.openscenegraph.org/documentation/OpenSceneGraphReferenceDocs/a00775.html 
	//		b) kocka: osg::Box(<k�z�ppont>, <oldalhossz>) - http://trac.openscenegraph.org/documentation/OpenSceneGraphReferenceDocs/a00072.html
	//		c) henger: osg::Cylinder(<k�z�ppont>, <sug�r>, <magass�g>) - http://trac.openscenegraph.org/documentation/OpenSceneGraphReferenceDocs/a00186.html
	//
	//	2. feladat: transzform�ci�k
	//		a) transzform�ci�s node-ok header f�jlja:
	//			#include <osg/MatrixTransform>
	//		b) transzform�ci�s node l�trehoz�sa:
	//			osg::ref_ptr<osg::MatrixTransform> trafo1 = new osg::MatrixTransform;
	//		c) transzform�ci� megad�sa:
	//			trafo1->setMatrix( osg::Matrix::scale(osg::Vec3(0.1f, 2, 0.1f) ));
	//		   (transzform�ci�k: 
	//				- osg::Matrix::translate - http://trac.openscenegraph.org/documentation/OpenSceneGraphReferenceDocs/a00490.html#a5f4e4ab467143942c2f580eafca84bfc
	//				- osg::Matrix::rotate - http://trac.openscenegraph.org/documentation/OpenSceneGraphReferenceDocs/a00490.html#aff5e82f94d3fcb3e9dad58d1f65d138b
	//				- osg::Matrix::scale - http://trac.openscenegraph.org/documentation/OpenSceneGraphReferenceDocs/a00490.html#a39c72138fd6d8b7528d25176d1c498a5 )
	//		d) a transzform�ci�s node egy group-node, a gyermekeire hat a traf� node transzform�ci�ja;
	//			adjuk teh�t hozza a transzform�land� objektumot gyermekk�nt a trafo1-nek:
	//
	//			trafo1->addChild( root.get() );
	//
	//		e) figyelj arra, hogy mostant�l a sz�nt�r gy�kere a trafo1 kell legyen! (de: pr�b�ljuk ki n�lk�le)
	//
	//			viewer.setSceneData( trafo1.get() );
	//
	//	3. feladat: rajzoljunk ki k�t 4x3x2-es t�glatestet, egyet a (-2,0,0)-ba, egyet pedig a (2,0,0).
	//			ehhez: osg::Group t�pus� legyen a gy�kere a sz�nt�rnek �s csak ez al� j�jjenek az eltol�sok!
	//
	//	4. feladat: jelen�ts�nk meg egy tehenet! Ehhez kell:
	//		a) f�jlbeolvas�: 
	//			#include <osgDB/ReadFile>
	//
	//		b) f�jl beolvas�sa: 
	//			osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( "cow.osg" );
	//
	//		c) r�tt� t�tel: 
	//			 viewer.setSceneData( model.get() );
    
	// hozzuk l�tre a viewer-t �s �ll�tsuk be a gy�keret megjelen�tend� adatnak
    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );
	
	// a (20,20) kezdeti poz�ci�ba hozzunk l�tre egy 640x480-as ablakot
    viewer.setUpViewInWindow(20, 20, 640, 480); 
    viewer.realize(); 

	// adjuk �t a vez�rl�st a viewer-nek
    return viewer.run();
}