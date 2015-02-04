#ifndef __BuildVoronoi_CPP
#define __BuildVoronoi_CPP

#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>
#include "Imagestuff.cpp"

#include "Voronoi.h"
#include "VPoint.h"
#include "TPoint3.h"
#include "TerrainStruct.h"

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

I want a fast rasterization method for drawing a voronoi cell with some caveats.
Here I'd like to do a radial drawing method...this measures the radial distance of any 
point to a given voronoi site position.  This is given from a posting at my blog spot 
on this subject.  We can point in a point in geometry testing method here...settling in 
this case for a ray test.  We start the ray originally at the cell site posiiton.  We cast 
a ray in 4 different directions (up, down, left, and right).  In this case each increment of 
the ray comes with a point in geometry test to ensure such a point is in the geometry.
Now each ray then spawns a orthogonal child rays in opposite directions.  In this case,
as we incrementally advance the ray (one integer unit), we use the same point in geometry test
.  If the test fails, then the ray is finished...all child rays are advanced respectively 
until finished, and then we increment the parent rays until they too are finished in the succession
of new children.

A point in geometry test comes by way of the direction of a given ray, by its position and 
by the position of vertices.  If we find that a ray is likely to intersect two or zero edges
then our point fails the test.  Note :  we only need to test in the direction of the ray, since
it is assumed precursor points did not fail and thus testing in the opposite direction is not
needed.  Also our test becomes easy since we are are working in the direction of the basis of 
our coordinate system, in this case, we hadn't need apply formula for intersections say of two 
lines...instead we find the edge segment vertices opposite coordinate points relative to the 
direction of the ray.  Thus if the ray is traversing the y axis we examine the x coordinate positions
of all edges.  Those edges whose x coordinate vertices are greater and respectively less than 
a given ray x position is the edge of intercept...if both the y coordinate positions of either edge
vertex are less than (while positive y ray direction testing) such ray position means the ray position
point is no longer in geometry (actually this is a bit crude)...more so we'd 
simply use the edges line information and project where the ray intercepts for such test.  
The opposite is true for negative y ray testing.  We can apply 
similar logic for ray x direction testing.  Lastly when parent ray testing on say the y axis,
for instance, we may need to perform a ray shift horizontally so that a given parent y direction
ray is shifted horizontally to conform to a position that is still interiror to the geometry,
and given below the polygon's max min vertex (respectively).  Procedurally the process is repeated
creating child horizontal ray's filling out the  remainder of the polygon's interior points, or likely
we could do this at the outset of parent ray construction (so knowing that a given point is interior).
All this comes by way also of the polygon's monotonicity.

Technically at the outset limitations to iteration and avoiding added edge checking can be done
at the outset of any ray's construction.  Since we know where intersection is to occur for 
a given edge, we can therefore limit any ray casting iterator to such maximum/minimum position
and forgo the necessity of edge testing.

I've added one second layered points building pass to the algorithm ensuring that any point
not covered are covered. 2nd pass avoids generally much added computational pass making use
of iterated maps and work done in the first pass.

*/
class BuildVoronoi{

	public:
		BuildVoronoi();
		terr::T3dCPointsMap * getHeightMapradial(void);
		terr::CPointsMap * getHeightMapradial2(void); //alternate coordinate addressing system
	private:
		double w        ;
		vor::Voronoi * v;
		vor::Vertices * ver; // vrcholy
		vor::Vertices * dir; // smìry, kterými se pohybují
		vor::Edges * edg;	 // hrany diagramu
		vor::Cells * cells;      //voronoi cell data gathered prior to edges data return
		vor::Cells * cellspost;  //post processed voronoi cell edges data container.
		VoronoiCell * cell;  //temporary cell map container for internal use
		vor::CoordtoCellMap * coordtocell;
		terr::T3dCPointsMap * rtnmap;  //voronoi cell map with radial normalized heightmap values.
		terr::CPointsMap * crtnmap;  // non pointer coordinate addressing
		Ogre::Log* tlog3;
		//vor::VPoint * cellpoint;                //temporary vertex point for internal use
		void rayCastLine(double f, double g, VPoint * start, VPoint * end);
		void rayCastLine(double f, double g, VPoint * start);  //overloaded function for incomplete edge
		vor::Vertices* traverseCellPath(vor::Vertices * pathlist, vor::VertEdges * bvertedges, vor::EdgeVerts * bedgeverts, VPoint * position);
		vor::Vertices* traverseCellPath(vor::Vertices * pathlist, VoronoiCell * cell, VPoint * position, bool pushfront, int ct);
		void centroid(VoronoiCell * cell);
		void gcentroid(VoronoiCell * cell);
		double solveX(double y, VEdge * edge);
		double solveY(double x, VEdge * edge);
		vor::Dvalues getMinMaxvalues(VPoint * origin, vor::VEdgemap * siteedgemap, bool xaxis);
		vor::CPointsMap * fillpoints(vor::CPointsMap * cpointsmap, vor::VEdgemap * siteedgemap, VPoint * sitepos, VPoint * basepos, bool top);
		VPoint * getBase(VPoint * basepos, VPoint * ymaxvert, vor::VEdgemap * siteedgemap);
		VPoint * getMaxYVertex(vor::VertEdges * bvertedges);
		VPoint * getMinYVertex(vor::VertEdges * bvertedges);
		vor::CPointsMap * buildPoints(vor::Cells * cellsone);
		
};

