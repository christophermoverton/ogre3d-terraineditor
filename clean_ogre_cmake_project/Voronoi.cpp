#ifndef __Voronoi_CPP
#define __Voronoi_CPP
//#include <math>       /* cos */
#include "cspline.cpp"
#include <vector>

#include <cmath>
#define PI 3.14159265

class Voronoi {
	public:
		Voronoi(int points, int size);
		vector<vector<vector<double> > > getHeightMap(double falloffdist, double tramount);
		vector<vector<vector<double> > > getHeightMapradial(double tramount);
		vector<vector<vector<double> > > getSimpleHeightMap(void);
	private:
		Ogre::Log* tlog;
		bool angleTest(vector<double> npos2, vector<double> pos);
		void buildrandomMap(void);
		double distance(vector<double> point1, vector<double> point2);
		vector<double> randompoint(int size);
		vector<double> directionVector(vector<double> point1, vector<double> point2);		
		vector<vector<double> > cpoints;
		vector<vector<double> > cpointsl; //special comparison node set
		vector<double> normvector(double distance, vector<double> pos);
		double nearestNodes2(int x, int y);
		double nearestNodes2(vector<double> pos);
		double distanceNodePosToEdge(vector<double> pos, vector<double> node, vector<double> edgepos,bool horiz);
		double distancevecNodePosToEdge(vector<double> pos, vector<double> node, vector<double> edgev ,vector<double> edgepos);
		vector<double> invert(vector<double> x);
		double angle(vector<double> x);
		double dotproduct(vector<double> x, vector<double> y);
		vector<double> scalem(double c, vector<double> x);
		vector<double> difference(vector<double> x, vector<double> y);
		vector<double> add(vector<double> x, vector<double> y);
		vector<vector<double> > nodeswithinaSquare(vector<double> pos, double dsize);
		int raytestPosition(vector<double> pos, double minval, int minvali);
		vector<double> nearestNodes2radial(int x, int y);
		vector<double> nearestNodes2radial(vector<double> pos);
		int nearestNodes2radial2(vector<double> pos);
		//this next function is purely to conform to the heightmap data reading data standard
		//given by the other noise function
		vector<vector<vector<double> > > convertTo3Darray(vector<vector<double> > inputvec);
	
		//vector<vector<double> > getHeightMap(void)
		int csize;
		Ogre::Vector4 falloffcoeff;
		vector<double> randomHeights;
		double cmaxval;
};

Voronoi::Voronoi(int points, int size){
	cpoints.resize(points);
	for (int i = 0; i < points; i++){
		cpoints[i] = randompoint(size);
	}
	csize = (double) size;
	vector<double> twopoints(2);
    	twopoints[0] = 0; 
    	twopoints[1] = 1;
    	cspline* cs = new cspline(twopoints,twopoints);
    	double tan1 = (double)Ogre::Math::Tan(Ogre::Math::DegreesToRadians(30));
    	falloffcoeff = cs->compute2nodecspline (tan1, tan1);
/*    
    	twopoints[0] = 0; 
    	twopoints[1] = 1;
    	cspline* cs = new cspline(twopoints,twopoints);
    	double tan1 = (double)Ogre::Math::Tan(Ogre::Math::DegreesToRadians(30));
    	falloffcoeff = cs->compute2nodecspline (tan1, tan1);
*/
}

void Voronoi::buildrandomMap(void){
	randomHeights.resize(csize);
	for (int i = 0; i < cpoints.size() ;i++){
		int v1 = rand() % 65536;
		double v2 = v1/(double)32768.0000000000f;
		v2 = v2-1.0000000000000f;
		randomHeights[i] = v2;		
	}
}

vector<double> Voronoi::invert(vector<double> x){
	vector<double> retvec(2);
	retvec[0] = x[1];
	retvec[1] = x[0];
	return retvec;
}

vector<double> Voronoi::scalem(double c, vector<double> x){
	vector<double> retvec(2);
	retvec[0] = c*x[0];
	retvec[1] = c*x[1];
	return retvec;
}

double Voronoi::dotproduct(vector<double> x, vector<double> y){
	return x[0]*y[0]+x[1]*y[1];
}

vector<double> Voronoi::difference(vector<double> x, vector<double> y){
	vector<double> retvec(2);
	retvec[0] = x[0] - y[0];
	retvec[1] = x[1] - y[1];
	return retvec;
}

vector<double> Voronoi::add(vector<double> x, vector<double> y){
	vector<double> retvec(2);
	retvec[0] = x[0] + y[0];
	retvec[1] = x[1] + y[1];
	return retvec;
}

