#include <OgreMatrix4.h>


class Falloffinterpolate{
	public:
		double tparam, crfalloff, ctramount;
		Ogre::Vector3 cpoint;
		Ogre::Vector3 cipoint;  //interpolated position
		Ogre::Vector3 ccpoint;  //center point for falloff radii
		Ogre::Vector4 cicoeff;
		Falloffinterpolate(Ogre::Vector3 point, Ogre::Vector3 clpoint, double rfalloff, Ogre::Vector4 icoeff, double tramount);
		Falloffinterpolate(std::vector<Ogre::Vector3> falloffSelection, Ogre::Vector3 clpoint,
		double rfalloff, Ogre::Vector4 icoeff, double tramount);
		~Falloffinterpolate(void);
		Ogre::Vector3 getIpoint(void);
		std::vector<Ogre::Vector3> getIpoints(void);
	private:
		double computetparam(void);  //normalized distance measure from ccpoint to cpoint.
		Ogre::Vector3 computeinterp(void);
		std::vector<Ogre::Vector3> cfalloffSelection;
		//std::vector<Ogre::Real> Ipoints;
		std::vector<Ogre::Vector3> Ipoints;

};

Falloffinterpolate::Falloffinterpolate(Ogre::Vector3 point, Ogre::Vector3 clpoint, double rfalloff, Ogre::Vector4 icoeff, double tramount){
	cpoint = point;
	ccpoint = clpoint;
	cicoeff = icoeff;
	crfalloff = rfalloff;
	ctramount = tramount;
	tparam = computetparam();
	cipoint = computeinterp();
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("test.log");
	std::ostringstream ss;
	ss<<"Faloffinterpolation********\n";
	ss<<"cpoint: "<< cpoint << "\n";
	ss<<"ccpoint: "<< ccpoint << "\n";
	ss<<"ctramount: "<< ctramount << "\n";
	tlog->logMessage(ss.str());
}

Falloffinterpolate::Falloffinterpolate(std::vector<Ogre::Vector3> falloffSelection, Ogre::Vector3 clpoint, double rfalloff, Ogre::Vector4 icoeff, double tramount){
	//cpoint = point;
	cfalloffSelection = falloffSelection;
	ccpoint = clpoint;
	cicoeff = icoeff;
	crfalloff = rfalloff;
	ctramount = tramount;
	Ipoints.resize(falloffSelection.size());
	std::ostringstream ss;
	for (int i = 0; i < falloffSelection.size(); i++){
		cpoint = falloffSelection[i];
		tparam = computetparam();
		cipoint = computeinterp();
		Ipoints[i] = Ogre::Vector3(cipoint.x,cipoint.y,cipoint.z);
		//ss<<"tparam: "<<tparam<<"\n";
		//ss<<"translate point:  "<<cipoint.y<<"\n";
	}
	//tparam = computetparam();
	//cipoint = computeinterp();
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("test.log");
	
	ss<<"Faloffinterpolation********\n";
	ss<<"cpoint: "<< cpoint << "\n";
	ss<<"ccpoint: "<< ccpoint << "\n";
	ss<<"ctramount: "<< ctramount << "\n";
	ss<<"coefficients: "<<cicoeff<<"\n";
	ss<<"compute 2^2: " << pow(-2.0,2) << "\n";
	ss<<"coefficent w: " << cicoeff.w << "\n";
	tlog->logMessage(ss.str());
}
Falloffinterpolate::~Falloffinterpolate(){
}

double Falloffinterpolate::computetparam(){
	//tparam is normalized relative crfalloff.  This should be a value between 0 and 1.
	// distance measure for tparam is done on xz plane strictly...we neglect height 
	//when selecting a set of vertices.
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("test.log");
	std::ostringstream ss;
	ss << "point 1:  "<< cpoint.x<<","<< cpoint.z << "\n";
	ss << "center point: "<< ccpoint.x<< ","<<ccpoint.z << "\n";
	tlog->logMessage(ss.str());
	return 1-(pow(pow(abs(cpoint.x - ccpoint.x), 2)+pow(abs(cpoint.z - ccpoint.z), 2), .5) / crfalloff); 
}

Ogre::Vector3 Falloffinterpolate::computeinterp(){
	//f(x_i)=a+bx_i+cx_i^2+dx_i^3. is the ordering on coefficients 
	//for the 2 node cspline method that we call upon.
	double x = cicoeff[0] + cicoeff[1]*abs(tparam) + cicoeff[2]*pow(tparam,2) + cicoeff[3]*pow(abs(tparam),3);
	//next we rescale this by the translation factor ctramount.
	double hpos = abs(x)*ctramount; //or height position
	//double hpos = tparam*ctramount;
	//next we translate this to the existing position and return this.
	//Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("test.log");
	//std::ostringstream ss;
	//ss << "translate amount:  "<< hpos << "\n";
	//tlog->logMessage(ss.str());

	return Ogre::Vector3(cpoint.x, cpoint.y+hpos, cpoint.z);
}

Ogre::Vector3 Falloffinterpolate::getIpoint(){
	return cipoint;
}

std::vector<Ogre::Vector3> Falloffinterpolate::getIpoints(){
	return Ipoints;
}