BuildVoronoi::BuildVoronoi(){
	w = 513;
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("Voronoi.log");
	Ogre::Log* tlog2 = Ogre::LogManager::getSingleton().createLog("Voronoi3.log");
	tlog3 = Ogre::LogManager::getSingleton().getLog("Voronoi3.log");
	std::ostringstream ss5;
	ss5 << "Hitting Voronoi test1 !" << "\n";
    	tlog->logMessage(ss5.str());
    	ss5.str(std::string());
	v = new vor::Voronoi();
	cellspost = new vor::Cells();
	crtnmap = new terr::CPointsMap();
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
			vor::VEdgemap * siteedgemap = siteposcell->edgemap;
			vor::Vertpair * vpair = new vor::Vertpair((*i)->start,(*i)->end);
			(*siteedgemap)[(*vpair)] = *i;
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

			vor::VEdgemap * siteedgemap = siteposcell->edgemap;
			vor::Vertpair * vpair = new vor::Vertpair((*i)->start,(*i)->end);
			(*siteedgemap)[(*vpair)] = *i;
			
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
			vor::VEdgemap * siteedgemap = siteposcell->edgemap;
			vor::Vertpair * vpair = new vor::Vertpair((*i)->start,(*i)->end);
			(*siteedgemap)[(*vpair)] = *i;
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

			vor::VEdgemap * siteedgemap = siteposcell->edgemap;
			vor::Vertpair * vpair = new vor::Vertpair((*i)->start,(*i)->end);
			(*siteedgemap)[(*vpair)] = *i;
			
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
		vor::VEdgemap * bedgemap = (*i).second->edgemap;
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
					/*
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
					*/
					if (*l != cellvertex){
						//compute line points with this neighbor vertex as boundary
						ss5 << "neighbor point x: " << (*l)->x << " , y: " << (*l)->y << "\n";
						
						for(vor::Vertices::iterator m = pointstrack->begin(); m != pointstrack->end();m++) {
							if (*m == *l){
								vor::Vertices * dup = (*i).second->duplicates;
								bool checkd = false;
								for(vor::Vertices::iterator n = dup->begin(); n != dup->end();n++) {
									if (*l == *n){
										checkd = true;
										break;
									}
								}
								if (!checkd){
									(*i).second->duplicates->push_back(cellvertex);
									(*i).second->duplicates->push_back(*m);
									stopcheck = true;
								break;
								}
							}
							
						}
						pointstrack->push_back(*l);
						
					}
					 
				}
			}
			(*pointstrack).clear();
			//while(!(*pointstrack).empty()) delete (*pointstrack).front(), (*pointstrack).pop_front();

		}

		//insert here recursive pathlist build
		vor::VertEdges::iterator j = bvertedges->begin();
		vor::Vertices * pathlist = new vor::Vertices();
		(*i).second->cellpolypoints = traverseCellPath(pathlist, (*i).second, (*j).first, false, 0);
		pathlist = (*i).second->cellpolypoints;
		vor::Vertices * dupls = (*i).second->duplicates;
		for(vor::Vertices::iterator l = dupls->begin(); l != dupls->end();l++) {
			ss5 << "Duplicates coordinate x: " << (*l)->x << ", y: " << (*l)->y << "\n";	
		}
		for(vor::Vertices::iterator l = pathlist->begin(); l != pathlist->end();l++) {
			ss5 << "Path coordinate x: " << (*l)->x << ", y: " << (*l)->y << "\n";	
		}
		gcentroid((*i).second);
		
		ss5 << "Centroid coordinate x: " << (*i).second->centroid->x << ", y: " << (*i).second->centroid->y << "\n";
		//now that we have centroid computed we have completed relaxation step...at this point you can refeed 
		//centroid coordinates for each back into this loop to recompute new voronoi diagram.  It appears that the given
		//formulation for a finite set of points on the polygon is not right...or something is off...I might try 
		//a geometric decomposition approach to see if the result is the same.
		///*	
		for(vor::VEdgemap::iterator j = bedgemap->begin(); j != bedgemap->end(); j++){
			vor::Vertpair vpair = (*j).first;  VPoint * v1p = vpair.first; VPoint * v2p = vpair.second;
			ss5 << "1rst x: "<< v1p->x << " 1rst y: " << v1p->y <<"\n";
			ss5 << "2nd x: "<< v2p->x << " 2nd y: " << v2p->y <<"\n";
		}
		//*/	
	}
