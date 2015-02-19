#ifndef __BuildNoise_CPP
#define __BuildNoise_CPP
#include <math.h>
//#include "terrainerosion/Math/PerlinNoise.h"
#include "noise.c"
#include "TerrainStruct.h"
#define PI              3.141592653589793238462643


//PerlinNoise * _perlin;
/*
double Noise3(void){
		//_perlin = new Perlin(513.0f,513.0f,2.0f);
	return 0.0f;
}

double Noise3(Vector point){
	return _perlin->Sample(point.x, point.y, point.z);
}
*/


double pfBm(Vector point, double frequency, double amplitude, double octaves, double gain);
//float size, float scale, float zdepth, float frequency, float amplitude, float octaves=6, float gain = .5f
terr::CPointsMap  pfbmbuild (double size, double scale, double frequency, double amplitude, double octaves, double gain);
double sin_bias(double a);
double tri_bias(double a);
double saw_bias(double a);
double soft(double a);
double sharp(double a);
double sharper(double a);
double shapes(double x,double y, int shape=0);
terr::CPointsMap   BLI_gNoisebuild(double size, double scale, int hard, int noisebasis);
terr::CPointsMap mg_MultiFractalbuild(double size, double scale, double H, double lacunarity, double octaves, int noisebasis);
terr::CPointsMap mg_HeteroTerrainbuild(double size, double scale, float H, float lacunarity, float octaves, float offset, int noisebasis);
terr::CPointsMap mg_HybridMultiFractalbuild(double size, double scale, float H, float lacunarity, float octaves, float offset, float gain, int noisebasis);
terr::CPointsMap mg_RidgedMultiFractalbuild(double size, double scale, float H, float lacunarity, float octaves, float offset, float gain, int noisebasis);
double marble_noise(double x, double y, double z, Vector origin, double size, 
                    int shape, int bias, int sharpnes, int noisebasis, 
		    double turb, double depth, double frequency);
terr::CPointsMap  pmarblebuild (double size, double scale, double frequency, double amplitude, double octaves,
				int noisebasis, int bias, int shape, int sharp);
//scale, frequency, amplitude, octaves, and gain are dummy variables (unused) for now in pmarblebuild    

//# some functions for marble_noise
double sin_bias(double a){
    return 0.5f + 0.5f * sin(a);
}
double tri_bias(double a){
    double b = 2.0f * PI;
    a = 1.0f - 2.0f * abs(floor((a * (1.0f/b))+0.5f) - (a*(1.0f/b)));
    return a;
}
double saw_bias(double a){
    double b = 2.0f * PI;
    int n = (int)(a/b);
    a -= n * b;
    if (a < 0) { a += b;}
    return a / b;
}

double soft(double a){
    return a;
}

double sharp(double a){
    return pow(a,0.5f);
}

double sharper(double a){
    return sharp(sharp(a));
}

double shapes(double x,double y, int shape){
    double s;
    if (shape == 1){
        //# ring
        x = x*2.0f;
        y = y*2.0f;
        s = (-cos(x*x+y*y)/(x*x+y*y+0.5));
    }
    else if (shape == 2){
        //# swirl
        x = x*2;
        y = y*2;
        s = (( x*sin( x*x+y*y ) + y*cos( x*x+y*y ) ) / (x*x+y*y+0.5f));
    }
    else if (shape == 3){
        //# bumps
        x = x*2.0f;
        y = y*2.0f;
        s = ((cos( x*PI ) + cos( y*PI ))-0.5f);
    }
    else if (shape == 4){
        //# y grad.
        s = (y*PI);
    }
    else if (shape == 5){
        //# x grad.
        s = (x*PI);
    }
    else{
        //# marble
        s = ((x+y)*5.0f);
    }
    return s;
}
//# marble_noise
double marble_noise(double x, double y, double z, Vector origin, double size, 
                    int shape, int bias, int sharpnes, int noisebasis, 
		    double turb, double depth, double frequency){
    x = x / size;
    y = y / size;
    z = z / size;
    double s = shapes(x,y,shape);

    x += origin.x;
    y += origin.y;
    z += origin.z;
    //Noise3( point )
    double h = 0.0f; double f = 1.0f;//frequency;//= 0.05f;
    Vector  point; point.x = x*f; point.y = y*f; point.z = z;
    double amp = 1.0f;
    //for (int i = 0; i < depth; i++) {
    //  h += Noise3(point)*amp; f /= 2.0f; point.x *= f; point.y *= f;
    //  amp *= 2.0f;
      //h += Noise3(point)*2.0f; f /= 2.0f; point.x *= f; point.y *= f;
      //h += Noise3(point)*4.0f; f /= 2.0f; point.x *= f; point.y *= f;
      //h += Noise3(point)*8.0f; f /= 2.0f;
    //}
    h = mg_fBm(x,y,z,0.8f, 2.0f, depth, noisebasis);
    double value = s + turb * h;//*4*1.3;

    if (bias == 1){
        value = tri_bias( value );
    }
    else if (bias == 2){
        value = saw_bias( value );
    }
    else{
        value = sin_bias( value );
    }
    if (sharpnes == 1){
        value = sharp( value );
    }
    else if (sharpnes == 2){
        value = sharper( value );
    }
    else{
        value = soft( value );
    }
    return value;
}

