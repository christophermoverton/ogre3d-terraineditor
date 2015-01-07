#ifndef __BuildVoronoi_CPP
#define __BuildVoronoi_CPP

#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>


#include "Voronoi.h"
#include "VPoint.h"

/*

While most Voronoi.log data showing voronoi cell's having vertices linked typically, to none, one, or two,
vertices which one might expect, there are some weird instances where vertices are linked 
to all vertices in a given cell set which is in error I believe (?), and then there are also instances
where there are numerous vertices in a given cell of which none are neighbor linked.  Not sure exactly how 
this is possible here.

Here are some post analysis thoughts on the above:

-With a circle event it appears that I need to ensure on the Voronoi function that
 not only vertex to edge data is appended but also ensuring that a new edge is written on 
 a insert parabola event.

*/
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
		VoronoiCell * cell;  //temporary cell map container for internal use
		//vor::VPoint * cellpoint;                //temporary vertex point for internal use
		void rayCastLine(double f, double g, VPoint * start, VPoint * end);
		void rayCastLine(double f, double g, VPoint * start);  //overloaded function for incomplete edge
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
		cell = (*i).second;
		vor::VertEdges * bvertedges = (*i).second->vertedges;
		vor::EdgeVerts * bedgeverts = (*i).second->edgeverts;
		
		for(vor::VertEdges::iterator j = bvertedges->begin(); j != bvertedges->end(); j++){
			VPoint * cellvertex = (*j).first;
			ss5 << "Vertex x: " << cellvertex->x << " , y: " << cellvertex->y << "\n";
			vor::Edges * instedges = (*j).second;
			for(vor::Edges::iterator k = instedges->begin(); k != instedges->end(); k++) {
				vor::Vertices * instverts = (*bedgeverts)[*k];  //edge vertices
				//to iterate a vertex's linked neighbor.
				//need to append neighbor vertices to avoid reiterating a ray construction.
				for(vor::Vertices::iterator l = instverts->begin(); l != instverts->end();l++) {
					if (*l != cellvertex){
						//compute line points with this neighbor vertex as boundary
						ss5 << "neighbor point x: " << (*l)->x << " , y: " << (*l)->y << "\n";
					}
				}
			}
		}
		
	}
	tlog->logMessage(ss5.str());
}


void BuildVoronoi::rayCastLine(double f, double g, VPoint * start, VPoint * end) {

	VPoint *  point;
	double valy;
	double incdec = .001f;
	if (end->x - start->x < 0){
		incdec = -.001f;  //-1 > -8    -8 +1 = -7 < 0 
	} 
	
	double valx = start->x + incdec;
	cell->cellpolypoints->push_back(start);	
	if (incdec < 0){
		while (valx > end->x){
			valy = f*valx + g;
			point = new VPoint(valx,valy);
			bool test1 = valx >= 0 and valy >= 0;
			bool test2 = valx <= w and valy <= w;
			if (test1 and test2) {
				cell->cellpolypoints->push_back(point);
			}
			
			valx = valx + incdec;
		}
	}
	else {
		while (valx < end->x){
			valy = f*valx + g;
			point = new VPoint(valx,valy);
			cell->cellpolypoints->push_back(point);
			valx = valx + incdec;
		}
	}
	cell->cellpolypoints->push_back(end);

}

void BuildVoronoi::rayCastLine(double f, double g, VPoint * start) {
	//
}
#endif
