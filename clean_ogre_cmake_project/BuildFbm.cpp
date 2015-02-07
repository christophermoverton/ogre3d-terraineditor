#ifndef __BuildFbm_CPP
#define __BuildFbm_CPP
#include <stdio.h>
#include <cstdlib>
#include "libfbm.hpp"
#include "Imagestuff.cpp"
#include <math.h>
#include "TerrainStruct.h"


using namespace std;

class BuildFbm{

	public:
		BuildFbm(double H, double seed);
		terr::CPointsMap    * crtnmap;
		terr::CPointsMap  getHeightMap();
};

struct MyContext : public libfbm::SGPContext
{
  MyContext(const libfbm::zvec& dim) : libfbm::SGPContext(dim, dim, "myctx") { }
  double cov(const libfbm::zvec& r)
  {
    double xp = 0;
    for ( size_t i = 0; i < r.size(); i++ )
        xp += -fabs(r[i]) / (5 + i * 17);
    return exp(xp);
  }
};

BuildFbm::BuildFbm(double H, double seed){
	crtnmap = new terr::CPointsMap();
	libfbm::zvec dim(2);
  	dim[0] = 64;
  	dim[1] = 64;
  	//MyContext ctx(dim);
	libfbm::FBMSteinContext ctx(H, 2, 513);  //Hurst exponent H ctx(H, *,*) controls smoothness of the terrain with larger values
	//being more smooth up to 1. 
	libfbm::gaussianRandomGenerator.setSeed((int) seed);
	libfbm::Field fbm(ctx, true);
	fbm.generate();
	//fbm.integrate();
  // print it out
	double size = 513.0f;
	ImageBuffer buffer(size);
    	FillColour* fill = new FillColour (&buffer);
	for ( int y = 0; y < fbm.getDim()[1]; y++ )
	{
      		for ( int x = 0; x < fbm.getDim()[0]; x++ ){
          		//printf("%s%g", x ? " " : "", fbm(x, y));
      			//printf("\n");
			terr::Coordpair * coordpair = new terr::Coordpair(x,y);
			Ogre::ColourValue col = Ogre::ColourValue(fbm(x,y),fbm(x,y),fbm(x,y));
			fill->setPixl((size_t)x, (size_t)y, col);
			(*crtnmap)[(*coordpair)] = (double) fbm(x,y);
    		}
	}
	buffer.saveImage("test7.png");
}

terr::CPointsMap  BuildFbm::getHeightMap(){
	return (*crtnmap);
}
#endif 
