#ifndef TerrainTexturesNode_CPP
#define TerrainTexturesNode_CPP
#include "TerrainStruct.h"
#include <stddef.h> 
//typedef std::string textureName; 
struct textureData{
    std::string                textureName; //actual resource name..not the name provided by Ogre's terrain manager
    int                          textureID;
    int                            layerID;
    terr::CPointsMap *   baseHeightMapData; //base format 513x513  can receive a null pointer for optional assignment
    std::string *                imageName;
    std::string *        normaltextureName;
    int *                  normaltextureID;
    terr::CVectorMaps * normalHeightMapData; //base format 513x513
    std::string *          normalimageName;
    /*
    textureData & operator=(const textureData & a){
        textureName = a.textureName;
        textureID = a.textureID;
	layerID = a.layerID;
	if (!a.baseHeightMapData){
		baseHeightMapData = 0;
	}
	else{
		(*(baseHeightMapData)) = (*(a.baseHeightMapData));
	}
	if (!a.imageName){
		imageName = 0;
	}
	else{
		(*(imageName)) = (*(a.imageName));
	}
	if (!a.normaltextureName){
		normaltextureName = 0;
	}
	else{
		(*(normaltextureName)) = (*(a.normaltextureName));
	}
	if (!a.normaltextureID){
		normaltextureID = 0;
	}
	else{
		(*(normaltextureID)) = (*(a.normaltextureID));
	}
	if (!a.normalHeightMapData){
		normalHeightMapData = 0;
	}
	else{
		(*(normalHeightMapData)) = (*(a.normalHeightMapData));
	}
	if (!a.normalimageName){
		normalimageName = 0;
	}
	else{
		(*(normalimageName)) = (*(a.normalimageName));
	}	
        return *this;
    }
    */
    textureData(void){
	textureName = std::string("");
	textureID = 0;
	layerID = 0;
	baseHeightMapData = 0;
	imageName = 0;
	normaltextureName = 0;
	normaltextureID = 0;
	normalHeightMapData = 0;
	normalimageName = 0;
    }  
    textureData(std::string ntextureName, int ntextureID, int nlayerID,
		terr::CPointsMap * nbaseHeightMapData, std::string * nimageName,
		std::string * nnormaltextureName, int * nnormaltextureID,
                terr::CVectorMaps * nnormalHeightMapData, std::string * nnormalimageName){
	textureName 	    =         ntextureName;
	textureID           =           ntextureID;
	layerID     	    =             nlayerID;
	baseHeightMapData   =   nbaseHeightMapData;
	imageName           =           nimageName;
	normaltextureName   =   nnormaltextureName;
	normaltextureID     =     nnormaltextureID;
	normalHeightMapData = nnormalHeightMapData;
	normalimageName     =     nnormalimageName;
    }
    textureData(std::string ntextureName, int ntextureID, int nlayerID,
		std::string * nimageName, std::string * nnormaltextureName, 
		int * nnormaltextureID, std::string * nnormalimageName){
	textureName 	    =         ntextureName;
	textureID           =           ntextureID;
	layerID     	    =             nlayerID;
	baseHeightMapData   =                    0;
	imageName           =           nimageName;
	normaltextureName   =   nnormaltextureName;
	normaltextureID     =     nnormaltextureID;
	normalHeightMapData =                    0;
	normalimageName     =     nnormalimageName;
    }
};

typedef std::map<std::string,textureData> textureNodeMap;

//TerrainTexturesNode is meant to augment terrain resource tracking.
//And espcially with respect to modifying existing terrain texture 
//data.  May include procedural textures setting on this, so that 
//a user can potentially re referencing settings for modifying
//texture data (maintaining a closer degree of self similarity).
//The idea here is to allow a user to freely switch back and forth
//between terrain textures (that could be loaded from files), or 
//generated procedurally, and in the procedural case allowing the 
//user to re reference also potentially settings used in generating
//such terrain firstly.  This would also include already computed
//heightmap data, in case any user needed to modify terrain textures
//without effecting the procedural generation process (e.g., resampling).
class TerrainTexturesNode{
   public:
	   static TerrainTexturesNode* Instance();
	   void setTextureNode(textureData texdata);
	   bool getTextureNode(std::string textureName, textureData & textureoutData);
	   textureNodeMap getTextureNodes(void);
   private:
	   static TerrainTexturesNode* m_pInstance;
	   textureNodeMap * texturenodes;
	   TerrainTexturesNode(){texturenodes = new textureNodeMap();};
	   TerrainTexturesNode(TerrainTexturesNode const&){};  //copy constructor
	   TerrainTexturesNode& operator=(TerrainTexturesNode const&){};  // assignment operator is private
	  
};

TerrainTexturesNode* TerrainTexturesNode::m_pInstance = 0; 

TerrainTexturesNode* TerrainTexturesNode::Instance()
{
   if (!m_pInstance){   // Only allow one instance of class to be generated.
      m_pInstance = new TerrainTexturesNode();
      //texturenodes = new textureNodeMap();
   }
   return m_pInstance;

}

void TerrainTexturesNode::setTextureNode(textureData texdata){
	std::string texname = texdata.textureName;
	(*texturenodes)[texname] = texdata;
}

bool TerrainTexturesNode::getTextureNode(std::string textureName, textureData & textureoutData){
	if ( (*(texturenodes)).find(textureName) == (*(texturenodes)).end()){
		return false;
	}
	else{
		textureoutData = (*(texturenodes))[textureName];
		return true;
	} 
}

textureNodeMap TerrainTexturesNode::getTextureNodes(void){
	return (*(texturenodes));
}  
#endif