vector<double> Voronoi::randompoint(int size){
	int v1 = rand() % size;
	int v2 = rand() % size;
	vector<double> pos(2);
	pos[0] = (double) v1 + .0000000000f;
	pos[1] = (double) v2 + .0000000000f;
	return pos;
}

vector<double> Voronoi::normvector(double distance, vector<double> pos){
	vector<double> retvec(2);
	retvec[0] = pos[0]/distance;
	retvec[1] = pos[1]/distance; 
	return retvec;
}

vector<double> Voronoi::directionVector(vector<double> point1, vector<double> point2){
	vector<double> dirpos(2);
	dirpos[0] = point2[0] - point1[0];
	dirpos[1] = point2[1] - point1[1];
	return dirpos;
}

double Voronoi::distance(vector<double> point1, vector<double> point2){
	return pow(pow(point1[0] - point2[0],2.0f) + pow(point1[1] - point2[1],2.0f),.5f);
}

double Voronoi::angle(vector<double> x){
	if (x[0] == 0.0f){
		return atan(x[1]/.00000000000001f);
	}
	else{
		return atan(x[1]/x[0]);
	}
}

bool Voronoi::angleTest(vector<double> npos2, vector<double> pos){
	double anglern2 = asin (npos2[1]/npos2[0]);
	double anglerp = asin(pos[1]/pos[0]);
	bool retval = false;
	if (anglerp < anglern2){
		return true; //rotate -90
	}  
	else{
		return false;
	}
}

double Voronoi::nearestNodes2(int x, int y){
	vector<double> inputvec(2);
	inputvec[0] = x;
	inputvec[1] = y;
	return nearestNodes2(inputvec);
}

double Voronoi::nearestNodes2(vector<double> pos){
		double minval = pow(2*pow((double)csize,2.0f), (double).5f);
		double minval2 = (double)0.0f;
		int minvali = 0;
		int minval2i = 0;
		for(int i = 0; cpoints.size(); i++){
			double dist = distance(cpoints[i], pos);
			if (dist < minval){
				minval2 = minval;
				minval2i = minvali;
				minval = dist;
				minvali = i;
			}
		} 
		//we found above the neareast and a nearest cell neighbor relative
		//to our position.  Now we need to find line defining the border between cells
		//or that this point is nearest to the boundary of our voronoi system.
		//to find the line we ray cast in the direction from such point to the nearest
		//cell node away from the cell node.  Set the origin of the ray to the point.
		vector<double> dirpos = directionVector(cpoints[minval], pos);
		vector<double> ndirvec = normvector(minval, dirpos);
		double distposnedge;
		//Ogre::Ray ray1 = Ogre::Ray(Ogre::Vector3(pos[0],pos[1],0),ndirvec);
		//we intersect test to see that we aren't closer to an edge boundary of the voronoi system
		if (abs(csize - pos[0]) < minval2){
			distposnedge = abs(csize - pos[0]); 
		}
		else if (abs(csize - pos[1]) < minval2){
			distposnedge = abs(csize - pos[1]);
		}
		else if (abs(0.0f - pos[0]) < minval2){
			distposnedge = abs(0.0f - pos[0]);
		}
		else if (abs(0.0f - pos[1]) < minval2){
			distposnedge = abs(0.0f - pos[1]);
		}
		else {
			//minval2 passes, then assumption that between cells the 
			//the minimum point whose position is given at such a point in the direction
			//from node1 to node2 and is equi distant from both such node is.
			double distn2n1 = distance(cpoints[minvali], cpoints[minval2i])/2.0f;
			
			//then we can apply a ray to intercept this point or use point slope
			//to make this easier if using point slope, we can localize a node position
			//as representing an origin point. 
			vector<double> pos_t(2);
			pos_t[1] = pos[1] - cpoints[minvali][1];
			pos_t[0] = pos[0] - cpoints[minvali][0];
			vector<double> node2_t(2);
			node2_t[1] = cpoints[minval2i][1] - cpoints[minvali][1];
			node2_t[0] = cpoints[minval2i][0] - cpoints[minvali][0];
			//now point slope could seem a bit more straight forward with a
			//new point of origin.
			//slope 
			double mnode21 = node2_t[1]/node2_t[0];
			double mpos_t1 = pos_t[1]/pos_t[0];
			//under the new coordinate system we say the point dist2n1 
			//is y = mx or (m^2x^2 + x^2)^1/2 = 2*distn2n1 means
			//4*distn2n1^2/(m^2+1) = x^2 or x_0 = 2 distn2n1 /(m^2+1)^1/2
			//and y_0 = 2m/(m^2+1)^1/2 * distn2n1 
			// point slope formula is given by (y-y0) = m(x-x0).
			//Our direction vector at such point is defined by a 90 degree rotation
			// of the distance vector (between both nodes) or (-y_d,x_d) where
			// x_d and y_d are the original direction coordinates of such vector.
			// so where our original m = node2_t[1]/node2_t[0]
			// then an orthogonal direction vector is m_n = 1/m = -node2_t[0]/node2_t[1].
			// Then using point slope formula we have
			// (y-2m/(m^2+1)^1/2*distn2n1) = m_n*(x - 2*distn2n1/(m^2+1)^1/2)
			// y - m_n*x -2m/(m^2+1)^1/2 * distn2n1 + 2*m_n*distn2n1/(m^2+1)^1/2 = 0
			// a = 1, b = -m_n , and c = -2*m/(m^2+1)^1/2 * distn2n1 + 2*m_n*distn2n1/(m^2+1)^1/2
			//now we make use of distance between point (x_0,y_0) and a line.
			//distance (ax +by + c) = |ax_0 + by_0 + c|/(a^2 + b^2)^1/2
			double m = mnode21;
			double m_n;
			if (angleTest(node2_t, pos_t)){
				m_n = 1.0f/m;
			}
			else{
				m_n = -1.0f/m;
			}
			//double m_n = -1.0f/m;
			double a = 1;
			double b = -m_n;
			double c = -2*m/pow((m*m+1),.5f) *distn2n1 + 2*m_n*distn2n1/pow(m*m+1,.5f);
			distposnedge = abs(a*pos_t[0] + b*pos_t[1]+ c)/pow(a*a+b*b,.5f);	
		}	
		
		return distposnedge;
	}

