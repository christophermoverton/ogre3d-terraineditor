#ifndef __LoadHeightMap_CPP
#define __LoadHeightMap_CPP
#include <vector>
//#include "Imagestuff.cpp"
#include "ColorGet.cpp"
#include "TerrainStruct.h"
#include "TPoint3.h"
#include "Imagestuff.cpp"
#include "BumpMap.cpp"
#include "ColorizeTerrainMap.cpp"
#include "BuildBicubicResample.cpp"
#include "BuildBiLinear.cpp"
#include "TerrainTexturesNode.cpp"
#include "TerrainTexturesSettings.cpp" 

class LoadHeightMap{
	public:
		LoadHeightMap(Ogre::Terrain* mterrain, Ogre::String filename, float maxheightscale);
		LoadHeightMap(Ogre::Terrain* mterrain, float maxheightscale, vector<vector<vector<double> > > heightmapvalues);
		LoadHeightMap(Ogre::Terrain* mterrain, float maxheightscale, terr::T3dCPointsMap * heightmapvalues);
		LoadHeightMap(Ogre::TerrainGroup* terraingroup, Ogre::Rectangle2D* mMiniScreen, 
			      float maxheightscale, terr::CPointsMap * heightmapvalues, double waterheight, 
			      double mountainheight, double texscale, bool modheight, TerrainTexturesSettings texsets);//std::string TextureName, double texsize, double normal);
		LoadHeightMap(Ogre::TerrainGroup* terraingroup, Ogre::Rectangle2D* mMiniScreen, 
			     float maxheightscale, terr::CPointsMaps * heightmaps, double waterheight, 
			     double mountainheight,double texscale, bool modheight, TerTexsets ttexsets);
		LoadHeightMap(Ogre::Terrain* mterrain, float maxheightscale, terr::T3dCPointsMap * heightmapvalues, float terrsize);
	private:
		void updateBlendMap(double alpha, int layerid);
		Ogre::TerrainGroup* cTerrainGroup;
		Ogre::Terrain * cmterrain;
	
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

LoadHeightMap::LoadHeightMap(Ogre::TerrainGroup* terraingroup, Ogre::Rectangle2D* mMiniScreen, 
			     float maxheightscale, terr::CPointsMap * heightmapvalues, double waterheight, 
			     double mountainheight,double texscale, bool modheight, TerrainTexturesSettings texsets) {
        //assumed heightmap values are normalized between [0,1]  will renormalize to [-1,1] and then scale
       
	cTerrainGroup = terraingroup; 
    	std::string TextureName = texsets.name;
	double texsize = texsets.size;
	double normal = texsets.normal;
	double worldsize = texsets.worldsize;
	double alpha = texsets.alpha;
	int layerid = texsets.layerid;
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().createLog("HeightMap.log");
	textureNodeMap terraintexnds = TerrainTexturesNode::Instance()->getTextureNodes(); //textureNodeMap defined in TerrainTexturesNode.cpp
	terraintexnds[TextureName].baseHeightMapData = heightmapvalues;  //setting the texture its given height map values assignment
	std::ostringstream ss5;
	double size = 513.0f;
	ImageBuffer buffer(size);
    	FillColour* fill = new FillColour (&buffer);
	if (modheight){
        	Ogre::TerrainGroup::TerrainIterator ti = cTerrainGroup->getTerrainIterator();
        	while(ti.hasMoreElements())
        	{
            		Ogre::Terrain* t = ti.getNext()->instance;
			t->dirty();
			for (int i = 0; i < size; i++){
				for(int j = 0; j < size; j++){
					terr::Coordpair * hcoord = new terr::Coordpair((double)i, (double)j);
					double posheight = 2*((*heightmapvalues)[(*hcoord)]-.5f)*maxheightscale;
					t->setHeightAtPoint((long) hcoord->first, (long) hcoord->second, posheight);

				}
			}
			t->dirtyLightmap();
			t->update();
		}
		for (int i = 0; i < size; i++){
			for (int j = 0; j < size; j++){
				terr::Coordpair * hcoord = new terr::Coordpair((double)i, (double)j);
				Ogre::ColourValue col = Ogre::ColourValue((*heightmapvalues)[(*hcoord)],(*heightmapvalues)[(*hcoord)],
									  (*heightmapvalues)[(*hcoord)]);
				fill->setPixl((size_t) hcoord->first, (size_t) hcoord->second, col);
			}
		}
	}

	if (modheight){
		Ogre::TerrainGroup::TerrainIterator ti = cTerrainGroup->getTerrainIterator();
		Ogre::Terrain* t = ti.getNext()->instance;
		buffer.saveImage("../media/materials/textures/test6.png");
		bump(1.0f*size, 1.0f, t, &texsets);//TextureName);  //originally 3*
		double max = t->getMaxHeight();
		double min = t->getMinHeight();

		colorizeterrainmap(1.0f*513.0f, min,  max, t, waterheight, mountainheight, &texsets); //originally 6*
		
		
	}
	else{
		terr::CPointsMap * heightmap;
		///*
		if (texsize > 513.0f){ 

			heightmap = BuildBiLinear(513.0f, texsize, heightmapvalues);

		}
		//*/
		else{ 
			heightmap = heightmapvalues; 
		}
		bump(texsize, normal, heightmap, &texsets);//TextureName);
		colorizeterrainmap(texsize, 0.0f,  1.0f, heightmap, waterheight, mountainheight, &texsets);
	}
	

	if (modheight){
		Ogre::TexturePtr pResource = Ogre::TextureManager::getSingleton().getByName("maTexture");
        	Ogre::Image imageOgre;
        	imageOgre.load(TextureName+".png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		imageOgre.flipAroundX();
		//imageOgre.resize(6*513,6*513,Ogre::Image::FILTER_BICUBIC);
		pResource->unload();
		pResource->loadImage(imageOgre);
		//delete &imageOgre;
	
	//Ogre::TexturePtr pResource3 = Ogre::TextureManager::getSingleton().getByName("Terrain");
	
		int terrtexvol = texsets.terraintextures;
		for (int i = 0; i < terrtexvol; i++){
	    		ss5.str(std::string());
			std::string Texname;
	    		if (i != 0) {
				ss5 <<TextureName<< i+1; 
				Texname = ss5.str();
				//ss5.str(std::string());
	    		}
			else{
				Texname = TextureName;
			}
	    		Ogre::TexturePtr pResource3 = Ogre::TextureManager::getSingleton().getByName(Texname);
            		Ogre::Image imageOgre3;
            		imageOgre3.load(TextureName+".png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	    		imageOgre3.flipAroundX();

	    		pResource3->unload();
	    		pResource3->loadImage(imageOgre3);
	    		Ogre::TexturePtr pResource2 = Ogre::TextureManager::getSingleton().getByName(Texname+"Normal");
            		Ogre::Image imageOgre2;
            		imageOgre2.load(TextureName+"normal.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	    		imageOgre2.flipAroundX();

	    		pResource2->unload();
	    		pResource2->loadImage(imageOgre2);

	    		delete &imageOgre2;
	    		delete &imageOgre3;
		}
	}
	else{
	     Ogre::TexturePtr pResource3 = Ogre::TextureManager::getSingleton().getByName(TextureName);
             Ogre::Image imageOgre3;
             imageOgre3.load(TextureName+".png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	     imageOgre3.flipAroundX();

	     pResource3->unload();
	     pResource3->loadImage(imageOgre3);
	     Ogre::TexturePtr pResource2 = Ogre::TextureManager::getSingleton().getByName(TextureName+"Normal");
             Ogre::Image imageOgre2;
             imageOgre2.load(TextureName+"normal.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	     imageOgre2.flipAroundX();

	     pResource2->unload();
	     pResource2->loadImage(imageOgre2);

	     delete &imageOgre2;
	     delete &imageOgre3;
	}
	//*/
	if (!modheight){updateBlendMap(alpha, layerid);}
	if (!modheight){
    		Ogre::TerrainGroup::TerrainIterator ti = cTerrainGroup->getTerrainIterator();
    		while(ti.hasMoreElements())
    		{
			Ogre::Terrain* t = ti.getNext()->instance;
			t->setLayerWorldSize(layerid,worldsize);
		}
	}

}

LoadHeightMap::LoadHeightMap(Ogre::TerrainGroup* terraingroup, Ogre::Rectangle2D* mMiniScreen, 
			     float maxheightscale, terr::CPointsMaps * heightmaps, double waterheight, 
			     double mountainheight,double texscale, bool modheight, TerTexsets ttexsets) {
	cTerrainGroup = terraingroup; 
	TerrainTexturesSettings texsets = ttexsets[0];
    	std::string TextureName = texsets.name;
	/*
	double texsize = texsets.size;
	double normal = texsets.normal;
	double worldsize = texsets.worldsize;
	double alpha = texsets.alpha;
	int layerid = texsets.layerid;
	*/
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().createLog("HeightMap.log");
	textureNodeMap terraintexnds = TerrainTexturesNode::Instance()->getTextureNodes(); 
	std::ostringstream ss5;
	double size = 513.0f;
	ImageBuffer buffer(size);
    	FillColour* fill = new FillColour (&buffer);
	ImageBuffer buffer2(size);
    	FillColour* fill2 = new FillColour (&buffer2);
	ImageBuffer buffer3(size);
    	FillColour* fill3 = new FillColour (&buffer3);
	ImageBuffer buffer4(size);
    	FillColour* fill4 = new FillColour (&buffer4);
	if (modheight){
        	//Ogre::TerrainGroup::TerrainIterator ti = cTerrainGroup->getTerrainIterator();
		int ter = 0;
        	//while(ti.hasMoreElements())
        	//{
		for (int g = 0; g<3; g++){
		   for(int h = 0; h<3; h++){
            		//Ogre::Terrain* t = ti.getNext()->instance;
			Ogre::Terrain* t = cTerrainGroup->getTerrain(g,h);
			t->dirty();
			int coord;
			if ((ter % 6) > 2){
				coord = (ter % 2)+2;
			}
			else{
				coord = (ter) % 2;
			}
			terr::CPointsMap dheightmapvalues = (*heightmaps)[coord];
			terr::CPointsMap * heightmapvalues = &dheightmapvalues;
			for (int i = 0; i < size; i++){
				for(int j = 0; j < size; j++){
					terr::Coordpair * hcoord = new terr::Coordpair((double)i, (double)j);
					double posheight = 2*((*heightmapvalues)[(*hcoord)]-.5f)*maxheightscale;
					t->setHeightAtPoint((long) hcoord->first, (long) hcoord->second, posheight);

				}
			}
			t->dirtyLightmap();
			t->update();
			ter++;
		//}
		   }
		}
		terr::CPointsMap dheightmapvalues1 = (*heightmaps)[0];
		terr::CPointsMap dheightmapvalues2 = (*heightmaps)[1];
		terr::CPointsMap dheightmapvalues3 = (*heightmaps)[2];
		terr::CPointsMap dheightmapvalues4 = (*heightmaps)[3];
		terr::CPointsMap * heightmapvalues1 = &dheightmapvalues1;
		terr::CPointsMap * heightmapvalues2 = &dheightmapvalues2;
		terr::CPointsMap * heightmapvalues3 = &dheightmapvalues3;
		terr::CPointsMap * heightmapvalues4 = &dheightmapvalues4;
		for (int i = 0; i < size; i++){
			for (int j = 0; j < size; j++){
				terr::Coordpair * hcoord = new terr::Coordpair((double)i, (double)j);
				Ogre::ColourValue col = Ogre::ColourValue((*heightmapvalues1)[(*hcoord)],(*heightmapvalues1)[(*hcoord)],
									  (*heightmapvalues1)[(*hcoord)]);
				fill->setPixl((size_t) hcoord->first, (size_t) hcoord->second, col);
				col = Ogre::ColourValue((*heightmapvalues2)[(*hcoord)],(*heightmapvalues2)[(*hcoord)],
									  (*heightmapvalues2)[(*hcoord)]);
				fill2->setPixl((size_t) hcoord->first, (size_t) hcoord->second, col);
				col = Ogre::ColourValue((*heightmapvalues3)[(*hcoord)],(*heightmapvalues3)[(*hcoord)],
									  (*heightmapvalues3)[(*hcoord)]);
				fill3->setPixl((size_t) hcoord->first, (size_t) hcoord->second, col);
				col = Ogre::ColourValue((*heightmapvalues4)[(*hcoord)],(*heightmapvalues4)[(*hcoord)],
									  (*heightmapvalues4)[(*hcoord)]);
				fill4->setPixl((size_t) hcoord->first, (size_t) hcoord->second, col);
			}
		}
	}
///*
	if (modheight){
		//Ogre::TerrainGroup::TerrainIterator ti = cTerrainGroup->getTerrainIterator();
		int cnt = 0;
		buffer.saveImage("../media/materials/textures/terrainheight.png");
		buffer2.saveImage("../media/materials/textures/terrainheight2.png");
		buffer3.saveImage("../media/materials/textures/terrainheight3.png");
		buffer4.saveImage("../media/materials/textures/terrainheight4.png");
        	//while(ti.hasMoreElements())
        	//{
		for (int g = 0; g<3; g++){
	   	  for(int h = 0; h<3; h++){
	     	    Ogre::Terrain* t = cTerrainGroup->getTerrain(g,h);
		    int bind;
	     	    if ((cnt % 6) > 2){   //or 2*world row slot length
		       bind = (cnt % 2)+2;
	     	    }
	     	    else{
		       bind = (cnt) % 2;
	     	    }
		    TerrainTexturesSettings texsets = ttexsets[bind];
		    //Ogre::Terrain* t = ti.getNext()->instance;
		    bump(1.0f*size, 1.0f, t, &texsets);//TextureName);  //originally 3*
		    double max = t->getMaxHeight();
		    double min = t->getMinHeight();

		    colorizeterrainmap(1.0f*513.0f, min,  max, t, waterheight, mountainheight, &texsets); //originally 6*
		    cnt += 1;
		  }
		}
		
	}

	if (modheight){
		Ogre::TexturePtr pResource = Ogre::TextureManager::getSingleton().getByName("maTexture");
        	Ogre::Image imageOgre;
        	imageOgre.load(TextureName+".png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		imageOgre.flipAroundX();
		pResource->unload();
		pResource->loadImage(imageOgre);
	}
//	*/
//	/*
	for (int i = 0; i<4; i++){
	    TerrainTexturesSettings texsets = ttexsets[i];
    	    std::string TextureName = texsets.name;
	    //double texsize = texsets.size;
	    //double normal = texsets.normal;
	    //double worldsize = texsets.worldsize;
	    //double alpha = texsets.alpha;
	    //int layerid = texsets.layerid;
	    Ogre::TexturePtr pResource3 = Ogre::TextureManager::getSingleton().getByName(TextureName);
            Ogre::Image imageOgre3;
            imageOgre3.load(TextureName+".png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	    imageOgre3.flipAroundX();

	    pResource3->unload();
	    pResource3->loadImage(imageOgre3);
	    Ogre::TexturePtr pResource2 = Ogre::TextureManager::getSingleton().getByName(TextureName+"Normal");
            Ogre::Image imageOgre2;
            imageOgre2.load(TextureName+"normal.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	    imageOgre2.flipAroundX();

	    pResource2->unload();
	    pResource2->loadImage(imageOgre2);
	
	    delete &imageOgre2;
	    delete &imageOgre3;
	}
//	*/

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

void LoadHeightMap::updateBlendMap(double alpha, int layerid){
    Ogre::TerrainGroup::TerrainIterator ti = cTerrainGroup->getTerrainIterator();
    while(ti.hasMoreElements())
    {
	Ogre::Terrain* t = ti.getNext()->instance;
    	Ogre::TerrainLayerBlendMap* blendMap0 = t->getLayerBlendMap(layerid);
    	float* pBlend0 = blendMap0->getBlendPointer();
    	for (Ogre::uint16 y = 0; y < t->getLayerBlendMapSize(); ++y)
    	{
        	for (Ogre::uint16 x = 0; x < t->getLayerBlendMapSize(); ++x)
        	{
        	    Ogre::Real tx, ty;
 
            	    blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            	    Ogre::Real height = t->getHeightAtTerrainPosition(tx, ty);
            	    //Ogre::Real val = (height - minHeight0) / fadeDist0;
            	    //val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            	    *pBlend0++ = alpha; //val;
        	}
    	}
    	blendMap0->dirty();
    	blendMap0->update();
    }
}
#endif
