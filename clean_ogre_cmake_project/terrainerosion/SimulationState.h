/****************************************************************************
    Copyright (C) 2012 Adrian Blumer (blumer.adrian@gmail.com)
    Copyright (C) 2012 Pascal Spörri (pascal.spoerri@gmail.com)
    Copyright (C) 2012 Sabina Schellenberg (sabina.schellenberg@gmail.com)

    All Rights Reserved.

    You may use, distribute and modify this code under the terms of the
    MIT license (http://opensource.org/licenses/MIT).
*****************************************************************************/

#ifndef STATE_H
#define STATE_H
#include <map>
#include "Grid2D.h"
#include "platform_includes.h"

#include "Math/PerlinNoise.h"

namespace terr{
	typedef std::pair<int, int>                            	     Coordpair     ;
	//typedef std::tuple< int, int, int>                            T3dCoord      ;
	typedef std::map<Coordpair, double>                           CPointsMap   ;
	//typedef std::map<TPoint3 *, double>                             T3dCPointsMap ;
	//typedef std::map<TPoint3 , double>                            T3dMap;
	typedef std::map<int, CPointsMap>                             CPointsMaps  ;
	//typedef std::map<int, T3dCPointsMap*>                           T3dCPointsMaps;
	


}
using namespace glm;

class SimulationState
{
public:
    Grid2D<float> water;
    Grid2D<float> terrain;
    Grid2D<float> suspendedSediment;
    //terr::CPointsMap * _mterrain;
    Grid2D<vec3> surfaceNormals;
public:

    SimulationState(uint w, uint h)
        :   water(w,h),
            terrain(w,h),
            suspendedSediment(w,h),
            surfaceNormals(w,h)
    {

        createPerlinTerrain();

        int l = 20;
        int mw = 20;
        int xmin = water.width() - l-mw;
        int xmax = water.width() - l;

        int ymin = water.height() - l-mw;
        int ymax = water.height() - l;

    }

    SimulationState(uint w, uint h, terr::CPointsMap* mterrain)
        :   water(w,h),
            terrain(w,h),
            suspendedSediment(w,h),
            surfaceNormals(w,h)
	    //_mterrain(mterrain)
    {
	
        for (uint y=0; y<water.height(); y++)
        {
            for (uint x=0; x<water.width(); x++)
            {
                water(y,x) = 0.0f;
		terr::Coordpair * coordpair = new terr::Coordpair((int)y,(int)x);
		terrain(y,x) = (*mterrain)[(*coordpair)];//->getHeightAtPoint((long)y, (long)x);
                //terrain(y,x) = h*4*1.3;
                suspendedSediment(y,x) = 0.0f;// 0.1*terrain(y,x);

            }
        }        

        int l = 20;
        int mw = 20;
        int xmin = water.width() - l-mw;
        int xmax = water.width() - l;

        int ymin = water.height() - l-mw;
        int ymax = water.height() - l;

    }

    void createPerlinTerrain()
    {
        PerlinNoise perlin;

        for (uint y=0; y<water.height(); y++)
        {
            for (uint x=0; x<water.width(); x++)
            {
                water(y,x) = 0.0f;

                float h = 0.0f; float f = 0.05f;
                h += perlin.Sample(y*f,x*f)*1; f /= 2;
                h += perlin.Sample(y*f,x*f)*2; f /= 2;
                h += perlin.Sample(y*f,x*f)*4; f /= 2;
                h += perlin.Sample(y*f,x*f)*8; f /= 2;
                terrain(y,x) = h*4*1.3;
                suspendedSediment(y,x) = 0.0f;// 0.1*terrain(y,x);

            }
        }
    }

    void getTerraindata(){
    }

    void createSteepTerrain()
    {
        for (uint y=0; y<water.height(); y++)
        {
            for (uint x=0; x<water.width(); x++)
            {
                water(y,x) = 0.0f;
                if (x > (water.height()/2)) {
                    terrain(y,x) = 0.2*(x-water.height()/2);
                } else {
                    terrain(y,x) = 0.2*(-x+water.height()/2);
                }
                if (y > (water.height()/2)) {
                    terrain(y,x) = std::max(terrain(y,x),0.2f*(y-water.height()/2));
                } else {
                    terrain(y,x) = std::max(terrain(y,x),0.2f*(-y+water.height()/2));
                }
                suspendedSediment(y,x) = 0.0f;
            }
        }
    }


};

#endif // STATE_H