double Voronoi::distancevecNodePosToEdge(vector<double> pos, vector<double> node, vector<double> edgev ,vector<double> edgepos){
	//edgepos is assumed to be a vector that is in the direction of the edge.
	//vector approach
	std::ostringstream ss5;
	vector<double> vecNodeToPos = difference(pos, node);
	vector<double> orthogvecedge(2);
	orthogvecedge[0] = -1.0f*edgev[1];
	orthogvecedge[1] = edgev[0];
	double odist = pow(dotproduct(orthogvecedge, orthogvecedge),.5f);
	vector<double> ndirvec = normvector(odist, orthogvecedge);
	vector<double> vecPosToEdgepos = difference(edgepos, pos);
	//to use the projection method here we need to have a vector whose tail coincides
	//at the position vector and whose head ends on the line that we wish to find 
	//such orthogonal distance to, coupling this with a unit vector that is orthogonal 
	//to the direction of such line.  ndirvec and vecPosToEdgepos are the vectors
	//we need.  So we project by simply taking the dot product of these vectors.
	//this gives us the corresponding orthogonal distance
	double orthogdist = abs(dotproduct(vecPosToEdgepos, ndirvec));
//		ss5 << "Orthogdist: " << orthogdist << "\n";
		//ss5 << "Min val2: "<< minval2 << "\n";
		//ss5 << "Min vali: " << minvali << "\n";
		//ss5 << "Min vali2: " << minval2i << "\n";
//		tlog->logMessage(ss5.str());		
//		ss5.str(std::string());	
	//now we can make use of trig to find the point of intercept from NodeToPosToEdge
	//the angle of the NodeToPos vector should aid us.
	double angleNodeToPos = angle(vecNodeToPos);
	return orthogdist;///sin(angleNodeToPos);
}

double Voronoi::distanceNodePosToEdge(vector<double> pos, vector<double> node, vector<double> edgepos,
					bool horiz){
		vector<double> pos_t(2);
		pos_t[1] = pos[1] - node[1];
		pos_t[0] = pos[0] - node[0];
		vector<double> edgepos_t(2);
		edgepos_t[0] = edgepos[0] - node[0];
		edgepos_t[1] = edgepos[1] - node[1];
		double medgepos_t;
		if (horiz){
			medgepos_t = 0.0f;
		}
		else{
			medgepos_t = 99999999999999999.99999f;
		}
		//now point slope could seem a bit more straight forward with a
		//new point of origin.
		//slope 
		double mpos_t1 = pos_t[1]/pos_t[0];
		//y-y0 = m(x-x0)
		// y = mpos_t1 *x first equation and y - edgepos_t[1] = medgepos_t*(x - edgepos_t[0])
		// a = mpos_t1, c = 0, b = medgepost_t, d = edgepos_t[1] - medgepos_t*edgepos_t[0]
		double a = mpos_t1;
		double c = 0.0f;
		double b = medgepos_t; 
		double d = edgepos_t[1] - medgepos_t*edgepos_t[0];
		double px = (d-c)/(a-b);
		double py = (a*d-b*c)/(a-b);
		vector<double> intercept(2);
		intercept[0] = px;
		intercept[1] = py;
		return distance(intercept, pos_t);  	
}