terr::CPointsMap  pmarblebuild (double size, double scale, double frequency, double amplitude, double octaves,
				int noisebasis, int bias, int shape, int sharp){
   _perlin = new PerlinNoise();
   terr::CPointsMap * points = new terr::CPointsMap();
   double maxval = (double)0.0f, minval = (double)100000000.0f; double val;
   for (int i = 0; i < size; i++){
	for (int j = 0; j < size; j++){
		terr::Coordpair * coordpair = new terr::Coordpair(i,j);
                //delta = size_me / (sub_d - 1)  size_me is mesh size and sub_d is the number of subdivision
                double x = (double) i * 513.0f/(512.0f);
		double y = (double) j * 513.0f/(512.0f);
		x = (double) x* 2.0f/ 513.0f;
		y = (double) y* 2.0f/ 513.0f;
		double z = 0.0f;
		Vector  point; point.x = x; point.y = y; point.z = z;
                Vector origin; origin.x = 0.0f; origin.y = 0.0f; origin.z = 0.0f;
                //(double x, double y, double z, Vector origin, double size, 
                    //int shape, int bias, int sharpnes, double turb, double depth )
		val = marble_noise( x, y, z, origin, scale, shape, bias, sharp, noisebasis,amplitude, octaves, frequency);
		(*points)[(*coordpair)] = val;
		if (val < minval){
			minval = val;
		}
		if (val > maxval){
			maxval = val;
		}		
	}
   }
   double scaleceiling;
   if (abs(minval)< abs(maxval)){
	scaleceiling = abs(maxval);
   }
   else { scaleceiling = abs(minval);}
   if (scaleceiling > 1){
   	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			terr::Coordpair * coordpair = new terr::Coordpair(i,j);
			(*points)[(*coordpair)] /= scaleceiling;
		}
   	}
   }   
   return (*points);
}

terr::CPointsMap   BLI_gNoisebuild(double size, double scale, int hard, int noisebasis){
   terr::CPointsMap * points = new terr::CPointsMap();
   double maxval = (double)0.0f, minval = (double)100000000.0f; double val;
   for (int i = 0; i < size; i++){
	for (int j = 0; j < size; j++){
		terr::Coordpair * coordpair = new terr::Coordpair(i,j);
		double x = (double) i ;/// scale;
		double y = (double) j ;/// scale;
		double z = 0.0f;
		Vector  point; point.x = x; point.y = y; point.z = z;
                //float mg_fBm(float x, float y, float z, float H, float lacunarity, float octaves, int noisebasis);
		val = BLI_gNoise(scale, x, y, z, hard, noisebasis);
		//val = (double) pfBm( point, frequency, amplitude, octaves, gain);
		(*points)[(*coordpair)] = val;
		if (val < minval){
			minval = val;
		}
		if (val > maxval){
			maxval = val;
		}		
	}
   }
   double scaleceiling;
   if (abs(minval)< abs(maxval)){
	scaleceiling = abs(maxval);
   }
   else { scaleceiling = abs(minval);}
   if (scaleceiling > 1){
   	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			terr::Coordpair * coordpair = new terr::Coordpair(i,j);
			(*points)[(*coordpair)] /= scaleceiling;
		}
   	}
   }   
   return (*points);
	
}

