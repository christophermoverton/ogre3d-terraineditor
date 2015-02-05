#ifndef __LoadHeightMap_CPP
#define __LoadHeightMap_CPP
#include <vector>
//#include "Imagestuff.cpp"
#include "ColorGet.cpp"
#include "TerrainStruct.h"
#include "TPoint3.h"
#include "Imagestuff.cpp"

class LoadHeightMap{
	public:
		LoadHeightMap(Ogre::Terrain* mterrain, Ogre::String filename, float maxheightscale);
		LoadHeightMap(Ogre::Terrain* mterrain, float maxheightscale, vector<vector<vector<double> > > heightmapvalues);
		LoadHeightMap(Ogre::Terrain* mterrain, float maxheightscale, terr::T3dCPointsMap * heightmapvalues);
		LoadHeightMap(Ogre::Terrain* mterrain, Ogre::Rectangle2D* mMiniScreen, float maxheightscale, terr::CPointsMap * heightmapvalues);
		LoadHeightMap(Ogre::Terrain* mterrain, float maxheightscale, terr::T3dCPointsMap * heightmapvalues, float terrsize);
	
};

LoadHeightMap::LoadHeightMap(Ogre::Terrain* mterrain, Ogre::String filename, float maxheightscale){
	//maxheightscale = 1.0f means that the terrain is scaled to a maximum possible height 
	//that is equivalent to the terrain's size.
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().createLog("HeightMap.log");
	std::ostringstream ss5;
	Ogre::Image* img = new Ogre::Image();
	ss5<< "Test" << "\n";
	tlog->logMessage(ss5.str());
//        ResourceGroup* Ogre::ResourceGroupManager::getResourceGroup 	( 	const String &  	name	) 	
	Ogre::DataStreamPtr a = Ogre::ResourceGroupManager::getSingleton().openResource (filename); 		
	img->load(filename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	//Ogre::Image imga = img->load(a);
	//Ogre::Image imgb = &imga;
	Ogre::PixelBox pixel_box = img->getPixelBox();
	Ogre::uint32 width = img->getWidth(); //x
	Ogre::uint32 height = img->getHeight(); //y
	/*
	std::map<PixelFormat,std::string> m;
	m[PF_A8R8G8B8]="A8R8G8B8";
	m[PF_A8B8G8R8]="A8B8G8R8";
	m[PF_B8G8R8A8]="B8G8R8A8";
	m[PF_R8G8B8A8]="R8G8B8A8";
	*/
	//technically image::load() loads with .png A8R8B8G8 
	//we stored the image R8B8G8A8 so we need to rotate channel 
	//values for proper decoding.
	/*
	if (m.find(img->getFormat())==m.end()){
		ss5<<"Not found format.\n";
	}// not found
	else{
		ss5<<"Image Format: "<<m[img->getFormat()]<<"\n";
	}
	*/
	ss5<< "Test" << "\n";
	ss5<< "width: "<< width << "\n";
	ss5<< "depth: "<< height << "\n";
	ss5<< "Ogre::PF_R8G8B8A8: " << Ogre::PF_R8G8B8A8 << "\n";
	ss5<< "format: "<<img->getFormat()<<"\n";
	ss5<< "size: " << mterrain->getSize() << "\n";
	/*
	std::vector<PixelFormat> a;
	a.push_back(PF_A8R8G8B8);
	a.push_back(
	for (int m = 0; m< 5; m++){
	
	}
	*/
	tlog->logMessage(ss5.str());
	ss5.str(std::string());
	mterrain->dirty();
	for (int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			Ogre::ColourValue colour = img->getColourAt((size_t) i, (size_t) j, (size_t) 0);
			//Ogre::Image::load() apparently decodes with A8R8B8G8 even though
			//the image were encoded with R8B8G8A8, so we rotate values for realign.
			//bgar no, agbr no, ragb no, bagr
			Ogre::ColourValue colrot = Ogre::ColourValue(256.0f*colour.r, 256.0f*colour.b, 256.0f*colour.g, 256.0f*colour.a);
/*			
			ss5 << "Red: " << colour.r << "\n";
			ss5 << "Green: " << colour.g << "\n";
			ss5 << "Blue: " << colour.b << "\n";
			ss5 << "Alpha: " << colour.a << "\n";
*/			
			ColorGet* cg = new ColorGet(1.0f);
			double nheight = cg->decode(colrot);

//			ss5<< "nheight: " << nheight<<"\n";
			double posheight = nheight*(double)mterrain->getSize()*maxheightscale;
//			ss5<< "posheigh: " << posheight << "\n";
			mterrain->setHeightAtPoint ((long) i, (long) j, posheight);			
		}
	}
	ColorGet* cg = new ColorGet(1.0f);
	Ogre::ColourValue acol = Ogre::ColourValue(255,255,255,255);
	ss5 << "Red: " << acol.r << "\n";
	ss5 << "Green: " << acol.g << "\n";
	ss5 << "Blue: " << acol.b << "\n";
	ss5 << "Alpha: " << acol.a << "\n";
	ss5 << "Test value decode: " << cg->decode(Ogre::ColourValue(255,255,255,255))<<"\n";
	Ogre::ColourValue colour = img->getColourAt((size_t) 0,(size_t) 0, (size_t) 0);
	ss5 << "Red: " << colour.r << "\n";
	ss5 << "Green: " << colour.g << "\n";
	ss5 << "Blue: " << colour.b << "\n";
	ss5 << "Alpha: " << colour.a << "\n";
	tlog->logMessage(ss5.str());
	mterrain->update();
}