int Voronoi::raytestPosition(vector<double> pos, double minval, int minvali){
		int reti = minvali;
		for(int i = 0; i < cpointsl.size(); i++){
/*			ss5 << "cpoint x: " << cpoints[i][0] << "\n";
			//ss5 << "dist: "<< dist << "\n";
			tlog->logMessage(ss5.str());
*///			ss5.str(std::string());
			double dist = distance(cpointsl[i], pos);
			//ss5 << "cpoint x: " << cpoint[i][0] << "\n";
/*			ss5 << "dist: "<< dist << "\n";
			ss5 << "iteration: " << j << "\n";
			tlog->logMessage(ss5.str());
*///			ss5.str(std::string());
			if (dist < minval){
				//minval2 = minval;
				//minval2i = minvali;
				//minval = dist;
				//minvali = i;
				reti = i;
				break;
			}
			
		} 
		return reti;
}



vector<double> Voronoi::nearestNodes2radial(int x, int y){
	vector<double> inputvec(2);
	inputvec[0] = (double)x + .000000000000f;
	inputvec[1] = (double)y + .000000000000f;
	return nearestNodes2radial(inputvec);
}

vector<vector<double> > Voronoi::nodeswithinaSquare(vector<double> pos, double dsize){
	vector<vector<double> > retset;
	for (int i=0; i < cpoints.size(); i++){
		if (cpoints[i][1] >= pos[1] - dsize/2.0f and cpoints[i][1] <= pos[1] + dsize/2.0f){
			if (cpoints[i][0] >= pos[0] - dsize/2.0f and cpoints[i][0] <= pos[0] + dsize/2.0f){
				retset.push_back(cpoints[i]);
			}
		}
	}
	return retset;
}