//*/
	vor::Coordpair * point1 = new vor::Coordpair(1,1);
	vor::Coordpair * point2 = new vor::Coordpair(1,1);
	bool test1 = (*point1) == (*point2);
	ss5 << "Point 1 equals Point 2 " << test1 << "\n";
	ss5 << "Number of cells: " << (*cellspost).size() << "\n";
	tlog2->logMessage(ss5.str());
	vor::CPointsMap * pointsmap = buildPoints(cellspost);
	
	
	double size = 513.0f;
	ImageBuffer buffer(size);
    	FillColour* fill = new FillColour (&buffer);
    	//buffer.saveImage("test1.png");
	///*
	rtnmap = new terr::T3dCPointsMap();
	vor::Coordpair * mcoordpair = new vor::Coordpair(w+1, w+1);
	//TPoint3 * rmcoordpair = new TPoint3(w+1,w+1, 0);
	//(*rtnmap)[rmcoordpair] = (*pointsmap)[(*mcoordpair)];
	double div = (*pointsmap)[(*mcoordpair)];
	for (int j = 0; j<size; j++){
		for (int k = 0; k<size; k++){
			//ss5<<"Color value: "<<noisevals[i][j]<<"\n";
			double mult = w/size; int jmn = j*mult; int kmn = k*mult; double jmm = (double)j*mult, kmm = (double)k*mult; 
			double jdiff = jmm-jmn, kdiff = kmm-kmn;
			if (jdiff >= .5){
				jmn += 1;
			}
			if (kdiff >= .5){
				kmn += 1;
			}
			vor::Coordpair * coordpair = new vor::Coordpair(jmn, kmn);
			terr::Coordpair * coordpair2 = new terr::Coordpair(jmn, kmn);
			TPoint3 * rtnmapcoord = new TPoint3(jmn, kmn, 0);
			if ((*pointsmap).find((*coordpair)) != (*pointsmap).end()){
				double colval = (*pointsmap)[(*coordpair)];
				
				(*rtnmap)[rtnmapcoord] = colval/div;
				(*crtnmap)[(*coordpair2)] = colval/div;
				Ogre::ColourValue col = Ogre::ColourValue(colval/div,colval/div,colval/div);
				fill->setPixl((size_t)j, (size_t)k, col);
			}
			else{
				
				coordpair = new vor::Coordpair(jmn+1, kmn);
				Ogre::ColourValue col;
				if ((*coordtocell).find((*coordpair)) != (*coordtocell).end()){
					VPoint * sitepos = (*coordtocell)[(*coordpair)];
					double colval = pow(pow(sitepos->y-kmn,2.0f) + pow(sitepos->x-jmn, 2.0f),.5f);
					(*rtnmap)[rtnmapcoord] = colval/div;
					(*crtnmap)[(*coordpair2)] = colval/div;
					col = Ogre::ColourValue(colval/div,colval/div,colval/div);
				}
				else {
					coordpair = new vor::Coordpair(jmn, kmn+1);
					if ((*coordtocell).find((*coordpair)) != (*coordtocell).end()){
						VPoint * sitepos = (*coordtocell)[(*coordpair)];
						double colval = pow(pow(sitepos->y-kmn,2.0f) + pow(sitepos->x-jmn, 2.0f),.5f);
						(*rtnmap)[rtnmapcoord] = colval/div;
						(*crtnmap)[(*coordpair2)] = colval/div;
						col = Ogre::ColourValue(colval/div,colval/div,colval/div);
					}
					else{
						(*rtnmap)[rtnmapcoord] = 1.0f;
						(*crtnmap)[(*coordpair2)] = 1.0f;
						col = Ogre::ColourValue(1.0f,1.0f,1.0f);
					}
				     }
				fill->setPixl((size_t)j, (size_t)k, col);
			}

			//fill->setPixl((size_t)j, (size_t)k, col);
		}
	}
	buffer.saveImage("test4.png");
	//*/
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

