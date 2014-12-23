#ifndef __BuildVoronoi_CPP
#define __BuildVoronoi_CPP

#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>


#include "Voronoi.h"
#include "VPoint.h"

class BuildVoronoi{

	public:
		BuildVoronoi();
	private:
		double w = 10000;
		vor::Voronoi * v;
		vor::Vertices * ver; // vrcholy
		vor::Vertices * dir; // smìry, kterými se pohybují
		vor::Edges * edg;	 // hrany diagramu
		vor::Cells * cells;
};

BuildVoronoi::BuildVoronoi(){
	v = new Voronoi();
	ver = new Vertices();
	dir = new Vertices();
	for(int i=0; i<50; i++) 
	{

		ver->push_back(new VPoint( w * (double)rand()/(double)RAND_MAX , w * (double)rand()/(double)RAND_MAX )); 
		dir->push_back(new VPoint( (double)rand()/(double)RAND_MAX - 0.5, (double)rand()/(double)RAND_MAX - 0.5)); 
	}
	edg = v->GetEdges(ver, w, w);
	std::cout << "voronois done!\n";
	cells = v->GetVoronoiCells();
}
#endif