vector<double> Voronoi::nearestNodes2radial(vector<double> pos){
		std::ostringstream ss5;
		double minval = pow(2.0f*pow(csize,2.0f), (double).5f);
		double minval2 = (double)0.0f;
		vector<int> minvalis(6);
		int minvali = 0;
		int minval2i = 0;

//		ss5 << "beginning nearestNodes2radial !!!!!!!" << "\n";
//		ss5 << "minval: "<< minval << "\n";
//		ss5 << "cpoint size: " << cpoints.size()<< "\n";
//		tlog->logMessage(ss5.str());
//		ss5.str(std::string());
//		int j = 1;
		for(int i = 0; i < cpoints.size(); i++){
/*			ss5 << "cpoint x: " << cpoints[i][0] << "\n";
			//ss5 << "dist: "<< dist << "\n";
			tlog->logMessage(ss5.str());
*///			ss5.str(std::string());
			double dist = distance(cpoints[i], pos);
			//ss5 << "cpoint x: " << cpoint[i][0] << "\n";
/*			ss5 << "dist: "<< dist << "\n";
			ss5 << "iteration: " << j << "\n";
			tlog->logMessage(ss5.str());
*/			ss5.str(std::string());
			if (dist < minval){
//				minval2 = minval;
//				minval2i = minvali;
				minval = dist;
				minvali = i;
			}
			
		}
		minval2 = pow(2.0f, (double).5f)*csize;
		minval2i = 0;
		for(int i = 0; i < cpoints.size(); i++){
/*			ss5 << "cpoint x: " << cpoints[i][0] << "\n";
			//ss5 << "dist: "<< dist << "\n";
			tlog->logMessage(ss5.str());
*///			ss5.str(std::string());
			double dist = distance(cpoints[i], pos);
			//ss5 << "cpoint x: " << cpoint[i][0] << "\n";
/*			ss5 << "dist: "<< dist << "\n";
			ss5 << "iteration: " << j << "\n";
			tlog->logMessage(ss5.str());
*/			ss5.str(std::string());
			if (dist >= minval and i != minvali and dist < minval2){
				minval2 = dist;
				minval2i = i;
				//minval = dist;
				//minvali = i;
			}
			
		}  
		//cpointsl = nodeswithinaSquare(pos, csize/2.0f);
/*		ss5 << "Min val: " << minval << "\n";
		ss5 << "Min val2: "<< minval2 << "\n";
		ss5 << "Min vali: " << minvali << "\n";
		ss5 << "Min vali2: " << minval2i << "\n";
		tlog->logMessage(ss5.str());		
*///		ss5.str(std::string());
		//we found above the neareast and a nearest cell neighbor relative
		//to our position.  Now we need to find line defining the border between cells
		//or that this point is nearest to the boundary of our voronoi system.
		//to find the line we ray cast in the direction from such point to the nearest
		//cell node away from the cell node.  Set the origin of the ray to the point.
		vector<double> dirpos = directionVector(cpoints[minvali], pos);
		if (minval == 0.0f){
			vector<double> altvec(2);
			vector<double> nod1Tonod2vec = difference(cpoints[minval2i], cpoints[minvali]);
			nod1Tonod2vec = scalem(.5f, nod1Tonod2vec);
			double dist = pow(dotproduct(nod1Tonod2vec,nod1Tonod2vec),.5f);
			altvec[0] = dist;
			altvec[1] = dist;
			return altvec;  //we by default return the a zero distance vector return set.
		}
		vector<double> ndirvec = normvector(minval, dirpos); //check div by zero issues

//***Disabled RayTest method much slower then direct solution methods, uncomment it if you like to see the method in action
/*
		Ogre::Ray posRay(Ogre::Vector3(pos[0],pos[1],0.0f), Ogre::Vector3(ndirvec[0],ndirvec[1],0.0f));
		vector<double> poscheck(2);
		vector<double> retvals(2);
		
		for (int i = 1; i < csize; i++){
			
			Ogre::Vector3 rayposinc = posRay.getPoint(i);
			poscheck[0] = rayposinc.x; poscheck[1] = rayposinc.y;
			if (rayposinc.x <= 0 or rayposinc.x >= csize){
				retvals[0] = 1;
				retvals[1] = 1;
				return retvals;
			}
			else if (rayposinc.y <= 0 or rayposinc.y >= csize){
				retvals[0] = 1;
				retvals[1] = 1;
				return retvals;
			}
			else{
				if (raytestPosition(poscheck, minval, minvali)!=minvali){
					retvals[0] = i-1;
					retvals[1] = i-1+minval;
					return retvals;
				}
			}
		}
*/
		//Ogre::Ray ray1 = Ogre::Ray(Ogre::Vector3(pos[0],pos[1],0),ndirvec);
		//we intersect test to see that we aren't closer to an edge boundary of the voronoi system
		vector<double> edgepos(2); vector<double> edgepos2(2); vector<double> edgepos3(2);
		edgepos[0] = csize;
		edgepos[1] = csize;
		edgepos2[0] = 0.0f;
		edgepos2[1] = csize;
		edgepos3[0] = csize;
		edgepos3[1] = 0.0f;
		vector<double> edgevec; vector<double> edgevec2;
		edgevec = difference(edgepos2,edgepos);  edgevec2 = difference(edgepos3,edgepos);
		vector<double> d1node(2), d2node(2), d3node(2), d4node(2);
		d1node[1] = edgepos[1];
		d1node[0] = cpoints[minvali][0];
		double d1 = distancevecNodePosToEdge(pos, cpoints[minvali], edgevec , edgepos);
		//double d1 = distanceNodePosToEdge(pos, cpoints[minvali], edgepos, true); //horizontal
		d2node[0] = edgepos[0];
		d2node[1] = cpoints[minvali][1];
		double d2 = distancevecNodePosToEdge(pos, cpoints[minvali], edgevec2 , edgepos);
		//double d2 = distanceNodePosToEdge(pos, cpoints[minvali], edgepos, false); //vertical
		edgepos[0] = 0.0f;
		edgepos[1] = 0.0f;		
		edgevec = difference(edgepos,edgepos2);  edgevec2 = difference(edgepos3,edgepos);
		d3node[0] = edgepos[0];
		d3node[1] = cpoints[minvali][1];
		double d3 = distancevecNodePosToEdge(pos, cpoints[minvali], edgevec , edgepos);
		//double d1 = distanceNodePosToEdge(pos, cpoints[minvali], edgepos, true); //horizontal
		d4node[0] = cpoints[minvali][0];
		d4node[1] = edgepos[1];
		double d4 = distancevecNodePosToEdge(pos, cpoints[minvali], edgevec2 , edgepos);
		//double d3 = distanceNodePosToEdge(pos, cpoints[minvali], edgepos, true); //horizontal
		//double d4 = distanceNodePosToEdge(pos, cpoints[minvali], edgepos, false); //vertical	double distposnedge
//		ss5 << "end of distanceNodePosToEdge in nearestNodes2radial !!!!!!!" << "\n";
//		tlog->logMessage(ss5.str());
//		ss5.str(std::string());
		vector<double> dset(4);
		dset[0] = d2; dset[1] = d3; dset[2] = d4;
		double compminval = abs(d1);
		vector<double> dnodep(2);
		vector<double> rtnnode(2);
		vector<vector<double> > dnodeset(3);
		dnodeset[0].resize(2); dnodeset[0] = d2node;
		dnodeset[1].resize(2); dnodeset[1] = d3node;
		dnodeset[2].resize(2); dnodeset[2] = d4node;
		rtnnode = d1node;
		for (int j = 0; j < 3; j++){
			if (abs(dset[j])<compminval){
				compminval = abs(dset[j]);
				rtnnode = dnodeset[j];
			}
		}
		vector<double> retvec(2);
		double distposnedge;	  

/*
		else if (compminval < minval2){
			distposnedge = d2;
			retvec[0] = distposnedge;
			retvec[1] = minval + distposnedge;
			return retvec; 
		}
		else if (compminval < minval2){
			distposnedge = d3;
			retvec[0] = distposnedge;
			retvec[1] = minval + distposnedge;
			return retvec; 
		}
		else if (compminval < minval2){
			distposnedge = d4;
			retvec[0] = distposnedge;
			retvec[1] = minval + distposnedge;
			return retvec; 
		}
*/
//		else {
			//****vector based alternate approach to algebraics given below*****
			//below we find the vector starting at node 1 to node2 (neighboring)
			//then we take the 1/2 distance intersection.  The direction of this
			//vector is moving from node 1 to node 2 direction wise.
			//To project onto the line that describes the voronoi cell boundary,
			//we know that the node mid point position intercepts this, and that
			//the direction vector between such nodes is orthogonal to such point.
			//so we normalize this and invert this for a 90 degree rotation.
			//then we need a vector from our knowns to a point on such line.
			//find a vector moving from position to node 2 then a vector moving
			//from node2 to the midpoint of node2 to node 1 vector, or subtract
			//from this the 1/2* node1tonode2vector.
			vector<double> nod1Tonod2vec = difference(cpoints[minval2i], cpoints[minvali]);
			nod1Tonod2vec = scalem(.5f, nod1Tonod2vec);
			vector<double> posTonod2vec = difference(cpoints[minval2i], pos);
			vector<double> posTopointintercept = difference(posTonod2vec,nod1Tonod2vec);
			double n2n1dist = pow(dotproduct(nod1Tonod2vec,nod1Tonod2vec),.5f);
			vector<double> Nn2n1dist = normvector(n2n1dist, nod1Tonod2vec);
			//vector<double> iNn2n1dist = invert(Nn2n1dist);
			//iNn2n1dist[0] = -1.0f*iNn2n1dist[0];
			double distpostoline = abs(dotproduct(posTopointintercept,Nn2n1dist));
			double angleNodeToPos = angle(posTopointintercept);
			if (compminval < distpostoline){
				distposnedge = d1;
				retvec[0] = compminval;
				vector<double> nod1Tonod2vec = difference(rtnnode, cpoints[minvali]);
				double dist = pow(dotproduct(nod1Tonod2vec,nod1Tonod2vec),.5f);
				retvec[1] = dist;
				return retvec; 
			}
			retvec[0] = distpostoline; ///cos(angleNodeToPos);
			retvec[1] = n2n1dist;
/*		
		ss5 << "Pos: " << pos[0]<< "," << pos[1] << "\n";
		ss5 << "Distance: " << distpostoline << "\n";
		ss5 << "Nod1Tonod2: "<< n2n1dist << "\n";
		ss5 << "PosTopointintercept: " << posTopointintercept[0]<< "," << posTopointintercept[1]<< "\n";
		tlog->logMessage(ss5.str());
		ss5.str(std::string());
*/

			return retvec; 	 //disable this line if you want to utilize the algebraic
			//method.  Generally not working right at the moment though below.

			//minval2 passes, then assumption that between cells the 
			//the minimum point whose position is given at such a point in the direction
			//from node1 to node2 and is equi distant from both such node is.
			ss5 << "beginning minval2 else in nearestNodes2radial !!!!!!!" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());	
			double distn2n1 = distance(cpoints[minvali], cpoints[minval2i])/2.0f;
