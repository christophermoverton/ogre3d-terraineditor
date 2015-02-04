#ifndef __BuildSimplex_CPP
#define __BuildSimplex_CPP

#include "SimplexNoise.h"
#include "TerrainStruct.h"
#include "Imagestuff.cpp"
#include "Array2D.h"
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <map>

using namespace std;

class BuildSimplex{

	public:
		BuildSimplex(float frequency, float amplitude, unsigned octaves);
		terr::T3dCPointsMap * rtnmap;
		terr::CPointsMap    * crtnmap;
		terr::T3dCPointsMap * getHeightMap();
		terr::CPointsMap    * getHeightMap2();
};

BuildSimplex::BuildSimplex(float frequency = .00002f, float amplitude = 10.0f, unsigned octaves = 16.0) {
        //get the command line arguments
	unsigned i;
        unsigned xgrid = 513;
        unsigned ygrid = 513;
        unsigned zgrid = 500;
        //unsigned octaves = 16;
        unsigned rseed = unsigned(time(NULL));
        string fchunk = "part";
        float lacunarity = 2.0f;
        float gain = 0.65f;
        int kernel = 3;
        float phi = 0.8f;
	//float frequency = .00002f;
	//float amplitude = 10.0f;  //default 50.0f
        //set the random seed
        srand(rseed);
        
        //make the Simplex noise object
        SimplexNoise noise(octaves,gain,lacunarity, frequency, amplitude);

        //make the height_map
        Array_2D<float> height_map(xgrid,ygrid);

        /**********NOISE TRACKING************/
        //loop!
        for (i = 0; i < 1; ++i)   //originally i<zgrid
        {
                //first, make the filename
                //      there are many (probably easier) ways to do this, but this is efficient and simple

                //now make noise!
                noise.makeSomeNoise(height_map,i);
                
                //give it some color
                //colorize(height_map,colmap,grad);
                
                //shade it
                //shader.render_directional_light(height_map,colmap,phi);
                
                //print out the height_map
                //colmap.print(fname);
                /*
                //finally, update the user (occasionally)
                if ((i & 15) == 15){
                        cout << i << " frames done. Estimated time remaining: "
                                 << secondsToString(int(double(time(NULL)-beginning) * double(zgrid - i) / (double)i))
                                 << ". " << endl;
                }
		*/


        }
	double size = 513.0f;
	ImageBuffer buffer(size);
    	FillColour* fill = new FillColour (&buffer);
    	//buffer.saveImage("test1.png");
	///*
	rtnmap = new terr::T3dCPointsMap();
	crtnmap = new terr::CPointsMap();
	//TPoint3 * rmcoordpair = new TPoint3(w+1,w+1, 0);
	//(*rtnmap)[rmcoordpair] = (*pointsmap)[(*mcoordpair)];
	
	for (int x = 0; x < height_map.width(); ++x){
		for (int y = 0; y < height_map.height(); ++y){
			//ss5<<"Color value: "<<noisevals[i][j]<<"\n";

			TPoint3 * rtnmapcoord = new TPoint3(x, y, 0);
			terr::Coordpair * coordpair = new terr::Coordpair(x,y);	
			double colval = (double)height_map(x,y);
				
			(*rtnmap)[rtnmapcoord] = colval;
			(*crtnmap)[(*coordpair)] = colval;	
			Ogre::ColourValue col = Ogre::ColourValue(colval,colval,colval);
			fill->setPixl((size_t)x, (size_t)y, col);
			


			//fill->setPixl((size_t)j, (size_t)k, col);
		}
	}
	buffer.saveImage("test5.png");
}

terr::T3dCPointsMap * BuildSimplex::getHeightMap(){
	return rtnmap;
}

terr::CPointsMap * BuildSimplex::getHeightMap2(){
	return crtnmap;
}
#endif
