/*
Copyright (c) 2009, Travis Archer
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
in the documentation and/or other materials provided with the distribution.

Neither the name of Morningside College nor the names of its contributors may be used to endorse or promote products derived 
from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, 
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/***************************************************************
*Fractal Terrain Generator
*created by Travis Archer
*
*This program creates a Fractal Terrain, using the
*	Diamond-Square technique described by Gavin S. P. Miller.
*
*1.0 initalizes variables (there are many)
*2.0 the map is created, this is the interesting part
*3.0 the map is outputted to a bitmap file. It should be noted
*	that the map is only a series of numbers before this step.
*	This step finds the flood level, and creates colors
*	accordingly. This part may be as interesting as 2.0 to some
*4.0 a webpage is created, displaying the bitmap
****************************************************************/
#ifndef __DiamondSquare_CPP
#define __DiamondSquare_CPP

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include "TerrainStruct.h"
#include <vector>

using namespace std;
typedef std::vector<std::vector<float> > floatvec2d;
/*****************************************FILENAME DEFINITION***/
const char oname[16]="test.bmp";

/*******************************************STRUCT DEFINITION***/
struct color{
	//v[0]=red, v[1]=green, v[2]=blue
	unsigned char v[3];
	
	color(unsigned char r, unsigned char g, unsigned char b){
		v[0]=r;
		v[1]=g;
		v[2]=b;
	}
};

/********************************************SIZE DEFINITIONS***/
unsigned hgrid = 513,//x dimension of the grid
			   vgrid = 513;//y dimension of the grid

/*****************************************FUNCTION PROTOTYPES***/
float random(float r);
color lerp(color c1, color c2, float value);//LERP = Linear intERPolation

//void fillMap(float map[][vgrid], float &min, float &max);// this is the algorithm part (the interesting part)
//void printMap(float map[][vgrid], float min, float max);//bitmap part
//void printPage(time_t beginning, time_t end);//webpage part

/********************************************************MAIN***/
class DiamondSquare {
	public:
		DiamondSquare(double size);
		terr::CPointsMap * crtnmap;
		terr::CPointsMap      getHeightMap();
	private:
		void fillMap(floatvec2d map, float &min, float &max);
		double csize;
};

DiamondSquare::DiamondSquare(double size){
//1.0 initialize some variables
	csize = size;
	crtnmap = new terr::CPointsMap();
	time_t beginning = time(NULL),//these two are used to time our algorithm
		   end;
	
	srand(beginning);//set the random seed
	hgrid = size;
	vgrid = size;
	//float map[hgrid][vgrid];//make the empty array
	floatvec2d map;//make the empty array
	map.resize(hgrid);
	//set the four corners to the default value
	int i,j,sgrid = ((hgrid<vgrid) ? hgrid : vgrid)-1;
	for (int i=0; i<hgrid; i++){
		map[i].resize(vgrid);
	}
	for (i=0;i<vgrid;i+=sgrid){
		for (j=0;j<hgrid;j+=sgrid){
			map[j][i]=10.0f;
		}
	}	
	float min,max;

	//now that the blank map is set up, unblank it
	fillMap(map, min, max);


	for (i=0;i<vgrid;i++){
		for (j=0;j<hgrid;j++){
			terr::Coordpair * coordpair = new terr::Coordpair(j,i);	
			(*crtnmap)[(*coordpair)]=map[j][i];
		}
	}
	//now that we have an interesting map, create a .BMP file
	//printMap(map, min, max);
	
	//finally, output a webpage
	//end = time(NULL);//set it to now
	//printPage(beginning, end);
}

terr::CPointsMap  DiamondSquare::getHeightMap(){
	return (*crtnmap);
}
/***************************************FUNCTION DECLARATIONS***/
color lerp(color c1, color c2, float value){
	color tcolor(0,0,0);
	
	for (int g=0;g<3;g++){
		if (c1.v[g]>c2.v[g])
			tcolor.v[g]=c2.v[g]+(unsigned char)(float(c1.v[g]-c2.v[g])*value);

		else
			tcolor.v[g]=c1.v[g]+(unsigned char)(float(c2.v[g]-c1.v[g])*value);
	}
	
	return (tcolor);
}

float random(float max){
	int r;
    float s;
    
	r = rand();
    s = (float)(r & 0x7fff)/(float)0x7fff;

    return (s * max);
}