/*			ss5 << "Reached minval2 else in nearestNodes2radial !!!!!!!" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());			
*/			//then we can apply a ray to intercept this point or use point slope
			//to make this easier if using point slope, we can localize a node position
			//as representing an origin point. 
			vector<double> pos_t(2);
			pos_t[1] = pos[1] - cpoints[minvali][1];
			pos_t[0] = pos[0] - cpoints[minvali][0];
			vector<double> node2_t(2);
			node2_t[1] = cpoints[minval2i][1] - cpoints[minvali][1];
			node2_t[0] = cpoints[minval2i][0] - cpoints[minvali][0];
			//now point slope could seem a bit more straight forward with a
			//new point of origin.
			//slope 
			double mnode21 = node2_t[1]/node2_t[0];
			double mpos_t1 = pos_t[1]/pos_t[0];
			//under the new coordinate system we say the point dist2n1 
			//is y = mx or (m^2x^2 + x^2)^1/2 = 2*distn2n1 means
			//4*distn2n1^2/(m^2+1) = x^2 or x_0 = 2 distn2n1 /(m^2+1)^1/2
			//and y_0 = 2m/(m^2+1)^1/2 * distn2n1 
			// point slope formula is given by (y-y0) = m(x-x0).
			//Our direction vector at such point is defined by a 90 degree rotation
			// of the distance vector (between both nodes) or (-y_d,x_d) where
			// x_d and y_d are the original direction coordinates of such vector.
			// so where our original m = node2_t[1]/node2_t[0]
			// then an orthogonal direction vector is m_n = 1/m = -node2_t[0]/node2_t[1].
			// Then using point slope formula we have
			// (y-2m/(m^2+1)^1/2*distn2n1) = m_n*(x - 2*distn2n1/(m^2+1)^1/2)
			// y - m_n*x -2m/(m^2+1)^1/2 * distn2n1 + 2*m_n*distn2n1/(m^2+1)^1/2 = 0
			// a = m_n , and c = 2*m/(m^2+1)^1/2 * distn2n1 - 2*m_n*distn2n1/(m^2+1)^1/2
			//now we also use the formula for the line between the point 
			//and node1 which is given by y = mpost_1*x
			//its coefficients are b = mpost_1, d = 0
			// using the intercept formula for two lines, P(d-c/(a-b),(ad-bc)/(a-b))
			double m = mnode21;
