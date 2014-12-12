#include <iostream>
#include <string>
#include <sstream>
#include <vector>
class Terrainheightselection{
	public:
		Terrainheightselection(Ogre::TerrainGroup* mTerraingroup, Ogre::Vector3 pos, double selsize);
		std::vector<Ogre::Vector3> getSelectionVerts(void);
	private:
		Ogre::TerrainGroup* cmTerraingroup;
		Ogre::Terrain* cterrain;
		float* cheightdata;
		double cselsize;
		Ogre::Vector3 cpos;
		//std::vector<Ogre::Vector3> getSelectionVerts(void);
				
		
};

Terrainheightselection::Terrainheightselection(Ogre::TerrainGroup* mTerraingroup, Ogre::Vector3 pos, double selsize){
	cmTerraingroup = mTerraingroup;
	cterrain = cmTerraingroup->getTerrain(0,0);
	cheightdata = cterrain->getHeightData();
	cselsize = selsize;
	cpos = pos;
	std::vector<Ogre::Vector3> selverts = getSelectionVerts();
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("test.log");
	std::ostringstream ss;
	ss<< sizeof(cheightdata);
	tlog->logMessage(ss.str());
	ss.str(std::string());
	ss<< cheightdata[9];
	tlog->logMessage(ss.str());
	Ogre::Real x = cterrain->getSize();
	ss.str(std::string());
	ss<< x;
	tlog->logMessage(ss.str());
	float xd = cterrain->getHeightAtWorldPosition(200,3000,200);
	ss.str(std::string());
	ss<< xd;
	tlog->logMessage(ss.str());
	ss.str(std::string());
	ss<< selverts.size();
	tlog->logMessage(ss.str());
}

std::vector<Ogre::Vector3> Terrainheightselection::getSelectionVerts(void){
	Ogre::Real posx, posz;
	Ogre::Real posxmin, posxmax, poszmin, poszmax;
	posx = cpos.x;
	posz = cpos.z;
	posxmin = posx - cselsize;
	posxmax = posx + cselsize;
	poszmin = posz - cselsize;
	poszmax = posz + cselsize;
	std::vector<Ogre::Vector3> veccont;
	for (int i = posxmin; i<posxmax+1; i++){
		for(int j = poszmin; j<poszmax+1; j++){
			double dist = pow(pow(i-posx,2)+pow(j-posz,2),.5);
			if (dist <= cselsize){
//				float y = cterrain->getHeightAtWorldPosition(i,3000,j);
				Ogre::Vector3 retvec;
				cterrain->getPoint(i,j,&retvec);
				veccont.push_back(Ogre::Vector3(i,retvec.y,j));
			}
		}
	}
	return veccont;
}
