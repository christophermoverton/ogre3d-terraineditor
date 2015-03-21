#ifndef TerrainTexturesSettings_CPP
#define TerrainTexturesSettings_CPP
#include <string>
#include "TerrainStruct.h"
#include <vector>

struct TerrainTexturesSettings
{
    double normal, size, maxheight, minheight, alpha, worldsize, texgradscale, texalpha;
    int layerid, terraintextures;
    std::string name;
    terr::CVectorMaps normals;
    terr::CPointsMap heightmap;
    //terr::CoordCPMap;  //continental terrain maps.
    TerrainTexturesSettings(double nnormal, double nsize, double nmaxheight, double nminheight, double nalpha,
                            std::string nname, int nid, double nworldsize, double ntexgradscale, double ntexalpha,
			    int nterraintextures){
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
	terraintextures = nterraintextures;
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
	terraintextures = 0;
    }
};

typedef std::vector<TerrainTexturesSettings> TerTexsets;
#endif
