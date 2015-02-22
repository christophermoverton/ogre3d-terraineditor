#ifndef __ColorizeTerrainMap_CPP
#define __ColorizeTerrainMap_CPP
#include "TerrainStruct.h"
#include "TPoint3.h"
#include "Imagestuff.cpp"
#include "DiamondSquare/DiamondSquare.cpp"
/*
struct color{
	//v[0]=red, v[1]=green, v[2]=blue
	unsigned char v[3];
	
	color(unsigned char r, unsigned char g, unsigned char b){
		v[0]=r;
		v[1]=g;
		v[2]=b;
	}
};
*/
void colorizeterrainmap(double size, double texscale, double min, double max, 
			terr::CPointsMap * heightmap, double waterheight, double mountainheight);
void colorizeterrainmap(double size, double min, double max, Ogre::Terrain* terrain, double waterheight, double mountainheight);
/*
color lerp(color c1, color c2, float value);

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
*/
void colorizeterrainmap(double size, double texscale, double min, double max, 
			terr::CPointsMap * heightmap, double waterheight, double mountainheight){
   float diff = max-min,
	flood = waterheight,
	mount = mountainheight;	
    //flood=0.25f,//flood level
    //     mount=0.85f;//mountain level
	
   flood*=diff;
   mount*=diff;
	
   //int i,j,k;
   color landlow(0,64,0),
         landhigh(133,182,116),  //116 133
         waterlow(0,0,55),
	 waterhigh(0,53,106),
	 mountlow(167,157,147), //147 167
         mounthigh(216,223,226);  //226 216
   ImageBuffer buffer(size*texscale);
   FillColour* fill = new FillColour (&buffer);
   color newcolor(0,0,0);
   for(int x = 0; x<size; x++){
	for (int y = 0; y<size; y++){
		for (int k = 0; k < texscale; k++){
			
			double subinck = ((double) k)/texscale;
			for (int l = 0; l < texscale; l++){
				double subincl = ((double) l)/texscale;
				terr::Coordpair * coordpair = new terr::Coordpair((double)x+subinck,(double)y+subincl);
				double height = (*heightmap)[(*coordpair)];
				//double height = (*heightmap)[terr::Coordpair((double)x, (double)y)];
				//if this point is below the floodline...
				if (height<flood) {
					newcolor=lerp(waterlow,waterhigh,height/flood);
				}
				//if this is above the mountain line...
				else if (height>mount){
					newcolor=lerp(mountlow,mounthigh,(height-mount)/(diff-mount));
				}	
				//if this is regular land
				else{
					newcolor=lerp(landlow,landhigh,(height-flood)/(mount-flood));
				}
	    			Ogre::ColourValue col = Ogre::ColourValue(newcolor.v[0]/255.0f,newcolor.v[1]/255.0f,newcolor.v[2]/255.0f);
	    			fill->setPixl((size_t)(texscale*x+k), (size_t)(texscale*y+l), col);	
				if (x == size-1 and y == size-1){break;}
				if (y == size-1){break;}
			}
			if (x == size-1 and y == size-1){break;}
			if (x == size-1){break;}
		}
	}	
   }
   buffer.saveImage("../media/materials/textures/test10.png");
}

void colorizeterrainmap(double size, double min, double max, Ogre::Terrain* terrain, double waterheight, double mountainheight){
   //since this call for terrain data in original size if I add a texscale method on this will likely need to interpolate positions
   //on the terrain map since the a higher scale resolution of such data will not be furnished by the terrain by typical getheight
   //methods.  Generally I think I can solve this using Ogre's interpolation functions for terrain data.
   float diff = max-min,
	 //flood=0.25f,//flood level
         //mount=0.85f;//mountain level
	flood = waterheight,
	mount = mountainheight,
	diff2 = 1.0f;
   //flood = 1000.0f,
   //mount = 3200.0f;
   float negmount = mount;	
   //flood*=diff;
   //mount*=diff;
   if (max<0){mount = -flood; flood = -negmount;}
	
   //int i,j,k;
   color landlow(0,64,0),
         landhigh(133,182,116),  //116 133
         waterlow(0,0,55),
	 waterhigh(0,53,106),
	 mountlow(167,157,147), //147 167
         mounthigh(216,223,226);  //226 216
   ImageBuffer buffer(size); ImageBuffer buffer2(size);
   FillColour* fill =  new FillColour (&buffer); 
   FillColour* fill2 = new FillColour (&buffer2);
   color newcolor(0,0,0);
   for(int x = 0; x<size; x++){
	for (int y = 0; y<size; y++){
		double posx = x/size; double posy = y/size;
		double height = (double)terrain->getHeightAtTerrainPosition(posx,posy);
		height /= (2*diff); height += .5f;
		//if this point is below the floodline...
		if (height<flood) {
			newcolor=lerp(waterlow,waterhigh,height/flood);
		}
		//if this is above the mountain line...
		else if (height>mount){
			newcolor=lerp(mountlow,mounthigh,(height-mount)/(diff-mount));
		}	
		//if this is regular land
		else{
			newcolor=lerp(landlow,landhigh,(height-flood)/(mount-flood));
		}
	    	Ogre::ColourValue col = Ogre::ColourValue(newcolor.v[0]/255.0f,newcolor.v[1]/255.0f,newcolor.v[2]/255.0f);
		//diffuse with zero specularity 
		Ogre::ColourValue col2 = Ogre::ColourValue(newcolor.v[0]/255.0f,newcolor.v[1]/255.0f,newcolor.v[2]/255.0f, 0.0f);  
	    	fill->setPixl((size_t)x, (size_t)y, col);
		//fill2->setPixl((size_t)x, (size_t)y, col2);	
	}
   }
   buffer.saveImage("../media/materials/textures/test10.png");
   //buffer2.saveImage("../media/materials/textures/test11.png");
}
#endif
