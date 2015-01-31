#ifndef __PerlinGenerator_CPP
#define __PerlinGenerator_CPP
#include "TerrainStruct.h"
#include "Perlin.cpp"
#include <vector>
using namespace std;
class PerlinGenerator{
	public:
		//PerlinGenerator(void);
		PerlinGenerator(float size, float scale);
		PerlinGenerator(float size, float scale, float zdepth);
		PerlinGenerator(float size, float scale, float zdepth, float frequency ,float amplitude,float octaves, float gain);
		vector<vector<double> > getNoisevalues(void);
		vector<vector<vector<double> > > getNoisevalues3d(void);
		terr::3dCPointsMap getNoisevalues3d(void);
	private:  
		double cscale;
		// 513 x 513 
		double csize;
		//Perlin* perlina;
		double zdepth;
		vector<vector<double> > cnoisevals;
		vector<vector<vector<double> > > cnoisevals3d;
		terr::3dCPointsMap * tcnoisevals3d;
		vector<vector<vector<vector<double> > > > cnoisepartials;				
};

PerlinGenerator::PerlinGenerator(float size, float scale){
	//default size = 513
//	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("Perlin.log");
	csize = size;
	cscale = scale;
	int maxnode = ((int)size)/scale + 2;
	Perlin* perlina = new Perlin(maxnode, maxnode);
/*
	std::ostringstream ss5;
	ss5<< "Test2" << "\n";
	ss5<< "Scale: "<< scale<<"\n";
	ss5<< "Size: " << size<< "\n";
	ss5<< "MaxNode: "<< maxnode << "\n";
	tlog->logMessage(ss5.str());
*/
	double maxval = (double)0.0f, minval = (double)0.0f;
	vector<vector<double> > noiseval((int)size); 
	for (int i = 0; i < size; i++){
		noiseval[i].resize(size);
		for(int j = 0; j < size; j++){
			double x = i/scale;
			double y = j/scale;
/*
			ss5.str(std::string());
			ss5<< "Test3: " << i<< ","<< j<< "\n";
			ss5<< "pos: " << x << ","<< y <<"\n";
			tlog->logMessage(ss5.str());
*/
			double val = perlina->getNoiseValue(x,y);
			if (val < minval){
				minval = val;
			}
			if (val > maxval){
				maxval = val;
			}
			noiseval[i][j] = val;
		}
	}
	//std::ostringstream ss5;
/*
	ss5.str(std::string());
	ss5<< "Test4" << "\n";
	tlog->logMessage(ss5.str());
*/
	//we assume that at least one value will be inclose proximity to 
	//a ceiling range of possible noise values here...
	//we can however limits the ceiling of values on greyscale returns further.
	//rescale values...we don't know what the possible range 
	//is formally on the set of returned noise values
	//so we test the min and max values for which ever
	//has the greatest magnitude 
	double scaleceiling;
	if (abs(minval)< abs(maxval)){
	//floor maxval to an int and then add 1 for ceiling.
		if ((int)maxval == abs(maxval)){
			scaleceiling = abs(maxval);
		}
		else{
			scaleceiling = (int)abs(maxval)+1;
		}
	}
	else{
		if (abs((int)minval) == abs(minval)){
			scaleceiling = abs(minval);
		}
		else{
			scaleceiling = (int)abs(minval)+1;
		}
	}
	//now we reiterate all noise points and divide by scaleceiling 
	//to ensure all points are within the range [-1,1]
	// divide this value by 1/2 to put it within range [-1/2, 1/2]
	// and then translate the value by +1/2 to put it within range 
	// [0,1]
	for (int i = 0; i < size; i++){	
		for(int j = 0; j < size; j++){
			noiseval[i][j] = (noiseval[i][j]/scaleceiling)/2 + .5;
		}
	}
	cnoisevals = noiseval;		
}

