class PlaneIntersection {
	public:
		PlaneIntersection(Ogre::Vector3 pos, Ogre::Ray mouseRay);
		Ogre::Vector3 getHeight(void);
		bool getTest(void);
	private:
//		Ogre::Vector3 getHeight(void);
//		bool getTest(void);
		bool ctest;
		Ogre::Vector3 cheight;
};

PlaneIntersection::PlaneIntersection(Ogre::Vector3 pos, Ogre::Ray mouseRay){
	//Ogre::Ray topLeft = mCamera->getCameraToViewportRay(left, top);
	Ogre::Vector3 dirvec = mouseRay.getDirection();
	//could use quarternions, but we can get orthogonal vectors, assuming
	// local x is in the direction of such vector
	// that a z rotation (2d) and a y rotation should yield orthogonal 
	//vector components.
	Ogre::Real sinsq = Ogre::Math::Sin(Ogre::Math::DegreesToRadians(90));
	Ogre::Real cossq = Ogre::Math::Cos(Ogre::Math::DegreesToRadians(90));
	//Ry = {{cost,0,sint},{0,1,0},{-sint,0,cost}}
	Ogre::Matrix3 Roty = Ogre::Matrix3(cossq,0,sinsq,0,1,0,-1*sinsq,0,cossq);
	//Rz = {{cost,-sint,0},{sint,cost,0},{0,0,1}}
	Ogre::Matrix3 Rotz = Ogre::Matrix3(cossq,-1*sinsq,0,sinsq,cossq,0,0,0,1);
	Ogre::Vector3 dirvecy = Roty*dirvec;  //rotated on y axis
	Ogre::Vector3 dirvecz = Rotz*dirvec;  //rotated on z axis
	Ogre::Ray rayy = Ogre::Ray(pos,dirvecy);
	Ogre::Ray rayz = Ogre::Ray(pos,dirvecz);
	Ogre::Vector3 point2 = rayy.getPoint(3);
	Ogre::Vector3 point3 = rayz.getPoint(3);
	
	Ogre::Plane tplane = Ogre::Plane(pos, point2, point3);         // front plane
	std::pair<bool,Ogre::Real> test = mouseRay.intersects(tplane);
	std::ostringstream ss;
	ctest = test.first;
	if (test.first){
		ss<<"Distance to intersection: "<<test.second << "\n";
		Ogre::Real tdist = test.second;
		Ogre::Vector3 theight = mouseRay.getPoint(tdist);
		cheight = theight;
		
	}
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("test.log");
	
//	ss.str(std::string());
	ss<<"Intersection of plane "<< test.first;
	tlog->logMessage(ss.str());
}

bool PlaneIntersection::getTest(){
	return ctest;
}

Ogre::Vector3 PlaneIntersection::getHeight(){
	return cheight;
}
