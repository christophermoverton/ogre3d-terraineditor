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
		vor::Cells * cells;      //voronoi cell data gathered prior to edges data return
		vor::Cells * cellspost;  //post processed voronoi cell edges data container.
		VoronoiCell * cell;  //temporary cell map container for internal use
		//vor::VPoint * cellpoint;                //temporary vertex point for internal use
		void rayCastLine(double f, double g, VPoint * start, VPoint * end);
		void rayCastLine(double f, double g, VPoint * start);  //overloaded function for incomplete edge
		vor::Vertices* traverseCellPath(vor::Vertices * pathlist, vor::VertEdges * bvertedges, vor::EdgeVerts * bedgeverts, VPoint * position);
		vor::Vertices* traverseCellPath(vor::Vertices * pathlist, VoronoiCell * cell, VPoint * position, bool pushfront);
		void centroid(VoronoiCell * cell);
};

BuildVoronoi::BuildVoronoi(){
	w = 10000;
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("Voronoi.log");
	Ogre::Log* tlog2 = Ogre::LogManager::getSingleton().createLog("Voronoi3.log");
	std::ostringstream ss5;
	ss5 << "Hitting Voronoi test1 !" << "\n";
    	tlog->logMessage(ss5.str());
    	ss5.str(std::string());
	v = new vor::Voronoi();
	cellspost = new vor::Cells();

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
	for(vor::Edges::iterator i = edg->begin(); i!= edg->end(); ++i)
	{
		/*  Comparing and building cell data from edg data (post processing method).
		*/
		if ((*cellspost).find((*i)->left) != (*cellspost).end()){

			VoronoiCell * siteposcell = (*cellspost)[(*i)->left];
			bool place, place2; place = false; place2 = false;
			for(vor::Vertices::iterator l = siteposcell->places->begin(); l != siteposcell->places->end();l++) {
				if (*l == (*i)->start){
					place = true;
				}
				if (*l == (*i)->end){
					place2 = true;
				}
				if (!(*i)->end){
					place2 = true;
				}
			}
			if (!place){
				siteposcell->places->push_back((*i)->start);
			}
			if (!place2){
				siteposcell->places->push_back((*i)->end);
			}
			place = false; place2 = false;
			vor::VertEdges * sitevertedges = siteposcell->vertedges;
			vor::EdgeVerts * siteedgeverts = siteposcell->edgeverts;
			///*
			if ((*sitevertedges).find((*i)->start) != (*sitevertedges).end()){
				for(vor::Edges::iterator l = (*sitevertedges)[(*i)->start]->begin(); l != (*sitevertedges)[(*i)->start]->end();l++) {
					if (*l == *i){
						place = true;
					}
				}
				if (!place){
					(*sitevertedges)[(*i)->start]->push_back(*i);
				}
				place = false;			
			}
			else{
				vor::Edges * nedges = new vor::Edges();
				nedges->push_back(*i);
			
				(*sitevertedges)[(*i)->start] = nedges;
				
			}
			if ((*sitevertedges).find((*i)->end) != (*sitevertedges).end()){
				for(vor::Edges::iterator l = (*sitevertedges)[(*i)->end]->begin(); l != (*sitevertedges)[(*i)->end]->end();l++) {
					if (*l == *i){
						place = true;
					}
					if (!(*i)->end){
						place = true;
						break;
					}
				}
				if (!place){
					(*sitevertedges)[(*i)->end]->push_back(*i);
				}
				place = false;	
			}
			else{
				vor::Edges * nedges = new vor::Edges();
				nedges->push_back(*i);
				if (!(*i)->end){
					place = true;
				}
				if (!place){
					(*sitevertedges)[(*i)->end] = nedges;
				}
				place = false;
				
			}
			if ((*siteedgeverts).find((*i)) != (*siteedgeverts).end()){
				for(vor::Vertices::iterator l = (*siteedgeverts)[(*i)]->begin(); l != (*siteedgeverts)[(*i)]->end();l++) {
					if (*l == (*i)->start){
						place = true;
					}
					if (*l == (*i)->end){
						place2 = true;
					}
					if (!(*i)->end){
						place2 = true;
					}
				}
				if (!place){
					(*siteedgeverts)[(*i)]->push_back((*i)->start);
				}
				if (!place2){
					(*siteedgeverts)[(*i)]->push_back((*i)->end);
				}
				place = false; place2 = false;

			}
			else{
				vor::Vertices * verts = new vor::Vertices();
				verts->push_back((*i)->start);
				if (!(*i)->end){
					place = true;
				}
				if (!place){				
					verts->push_back((*i)->end);
				}
				place = false;
				(*siteedgeverts)[(*i)] = verts;
				
			}
			//*/
			
		}
		else{
			(*cellspost)[(*i)->left] = new VoronoiCell((*i)->left);
			VoronoiCell * siteposcell = (*cellspost)[(*i)->left];
			siteposcell->places->push_back((*i)->start);
			siteposcell->places->push_back((*i)->end);
			///*
			vor::VertEdges * sitevertedges = siteposcell->vertedges;
			vor::EdgeVerts * siteedgeverts = siteposcell->edgeverts;
			
			vor::Edges * nedges = new vor::Edges();
			nedges->push_back(*i);
			bool place = false;
			(*sitevertedges)[(*i)->start] = nedges;
			if (!(*i)->end){
				place = true;
			}
			if (!place){
				(*sitevertedges)[(*i)->end] = nedges;
			}
			place = false;
			vor::Vertices * verts = new vor::Vertices();
			verts->push_back((*i)->start);
			if (!(*i)->end){
				place = true;
			}
			if (!place){
				verts->push_back((*i)->end);
			}
			place = false;
			(*siteedgeverts)[(*i)] = verts;
			//*/
		}
		///*
		if ((*cellspost).find((*i)->right)!= (*cellspost).end()){

			VoronoiCell * siteposcell = (*cellspost)[(*i)->right];
			bool place, place2; place = false; place2 = false;
			for(vor::Vertices::iterator l = siteposcell->places->begin(); l != siteposcell->places->end();l++) {
				if (*l == (*i)->start){
					place = true;
				}
				if (*l == (*i)->end){
					place2 = true;
				}
				if (!(*i)->end){
					place2 = true;
				}
			}
			if (!place){
				siteposcell->places->push_back((*i)->start);
			}
			if (!place2){
				siteposcell->places->push_back((*i)->end);
			}
			place = false; place2 = false;
			vor::VertEdges * sitevertedges = siteposcell->vertedges;
			vor::EdgeVerts * siteedgeverts = siteposcell->edgeverts;
			///*
			if ((*sitevertedges).find((*i)->start) != (*sitevertedges).end()){
				for(vor::Edges::iterator l = (*sitevertedges)[(*i)->start]->begin(); l != (*sitevertedges)[(*i)->start]->end();l++) {
					if (*l == *i){
						place = true;
					}
				}
				if (!place){
					(*sitevertedges)[(*i)->start]->push_back(*i);
				}
				place = false;			
			}
			else{
				vor::Edges * nedges = new vor::Edges();
				nedges->push_back(*i);
			
				(*sitevertedges)[(*i)->start] = nedges;
				
			}
			if ((*sitevertedges).find((*i)->end) != (*sitevertedges).end()){
				for(vor::Edges::iterator l = (*sitevertedges)[(*i)->end]->begin(); l != (*sitevertedges)[(*i)->end]->end();l++) {
					if (*l == *i){
						place = true;
					}
					if (!(*i)->end){
						place = true;
						break;
					}
				}
				if (!place){
					(*sitevertedges)[(*i)->end]->push_back(*i);
				}
				place = false;	
			}
			else{
				vor::Edges * nedges = new vor::Edges();
				nedges->push_back(*i);
				if (!(*i)->end){
					place = true;
				}
				if (!place){
					(*sitevertedges)[(*i)->end] = nedges;
				}
				place = false;
				
			}
			if ((*siteedgeverts).find((*i)) != (*siteedgeverts).end()){
				for(vor::Vertices::iterator l = (*siteedgeverts)[(*i)]->begin(); l != (*siteedgeverts)[(*i)]->end();l++) {
					if (*l == (*i)->start){
						place = true;
					}
					if (*l == (*i)->end){
						place2 = true;
					}
					if (!(*i)->end){
						place2 = true;
					}
				}
				if (!place){
					(*siteedgeverts)[(*i)]->push_back((*i)->start);
				}
				if (!place2){
					(*siteedgeverts)[(*i)]->push_back((*i)->end);
				}
				place = false; place2 = false;

			}
			else{
				vor::Vertices * verts = new vor::Vertices();

				verts->push_back((*i)->start);
				if (!(*i)->end){
					place = true;
				}
				if (!place){				

					verts->push_back((*i)->end);
				}
				place = false;
				(*siteedgeverts)[(*i)] = verts;
				
			}
			//*/
			
		}
		else{
			(*cellspost)[(*i)->right] = new VoronoiCell((*i)->right);
			VoronoiCell * siteposcell = (*cellspost)[(*i)->right];
			siteposcell->places->push_back((*i)->start);
			siteposcell->places->push_back((*i)->end);
			///*
			vor::VertEdges * sitevertedges = siteposcell->vertedges;
			vor::EdgeVerts * siteedgeverts = siteposcell->edgeverts;
			
			vor::Edges * nedges = new vor::Edges();

			nedges->push_back(*i);

			
			bool place = false;
			(*sitevertedges)[(*i)->start] = nedges;
			if (!(*i)->end){
				place = true;
			}
			if (!place){
				(*sitevertedges)[(*i)->end] = nedges;
			}
			place = false;
			vor::Vertices * verts = new vor::Vertices();
			verts->push_back((*i)->start);
			if (!(*i)->end){
				place = true;
			}
			if (!place){
				verts->push_back((*i)->end);
			}
			place = false;
			(*siteedgeverts)[(*i)] = verts;
			//*/
		}
	}

	ss5.str(std::string());
///*
	for(vor::Cells::iterator i = cellspost->begin(); i!= cellspost->end(); ++i)
	{
		ss5 << "Cell: " << (* (*i).second->sitePos).x << "," <<(* (*i).second->sitePos).y << "\n"; 
		vor::Vertices * verts = (*i).second->places;
		for(vor::Vertices::iterator j = verts->begin(); j != verts->end(); j++){
			ss5 << "x: " << (*j)->x << ", y: " << (*j)->y << "\n";
		}
		//cell = (*i).second;
		vor::VertEdges * bvertedges = (*i).second->vertedges;
		vor::EdgeVerts * bedgeverts = (*i).second->edgeverts;
		for(vor::VertEdges::iterator j = bvertedges->begin(); j != bvertedges->end(); j++){
			VPoint * cellvertex = (*j).first;
			ss5 << "Vertex x: " << cellvertex->x << " , y: " << cellvertex->y << "\n";
			vor::Edges * instedges = (*j).second;
			vor::Vertices * pointstrack = new vor::Vertices();
			bool stopcheck = false;
			for(vor::Edges::iterator k = instedges->begin(); k != instedges->end(); k++) {
				vor::Vertices * instverts = (*bedgeverts)[*k];  //edge vertices
				
				//to iterate a vertex's linked neighbor.
				//need to append neighbor vertices to avoid reiterating a ray construction.
				for(vor::Vertices::iterator l = instverts->begin(); l != instverts->end();l++) {
					if (!stopcheck){
						for(vor::Vertices::iterator m = pointstrack->begin(); m != pointstrack->end();l++) {
							if (*m == *l){
								(*i).second->duplicates->push_back(cellvertex);
								(*i).second->duplicates->push_back(*m);
								stopcheck = true;
								break;
							}
							
						}
					}
					if (*l != cellvertex){
						//compute line points with this neighbor vertex as boundary
						ss5 << "neighbor point x: " << (*l)->x << " , y: " << (*l)->y << "\n";
					}
					pointstrack->push_back(*l);
				}
			}

		}

		//insert here recursive pathlist build
		vor::VertEdges::iterator j = bvertedges->begin();
		vor::Vertices * pathlist = new vor::Vertices();
		cell->cellpolypoints = traverseCellPath(pathlist, cell, *j, false);		
	}
//*/
	tlog2->logMessage(ss5.str());
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

vor::Vertices* BuildVoronoi::traverseCellPath(vor::Vertices * pathlist, vor::VertEdges * bvertedges, vor::EdgeVerts * bedgeverts, VPoint * position){
	//reursive function traverses voronoi cell path 
	//position should automatically be populated in pathlist (initially). pathlist->push_back(position);
	vor::Edges * edges = (*bvertedges)[position];
	
	for (vor::Edges::iterator k = edges->begin(); k != edges->end(); k++){
		vor::Vertices * instverts = (*bedgeverts)[*k];
		for(vor::Vertices::iterator l = instverts->begin(); l != instverts->end();l++) {
			bool checkpath = false;
			VPoint * matchpoint;
			for (vor::Vertices::iterator m = pathlist->begin(); m != pathlist->end();m++){
				if (*l == *m){
					checkpath = true;
					matchpoint = new VPoint((*m)->x, (*m)->y);
				
				}
			}
			if (!checkpath){
				
				vor::Vertices * pathlist2 = traverseCellPath(pathlist, bvertedges, bedgeverts, *l);
				for (vor::Vertices::iterator m = pathlist2->begin(); m != pathlist2->end();m++){  
					//this will likely need to do ordered insertions on the list stack.
					 //push_back and push_front may not be adequate list populating, or
					 //in other words there is possibility for an interior edge crossing.
					
				}
			}
			else {
			}
		}
	}
	return pathlist;
}

vor::Vertices* BuildVoronoi::traverseCellPath(vor::Vertices * pathlist, VoronoiCell * cell, VPoint * position, bool pushfront){
	//reursive function traverses voronoi cell path 
	//position should automatically be populated in pathlist (initially). pathlist->push_back(position);
	vor::VertEdges * bvertedges = cell->vertedges;
	vor::EdgeVerts * bedgeverts = cell->edgeverts;
	vor::Edges * edges = (*bvertedges)[position];
	vor::Vertices * pointstrack = cell->duplicates;
	if (!pushfront){
		pathlist->push_back(position);
	}
	else{
		pathlist->push_front(position);
	}
	//bool pushfront = false;
	int ocount = 0;
	for (vor::Edges::iterator k = edges->begin(); k != edges->end(); k++){
		vor::Vertices * instverts = (*bedgeverts)[*k];
		for(vor::Vertices::iterator l = instverts->begin(); l != instverts->end();l++) {
			bool checkpath = false;
			//VPoint * matchpoint;
			for (vor::Vertices::iterator m = pathlist->begin(); m != pathlist->end();m++){
				if (*l == *m){
					checkpath = true;
					//matchpoint = new VPoint((*m)->x, (*m)->y);
				
				}
			}
			if (!checkpath){
				bool checkpath2 = false;
				for (vor::Vertices::iterator n = pointstrack->begin(); n != pointstrack->end();n++){
					if (*l == *n){
						checkpath2 = true;
						pathlist->push_front(*l);
						break;
					}
				}
				if (!checkpath2){
					if (ocount > 0){
						pushfront = true;
					}
					vor::Vertices * pathlist = traverseCellPath(pathlist, cell, *l, pushfront);
					ocount += 1;

				}
			}
			
		}
	}
	return pathlist;
}

void BuildVoronoi::centroid(VoronoiCell * cell){
	vor::Vertices * cellverts = cell->cellpolypoints;
	vor::Vertices::iterator i = cellverts->begin();
	double A = 0; double xc = 0; double yc = 0;
	double xi1,xi2, yi1,yi2;
	while (i != cellverts->end()){
		xi1 = *i->x; yi1 = *i->y;
		i++;
		//if (i != cellverts->end()){
			xi2 = *i->x; yi2 = *i->y;
			xc = (xi1 + xi2)*(xi1*yi2+xi2*yi1) + xc;
			yc = (yi1 + yi2)*(xi1*yi2+xi2*yi1) + yc;
			A = xi1*yi2+xi2*yi1 + A;
		//}
		//else{
		//	break;
		//}
	}
	A = .5f*A;
	xc = 1.0f/(6.0f*A)*xc;
	yc = 1.0f/(6.0f*A)*yc;
	cell->centroid = new VPoint(xc, yc);
}
#endif
