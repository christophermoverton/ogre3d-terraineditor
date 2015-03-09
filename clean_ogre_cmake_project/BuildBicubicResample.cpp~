#ifndef __BuildBicubicResample_CPP
#define __BuildBicubicResample_CPP
#include "TerrainStruct.h"
#include "BicubicResample.cpp"
#include <vector>
//RSize is the resample size, size is the original size
//This Resampling method is not generally to be used with terrain heightmaps, 
//but instead used with procedural texture generators.  One because texture 
//generators that do not make use of Ogre's terrain managers will not have the advantage 
//of interpolators integration that make life easier in terms of grabbing heightmap 
//sample positions.  Thus it will be assumed that the user has actually produced 
//texture heightmaps that are size + 2 in dimension actually, so that a central 
//difference scheme can be applied here.  The actual terrain dimensions will be centered at
// 1...size+1  leaving position size+2 and 0 un used in terms of rendering but used 
//in terms of cross derivative computation.  Alternately if working with fixed images...at 
//the moment (until finding an alternate approximation method for the endpoints), 
//one could re sample the image neglecting the boundaries.

//Applications outside of resizing textures?  
//We can independent of Ogre's terrain managers build this interpolating class on a given 
//procedurally produced height map set and re sample heights over a smaller partition
//which can be applied in such a way that we need not deal with the problem of computing
//terrain heightmap values over a finer partition using the procedural method (which is slower).  
//Thus if I wanted to produce from a 513x513
//grid a 5000 x 5000 grid with the same heightmap field I can do this, partition, and load
//each terrain chunk into Ogre's terrain manager for world rendering.  In this way I also 
//need not deal with the problem of computing for each terrain the individual procedural
//values (but could do this later as needed) and reload a higher resolution version and
//save this chunk of data based upon the users actions...thus having a high and lower
//resolution set of the same terrain (one for fast computational rendering), and the
//other based upon a streaming as needed threaded computational set.