//			double m_n;
/*
			if (angleTest(node2_t, pos_t)){
				m_n = 1.0f/m;
			}
			else{
				m_n = -1.0f/m;
			}
*/
			double m_n = -1.0f/m;
			double a = m_n;
			double b = mpos_t1;
			double c = 2*m/pow((m*m+1),.5f) *distn2n1 - 2*m_n*distn2n1/pow(m*m+1,.5f);
			double d = 0.0f;
			double px = (d-c)/(a-b);
			double py = (a*d-b*c)/(a-b);
			vector<double> pvec(2);
			pvec[0] = px;
			pvec[1] = py;
			distposnedge = distance(pvec, pos_t);
			//double distposnedge = abs(a*pos_t[0] + b*pos_t[1]+ c)/pow(a*a+b*b,.5f);	
//		}	
		vector<double> distances(2);
		double distn1pos = distance(cpoints[minvali], pos);
		distances[0] = distposnedge;
		distances[1] = distposnedge+distn1pos;  //total length on the given radial axis.
		return distances;
	}

int Voronoi::nearestNodes2radial2(vector<double> pos){
//simple set checking cell algorithm 
		std::ostringstream ss5;
		double minval = 0.0f;//cmath::pow(2*cmath::pow(csize,2), (double).5f);
		double minval2 = (double)0.0f;
		vector<int> minvalis(6);
		int minvali = 0;
		int minval2i = 0;
//		ss5 << "beginning nearestNodes2radial !!!!!!!" << "\n";
//		ss5 << "minval: "<< minval << "\n";
//		ss5 << "cpoint size: " << cpoints.size()<< "\n";
//		tlog->logMessage(ss5.str());
//		ss5.str(std::string());
//		int j = 1;
		for(int i = 0; i < cpoints.size(); i++){
/*			ss5 << "cpoint x: " << cpoints[i][0] << "\n";
			//ss5 << "dist: "<< dist << "\n";
			tlog->logMessage(ss5.str());
*///			ss5.str(std::string());
			double dist = distance(cpoints[i], pos);
			//ss5 << "cpoint x: " << cpoint[i][0] << "\n";
/*			ss5 << "dist: "<< dist << "\n";
			ss5 << "iteration: " << j << "\n";
			tlog->logMessage(ss5.str());
*/			ss5.str(std::string());
			if (dist < minval){
				minval2 = minval;
				minval2i = minvali;
				minval = dist;
				minvali = i;
			}
			
		} 
		return minvali;


	}

