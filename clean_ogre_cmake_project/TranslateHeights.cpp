#ifndef __TranslateHeights_CPP
#define __TranslateHeights_CPP
#include "ColorizeTerrainMap.cpp"
#include "TerrainTexturesNode.cpp" 
class TranslateHeights {
	public:
		TranslateHeights(Ogre::Terrain* terrain, Ogre::Rectangle2D* mMiniScreen,Ogre::Camera* mCamera, float translatefactor,
				 double waterheight, double mountheight, TerrainTexturesSettings texsets);
		void getVerts(void);
	private:
		float ctranslatefactor, cSize, cMaxheight, cMinheight, cmountheight, cwaterheight;
		//std::string cTextureName;
		TerrainTexturesSettings ctexsets;
		Ogre::Terrain* cterrain;
		Ogre::Rectangle2D* cmMiniScreen;
		//Ogre::Log* tlog;
		std::vector<Ogre::Vector3> cverts;
		void rescaleHeights(void);
};

TranslateHeights::TranslateHeights(Ogre::Terrain* terrain, Ogre::Rectangle2D* mMiniScreen,Ogre::Camera* mCamera, float translatefactor,
				   double waterheight, double mountheight, TerrainTexturesSettings texsets){
	//tlog = Ogre::LogManager::getSingleton().getLog("ScaleHeight.log");
	cterrain = terrain;
	cmMiniScreen = mMiniScreen;
	ctranslatefactor = translatefactor;
	cSize = (float) cterrain->getSize();
	//cTextureName = TextureName;
	ctexsets = texsets;
	cmountheight = mountheight;
	cwaterheight = waterheight;
	getVerts();
//	rescaleHeights();
	Ogre::Vector3 campos = mCamera->getPosition();
	float camheight = cterrain->getHeightAtWorldPosition(campos.x,campos.y +ctranslatefactor + 200.0f,campos.z);
	mCamera->setPosition(Ogre::Vector3(campos.x, camheight, campos.z));
}

void TranslateHeights::getVerts(void){
	std::vector<Ogre::Vector3> veccont;
	std::ostringstream ss5;
	cterrain->dirty();
	double x, y, height, maxheight, minheight;
	maxheight = (double) cterrain->getMaxHeight();
	minheight = (double) cterrain->getMinHeight();
	//double scalediv = cScalefactor-maxheight;
	for (int i = 0; i< cSize; i++){
		for(int j = 0; j< cSize; j++){
			//x = float(i)/cSize;  //normalize terrain pos coordinates 
			//y = float(i)/cSize;  //normalize terrain pos coordinates for input
			//double dist = pow(pow(i-posx,2)+pow(j-posz,2),.5);
			//if (dist <= cselsize){
//				float y = cterrain->getHeightAtWorldPosition(i,3000,j);
			//Ogre::Vector3 retvec;
			height = (double)cterrain->getHeightAtPoint(i,j);
/*
			ss5 << "Height: " << height << "\n";
			ss5 << "Scaled Height: " << height/maxheight*cScalefactor<<"\n";
*/
			cterrain->setHeightAtPoint(i,j, height + ctranslatefactor);
//			veccont.push_back(Ogre::Vector3(i,j,height));
/*
			if (height > maxheight) {
				maxheight = height;
			}
			if (height < minheight){
				minheight = height;
			}
*/
			//}
		}
	}
	cMaxheight = maxheight;
	cMinheight = minheight;
	bump(513.0f, 0.0f, cterrain, &ctexsets);
	colorizeterrainmap(513.0f, minheight, maxheight, cterrain, cwaterheight, cmountheight, &ctexsets);
	Ogre::TexturePtr pResource = Ogre::TextureManager::getSingleton().getByName("maTexture");
        Ogre::Image imageOgre;
	textureNodeMap terraintexnds = TerrainTexturesNode::Instance()->getTextureNodes();
	std::string TextureName = ctexsets.name;
	std::string imagename = TextureName+".png";
        imageOgre.load(imagename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	pResource->unload();
	pResource->loadImage(imageOgre);
	delete &imageOgre;
	//pResource->reload();
        /*
	Ogre::MaterialManager::getSingleton().remove("RttMat");
	Ogre::MaterialPtr renderMaterial = Ogre::MaterialManager::getSingleton().create("RttMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("maTexture");
	cmMiniScreen->setMaterial("RttMat");
	*/
//	cverts = veccont;
	//ss5 << "Max Height: "<< cMaxheight << "\n";
	//ss5 << "Min Height: "<< cMinheight << "\n";
	//ss5 << "Size: " << cSize << "\n";
	//tlog->logMessage(ss5.str());
	cterrain->update();
}
#endif