vor::Vertices* BuildVoronoi::traverseCellPath(vor::Vertices * pathlist, VoronoiCell * cell, VPoint * position, bool pushfront, int ct){
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
				if (((*l)->x == (*m)->x) and ((*l)->y == (*m)->y)){
					checkpath = true;
					//matchpoint = new VPoint((*m)->x, (*m)->y);
				
				}
			}
			if (!checkpath){
				bool checkpath2 = false;
				for (vor::Vertices::iterator n = pointstrack->begin(); n != pointstrack->end();n++){
					if (((*l)->x == (*n)->x) and ((*l)->y == (*n)->y)){
						checkpath2 = true;
						//pathlist->push_front(*l);
						break;
					}
				}
				if (!checkpath2){
					if (ocount > 0){
						pushfront = true;
					}

					//if (ct < 1){
					//	ct += 1;
				
					pathlist = traverseCellPath(pathlist, cell, *l, pushfront, ct);
					//}

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
		xi1 = (*i)->x; yi1 = (*i)->y;
		i++;
		//if (i != cellverts->end()){
			xi2 = (*i)->x; yi2 = (*i)->y;
			xc = (xi1 + xi2)*(xi1*yi2-xi2*yi1) + xc;
			yc = (yi1 + yi2)*(xi1*yi2-xi2*yi1) + yc;
			A = xi1*yi2-xi2*yi1 + A;
		//}
		//else{
		//	break;
		//}
	}
	A = .5f*A;
	xc = (1.0f/(6.0f*A))*xc;
	yc = (1.0f/(6.0f*A))*yc;
	cell->centroid = new VPoint(xc, yc);
}

void BuildVoronoi::gcentroid(VoronoiCell * cell){
	VPoint * sitepos = cell->sitePos;
	vor::Vertices * cellverts = cell->cellpolypoints;
	vor::Vertices::iterator i = cellverts->begin();
	double A = 0; double xc = 0; double yc = 0;
	double xi1,xi2,xi3, yi1,yi2,yi3,xct,yct,At;
	xi3 = sitepos->x; yi3 = sitepos->y;
	while (i != cellverts->end()){
		xi1 = (*i)->x; yi1 = (*i)->y;
		i++;
		//if (i != cellverts->end()){
		xi2 = (*i)->x; yi2 = (*i)->y;
		xct = 1.0f/3.0f*(xi1 +xi2 + xi3);
		yct = 1.0f/3.0f*(yi1 +yi2 + yi3);
		At = abs(xi1*(yi2-yi3) + xi2*(yi3-yi1)+xi3*(yi1-yi2));
		xc = At*xct + xc;
		yc = At*yct + yc;
		A = At + A;
		//}
		//else{
		//	break;
		//}
	}
	
	xc = xc/A;
	yc = yc/A;
	cell->centroid = new VPoint(xc, yc);
}

double BuildVoronoi::solveX(double y, VEdge * edge){
	//y = f*x + g
	double f = edge->f; double g = edge->g;
	return 1.0f/f * (y-g);
}

double BuildVoronoi::solveY(double x, VEdge * edge){
	//y = f*x + g
	double f = edge->f; double g = edge->g;
	return f*x+g;
}

