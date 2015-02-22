#include <vector>
#include "ColorizeTerrainMap.cpp"

class ScaleHeights {
	public:
		ScaleHeights(Ogre::Terrain* terrain, Ogre::Rectangle2D* mMiniScreen,Ogre::Camera* mCamera, float scalefactor,
		double waterheight, double mountheight);
		void getVerts(void);
	private:
		float cScalefactor, cSize, cMaxheight, cMinheight, cmountheight, cwaterheight;
		Ogre::Terrain* cterrain;
		Ogre::Rectangle2D* cmMiniScreen;
		Ogre::Log* tlog;
		std::vector<Ogre::Vector3> cverts;
		void rescaleHeights(void);
};

ScaleHeights::ScaleHeights(Ogre::Terrain* terrain, Ogre::Rectangle2D* mMiniScreen,Ogre::Camera* mCamera, float scalefactor,
			   double waterheight, double mountheight){
	tlog = Ogre::LogManager::getSingleton().getLog("ScaleHeight.log");
	cterrain = terrain;
	cmMiniScreen = mMiniScreen;
	cScalefactor = scalefactor;
	cSize = (float) cterrain->getSize();
	cmountheight = mountheight;
	cwaterheight = waterheight;
	getVerts();
//	rescaleHeights();
	Ogre::Vector3 campos = mCamera->getPosition();
	float camheight = cterrain->getHeightAtWorldPosition(campos.x,cScalefactor + 200.0f,campos.z);
	mCamera->setPosition(Ogre::Vector3(campos.x, camheight, campos.z));
}

void ScaleHeights::rescaleHeights(){
	float scalediv = cMaxheight - cMinheight;
	Ogre::Vector3 pos;
	cterrain->dirty();
	for (int i = 0;i < cverts.size(); i++){
		cterrain->setHeightAtPoint(cverts[i].x,cverts[i].y, ((cverts[i].z/scalediv)-cMinheight+10.0f)*cScalefactor);
	}
	cterrain->update();
}

void ScaleHeights::getVerts(void){
	std::vector<Ogre::Vector3> veccont;
	std::ostringstream ss5;
	cterrain->dirty();
	double x, y, height, maxheight, minheight;
	maxheight = (double) cterrain->getMaxHeight();
	minheight = (double) cterrain->getMinHeight();
	double scalediv = abs(maxheight-minheight);
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
//*/
			cterrain->setHeightAtPoint(i,j, (height/scalediv)*cScalefactor);
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
	colorizeterrainmap(513.0f, minheight, maxheight, cterrain,cwaterheight, cmountheight);
	Ogre::TexturePtr pResource = Ogre::TextureManager::getSingleton().getByName("maTexture");
        Ogre::Image imageOgre;
        imageOgre.load("test10.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	pResource->unload();
	pResource->loadImage(imageOgre);
	//pResource->reload();
	Ogre::MaterialManager::getSingleton().remove("RttMat");
	Ogre::MaterialPtr renderMaterial = Ogre::MaterialManager::getSingleton().create("RttMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("maTexture");
	cmMiniScreen->setMaterial("RttMat");
//	cverts = veccont;
	ss5 << "Max Height: "<< cMaxheight << "\n";
	ss5 << "Min Height: "<< cMinheight << "\n";
	ss5 << "Size: " << cSize << "\n";
	tlog->logMessage(ss5.str());
	cterrain->update();
}
