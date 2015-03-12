#ifndef __ScaleHeights_CPP
#define __ScaleHeights_CPP
#include <vector>
#include "ColorizeTerrainMap.cpp"
#include "TerrainTexturesNode.cpp"

class ScaleHeights {
	public:
		ScaleHeights(Ogre::TerrainGroup* terraingroup, Ogre::Rectangle2D* mMiniScreen,Ogre::Camera* mCamera, float scalefactor,
		double waterheight, double mountheight, TerrainTexturesSettings texsets);
		void getVerts(void);
	private:
		float cScalefactor, cSize, cMaxheight, cMinheight, cmountheight, cwaterheight;
		//std::string cTextureName;
		TerrainTexturesSettings ctexsets;
		Ogre::Terrain* cterrain;
		Ogre::TerrainGroup* cterraingroup;
		Ogre::Rectangle2D* cmMiniScreen;
		Ogre::Log* tlog;
		std::vector<Ogre::Vector3> cverts;
		void rescaleHeights(void);
		
};

ScaleHeights::ScaleHeights(Ogre::TerrainGroup* terraingroup, Ogre::Rectangle2D* mMiniScreen,Ogre::Camera* mCamera, float scalefactor,
			   double waterheight, double mountheight, TerrainTexturesSettings texsets){
	tlog = Ogre::LogManager::getSingleton().getLog("ScaleHeight.log");
	cterraingroup = terraingroup;
	//cterrain = terrain;
	cmMiniScreen = mMiniScreen;
	cScalefactor = scalefactor;
	//cTextureName = texsets->name;
	ctexsets = texsets;
	cmountheight = mountheight;
	cwaterheight = waterheight;
	getVerts();
//	rescaleHeights();
	Ogre::Vector3 campos = mCamera->getPosition();
	float camheight = cterraingroup->getHeightAtWorldPosition(campos.x,cScalefactor + 200.0f,campos.z);
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
	double x, y, height, maxheight, minheight;
	double scalediv = abs(maxheight-minheight);
        Ogre::TerrainGroup::TerrainIterator ti = cterraingroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            	Ogre::Terrain* t = ti.getNext()->instance;
		cSize = (float) t->getSize();
		maxheight = (double) t->getMaxHeight();
		minheight = (double) t->getMinHeight();
		t->dirty();
		for (int i = 0; i< cSize; i++){
			for(int j = 0; j< cSize; j++){
			//x = float(i)/cSize;  //normalize terrain pos coordinates 
			//y = float(i)/cSize;  //normalize terrain pos coordinates for input
			//double dist = pow(pow(i-posx,2)+pow(j-posz,2),.5);
			//if (dist <= cselsize){
//				float y = cterrain->getHeightAtWorldPosition(i,3000,j);
			//Ogre::Vector3 retvec;
			height = (double)t->getHeightAtPoint(i,j);
/*
			ss5 << "Height: " << height << "\n";
			ss5 << "Scaled Height: " << height/maxheight*cScalefactor<<"\n";
//*/
			t->setHeightAtPoint(i,j, (height/scalediv)*cScalefactor);
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
		t->update();
	}
	cMaxheight = maxheight;
	cMinheight = minheight;
	textureNodeMap terraintexnds = TerrainTexturesNode::Instance()->getTextureNodes();
	std::string TextureName = ctexsets.name;
	std::string imagename = TextureName+".png";//terraintexnds[].imageName;
	ti = cterraingroup->getTerrainIterator();
	Ogre::Terrain* t = ti.getNext()->instance;
	bump(513.0f, 0.0f, t, &ctexsets);
	colorizeterrainmap(513.0f, minheight, maxheight, t,cwaterheight, cmountheight, &ctexsets);
	Ogre::TexturePtr pResource = Ogre::TextureManager::getSingleton().getByName("maTexture");
        Ogre::Image imageOgre;
        imageOgre.load(imagename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	pResource->unload();
	pResource->loadImage(imageOgre);
	//pResource->reload();
        /*
	Ogre::MaterialManager::getSingleton().remove("RttMat");
	Ogre::MaterialPtr renderMaterial = Ogre::MaterialManager::getSingleton().create("RttMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("maTexture");
	cmMiniScreen->setMaterial("RttMat");
	*/
//	cverts = veccont;
	ss5 << "Max Height: "<< cMaxheight << "\n";
	ss5 << "Min Height: "<< cMinheight << "\n";
	ss5 << "Size: " << cSize << "\n";
	tlog->logMessage(ss5.str());
}


#endif