vor::Dvalues BuildVoronoi::getMinMaxvalues(VPoint * origin, vor::VEdgemap * siteedgemap, bool xaxis){
	std::ostringstream ss5;
	vor::Dvalues dvals; //= new vor::Dvalues();
	
	double vmin;
	if (xaxis){
		vmin = origin->x;
	}
	else{
		vmin = origin->y;
	}
	double vmax = 0; int ct = 0; bool siteapl = false, siteapr = false;
	for (vor::VEdgemap::iterator j = siteedgemap->begin(); j!=siteedgemap->end(); j++){
		vor::Vertpair vpair = (*j).first;
		VPoint * v1 = vpair.first; VPoint * v2 = vpair.second;
		
		if (xaxis){
			if ((v1->y <= origin->y) and (v2->y >= origin->y)){
				double x = solveX(origin->y,(*j).second);
				if ((abs(x-origin->x) < 1) and (x <= origin->x)){
					siteapl = true;
				}
				if ((abs(x-origin->x) < 1) and (x >= origin->x)){
					siteapr = true;
				}
				if (x > origin->x){
					if (x > vmax){
						vmax = x;
					}
				}
				else{
					if (x < vmin){
						vmin = x;
					}
				}

			}
			else if ((v1->y >= origin->y) and (v2->y <= origin->y)) {
				double x = solveX(origin->y,(*j).second);
				if (abs(x-origin->x) < 1 and (x <= origin->x)){
					siteapl = true;
				}
				if (abs(x-origin->x) < 1 and (x >= origin->x)){
					siteapr = true;
				}
				if (x > origin->x){
					if (x > vmax){
						vmax = x;
					}
				}
				else{
					if (x < vmin){
						vmin = x;
					}
				}
			}
			

		}
		else{
			if ((v1->x <= origin->x) and (v2->x >= origin->x)){
				double y = solveY(origin->x,(*j).second);
				if (abs(y-origin->y) < 1){
					siteapr = true;
				}
				if (y > origin->y){
					if (y > vmax){
						vmax = y;
					}
				}
				else{
					if (y < vmin){
						vmin = y;
					}
				}

			}
			else if ((v1->x >= origin->x) and (v2->x <= origin->x)) {
				double y = solveY(origin->x,(*j).second);
				if (abs(y-origin->y) < 1){
					siteapr = true;
				}
				if (y > origin->y){
					if (y > vmax){
						vmax = y;
					}
				}
				else{
					if (y < vmin){
						vmin = y;
					}
				}
			}
		}
	}
	if (xaxis){
		//this condition set causes failure when called through reiterated basepos set
		//need to exclude a basepos which is a max/min position point on the voronoi
		//cell 
		if (vmax < origin->x){
			vmax = origin->x;

		}
		if (vmin > origin->x){
			vmin = origin->x;
		}
		if ((vmin == origin->x) and (!siteapl)){
			vmin = 0;
		}
		if ((vmax == origin->x) and (!siteapr)){
			vmax = w;
		}
	}
	else {
		if (vmax < origin->y){
			vmax = origin->y;
			ss5 << "Vmax equals origin, Cell site: " << origin->x << "," << origin->y << "\n";
			tlog3->logMessage(ss5.str());
		}
		if ((vmin == origin->y) and (!siteapr)){
			vmin = 0;
		}
		if ((vmax == origin->y) and (!siteapr)){
			vmax = w;
		}		
		ss5 << "Cell site: " << origin->x << "," << origin->y << "\n";
		if (vmax > w){
			ss5 << "ymax: " << w << "\n";
		}
		else{
			ss5 << "ymax: " << vmax << "\n";
		}
		if (vmin < 0){
			ss5 << "ymin: " << 0 << "\n";
		}
		else{
			ss5 << "ymin: " << vmin << "\n";
		}
		tlog3->logMessage(ss5.str());
	}
	if (vmax > w){
		vmax = w;
	}
	if (vmin < 0){
		vmin = 0;
	}

	dvals.push_back(vmin);
	dvals.push_back(vmax);

	return dvals;
}

VPoint * BuildVoronoi::getMaxYVertex(vor::VertEdges * bvertedges){
	VPoint * maxpoint;
	double ymax = 0;
	for (vor::VertEdges::iterator i = bvertedges->begin(); i!=bvertedges->end();i++){
		if ((*i).first->y > ymax){
			maxpoint = (*i).first;
			ymax = (*i).first->y;
		}
	}

	return maxpoint;
}

VPoint * BuildVoronoi::getMinYVertex(vor::VertEdges * bvertedges){
	VPoint * minpoint;
	vor::VertEdges::iterator j = bvertedges->begin();
	double ymin = (*j).first->y;
	for (vor::VertEdges::iterator i = bvertedges->begin(); i!=bvertedges->end();i++){
		if ((*i).first->y < ymin){
			minpoint = (*i).first;
			ymin = (*i).first->y;
		}
	}

	return minpoint;
}

