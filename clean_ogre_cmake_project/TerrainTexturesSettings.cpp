#ifndef TerrainTexturesSettings_CPP
#define TerrainTexturesSettings_CPP
#include <string>
#include "TerrainStruct.h"

struct TerrainTexturesSettings
{
    double normal, size, maxheight, minheight, alpha, worldsize, texgradscale, texalpha;
    int layerid;
    std::string name;
    terr::CVectorMaps normals;
    TerrainTexturesSettings(double nnormal, double nsize, double nmaxheight, double nminheight, double nalpha,
                            std::string nname, int nid, double nworldsize, double ntexgradscale, double ntexalpha){
	normal = nnormal;
	size = nsize;
	maxheight = nmaxheight;
	minheight = nminheight;
	alpha = nalpha;
	name = nname;
	layerid = nid;
	worldsize = nworldsize;
	texgradscale = ntexgradscale;
	texalpha = ntexalpha;
    }
    TerrainTexturesSettings(void){
	normal = 0.0f;
	size = 0.0f;
	maxheight = 0.0f;
	minheight = 0.0f;
	alpha = 0.0f;
	name = std::string("");
	layerid = 0;
	worldsize = 12000.0f;
	texgradscale = 0.0f;
	texalpha = 1.0f;
    }
};
#endif
