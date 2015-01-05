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
		double w        ;
		vor::Voronoi * v;
		vor::Vertices * ver; // vrcholy
		vor::Vertices * dir; // smìry, kterými se pohybují
		vor::Edges * edg;	 // hrany diagramu
		vor::Cells * cells;
		vor::CellPolyPoints * bcellpolypoints;  //temporary cell map container for internal use
		vor::VPoint * cellpoint;                //temporary vertex point for internal use
		rayCastLine(double f, double g, VPoint * start, VPoint * end);
};

BuildVoronoi::BuildVoronoi(){
	w = 10000;
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("Voronoi.log");
	std::ostringstream ss5;
	ss5 << "Hitting Voronoi test1 !" << "\n";
    	tlog->logMessage(ss5.str());
    	ss5.str(std::string());
	v = new vor::Voronoi();

	ver = new vor::Vertices();
	dir = new vor::Vertices();
	for(int i=0; i<50; i++) 
	{

		ver->push_back(new VPoint( w * (double)rand()/(double)RAND_MAX , w * (double)rand()/(double)RAND_MAX )); 
		dir->push_back(new VPoint( (double)rand()/(double)RAND_MAX - 0.5, (double)rand()/(double)RAND_MAX - 0.5)); 
	}
	edg = v->GetEdges(ver, w, w);
	std::cout << "voronois done!\n";
	ss5 << "Hitting Voronoi test2 !" << "\n";
    	tlog->logMessage(ss5.str());
    	ss5.str(std::string());
	cells = v->GetVoronoiCells();
	for(vor::Cells::iterator i = cells->begin(); i!= cells->end(); ++i)
	{
		ss5 << "Cell: " << (* (*i).second->sitePos).x << "," <<(* (*i).second->sitePos).y << "\n"; 
		vor::Vertices * verts = (*i).second->places;
		for(vor::Vertices::iterator j = verts->begin(); j != verts->end(); j++){
			ss5 << "x: " << (*j)->x << ", y: " << (*j)->y << "\n";
		}
		vor::VertEdges * bvertedges = (*i).second->vertedges;
		vor::EdgeVerts * bedgeverts = (*i).second->edgeverts;
		for(vor::VertEdges::iterator j = bvertedges->begin(); j != bvertedges->end(); j++){
			VPoint * cellvertex = (*j);
			VEdges * instedges = (*j)->second();
			for(vor::VEdges::iterator k = instedges->begin(); k != instedges->end(); k++) {
				vor::Vertices instverts = (*bedgeverts)[*k];  //edge vertices
				//to iterate a vertex's linked neighbor.
				//need to append neighbor vertices to avoid reiterating a ray construction.
				for(vor::Vertices::iterator l = instverts->begin(); l != instverts->end();l++) {
					if (*l != cellvertex){
						//compute line points with this neighbor vertex as boundary
						
					}
				}
			}
		}
	}
	tlog->logMessage(ss5.str());
}

BuildVoronoi::rayCastLine(double f, double g, VPoint * start, VPoint * end) {
	
}
#endif