PerlinGenerator::PerlinGenerator(float size, float scale, float zdepth){
	//default size = 513
//	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("Perlin.log");
	csize = (double)size;
	cscale = (double)scale;
	int maxnode = ((int)size)/(int)scale + 2;
	Perlin* perlina = new Perlin(maxnode, maxnode, (int)zdepth);
	
/*
	std::ostringstream ss5;
	ss5<< "Test2" << "\n";
	ss5<< "Scale: "<< scale<<"\n";
	ss5<< "Size: " << size<< "\n";
	ss5<< "MaxNode: "<< maxnode << "\n";
	tlog->logMessage(ss5.str());
*/
	double maxval = (double)0.0f, minval = (double)0.0f;
	vector<vector<vector<double> > > noiseval((int)size);
	vector<vector<vector<vector<double> > > > noisepartials((int)size);
	//vector<float> retval(4); 
	for (int i = 0; i < size; i++){
		noiseval[i].resize((int)size);
		noisepartials[i].resize((int)size);
		for(int j = 0; j < size; j++){
		   noiseval[i][j].resize((int)zdepth);
		   noisepartials[i][j].resize((int)zdepth);
		   for (int k = 0; k < zdepth-1; k++){
			noisepartials[i][j][k].resize(3);
			double x = i/scale;
			double y = j/scale;
			double z = k/scale;

/*
			ss5.str(std::string());
			ss5<< "Test3: " << i<< ","<< j<< "\n";
			ss5<< "pos: " << x << ","<< y <<"\n";
			tlog->logMessage(ss5.str());
*/
			double val = perlina->getNoiseValue(x,y,z);
			
	//		perlina->dnoise3f( &retval, x, y, z );
	//		noiseval[i][j][k] = retval[0];
//			float val = retval[0];
	//		noisepartials[i][j][k][1] = retval[1];
	//		noisepartials[i][j][k][2] = retval[2];
	//		noisepartials[i][j][k][3] = retval[3];
			if (val < minval){
				minval = val;
			}
			if (val > maxval){
				maxval = val;
			}
			noiseval[i][j][k] = val;
		   }
		}
	}
	//std::ostringstream ss5;
/*
	ss5.str(std::string());
	ss5<< "Test4" << "\n";
	tlog->logMessage(ss5.str());
*/
	//we assume that at least one value will be inclose proximity to 
	//a ceiling range of possible noise values here...
	//we can however limits the ceiling of values on greyscale returns further.
	//rescale values...we don't know what the possible range 
	//is formally on the set of returned noise values
	//so we test the min and max values for which ever
	//has the greatest magnitude 
	float scaleceiling;
	if (abs(minval)< abs(maxval)){
	//floor maxval to an int and then add 1 for ceiling.
		if ((int)maxval == abs(maxval)){
			scaleceiling = abs(maxval);
		}
		else{
			scaleceiling = (int)abs(maxval)+1;
		}
	}
	else{
		if (abs((int)minval) == abs(minval)){
			scaleceiling = abs(minval);
		}
		else{
			scaleceiling = (int)abs(minval)+1;
		}
	}
	//now we reiterate all noise points and divide by scaleceiling 
	//to ensure all points are within the range [-1,1]
	// divide this value by 1/2 to put it within range [-1/2, 1/2]
	// and then translate the value by +1/2 to put it within range 
	// [0,1]
	for (int i = 0; i < size; i++){	
		for(int j = 0; j < size; j++){
		   for(int k = 0; k < (int)zdepth-1; k++){
			//noiseval[i][j][k] = (noiseval[i][j][k]/scaleceiling)/2 + .5;
			noiseval[i][j][k] = (noiseval[i][j][k]/scaleceiling);
		   }
		}
	}
	cnoisevals3d = noiseval;
	//cnoisepartials = noisepartials;		
}

