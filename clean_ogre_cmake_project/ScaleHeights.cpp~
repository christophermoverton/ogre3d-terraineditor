#include <vector>

class ScaleHeights {
	public:
		ScaleHeights(Ogre::Terrain* terrain, Ogre::Camera* mCamera, float scalefactor);
		void getVerts(void);
	private:
		float cScalefactor, cSize, cMaxheight, cMinheight;
		Ogre::Terrain* cterrain;
		Ogre::Log* tlog;
		std::vector<Ogre::Vector3> cverts;
		void rescaleHeights(void);
};

ScaleHeights::ScaleHeights(Ogre::Terrain* terrain, Ogre::Camera* mCamera, float scalefactor){
	tlog = Ogre::LogManager::getSingleton().getLog("ScaleHeight.log");
	cterrain = terrain;
	cScalefactor = scalefactor;
	cSize = (float) cterrain->getSize();
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
	double scalediv = cScalefactor-maxheight;
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
			cterrain->setHeightAtPoint(i,j, height/maxheight*cScalefactor);
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
//	cverts = veccont;
	ss5 << "Max Height: "<< cMaxheight << "\n";
	ss5 << "Min Height: "<< cMinheight << "\n";
	ss5 << "Size: " << cSize << "\n";
	tlog->logMessage(ss5.str());
	cterrain->update();
}