terr::CPointsMap mg_MultiFractalbuild(double size, double scale, double H, double lacunarity, double octaves, int noisebasis){
   terr::CPointsMap * points = new terr::CPointsMap();
   double maxval = (double)0.0f, minval = (double)100000000.0f; double val;
   for (int i = 0; i < size; i++){
	for (int j = 0; j < size; j++){
		terr::Coordpair * coordpair = new terr::Coordpair(i,j);
		double x = (double) i / scale;
		double y = (double) j / scale;
		double z = 0.0f;
		Vector  point; point.x = x; point.y = y; point.z = z;
                //float mg_fBm(float x, float y, float z, float H, float lacunarity, float octaves, int noisebasis);
		val = mg_MultiFractal(x, y, z, H, lacunarity, octaves, noisebasis);
		//val = (double) pfBm( point, frequency, amplitude, octaves, gain);
		(*points)[(*coordpair)] = val;
		if (val < minval){
			minval = val;
		}
		if (val > maxval){
			maxval = val;
		}		
	}
   }
   double scaleceiling;
   if (abs(minval)< abs(maxval)){
	scaleceiling = abs(maxval);
   }
   else { scaleceiling = abs(minval);}
   if (scaleceiling > 1){
   	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			terr::Coordpair * coordpair = new terr::Coordpair(i,j);
			(*points)[(*coordpair)] /= scaleceiling;
		}
   	}
   }   
   return (*points);
}

terr::CPointsMap mg_HeteroTerrainbuild(double size, double scale, float H, float lacunarity, float octaves, float offset, int noisebasis){
    //mg_HeteroTerrain(float x, float y, float z, float H, float lacunarity, float octaves, float offset, int noisebasis)
   terr::CPointsMap * points = new terr::CPointsMap();
   double maxval = (double)0.0f, minval = (double)100000000.0f; double val;
   for (int i = 0; i < size; i++){
	for (int j = 0; j < size; j++){
		terr::Coordpair * coordpair = new terr::Coordpair(i,j);
		double x = (double) i / scale;
		double y = (double) j / scale;
		double z = 0.0f;
		Vector  point; point.x = x; point.y = y; point.z = z;
                //float mg_fBm(float x, float y, float z, float H, float lacunarity, float octaves, int noisebasis);
		val = mg_HeteroTerrain(x, y, z, H, lacunarity, octaves, offset, noisebasis);
		//val = (double) pfBm( point, frequency, amplitude, octaves, gain);
		(*points)[(*coordpair)] = val;
		if (val < minval){
			minval = val;
		}
		if (val > maxval){
			maxval = val;
		}		
	}
   }
   double scaleceiling;
   if (abs(minval)< abs(maxval)){
	scaleceiling = abs(maxval);
   }
   else { scaleceiling = abs(minval);}
   if (scaleceiling > 1){
   	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			terr::Coordpair * coordpair = new terr::Coordpair(i,j);
			(*points)[(*coordpair)] /= scaleceiling;
		}
   	}
   }   
   return (*points);
}

terr::CPointsMap mg_HybridMultiFractalbuild(double size, double scale, float H, float lacunarity, float octaves, float offset, float gain, int noisebasis){
    //mg_HeteroTerrain(float x, float y, float z, float H, float lacunarity, float octaves, float offset, int noisebasis)
   terr::CPointsMap * points = new terr::CPointsMap();
   double maxval = (double)0.0f, minval = (double)100000000.0f; double val;
   for (int i = 0; i < size; i++){
	for (int j = 0; j < size; j++){
		terr::Coordpair * coordpair = new terr::Coordpair(i,j);
		double x = (double) i / scale;
		double y = (double) j / scale;
		double z = 0.0f;
		Vector  point; point.x = x; point.y = y; point.z = z;
                //float mg_fBm(float x, float y, float z, float H, float lacunarity, float octaves, int noisebasis);
		val = mg_HybridMultiFractal(x,y,z, H, lacunarity, octaves, offset, gain, noisebasis);
		//val = (double) pfBm( point, frequency, amplitude, octaves, gain);
		(*points)[(*coordpair)] = val;
		if (val < minval){
			minval = val;
		}
		if (val > maxval){
			maxval = val;
		}		
	}
   }
   double scaleceiling;
   if (abs(minval)< abs(maxval)){
	scaleceiling = abs(maxval);
   }
   else { scaleceiling = abs(minval);}
   if (scaleceiling > 1){
   	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			terr::Coordpair * coordpair = new terr::Coordpair(i,j);
			(*points)[(*coordpair)] /= scaleceiling;
		}
   	}
   }   
   return (*points);
}