PerlinGenerator::PerlinGenerator(float size, float scale, float zdepth, float frequency, float amplitude, float octaves=6, float gain = 1.5f){
	//frequency is typically a number ranging inside the set (0,1].
	//default size = 513
	tcnoisevals3d = new terr::3dCPointsMap(); 
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("Perlin.log");
	csize = (double)size;
	cscale = (double)scale; //could be values likely ranging from 1.0f to size
	double cfreq = (double)frequency;
	double camp = (double)amplitude; 
	//int maxnode = ((int)size)/scale + 2;
	int maxnode = (int)size;
	//float frequency = 1.0f/((float)maxnode-1.0f);
	//float frequency = 1.0f/(size);
	Perlin* perlina = new Perlin(maxnode, maxnode, (int)zdepth);
/*
	std::ostringstream ss5;
	ss5<< "Test2" << "\n";
	ss5<< "Scale: "<< scale<<"\n";
	ss5<< "Size: " << size<< "\n";
	ss5<< "MaxNode: "<< maxnode << "\n";
	tlog->logMessage(ss5.str());
*/
	double maxval = (double)0.0f, minval = (double)0.0f;
	vector<vector<vector<double> > > noiseval((int)size);
	vector<vector<vector<vector<double> > > > noisepartials((int)size);
	//vector<float> retval(4); 
	for (int i = 0; i < size; i++){
		noiseval[i].resize((int)size);
		noisepartials[i].resize((int)size);
		for(int j = 0; j < size; j++){
		   noiseval[i][j].resize((int)zdepth);
		   noisepartials[i][j].resize((int)zdepth);
		   for (int k = 0; k < zdepth-1; k++){
			noisepartials[i][j][k].resize(3);
			double x = (double)i/scale;
			double y = (double)j/scale;
			double z = (double)k/scale;

/*
			ss5.str(std::string());
			ss5<< "Test3: " << i<< ","<< j<< "\n";
			ss5<< "pos: " << x << ","<< y <<"\n";
			tlog->logMessage(ss5.str());
*/
			//implementing Fbm here
			double val = (double)0;
//			float frequency = 1.0f/((float)maxnode-1.0f);
			double frequency = cfreq;
			double lacunarity = 2.0f; //ranges from (1.8715 to 2.1042) in dealing with artifacts
//			float gain = 1.5f; //standard .5
//			float amplitude = gain;
			double amplitude = camp;
			double initval = 0.0f;
			double dx = 0.0f;
			double dy = 0.0f;
			double partvalx = 0.0f;
			double partvaly = 0.0f;
			for (int l = 0; l < octaves; l++){
/*
				ss5.str(std::string());
				ss5<< "Test3: " << "\n";
				ss5<< "pos fbm: " << x*frequency << ","<< y*frequency <<"\n";
				tlog->logMessage(ss5.str());				
*/
				initval = perlina->getNoiseValue(x * frequency, y * frequency, z*frequency);
				partvalx = perlina->get3dNoisePartials()[0];
				partvaly = perlina->get3dNoisePartials()[1];
				dx += partvalx;
				dy += partvaly;
			        val += perlina->getNoiseValue(x * frequency, y * frequency, z*frequency) * amplitude/(1+dx*dx+dy*dy);         
        			frequency *= lacunarity;
        			amplitude *= gain;
			}
			//float val = perlina->getNoiseValue(x,y,z);
			
	//		perlina->dnoise3f( &retval, x, y, z );
	//		noiseval[i][j][k] = retval[0];
//			float val = retval[0];
	//		noisepartials[i][j][k][1] = retval[1];
	//		noisepartials[i][j][k][2] = retval[2];
	//		noisepartials[i][j][k][3] = retval[3];
			if (val < minval){
				minval = val;
			}
			if (val > maxval){
				maxval = val;
			}
			noiseval[i][j][k] = val;
		   }
		}
	}
	//std::ostringstream ss5;
/*
	ss5.str(std::string());
	ss5<< "Test4" << "\n";
	tlog->logMessage(ss5.str());
*/
	//we assume that at least one value will be inclose proximity to 
	//a ceiling range of possible noise values here...
	//we can however limits the ceiling of values on greyscale returns further.
	//rescale values...we don't know what the possible range 
	//is formally on the set of returned noise values
	//so we test the min and max values for which ever
	//has the greatest magnitude 
	double scaleceiling;
	if (abs(minval)< abs(maxval)){
	//floor maxval to an int and then add 1 for ceiling.
		if ((int)maxval == abs(maxval)){
			scaleceiling = abs(maxval);
		}
		else{
			scaleceiling = (int)abs(maxval)+1;
		}
	}
	else{
		if (abs((int)minval) == abs(minval)){
			scaleceiling = abs(minval);
		}
		else{
			scaleceiling = (int)abs(minval)+1;
		}
	}
	//now we reiterate all noise points and divide by scaleceiling 
	//to ensure all points are within the range [-1,1]
	// divide this value by 1/2 to put it within range [-1/2, 1/2]
	// and then translate the value by +1/2 to put it within range 
	// [0,1]
	for (int i = 0; i < size; i++){	
		for(int j = 0; j < size; j++){
		   for(int k = 0; k < (int)zdepth-1; k++){
			//noiseval[i][j][k] = (noiseval[i][j][k]/scaleceiling)/2 + .5;
			terr::3dCoord * coord = new terr::3dCoord(i,j,k);
			noiseval[i][j][k] = (noiseval[i][j][k]/scaleceiling);
			(*tcnoisevals3d)[(*coord)] = (noiseval[i][j][k]/scaleceiling);
			
		   }
		}
	}
	cnoisevals3d = noiseval;
	//cnoisepartials = noisepartials;
	
}

vector<vector<double> > PerlinGenerator::getNoisevalues(){
	return cnoisevals;
}

vector<vector<vector<double> > > PerlinGenerator::getNoisevalues3d(){
	return cnoisevals3d;
}

terr::3dCPointsMap PerlinGenerator::getNoisevalues3d(){
	return (*tcnoisevals3d);
}

#endif