vor::CPointsMap * BuildVoronoi::fillpoints(vor::CPointsMap * cpointsmap, vor::VEdgemap * siteedgemap, VPoint * sitepos, VPoint * basepos, bool top){
	double y = basepos->y;
	vor::Coordpair * coordpair1 = new vor::Coordpair(basepos->x, basepos->y);
	(*cpointsmap)[(*coordpair1)] = pow(pow(basepos->y-sitepos->y,2.0f) + pow(basepos->x-sitepos->x, 2.0f),.5f);
	double x = basepos->x;
	vor::Dvalues yvals = getMinMaxvalues(basepos, siteedgemap, false);
	VPoint * mxpnt = new VPoint(w+1,w+1);
	vor::Coordpair *coordpair2 = new vor::Coordpair(w+1,w+1);
	double maxval = (*cpointsmap)[(*coordpair2)]; double dist;
	if (top){
		
		double ymax = yvals[1];
		while(y < ymax){
			VPoint * o = new VPoint(x,y);
			vor::Dvalues dvals = getMinMaxvalues(o, siteedgemap, true);
			double dmax = dvals[1]; double dmin = dvals[0]; int i = x; int j = y; int k = y;
			while (i <= dmax){
				VPoint * ipos = new VPoint(i, j);
				dist = pow(pow(sitepos->y-j,2.0f) + pow(sitepos->x-i, 2.0f),.5f);
				vor::Coordpair * coordpair = new vor::Coordpair(i, j);
				(*cpointsmap)[(*coordpair)] = dist;
				(*coordtocell)[(*coordpair)] = sitepos;
				if (maxval < dist){
					maxval = dist;
				}
				//(*pointsmap)[ipos] = pow(pow(sitepos->y-j,2.0f) + pow(sitepos->x-i, 2.0f),.5f);
				i += 1;
			}
			i = x;
			while (i >= dmin){
				VPoint * ipos = new VPoint(i, j);
				dist = pow(pow(sitepos->y-j,2.0f) + pow(sitepos->x-i, 2.0f),.5f);
				vor::Coordpair * coordpair = new vor::Coordpair(i, j);
				(*cpointsmap)[(*coordpair)] = dist;
				(*coordtocell)[(*coordpair)] = sitepos;
				if (maxval < dist){
					maxval = dist;
				}
				//(*pointsmap)[ipos] = pow(pow(sitepos->y-j,2.0f) + pow(sitepos->x-i, 2.0f),.5f);
				i -= 1;
			}
			y += 1;
		}
	}
	else{
		double ymin = yvals[0];
		while(y > ymin){
			VPoint * o = new VPoint(x,y);
			vor::Dvalues dvals = getMinMaxvalues(o, siteedgemap, true);
			double dmax = dvals[1]; double dmin = dvals[0]; int i = x; int j = y; int k = y;
			while (i <= dmax){
				VPoint * ipos = new VPoint(i, j);
				dist = pow(pow(sitepos->y-j,2.0f) + pow(sitepos->x-i, 2.0f),.5f);
				vor::Coordpair * coordpair = new vor::Coordpair(i, j);
				(*cpointsmap)[(*coordpair)] = dist;
				(*coordtocell)[(*coordpair)] = sitepos;
				if (maxval < dist){
					maxval = dist;
				}
				//(*pointsmap)[ipos] = pow(pow(sitepos->y-j,2.0f) + pow(sitepos->x-i, 2.0f),.5f);
				i += 1;
			}
			i = x;
			while (i >= dmin){
				VPoint * ipos = new VPoint(i, j);
				dist = pow(pow(sitepos->y-j,2.0f) + pow(sitepos->x-i, 2.0f),.5f);
				vor::Coordpair * coordpair = new vor::Coordpair(i, j);
				(*cpointsmap)[(*coordpair)] = dist;
				(*coordtocell)[(*coordpair)] = sitepos;
				if (maxval < dist){
					maxval = dist;
				}
				//(*pointsmap)[ipos] = pow(pow(sitepos->y-j,2.0f) + pow(sitepos->x-i, 2.0f),.5f);
				i -= 1;
			}
			y -= 1;
		}
	}
	//VPoint * mxpnt = new VPoint(w+1,w+1);
	(*cpointsmap)[(*coordpair2)] = maxval;
	return cpointsmap;
}

