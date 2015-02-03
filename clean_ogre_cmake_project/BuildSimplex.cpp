#ifndef __BuildSimplex_CPP
#define __BuildSimplex_CPP

#include "SimplexNoise.h"

#include "Array2D.h"
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <map>

class BuildSimplex{

	public:
		BuildSimplex();
};

void BuildSimplex::BuildSimplex{
        //get the command line arguments
        unsigned xgrid = 500;
        unsigned ygrid = 200;
        unsigned zgrid = 500;
        unsigned octaves = 16;
        unsigned rseed = unsigned(time(NULL));
        string fchunk = "part";
        float lacunarity = 2.0f;
        float gain = 0.65f;
        int kernel = 3;
        float phi = 0.8f;
        //set the random seed
        srand(rseed);
        
        //make the Simplex noise object
        SimplexNoise noise(octaves,gain,lacunarity);

        //make the height_map
        Array_2D<float> height_map(xgrid,ygrid);

        /**********NOISE TRACKING************/
        //loop!
        for (i = 0; i < zgrid; ++i)
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
}
#endif
