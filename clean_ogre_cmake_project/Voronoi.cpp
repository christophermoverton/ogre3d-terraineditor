#include <math.h>       /* cos */

#define PI 3.14159265
class Voronoi {
	public:
		Voronoi(int points, int size);
	private:
		double distance(vector<double> point1, vector<double> point2);
		vector<double> randompoint(void);
		vector<vector<double> > > cpoints;
		int csize;
};

Voronoi::Voronoi(int points, int size){
	cpoints.resize(points);
	for (int i = 0; i < points; i++){
		cpoints[i] = randompoint(size);
	}
	csize = size;
}

vector<double> Voronoi::randompoint(int size){
	int v1 = rand() % size;
	int v2 = rand() % size;
	vector<double> pos(2);
	pos[0] = v1;
	pos[1] = v2;
	return pos;
}

Ogre::Vector3 Voronoi::normvector(double distance, vector<double> pos){
	return Ogre::Vector3(pos[0]/distance, pos[1]/distance,0);
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

double<vector> Voronoi::mirrorTest(vector<double> node1, vector<double> node2, vector<double> pos){
	//sign testing direction vectors, 
	double distn2n1 = distance(node1, node2);
	double n2n1x = node2
	
}

vector<double> Voronoi::nearestNodes2(vector<double> pos){
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
		Ogre::Vector3 ndirvec = normvector(minval, dirpos);
		Ogre::Ray ray1 = Ogre::Ray(Ogre::Vector3(pos[0],pos[1],0),ndirvec);
		//we intersect test to see that we aren't closer to an edge boundary of the voronoi system
		if (abs(513.0f - pos[0]) < minval2){
		}
		else if (abs(513.0f - pos[1]) < minval2){
		}
		else if (abs(0.0f - pos[0]) < minval2){
		}
		else if (abs(0.0f - pos[1]) < minval2){
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
			//is y = mx or (mx^2 + x^2)^1/2 = 2*distn2n1 means
			//4*distn2n1^2/(m+1) = x^2 or x = 2 distn2n1 /(m+1)^1/2
			//and y = 2m/(m+1)^1/2 * distn2n1 		
		}	
		//next we mirror the ndirvec.  We need to perform a mirror test
		
	}
double Voronoi::getHeightMap(){
	for (int i = 0; i < csize; i++){
		for(int j = 0; j < csize; j++){
			
		}
	}
}