VPoint * BuildVoronoi::getBase(VPoint * basepos, VPoint * ymaxvert, vor::VEdgemap * siteedgemap){
	vor::Dvalues dvals = getMinMaxvalues(basepos, siteedgemap, true);
	double xmin = dvals[0]; double xmax = dvals[1];
	if ((ymaxvert->x <= xmax) and (ymaxvert->x >= xmin)){
		VPoint * rpoint = new VPoint(ymaxvert->x, basepos->y);
		return rpoint;	
	}
	else{
		VPoint * rpoint;
		if(ymaxvert->x > xmax){
			rpoint = new VPoint(xmax, basepos->y);
		}
		else if (ymaxvert->x < xmin){
			rpoint = new VPoint(xmin, basepos->y);
		}
		return rpoint;
	}
}

vor::CPointsMap * BuildVoronoi::buildPoints(vor::Cells * cellsone){
	//Using a up and down vertical rays from the cell site and then spawning child horizontal rays 
	//to trace point drawing.
	//Likely to use a recursive or while loop condition to ladder up and down 
	//the polygon (since starting at the cell site the given corresponding ray y max in poly
	//may not correspond to the poly's maximum y vertex position.  So will need to use a
	//a horizontal ladder upon reaching y max that either chooses a new x position spawning 
	// a new vertical y ray to reach such max vertex y, or choose a subdivision of x repeatedly 
	//until choosing a point in the polygon and then repeating this process until honing on 
	//the poly's absolute y max.  A similar approach is used in hitting the poly's vertices y
	//min.
	std::ostringstream ss5;
	vor::PointsMap * pointsmap = new vor::PointsMap();
	vor::CPointsMap * cpointsmap = new vor::CPointsMap();
	coordtocell = new vor::CoordtoCellMap();
	double maxval = 0.0f;
	for(vor::Cells::iterator i = cellsone->begin(); i != cellsone->end(); i++) {
		VoronoiCell * cellp = (*i).second;
		vor::VEdgemap * siteedgemap = cellp->edgemap;
		vor::VertEdges * bvertedges = cellp->vertedges;
		VPoint * sitepos = cellp->sitePos;
		//get y max and y min from sitepos->x.
		bool checkp = false; bool checkp2 = false;
		double ymax = 0, ymin = 0; int ct = 1;
		
		/*
		for (vor::VEdgemap::iterator j = siteedgemap->begin(); j!=siteedgemap->end(); j++){
			vor::Vertpair vpair = (*j).first;
			VPoint * v1 = vpair.first; VPoint * v2 = vpair.second;
			if ((v1->x <= sitepos->x) and (v2->x >= sitepos->x)){
				double y = solveY(sitepos->x,(*j).second);
				if (y > sitepos->y){
					ymax = y;
				}
				else{
					ymin = y;
				}
				checkp = true;
				if (ct > 1) {
					checkp2 = true;
				}
				ct += 1;
			}
		}
		if (ymax < sitepos->y){
			ymax = w;
		}
		if (ymax > w){
			ymax = w;
		}
		if (ymin < 0){
			ymin = 0;
		}
		*/
		vor::Dvalues dymax = getMinMaxvalues(sitepos, siteedgemap, false);
		ymax = dymax[1], ymin = dymax[0];
		//positive y iteration
		double y = sitepos->y;
		(*pointsmap)[sitepos] = 0.0f;
		double x = sitepos->x;
		while(y < ymax){
			VPoint * o = new VPoint(x,y);
			vor::Dvalues dvals = getMinMaxvalues(o, siteedgemap, true);
			double dmax = dvals[1]; double dmin = dvals[0], dist; int l = x; int m = y; int k = y;
			 
			while (l <= dmax){
				VPoint * ipos = new VPoint(l, m);
				dist = pow(pow(sitepos->y-m,2.0f) + pow(sitepos->x-l, 2.0f),.5f);
				(*pointsmap)[ipos] = dist;
				vor::Coordpair * coordpair = new vor::Coordpair(l, m);
				(*cpointsmap)[(*coordpair)] = dist;
				(*coordtocell)[(*coordpair)] = sitepos;
				if (maxval < dist){
					maxval = dist;
				}
				l += 1;
			}
			l = x;
			while (l >= dmin){
				VPoint * ipos = new VPoint(l, m);
				dist = pow(pow(sitepos->y-m,2.0f) + pow(sitepos->x-l, 2.0f),.5f);
				(*pointsmap)[ipos] = dist;
				vor::Coordpair * coordpair = new vor::Coordpair(l, m);
				(*cpointsmap)[(*coordpair)] = dist;
				(*coordtocell)[(*coordpair)] = sitepos;
				if (maxval < dist){
					maxval = dist;
				}
				l -= 1;
			}
			y += 1;
		}
		
		y = sitepos->y;
		while(y > ymin){
			VPoint * o = new VPoint(x,y);
			vor::Dvalues dvals = getMinMaxvalues(o, siteedgemap, true);
			double dmax = dvals[1]; double dmin = dvals[0], dist; int l = x; int m = y; int k = y;
			while (l <= dmax){
				VPoint * ipos = new VPoint(l, m);
				dist = pow(pow(sitepos->y-m,2.0f) + pow(sitepos->x-l, 2.0f),.5f);
				(*pointsmap)[ipos] = dist;
				vor::Coordpair * coordpair = new vor::Coordpair(l, m);
				(*cpointsmap)[(*coordpair)] = dist;
				(*coordtocell)[(*coordpair)] = sitepos;
				if (maxval < dist){
					maxval = dist;
				}
				//(*pointsmap)[ipos] = pow((pow((sitepos->y-m),2.0f) + pow((sitepos->x-l), 2.0f)),.5f);
				l += 1;
			}
			l = x;
			while (l >= dmin){
				
				VPoint * ipos = new VPoint(l, m);
				dist = pow(pow(sitepos->y-m,2.0f) + pow(sitepos->x-l, 2.0f),.5f);
				(*pointsmap)[ipos] = dist;
				vor::Coordpair * coordpair = new vor::Coordpair(l, m);
				(*cpointsmap)[(*coordpair)] = dist;
				(*coordtocell)[(*coordpair)] = sitepos;
				if (maxval < dist){
					maxval = dist;
				}
				//(*pointsmap)[ipos] = pow((pow((sitepos->y-m),2.0f) + pow((sitepos->x-l), 2.0f)),.5f);
				l -= 1;
			}
			y -= 1;
		}
		VPoint * mxpnt = new VPoint(w+1,w+1);
		(*pointsmap)[mxpnt] = maxval;
		vor::Coordpair * coordpair = new vor::Coordpair(w+1, w+1);
		(*cpointsmap)[(*coordpair)] = maxval;
                ///*  This code block causing issues on finishing Voronoi cell...need to resolve this.
		VPoint * basepos = new VPoint(sitepos->x, ymax-1);
		VPoint * ymaxvert = getMaxYVertex(bvertedges);
		ss5 << "Cell site: " << sitepos->x << " , " << sitepos->y << "\n";
		ss5 << "Max Vertex: " << ymaxvert->x << " , " << ymaxvert->y << "\n";
		//VPoint * bpoint = getBase(basepos, ymaxvert, siteedgemap);

		///*

		ct = 0;
		if (!(ymax == w)){

			while (basepos->y < ymaxvert->y){
				basepos = getBase(basepos, ymaxvert, siteedgemap);
				vor::Dvalues dvals = getMinMaxvalues(basepos, siteedgemap, false);
				ymax = dvals[1];
				ss5 << "y max Vertex: " << ymax << "\n";
				cpointsmap = fillpoints(cpointsmap, siteedgemap, sitepos, basepos, true);
				basepos = new VPoint(basepos->x,ymax);
				if (ymax >= w){
					break;
				}
				if (ymax <= 0){
					break;
				}
				if (ct > 5){
					break;
				}
				ct += 1;
			}
		}
		basepos = new VPoint(sitepos->x, ymin+1);
		VPoint * yminvert = getMinYVertex(bvertedges);
		ct = 0;
		ss5 << "Min Vertex: " << yminvert->x << " , " << yminvert->y << "\n";
		///*
		if (!(ymin == 0)){
			while (basepos->y > yminvert->y){
				basepos = getBase(basepos, yminvert, siteedgemap);
				vor::Dvalues dvals = getMinMaxvalues(basepos, siteedgemap, false);
				ymin = dvals[0];
				ss5 << "y min Vertex: " << ymin << "\n";
				cpointsmap = fillpoints(cpointsmap, siteedgemap, sitepos, basepos, false);
				basepos = new VPoint(basepos->x,ymin);
				if (ymin == 0){
					break;
				}
				if (ymin == w){
					break;
				}
				if (ct > 5){
					break;
				}
				ct += 1;
			}
		}
		//*/
	}
	tlog3->logMessage(ss5.str());
	return cpointsmap;  
}

terr::T3dCPointsMap * BuildVoronoi::getHeightMapradial(void){
	return rtnmap;
}

terr::CPointsMap * BuildVoronoi::getHeightMapradial2(void){
	return crtnmap;
}
#endif