LoadHeightMap::LoadHeightMap(Ogre::Terrain* mterrain, float maxheightscale, vector<vector<vector<double> > > heightmapvalues){
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().createLog("HeightMap.log");
	std::ostringstream ss5;
	mterrain->dirty();
	for (int i = 0; i<heightmapvalues.size();i++){
		for(int j = 0; j<heightmapvalues[i].size(); j++){
			for(int k = 0; k < heightmapvalues[i][j].size()-1; k++){
				double posheight = heightmapvalues[i][j][k]*maxheightscale;
				mterrain->setHeightAtPoint ((long) i, (long) j, posheight);
				//ss5<< "posheigh: " << posheight << "\n";
			}
		}
	}
	//tlog->logMessage(ss5.str());
	mterrain->update();
}

LoadHeightMap::LoadHeightMap(Ogre::Terrain* mterrain, Ogre::Rectangle2D* mMiniScreen, float maxheightscale, terr::CPointsMap * heightmapvalues){
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().createLog("HeightMap.log");
	std::ostringstream ss5;
	double size = 513.0f;
	ImageBuffer buffer(size);
    	FillColour* fill = new FillColour (&buffer);
	mterrain->dirty();
	for (terr::CPointsMap::iterator i = heightmapvalues->begin(); i != heightmapvalues->end(); i++){
		terr::Coordpair hcoord = (*i).first;
		double posheight = (*i).second*maxheightscale;
		//ss5<< "posheigh: " << posheight << "\n";
		mterrain->setHeightAtPoint((long) hcoord.first, (long) hcoord.second, posheight);
		Ogre::ColourValue col = Ogre::ColourValue((*i).second,(*i).second,(*i).second);
		fill->setPixl((size_t) hcoord.first, (size_t) hcoord.second, col);
	}
	mterrain->update();
	buffer.saveImage("../media/materials/textures/test6.png");
	/*
	Ogre::MaterialManager::getSingleton().remove("RttMat");
        Ogre::Image imageOgre;
        imageOgre.load("test6.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        Ogre::TexturePtr m_texture;

                // Texture creation 1
        m_texture = Ogre::TextureManager::getSingletonPtr()->createManual(
                "maTexture",
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                Ogre::TEX_TYPE_2D,
                imageOgre.getWidth(),
                imageOgre.getHeight(),
                0,
                Ogre::PF_R8G8B8A8, Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
	Ogre::MaterialPtr renderMaterial = Ogre::MaterialManager::getSingleton().create("RttMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	
        //renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	m_texture->loadImage(imageOgre);
        renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("maTexture");
        //lTextureUnit->setTextureName("test6.png", Ogre::TEX_TYPE_2D);
        //lTextureUnit->setTextureCoordSet(0);
        mMiniScreen->setMaterial("RttMat");
	*/
	Ogre::TexturePtr pResource = Ogre::TextureManager::getSingleton().getByName("maTexture");
        Ogre::Image imageOgre;
        imageOgre.load("test6.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	pResource->unload();
	pResource->loadImage(imageOgre);
	//pResource->reload();
	Ogre::MaterialManager::getSingleton().remove("RttMat");
	Ogre::MaterialPtr renderMaterial = Ogre::MaterialManager::getSingleton().create("RttMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("maTexture");
	mMiniScreen->setMaterial("RttMat");
	tlog->logMessage(ss5.str());
	//mterrain->update();
}

LoadHeightMap::LoadHeightMap(Ogre::Terrain* mterrain, float maxheightscale, terr::T3dCPointsMap * heightmapvalues){
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().createLog("HeightMap.log");
	std::ostringstream ss5;
	mterrain->dirty();
	for (terr::T3dCPointsMap::iterator i = heightmapvalues->begin(); i != heightmapvalues->end(); i++){
		TPoint3 * hcoord = (*i).first;
		double posheight = (*i).second*maxheightscale;
		ss5<< "posheigh: " << posheight << "\n";
		mterrain->setHeightAtPoint((long) hcoord->x, (long) hcoord->y, posheight);
	}
	mterrain->update();
	/*
	for (int i = 0; i<heightmapvalues.size();i++){
		for(int j = 0; j<heightmapvalues[i].size(); j++){
			for(int k = 0; k < heightmapvalues[i][j].size()-1; k++){
				double posheight = heightmapvalues[i][j][k]*maxheightscale;
				mterrain->setHeightAtPoint ((long) i, (long) j, posheight);
				//ss5<< "posheigh: " << posheight << "\n";
			}
		}
	}
	*/
	tlog->logMessage(ss5.str());
	//mterrain->update();
}

LoadHeightMap::LoadHeightMap(Ogre::Terrain* mterrain, float maxheightscale, terr::T3dCPointsMap * heightmapvalues, float terrsize){
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().createLog("HeightMap.log");
	std::ostringstream ss5;
	mterrain->dirty();
	terr::CPointsMap * pointsvals = new terr::CPointsMap();
	for (terr::T3dCPointsMap::iterator i = heightmapvalues->begin(); i != heightmapvalues->end(); i++){
		TPoint3 * hcoord = (*i).first;
		terr::Coordpair * coordp = new terr::Coordpair(hcoord->x, hcoord->y);
		double posheight = (*i).second*maxheightscale;
		ss5<< "posheigh: " << posheight << "\n";
		(*pointsvals)[(*coordp)] = posheight;
	}
	
	for (int x = 0; x < terrsize; x++){
		for(int y = 0; y < terrsize; y++) {
			terr::Coordpair * hcoord = new terr::Coordpair(x,y);
			mterrain->setHeightAtPoint((long)x, (long)y, (*pointsvals)[(*hcoord)]);
		}
	}

	mterrain->update();

	tlog->logMessage(ss5.str());
	//mterrain->update();
}
#endif