vector<vector<vector<double> > > Voronoi::getHeightMapradial(double tramount){
	tramount = 1.0f;
	tlog = Ogre::LogManager::getSingleton().getLog("Voronoi.log");
	std::ostringstream ss5;
	vector<vector<double> > points(csize);
	ss5 << "Beginning !!!!!!!" << "\n";
	tlog->logMessage(ss5.str());
	ss5.str(std::string());
	cmaxval = 0.0000f;
	for (int i = 0; i < csize; i++){
		points[i].resize(csize);
		for(int j = 0; j < csize; j++){
			vector<double> distances = nearestNodes2radial(i, j);
			//compute t param 
			//ss5 << "Reached distances !!!!!!!" << "\n";
			//tlog->logMessage(ss5.str());
			//ss5.str(std::string());
			double tparam = distances[0]/distances[1];

			if (abs(tparam) > cmaxval) {
				//tparam = (double) 1.0f;
				cmaxval = abs(tparam);
				//tparam = 1 - tparam;
			}
			else{
				//tparam = 1 - tparam;
			}

			//Falloffinterpolate fint = Falloffinterpolate(tparam, distances[1], falloffcoeff, tramount);			
			//double intpoint = fint.getTPoint();
			//points[i][j] = intpoint;
			points[i][j] = tparam;
			if (tparam > 1){
				ss5 << "Point: " << i << ","<< j <<":"<< tparam << "\n";
				tlog->logMessage(ss5.str());
				ss5.str(std::string());
			}
			//ss5 << "Point: " << i << ","<< j <<":"<< intpoint << "\n";
			//tlog->logMessage(ss5.str());
			//ss5.str(std::string());
		}
	}
	ss5 << "before convert" << "\n";
	tlog->logMessage(ss5.str());
	ss5.str(std::string());
	vector<vector<vector<double> > > rpoints = convertTo3Darray(points);
	ss5 << "Reached convert !!!!!!!" << "\n";
	tlog->logMessage(ss5.str());
	ss5.str(std::string());
	return rpoints;
}

vector<vector<vector<double> > > Voronoi::convertTo3Darray(vector<vector<double> > inputvec){
	vector<vector<vector<double> > > retvec(csize);
	for (int i = 0; i < csize; i++){
		retvec[i].resize(csize);
		for (int j = 0; j < csize; j++){
			retvec[i][j].resize(2);
			for (int k = 0; k < 2; k++){
				retvec[i][j][k] = inputvec[i][j];///cmaxval;  //parameterizing constant applied
			}
		}
	}
	return retvec;
}

vector<vector<vector<double> > > Voronoi::getSimpleHeightMap(){
	tlog = Ogre::LogManager::getSingleton().getLog("Voronoi.log");
	std::ostringstream ss5;
	vector<vector<double> > points(csize);
	buildrandomMap();
	for (int i = 0; i < csize; i++){
		points[i].resize(csize);
		for(int j = 0; j < csize; j++){
			vector<double> invec(2);
			invec[0] = i; invec[1] = j;
			int vcellindex = nearestNodes2radial2(invec);
			//compute t param 
			double tparam = randomHeights[vcellindex];
/*
			if (tparam > 1) {
				//tparam = (double) 1.0f;
				tparam = 0.0f;
			}
			else{
				tparam = 1 - tparam;
			}
*/
			//Falloffinterpolate fint = Falloffinterpolate(tparam, falloffdist, falloffcoeff, tramount);			
			//double intpoint = fint.getTPoint();
			points[i][j] = tparam;
		}
	}
	vector<vector<vector<double> > > rpoints = convertTo3Darray(points);
	return rpoints;
}

vector<vector<vector<double> > > Voronoi::getHeightMap(double falloffdist, double tramount){
	tlog = Ogre::LogManager::getSingleton().getLog("Voronoi.log");
	std::ostringstream ss5;
	vector<vector<double> > points(csize);
	for (int i = 0; i < csize; i++){
		points[i].resize(csize);
		for(int j = 0; j < csize; j++){
			double dist = nearestNodes2(i, j);
			//compute t param 
			double tparam = dist / falloffdist;
			if (tparam > 1) {
				//tparam = (double) 1.0f;
				tparam = 0.0f;
			}
			else{
				tparam = 1 - tparam;
			}
			Falloffinterpolate fint = Falloffinterpolate(tparam, falloffdist, falloffcoeff, tramount);			
			double intpoint = fint.getTPoint();
			points[i][j] = intpoint;
		}
	}
	vector<vector<vector<double> > > rpoints = convertTo3Darray(points);
	return rpoints;
}

#endif
