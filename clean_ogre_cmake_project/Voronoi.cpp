#include <math.h>       /* cos */
#include "cspline.cpp"
#define PI 3.14159265

class Voronoi {
	public:
		Voronoi(int points, int size);
		vector<vector<double> > getHeightMap(void);
	private:
		double distance(vector<double> point1, vector<double> point2);
		vector<double> randompoint(void);
		vector<double> directionVector(vector<double> point1, vector<double> point2);		
		vector<vector<double> > > cpoints;
		vector<double> normvector(double distance, vector<double> pos);
		double nearestNodes2(int x, int y);
		double nearestNodes2(vector<double> pos);
		double distanceNodePosToEdge(vector<double> pos, vector<double> node, vector<double> edgepos,bool horiz);
		double nearestNodes2radial(int x, int y);
		double nearestNodes2radial(vector<double> pos);
		//vector<vector<double> > getHeightMap(void)
		int csize;
		Ogre::Vector4 falloffcoeff;
};

Voronoi::Voronoi(int points, int size){
	cpoints.resize(points);
	for (int i = 0; i < points; i++){
		cpoints[i] = randompoint(size);
	}
	csize = size;
	vector<double> twopoints(2);
    	twopoints[0] = 0; 
    	twopoints[1] = 1;
    	cspline* cs = new cspline(twopoints,twopoints);
    	double tan1 = (double)Ogre::Math::Tan(Ogre::Math::DegreesToRadians(30));
    	falloffcoeff = cs->compute2nodecspline (tan1, tan1);    std::vector<double> twopoints(2);
    	twopoints[0] = 0; 
    	twopoints[1] = 1;
    	cspline* cs = new cspline(twopoints,twopoints);
    	double tan1 = (double)Ogre::Math::Tan(Ogre::Math::DegreesToRadians(30));
    	falloffcoeff = cs->compute2nodecspline (tan1, tan1);
}

vector<double> Voronoi::randompoint(int size){
	int v1 = rand() % size;
	int v2 = rand() % size;
	vector<double> pos(2);
	pos[0] = v1;
	pos[1] = v2;
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
	return pow(pow(point1[0] - point2[0],2) + pow(point1[1] - point2[1]),(double).5f);
}

double Voronoi::nearestNodes2(int x, int y){
	vector<double> inputvec(2);
	inputvec[0] = x;
	inputvec[1] = y;
	return nearestNodes2(inputvec);
}

double Voronoi::nearestNodes2(vector<double> pos){
		double minval = pow(2*pow(csize,2), (double).5f);
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
		Ogre::Ray ray1 = Ogre::Ray(Ogre::Vector3(pos[0],pos[1],0),ndirvec);
		//we intersect test to see that we aren't closer to an edge boundary of the voronoi system
		if (abs(csize - pos[0]) < minval2){
			double distposnedge = abs(csize - pos[0]); 
		}
		else if (abs(csize - pos[1]) < minval2){
			double distposnedge = abs(csize - pos[1]);
		}
		else if (abs(0.0f - pos[0]) < minval2){
			double distposnedge = abs(0.0f - pos[0]);
		}
		else if (abs(0.0f - pos[1]) < minval2){
			double distposnedge = abs(0.0f - pos[1]);
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
			mnode21 = node2_t[1]/node2_t[0];
			mpos_t1 = pos_t[1]/pos_t[0];
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
			double m_n = -1.0f/m;
			double a = 1;
			double b = -m_n;
			double c = -2*m/pow((m*m+1),.5f) *distn2n1 + 2*m_n*distn2n1/pow(m*m+1,.5f);
			double distposnedge = abs(a*pos_t[0] + b*pos_t[1]+ c)/pow(a*a+b*b,.5f);	
		}	
		
		return distposnedge;
	}