void DiamondSquare::fillMap(floatvec2d map, float &min, float &max)
{
//set up some variable
	hgrid = csize; vgrid = csize;
	unsigned sgrid=((hgrid<vgrid) ? hgrid : vgrid)-1;//whichever is smaller (minus 1)
	
	int	i,j,k,//iterators
		x,y,//location variables
		offset=sgrid;//offset is the width of the square or diamond we are working with
		
	
	float range=10.0f,//range for random numbers
		  rangeModifier=0.65f,//the range is multipiled by this number each pass, making the map smoother
		  total,//variable for storing a mean value
		  temp;//stores the new value for a slot so more calculations can be done

	min=10.0f;
	max=0.0f;//for averaging

//get started
	//2.1 while the size of the squares and diamonds is larger than 1...
	while (offset>1){
		//reset variables
		y=0;
		
		//2.1.1 Diamond (create diamonds from squares)
		while (y<(vgrid-1)){
			x=0;
			while (x<(hgrid-1)){
				//set the proper map location to the proper value: the mean of it's 4 corners+a random value (minus half the range, so it generates a random number that can be negative
				temp=(map[x][y]+map[x+offset][y]+map[x][y+offset]+map[x+offset][y+offset])/4 + random(range)-(range/2);
				if (temp<min)
					min=temp;
				if (temp>max)
					max=temp;
				
				map[x+(offset/2)][y+(offset/2)]=temp;
				
				//advance x
				x+=offset;
			}//end of diamond x while loop
			
			//advance y
			y+=offset;
		}//end of diamond y<gridsize loop
		
		//2.1.2 Square (create squares from diamonds) This is the tough part
		y=0;
		while (y<vgrid){
			//set x to the correct offset
			x=(y+(offset/2))%offset;
			
			while (x<hgrid){
				total=0;
				
				//wrapping exceptions
				if (x==0)//left edge
					total+=map[(hgrid-1)-(offset/2)][y];
				else
					total+=map[x-(offset/2)][y];
				if (x==(hgrid-1))//right edge
					total+=map[(offset/2)][y];
				else
					total+=map[x+(offset/2)][y];
				if (y==0)//top edge
					total+=map[x][(vgrid-1)-(offset/2)];
				else
					total+=map[x][y-(offset/2)];
				if (y==(vgrid-1))//bottom edge
					total+=map[x][(offset/2)];
				else
					total+=map[x][y+(offset/2)];
				
				total/=4;
				
				temp=total+random(range)-(range/2);
				if (temp<min)
					min=temp;
				if (temp>max)
					max=temp;
				map[x][y]=temp;
				
				//advance x
				x+=offset;
			}//end of square x<=gridsize loop
			
			//advance y (only half as much as x)
			y+=offset/2;
		}//end of square y<=gridsize loop
		
		//2.1.3 End of pass adjustments
		range*=rangeModifier;
		offset/=2;//shrink the square/diamond width
	}
}

/*
void printMap(float map[][vgrid], float min, float max)
{
//set up some variables
	float diff = max-min,
		  flood=0.5f,//flood level
		  mount=0.85f;//mountain level
	
	flood*=diff;
	mount*=diff;
	
	int i,j,k;
	
	char c;
	
	//these can be changed for interesting results
	color landlow(0,64,0),
		  landhigh(116,182,133),
		  waterlow(55,0,0),
		  waterhigh(106,53,0),
		  mountlow(147,157,167),
		  mounthigh(226,223,216);

//3.0 output to file
	//3.1 Begin the file
	//3.1.1 open output file
	ofstream out;
	out.open(oname, ofstream::binary);
	if (!(out.is_open())){
		cout << "Target file opening error"<<endl;
		exit(0);
	}
	
	//3.1.2 copy the header
		//3.1.2.1 magic number
		out.put(char(66));
		out.put(char(77));
		
		//3.1.2.2 filsize/unused space
		for (i=0;i<8;i++)
			out.put(char(0));
		
		//3.1.2.3 data offset
		out.put(char(54));
		
		//3.1.2.4 unused space
		for (i=0;i<3;i++)
			out.put(char(0));
		
		//3.1.2.5 header size
		out.put(char(40));
		
		//3.1.2.6 unused space
		for (i=0;i<3;i++)
			out.put(char(0));
		
		//3.1.2.7 file width (trickier)
		out.put(char(hgrid%256));
		out.put(char((hgrid>>8)%256));
		out.put(char((hgrid>>16)%256));
		out.put(char((hgrid>>24)%256));
		
		//3.1.2.8 file height (trickier)
		out.put(char(vgrid%256));
		out.put(char((vgrid>>8)%256));
		out.put(char((vgrid>>16)%256));
		out.put(char((vgrid>>24)%256));
		
		//3.1.2.9 color planes
		out.put(char(1));
		out.put(char(0));
		
		//3.1.2.10 bit depth
		out.put(char(24));
		
		//3.1.2.11 the rest
		for (i=0;i<25;i++)
			out.put(char(0));
	
	//3.2 put in the elements of the array
	color newcolor(0,0,0);
	for (i=(vgrid-1);i>=0;i--){//bitmaps start with the bottom row, and work their way up...
		for (j=0;j<hgrid;j++){//...but still go left to right
			map[j][i]-=min;
			//if this point is below the floodline...
			if (map[j][i]<flood)
				newcolor=lerp(waterlow,waterhigh,map[j][i]/flood);
			
			//if this is above the mountain line...
			else if (map[j][i]>mount)
				newcolor=lerp(mountlow,mounthigh,(map[j][i]-mount)/(diff-mount));
			
			//if this is regular land
			else
				newcolor=lerp(landlow,landhigh,(map[j][i]-flood)/(mount-flood));
			
			out.put(char(newcolor.v[0]));//blue
			out.put(char(newcolor.v[1]));//green
			out.put(char(newcolor.v[2]));//red
		}
		//round off the row
		for (k=0;k<(hgrid%4);k++)
			out.put(char(0));
	}
	
	//3.3 end the file
	out.close();
}

void printPage(time_t beginning, time_t end)
{
	double timeTaken = beginning-end;
	cout<<"Content-Type: text/html\n\n"
		<<"<html><head><title>FTG Page</title></head>\n"
		<<"<body>\n"
		<<"<h2>Fractal Terrain Generator Page</h2>\n"
		<<"<img src=\"test.bmp\" /><br />\n"
		<<"This took " << timeTaken << " seconds to create.<br />\n"
		<<"</body>\n"
		<<"</html>\n";
}
*/
#endif
