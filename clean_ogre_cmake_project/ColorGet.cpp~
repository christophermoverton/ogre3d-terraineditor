#ifndef __ColorGet_CPP
#define __ColorGet_CPP

class ColorGet{
	public:
		//ColorGet(void);
		ColorGet(float normval);
		Ogre::ColourValue getCol(void);
		double decode(Ogre::ColourValue col);
	private:
		Ogre::ColourValue ccolour;
};

//ColorGet::ColorGet(){
//}
//#ifndef __ColorGet_h
//#define __ColorGet_h

ColorGet::ColorGet(float normval){
	//we assume normalval is supplied in the range of [0,1]
	double val = (double)normval*(double)4228250625.0f;
	//convert to rgba channel 
	double Apre = val/double(pow(255.0f,3));
	int A = (int)Apre;
	double Aremain = Apre-(double)A;
	double Bpre = Aremain*(double)255.0f;
	int B = (int)Bpre;
	double Bremain = Bpre-(double)B;
	double Gpre = Bremain*(double)255.0f;
	int G = (int)Gpre;
	double Gremain = Gpre-(double)G;
	double Rpre = Gremain*(double)255.0f;
	int R = (Rpre-(int)Rpre <0.5f ? (int)Rpre : (int)Rpre+1);
	ccolour = Ogre::ColourValue(R,G,B,A);
}

Ogre::ColourValue ColorGet::getCol(){
	return ccolour;
}

double ColorGet::decode(Ogre::ColourValue col){
	double retval;
	float A = col.a;
	float R = col.r;
	float G = col.g;
	float B = col.b;
	//r+255*g+255^2*b+255^3*a                             4294967295
	retval = ((double)R+double(255.0f*G)+double(pow(255.0f,2))*(double)B+double(pow(255.0f,3))*(double)A)/(double)4228250625.0f;
	return retval;
}

#endif // #ifndef __ITutorial02_h_