double Voronoi::distanceNodePosToEdge(vector<double> pos, vector<double> node, vector<double> edgepos,
					bool horiz){
		vector<double> pos_t(2);
		pos_t[1] = pos[1] - node[1];
		pos_t[0] = pos[0] - node[0];
		vector<double> edgepos_t(2);
		double edgepos_t[0] = edgepos[0] - node[0];
		double edgepos_t[1] = edgepos[1] - node[1];
		if horiz{
			double medgepos_t = 0.0f;
		}
		else{
			double medgepos_t = 99999999999999999.99999f
		}
		//now point slope could seem a bit more straight forward with a
		//new point of origin.
		//slope 
		mpos_t1 = pos_t[1]/pos_t[0];
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

vector<double> Voronoi::nearestNodes2radial(int x, int y){
	vector<double> inputvec(2);
	inputvec[0] = x;
	inputvec[1] = y;
	return nearestNodes2radial(inputvec);
}

vector<double> Voronoi::nearestNodes2radial(vector<double> pos){
		double minval = pow(2*pow(csize,2), (double).5f);
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
		Ogre::Ray ray1 = Ogre::Ray(Ogre::Vector3(pos[0],pos[1],0),ndirvec);
		//we intersect test to see that we aren't closer to an edge boundary of the voronoi system
		vector<double> edgepos(2);
		edgepos[0] = csize;
		edgepos[1] = csize;
		double distance = distanceNodePosToEdge(pos, cpoints[minvali], edgepos, true); //horizontal
		double distance2 = distanceNodePosToEdge(pos, cpoints[minvali], edgepos, false); //vertical
		edgepos[0] = 0.0f;
		edgepos[1] = 0.0f;
		double distance3 = distanceNodePosToEdge(pos, cpoints[minvali], edgepos, true); //horizontal
		double distance4 = distanceNodePosToEdge(pos, cpoints[minvali], edgepos, false); //vertical		  
		if (distance < minval2){
			double distposnedge = distance; 
		}
		else if (distance2 < minval2){
			double distposnedge = distance2;
		}
		else if (distance3 < minval2){
			double distposnedge = distance3;
		}
		else if (distance4 < minval2){
			double distposnedge = distance4;
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
			mnode21 = node2_t[1]/node2_t[0];
			mpos_t1 = pos_t[1]/pos_t[0];
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
			// a = -m_n , and c = -2*m/(m^2+1)^1/2 * distn2n1 + 2*m_n*distn2n1/(m^2+1)^1/2
			//now we also use the formula for the line between the point 
			//and node1 which is given by y = mpost_1*x
			//its coefficients are b = -mpost_1, d = 0
			// using the intercept formula for two lines, P(d-c/(a-b),(ad-bc)/(a-b))
			double m = mnode21;
			double m_n = -1.0f/m;
			double a = -m_n;
			double b = -m_post_1;
			double c = -2*m/pow((m*m+1),.5f) *distn2n1 + 2*m_n*distn2n1/pow(m*m+1,.5f);
			double d = 0.0f;
			double px = d-c/(a-b);
			double py = (a*d-b*c)/(a-b);
			vector<double> pvec(2);
			pvec[0] = px;
			pvec[1] = py;
			double distposnedge = distance(pvec, pos_t)
			//double distposnedge = abs(a*pos_t[0] + b*pos_t[1]+ c)/pow(a*a+b*b,.5f);	
		}	
		vector<double> distances(2);
		double distn1pos = distance(cpoints[minvali], pos)
		distances[0] = distposnedge;
		distances[1] = distposnedge+distn1pos;  //total length on the given radial axis.
		return distances;
	}

vector<vector<double> > Voronoi::getHeightMapradial(double tramount){
	for (int i = 0; i < csize; i++){
		for(int j = 0; j < csize; j++){
			vector<double> distances = nearestNodes2radial(i, j);
			//compute t param 
			double tparam = distances[0] / distances[1];
/*
			if (tparam > 1) {
				//tparam = (double) 1.0f;
				tparam = 0.0f;
			}
			else{
				tparam = 1 - tparam;
			}
*/
			Falloffinterpolate fint = Falloffinterpolate(tparam, distances[1], falloffcoeff, tramount);			
			double intpoint = fint.getTPoint(void);
		}
	}
}

vector<vector<double> > Voronoi::getHeightMap(double falloffdist, double tramount){
	for (int i = 0; i < csize; i++){
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
			double intpoint = fint.getTPoint(void);
		}
	}
}
