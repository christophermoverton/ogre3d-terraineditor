#ifndef __BuildBiLinear_CPP
#define __BuildBiLinear_CPP
#include "TerrainStruct.h"

terr::CPointsMap * BuildBiLinear( double size, double RSize, terr::CPointsMap * heightmap);
/*
double lerp(float a0, float a1, float w) {
     return (1.0 - w)*a0 + w*a1;
}
*/
terr::CPointsMap * BuildBiLinear( double size, double RSize, terr::CPointsMap * heightmap){
	terr::CPointsMap * Rheightmap = new terr::CPointsMap();

	for (int i = 0; i < RSize; i++){
		for(int j = 0; j < RSize; j++){
			//bilinear interpolation is similar to cubic in terms of idea.
			//we interpolate along y (vertical) axis first, and then interpolate on 
			//the horizontal.  We only need to worry about the grid [0,0], [0,1],[1,0],[1,1] 
			//points so computations are smaller.  I've included a quintic smoothing
			//equation to see what happens here...supposed to reduce artifacts.
			double x = ((double) i) * (size-2.0f)/(RSize-1.0f);
			double y = ((double) j) * (size-2.0f)/(RSize-1.0f);	
			///*		 
			//int p0x = ((x == size ? (int)x - 1 : (int)x); int p0y = ((y == size ? (int)y - 1 : (int)y);
			int p1x = (int)x; int p1y = (int)y;
			double sx = x -p1x; double sy = y-p1y;
			int p2x = (int)x+1; int p2y = (int)y+1;
			terr::Coordpair * p11 = new terr::Coordpair(p1x,p1y);
			terr::Coordpair * p12 = new terr::Coordpair(p1x,p2y);
			terr::Coordpair * p21 = new terr::Coordpair(p2x,p1y);
			terr::Coordpair * p22 = new terr::Coordpair(p2x,p2y);
     			double u = 6.0f*pow(sx,5) - 15*pow(sx,4)+10*pow(sx,3);
     			double v = 6.0f*pow(sy,5) - 15*pow(sy,4)+10*pow(sy,3);
			double h11 = (*heightmap)[(*p11)];
			double h12 = (*heightmap)[(*p12)];
			double h21 = (*heightmap)[(*p21)];
			double h22 = (*heightmap)[(*p22)];
			double iy1 = lerp(h11,h12,v);  
			double iy2 = lerp(h21,h22,v);
			double h = lerp(iy1,iy2,u);
			terr::Coordpair * coord = new terr::Coordpair(i,j);
			(*Rheightmap)[(*coord)] = h;
		}
	}
	return Rheightmap;
}
#endif