terr::CPointsMap mg_RidgedMultiFractalbuild(double size, double scale, float H, float lacunarity, float octaves, float offset, float gain, int noisebasis){
   terr::CPointsMap * points = new terr::CPointsMap();
   double maxval = (double)0.0f, minval = (double)100000000.0f; double val;
   for (int i = 0; i < size; i++){
	for (int j = 0; j < size; j++){
		terr::Coordpair * coordpair = new terr::Coordpair(i,j);
		double x = (double) i / scale;
		double y = (double) j / scale;
		double z = 0.0f;
		Vector  point; point.x = x; point.y = y; point.z = z;
                //float mg_fBm(float x, float y, float z, float H, float lacunarity, float octaves, int noisebasis);
		val = mg_RidgedMultiFractal(x,y,z, H, lacunarity, octaves, offset, gain, noisebasis);
		//val = (double) pfBm( point, frequency, amplitude, octaves, gain);
		(*points)[(*coordpair)] = val;
		if (val < minval){
			minval = val;
		}
		if (val > maxval){
			maxval = val;
		}		
	}
   }
   double scaleceiling;
   if (abs(minval)< abs(maxval)){
	scaleceiling = abs(maxval);
   }
   else { scaleceiling = abs(minval);}
   if (scaleceiling > 1){
   	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			terr::Coordpair * coordpair = new terr::Coordpair(i,j);
			(*points)[(*coordpair)] /= scaleceiling;
		}
   	}
   }   
   return (*points);
}

terr::CPointsMap  pfbmbuild (double size, double scale, double frequency, double amplitude, double octaves, double gain){
   _perlin = new PerlinNoise();
   terr::CPointsMap * points = new terr::CPointsMap();
   double maxval = (double)0.0f, minval = (double)100000000.0f; double val;
   for (int i = 0; i < size; i++){
	for (int j = 0; j < size; j++){
		terr::Coordpair * coordpair = new terr::Coordpair(i,j);
		double x = (double) i / scale;
		double y = (double) j / scale;
		double z = 0.0f;
		Vector  point; point.x = x; point.y = y; point.z = z;
                //float mg_fBm(float x, float y, float z, float H, float lacunarity, float octaves, int noisebasis);
		val = mg_fBm(x,y,z,frequency, 2.0f, octaves, 0);
		//val = (double) pfBm( point, frequency, amplitude, octaves, gain);
		(*points)[(*coordpair)] = val;
		if (val < minval){
			minval = val;
		}
		if (val > maxval){
			maxval = val;
		}		
	}
   }
   double scaleceiling;
   if (abs(minval)< abs(maxval)){
	scaleceiling = abs(maxval);
   }
   else { scaleceiling = abs(minval);}
   if (scaleceiling > 1){
   	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			terr::Coordpair * coordpair = new terr::Coordpair(i,j);
			(*points)[(*coordpair)] /= scaleceiling;
		}
   	}
   }   
   return (*points);
}

double
pfBm( Vector point, double freq, double amplitude, double octaves, double gain)
{

      double            value;//, Noise3();
      //int               i;
      //static int        first = TRUE;


      value = 0.0;            /* initialize vars to proper values */
      double frequency = 1.0;//*freq;
      double amp = 1.0f;//amplitude;
      point.x *= frequency;
      point.y *= frequency;
      point.z *= frequency;

      /* inner loop of spectral construction */
      for (int i=0; i<octaves; i++) {
            //value += Noise3( point ) * amp;
	    frequency /= 2.0f;	
            point.x *= frequency;
            point.y *= frequency;
            point.z *= frequency;
	    amp *= 2.0f;//gain;
      } /* for */


      return( value );

} /* fBm() */

#endif
