#ifndef TerrainTexturesSettings_CPP
#define TerrainTexturesSettings_CPP
#include <string>

struct TerrainTexturesSettings
{
    double normal, size, maxheight, minheight, alpha, worldsize;
    int layerid;
    std::string name;
    TerrainTexturesSettings(double nnormal, double nsize, double nmaxheight, double nminheight, double nalpha,
                            std::string nname, int nid, double nworldsize){
	normal = nnormal;
	size = nsize;
	maxheight = nmaxheight;
	minheight = nminheight;
	alpha = nalpha;
	name = nname;
	layerid = nid;
	nworldsize = worldsize;
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
    }
};
#endif