double A[16][16] = {  
  { 1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
  { 0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
  {-3,  3,  0,  0, -2, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
  { 2, -2,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
  { 0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0},
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0},
  { 0,  0,  0,  0,  0,  0,  0,  0, -3,  3,  0,  0, -2, -1,  0,  0},
  { 0,  0,  0,  0,  0,  0,  0,  0,  2, -2,  0,  0,  1,  1,  0,  0},
  {-3,  0,  3,  0,  0,  0,  0,  0, -2,  0, -1,  0,  0,  0,  0,  0},
  { 0,  0,  0,  0, -3,  0,  3,  0,  0,  0,  0,  0, -2,  0, -1,  0},
  {-9, -9, -9,  9,  6,  3, -6, -3,  6, -6,  3, -3,  4,  2,  2,  1},
  {-6,  6,  6, -6, -3, -3,  3,  3, -4,  4, -2,  2, -2, -2, -1, -1},
  { 2,  0, -2,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0},
  { 0,  0,  0,  0,  2,  0, -2,  0,  0,  0,  0,  0,  1,  0,  1,  0},
  {-6,  6,  6, -6, -4, -2,  4,  2, -3,  3, -3,  3, -2, -1, -2, -1},
  { 4, -4, -4,  4,  2,  2, -2, -2,  2, -2,  2, -2,  1,  1,  1,  1}
  };  //technically A^-12
terr::CPointsMap * BuildBicubicResampl( double size, double RSize, terr::CPointsMap * heightmap);
double bicubicInterpolate2 (double arr[], double x, double y);
double bicubicInterpolate2(double arr[], double x, double y){
   double a[16];
   for (int i = 0; i < 16; i++){
	double sum = 0.0f;
	for (int j = 0; j < 16; j++){
		sum += A[i][j]*arr[j];
	}
	a[i] = sum;
   }
   double ret = 0.0f;
   for (int i = 0; i<3; i++){
	for (int j = 0; j<3; j++){
		ret += a[i+j]*pow(x,i)*pow(y,j);  //a(i,j)
	}
   }
   return ret;
}

double partialx(double x, double y, terr::CPointsMap heightmap){
	//central difference (more accurate relative to forward and backwards differencing)
	terr::Coordpair * x1py = new terr::Coordpair(x+1,y);
	terr::Coordpair * x1my = new terr::Coordpair(x-1,y);
	return (heightmap[(*x1py)] - heightmap[(*x1my)])/2.0f;
}

double partialy(double x, double y, terr::CPointsMap heightmap){
	//central difference (more accurate relative to forward and backwards differencing)
	terr::Coordpair * xy1p = new terr::Coordpair(x,y+1);
	terr::Coordpair * xy1m = new terr::Coordpair(x,y-1);
	return (heightmap[(*xy1p)] - heightmap[(*xy1m)])/2.0f;
}

double partialxy(double x, double y, terr::CPointsMap heightmap){
	//finite difference central methods 
	//(H(x+1,y+1)-H(x+1,y) -H(x,y+1)+2H(x,y) - H(x-1,y)-H(x,y-1)+H(x-1,y-1))/2
	//or (H(x+1,y+1) -H(x+1,y-1) - H(x-1,y+1) + H(x-1,y-1))/4
	terr::Coordpair * x1py1p = new terr::Coordpair(x+1, y+1);
	terr::Coordpair * x1py1m = new terr::Coordpair(x+1, y-1);
	terr::Coordpair * x1my1p = new terr::Coordpair(x-1, y+1);
	terr::Coordpair * x1my1m = new terr::Coordpair(x-1, y-1);
	return (heightmap[(*x1py1p)]-heightmap[(*x1py1m)] -heightmap[(*x1my1p)] +heightmap[(*x1my1m)])/4.0f;
}

terr::CPointsMap * BuildBicubicResampl( double size, double RSize, terr::CPointsMap * heightmap){
	terr::CPointsMap * Rheightmap = new terr::CPointsMap();
			//if we want to produce from a 515 x515 grid rsize we need to have the first min at floor 1,1 
			//and final max at ceiling size 513,513 we need a 514 position in the central difference compute,
			//so we'd need a total of 515 x 515 position in resampling a 513x513 grid.
	for (int i = 0; i < RSize; i++){
		for(int j = 0; j < RSize; j++){
			//double x = (((double) i)*(size-19.0f))/(RSize-1.0f) + 1.0f;  //first row column always floored to 1 allowing central difference
			//double y = (((double) j)*(size-19.0f))/(RSize-1.0f) +1.0f;  //max row column always at ceiling = size
			//compiler not liking above?!  Its arbitrarily set with a ratio that should yield values inside boundary conditions!
			double x = ((double) i) * (size-2.0f)/(RSize-1.0f);
			double y = ((double) j) * (size-2.0f)/(RSize-1.0f);	
			///*		 
			//int p0x = ((x == size ? (int)x - 1 : (int)x); int p0y = ((y == size ? (int)y - 1 : (int)y);
			int p1x = (int)x; int p1y = (int)y;
			double locx = x -p1x; double locy = y-p1y;
			//int p1x = ((x == size ? (int)x : (int)x + 1); int p1y = ((y == size ? (int)y : (int)y + 1); 
			int p2x = (int)x+1; int p2y = (int)y+1;
			int p0x = (x <= 0 ? p1x : p1x-1); int p0y = (y <= 0 ? p1y : p1y-1);
			int p3x = (x >= (size-2.0f) ? p2x : p2x+1); int p3y = (y >= (size-2.0f) ? p2y : p2y+1); 
			//coefficient ordering is of the following form:
			// arr[0] = (H(p0x,p0y), H(p1x,p0y), H(p0x,p1y), H(p1x,p1y))
			// arr[1] = (partial x H(p0x,p0y) partial x H(p1x,p0y) partial x H(p0x,p1y), partial x H(p1x,p1y))
			//arr[2] = (partial y H(p0x,p0y) partial y H(p1x,p0y) partial y H(p0x,p1y), partial y H(p1x,p1y) )
			//arr[3] = (mpartial xy H(p0x,p0y) mpartial xy H(p1x,p0y) mpartial xy H(p0x,p1y), mpartial xy H(p1x,p1y))


			
			terr::Coordpair * p00 = new terr::Coordpair(p0x,p0y);
			terr::Coordpair * p01 = new terr::Coordpair(p0x,p1y);
			terr::Coordpair * p02 = new terr::Coordpair(p0x,p2y);
			terr::Coordpair * p03 = new terr::Coordpair(p0x,p3y);
			terr::Coordpair * p10 = new terr::Coordpair(p1x,p0y);
			terr::Coordpair * p11 = new terr::Coordpair(p1x,p1y);
			terr::Coordpair * p12 = new terr::Coordpair(p1x,p2y);
			terr::Coordpair * p13 = new terr::Coordpair(p1x,p3y);
			terr::Coordpair * p20 = new terr::Coordpair(p2x,p0y);
			terr::Coordpair * p21 = new terr::Coordpair(p2x,p1y);
			terr::Coordpair * p22 = new terr::Coordpair(p2x,p2y);
			terr::Coordpair * p23 = new terr::Coordpair(p2x,p3y);
			terr::Coordpair * p30 = new terr::Coordpair(p3x,p0y);
			terr::Coordpair * p31 = new terr::Coordpair(p3x,p1y);
			terr::Coordpair * p32 = new terr::Coordpair(p3x,p2y);
			terr::Coordpair * p33 = new terr::Coordpair(p3x,p3y);
			/*
			double arr[4][4] = { {(*heightmap)[(*p00)], (*heightmap)[(*p01)], (*heightmap)[(*p02)],
				 (*heightmap)[(*p03)]} ,
				{(*heightmap)[(*p10)], (*heightmap)[(*p11)], (*heightmap)[(*p12)],
				 (*heightmap)[(*p13)]},
				{(*heightmap)[(*p20)], (*heightmap)[(*p21)], (*heightmap)[(*p22)],
				 (*heightmap)[(*p23)]},
				{(*heightmap)[(*p30)], (*heightmap)[(*p31)], (*heightmap)[(*p32)],
				 (*heightmap)[(*p33)]}
			       };
			*/
			std::vector<double> ay(4,0);
			std::vector<std::vector<double> >  a(4,ay);
			a[0][0] = (*heightmap)[(*p00)]; a[0][1] = (*heightmap)[(*p01)]; 
			a[0][2] = (*heightmap)[(*p02)]; a[0][3] = (*heightmap)[(*p03)]; 
			a[1][0] = (*heightmap)[(*p10)]; a[1][1] = (*heightmap)[(*p11)]; 
			a[1][2] = (*heightmap)[(*p12)]; a[1][3] = (*heightmap)[(*p13)];
			a[2][0] = (*heightmap)[(*p20)]; a[2][1] = (*heightmap)[(*p21)]; 
			a[2][2] = (*heightmap)[(*p22)]; a[2][3] = (*heightmap)[(*p23)];
			a[3][0] = (*heightmap)[(*p30)]; a[3][1] = (*heightmap)[(*p31)]; 
			a[3][2] = (*heightmap)[(*p32)]; a[3][3] = (*heightmap)[(*p33)];   
			/*			
			for (int k = 0; k < 4; k++){
				for (int l = 0; l < 4; l++){
					a[k][l] = arr[k][l];
				}
			} 
			/*

			//*/
			double height = bicubicInterpolate (&a, locx, locy); //slower matrix compute method
			terr::Coordpair * coord = new terr::Coordpair(i,j);
			(*Rheightmap)[(*coord)] = height;
		}
	}
	return Rheightmap;
}


terr::CPointsMap BuildBicubicResample( double size, double RSize, terr::CPointsMap heightmap){
	terr::CPointsMap * Rheightmap = new terr::CPointsMap();
			//if we want to produce from a 515 x515 grid rsize we need to have the first min at floor 1,1 
			//and final max at ceiling size 513,513 we need a 514 position in the central difference compute,
			//so we'd need a total of 515 x 515 position in resampling a 513x513 grid.
	for (int i = 0; i < RSize; i++){
		for(int j = 0; j < RSize; j++){
			double x = ((double) i)*(size-3.0f)/(RSize-1.0f) +1.0f;  //first row column always floored to 1 allowing central difference
			double y = ((double) j)*(size-3.0f)/(RSize-1.0f) +1.0f;  //max row column always at ceiling = size 
			//int p0x = ((x == size ? (int)x - 1 : (int)x); int p0y = ((y == size ? (int)y - 1 : (int)y);
			double p0x = (int)x; double p0y = (int)y;
			//int p1x = ((x == size ? (int)x : (int)x + 1); int p1y = ((y == size ? (int)y : (int)y + 1); 
			double p1x = (int)x+1; double p1y = (int)y+1;
			//coefficient ordering is of the following form:
			// arr[0] = (H(p0x,p0y), H(p1x,p0y), H(p0x,p1y), H(p1x,p1y))
			// arr[1] = (partial x H(p0x,p0y) partial x H(p1x,p0y) partial x H(p0x,p1y), partial x H(p1x,p1y))
			//arr[2] = (partial y H(p0x,p0y) partial y H(p1x,p0y) partial y H(p0x,p1y), partial y H(p1x,p1y) )
			//arr[3] = (mpartial xy H(p0x,p0y) mpartial xy H(p1x,p0y) mpartial xy H(p0x,p1y), mpartial xy H(p1x,p1y))


			double arr[16];
			terr::Coordpair * p0xp0y = new terr::Coordpair(p0x,p0y);
			terr::Coordpair * p1xp0y = new terr::Coordpair(p1x,p0y);
			terr::Coordpair * p0xp1y = new terr::Coordpair(p0x,p1y);
			terr::Coordpair * p1xp1y = new terr::Coordpair(p1x,p1y);
			arr[0] = heightmap[(*p0xp0y)]; arr[1] = heightmap[(*p1xp0y)];
			arr[2] = heightmap[(*p0xp1y)]; arr[3] = heightmap[(*p1xp1y)];
			arr[4] = partialx(p0x, p0y, heightmap); arr[5] = partialx(p1x, p0y, heightmap);
			arr[6] = partialx(p0x, p1y, heightmap); arr[7] = partialx(p1x, p1y, heightmap);
			arr[8] = partialy(p0x, p0y, heightmap); arr[9] = partialy(p1x, p0y, heightmap);
			arr[10] = partialy(p0x, p1y, heightmap); arr[11] = partialy(p1x, p1y, heightmap);
			arr[12] = partialxy(p0x, p0y, heightmap); arr[13] = partialxy(p1x, p0y, heightmap);
			arr[14] = partialxy(p0x, p1y, heightmap); arr[15] = partialxy(p1x, p1y, heightmap);
			double height = bicubicInterpolate2 (arr, x, y); //slower matrix compute method
			terr::Coordpair * coord = new terr::Coordpair(i,j);
			(*Rheightmap)[(*coord)] = height;
		}
	}
	return *Rheightmap;
}


#endif
