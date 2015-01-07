
#include "Voronoi.h"
#include <iostream>
#include <sstream>

#include <algorithm>
#include <set>
#include <cmath>
#include <fstream>

using namespace vor;

//while division by zero errors may be rare for a random point set generation in this algorithm.
//This algorithm appears not to be clearly resolved for division by zero problems.
/*
Algorithm description notes:  

Generically speaking the overlaying structure of the algorithm follows the 
Fortune model.  There is a key point however to be had in following:

-parabola insertion events, this with a site node event means 
 pulling the nearest parent parabola to a given site node which is adding
 a new parabola, and adding to this a new edge.  This can be a bit tricky 
 since the directrix line (wavefront line) which controls the shape and size
 of the parabola is always changing and hence any x position that a new site
 node occcupies in time could have depending on the beach front have a differring
 parent parabola returned for such position even with smaller variations in the 
 wave front line.  This algorithm tracks to a parent parabola using a binary 
 tree search of child parabola intersections starting from the root (parent 
 of all parent parabolas) until hitting a nearest child leaf parabola (beach front parabola).
 to such position.  Then having this parabola determines the site to site or edge
 property of parent child parabola intersection upon initialization.

 Furthered details of this search utilizes either a minimum or maximum value 
 of the parabolic curve intersections depending on either child site nodes y position
in relation to one another.  Obviously one child site node, for instance, having 
a higher y position, for instance, relative another child site node means as the wave front
line is descending on the y axis that the high y position site node will form a larger 
parabola which in turn determines the rapidity of intersection.  For instance, two narrow parabolas
spread apart will have a slower convergence to intersection, relative to parabolas whose site nodes
are spread further apart on the y axis.  A decision criteria to choose a minimum or maximum value 
of a two point intersection set is given for the intersection equations (since this is a quadratic
solution).  Further where the intersection point lay in relation to the new node determines 
whether a left or right parabola is chosen... a point intersection to the right of the new site 
node means a left parabola is chosen (from the left right child tree) and conversely to the right 
in the opposite case.  

Where I have appended data for added organizational purposes for tracking:

-parabola deletion events (or otherwise known as circle events).  A circle event in the fortune
model occurs where no new site is found inside the circle for three sites forming the circle event.
If the case where a site node is found occupying the circle of the three nodes, 
a written caveat to this is found logically, for instance, inside the parabola 
insertion event where a cross check is made to ensure that a found parabola that is marked as 
circle event will automatically delete such circle event.  Otherwise, when the parabolic deletion 
(circle event) is good, then I append vertex data to all relevant voronoi cells.  This should be represented
in three sites (or cells as I have worded it).  Adding to this that at such deletion event, there is 
a new edge formed by a grandparent and child left node remaining (where the old parent parabola is 
squeezed out), this data is also appended, or at least should be appended.  

Problems:  Somehow data is appearing not right in some instances.  For instances, seeing more than 
two edges radiating internally throughout the cell which appear to be in violation to the voronoi 
diagrams typified structure...that is where such edge technically crosses the interior of such cell
as opposed to forming a boundary of the cell.  Thus examining where appending data is going awry here.
Or if there is potentially an error with the algorithm itself.  I do have some questions about the circle
test for instance, it may be, for instance, better to retain the radius of the circle and then compare the 
a given site node (prior to a circle event) for parabola insertion whether or not the point is at a distance
less than the radius of such circle when comparing the distance between the new site and the given 
intersection point (as alternate means to deleting a circle event).
*/

Voronoi::Voronoi()
{
	edges = 0;
}

Edges * Voronoi::GetEdges(Vertices * v, int w, int h)
{
	//Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("Voronoi.log");
  	std::ofstream myfile;
  	myfile.open ("Voronoi2.txt");
	std::ostringstream ss5;

	places = v;
	width = w;
	height = h;
	root = 0;
	ss5 << "Hitting Voronoi test2 !" << "\n";
//    	tlog->logMessage(ss5.str());
	myfile << ss5.str();
	
//	if(!edges) edges = new Edges();
//	if(!cells) cells = new Cells();
	edges = new Edges();
	cells = new Cells();
	cellsdat = new Cellsdat();
/*
	else 
	{
		for(Vertices::iterator	i = points.begin(); i != points.end(); ++i) delete (*i);
		for(Edges::iterator		i = edges->begin(); i != edges->end(); ++i) delete (*i);
		//for(Cells::iterator		i = cells->begin(); i != cells->end(); ++i) (*cells).erase( (*i).first);
		//cells->RemoveAll();
		points.clear();
		edges->clear();
		//(*cells).erase((*cells).begin(),(*cells).end());
		//cells->clear();
	
	}
*/
	ss5 << "Hitting Voronoi test3 !" << "\n";
//    	tlog->logMessage(ss5.str());
	myfile << ss5.str();
	
	for(Vertices::iterator i = places->begin(); i!=places->end(); ++i)
	{
		queue.push(new VEvent( *i, true));
		(*cells)[*i] = new VoronoiCell(*i);
	}
	ss5 << "Hitting Voronoi test2 !" << "\n";
//    	tlog->logMessage(ss5.str());
	myfile << ss5.str();
	myfile.close();
    	ss5.str(std::string());
	VEvent * e;
	while(!queue.empty())
	{
		e = queue.top();
		queue.pop();
		ly = e->point->y;
		if(deleted.find(e) != deleted.end()) { delete(e); deleted.erase(e); continue;}
		if((*cells).find((e->point)) != (*cells).end()){
			cell = (*cells)[(e->point)];  //should be assignment to a pointer so may need & on the right
		} 
		if(e->pe) InsertParabola(e->point);

		else RemoveParabola(e);
		delete(e);
	}
	ss5 << "Hitting Voronoi test3 !" << "\n";
//    	tlog->logMessage(ss5.str());
//	myfile << ss5.str();
    	ss5.str(std::string());	
	FinishEdge(root);

	for(Edges::iterator i = edges->begin(); i != edges->end(); ++i)
	{
		if( (*i)->neighbour) 
		{
			(*i)->start = (*i)->neighbour->end;
			delete (*i)->neighbour;
		}
	}
	
	return edges;
}

void	Voronoi::InsertParabola(VPoint * p)
{
	if(!root){root = new VParabola(p); return;}

	if(root->isLeaf && root->site->y - p->y < 1) // degenerovaný pøípad - obì spodní místa ve stejné výšce
	{
		VPoint * fp = root->site;
		root->isLeaf = false;
		root->SetLeft( new VParabola(fp) );
		root->SetRight(new VParabola(p)  );
		VPoint * s = new VPoint((p->x + fp->x)/2, height); // zaèátek hrany uprostøed míst
		points.push_back(s);
		if(p->x > fp->x) {
			root->edge = new VEdge(s, fp, p);
			cell -> edges->push_back(root->edge);
		} // rozhodnu, který vlevo, který vpravo
		else {
			root->edge = new VEdge(s, p, fp);
			cell -> edges->push_back(root->edge);
		}
		edges->push_back(root->edge);
		return;
	}

	VParabola * par = GetParabolaByX(p->x);
	
	if(par->cEvent)
	{
		deleted.insert(par->cEvent);
		par->cEvent = 0;
	}
	// start is determined using GetY which has an intercept x position at the 
	// position of GetParabolaByX function..this means the 
	// x intersection position on the returned child node that our inserted
	//parabola will also coincide.  par-> site is used in generating 
	// the parabolic equation that a new node site position will coincide on
	// as given to sufficiency by previous tests.  Hmmm, sort of confused on this
	//however as the GetY uses another child node in generating 
	//a parabola.  Then in turn inputs p-> x into such parabolic equation
	//which determines a position on the other child node parabolic curve
	// that is matched to our present node's x position. This is part of the 
	//algorithm it appears.

	VPoint * start = new VPoint(p->x, GetY(par->site, p->x));
	points.push_back(start);

	VEdge * el = new VEdge(start, par->site, p);
	VEdge * er = new VEdge(start, p, par->site);

	el->neighbour = er;
	cell -> edges->push_back(el);

	if((*cells).find((par->site)) != (*cells).end()){
		VoronoiCell * lpcell = (*cells)[(par->site)];  //should be assignment to a pointer so may need & on the right
		lpcell->edges->push_back(er);
	} 
	edges->push_back(el);

	// pøestavuju strom .. vkládám novou parabolu
	par->edge = er;
	par->isLeaf = false;

	VParabola * p0 = new VParabola(par->site);
	VParabola * p1 = new VParabola(p);
	VParabola * p2 = new VParabola(par->site);

	par->SetRight(p2);
	par->SetLeft(new VParabola());
	par->Left()->edge = el;

	par->Left()->SetLeft(p0);
	par->Left()->SetRight(p1);
	
	// It appears in the parabola header...this comes from 
	//setLeft() or setRight() parabola functions.  It seems for clarity this might be better
	//expressed with separated set function calls as opposed to making both implicit in a call
	//that seems to appears for the parent (by convention) only.   

	CheckCircle(p0);
	CheckCircle(p2);
}

void	Voronoi::RemoveParabola(VEvent * e)
{
	VParabola * p1 = e->arch;

	VParabola * xl = VParabola::GetLeftParent(p1);
	VParabola * xr = VParabola::GetRightParent(p1);

	VParabola * p0 = VParabola::GetLeftChild(xl);
	VParabola * p2 = VParabola::GetRightChild(xr);

	if(p0 == p2) std::cout << "chyba - pravá a levá parabola má stejné ohnisko!\n";

	if(p0->cEvent){ deleted.insert(p0->cEvent); p0->cEvent = 0; }
	if(p2->cEvent){ deleted.insert(p2->cEvent); p2->cEvent = 0; }

	VPoint * p = new VPoint(e->point->x, GetY(p1->site, e->point->x));
	points.push_back(p);

	/*  call to instance circle event data and append vertex (intersection) data
	    for a remove parabola method call.  Ensuring that the three originated 
	    site nodes are referenced in appending such vertex data. Given below.
	*/
	VoronoiCellDat * celldat = (*cellsdat)[e];
	
	//cell->places->push_back(s);
	if((*cells).find((celldat->sitePos)) != (*cells).end()){
		VoronoiCell * siteposcell = (*cells)[celldat->sitePos];  //should be assignment to a pointer so may need & on the right
		siteposcell->places->push_back(celldat->intersection);
		VertEdges * sitevertedges = siteposcell->vertedges;
		EdgeVerts * siteedgeverts = siteposcell->edgeverts;
		if ((*sitevertedges).find(celldat->intersection)!= (*sitevertedges).end()) {
			(*sitevertedges)[celldat->intersection]->push_back(celldat->siteEdge);
			
		}
		else {
			vor::Edges * nedges = new Edges();
			nedges->push_back(celldat->siteEdge);
			(*sitevertedges)[celldat->intersection] = nedges;
			
		}
		
		//(*sitevertedges)[celldat->intersection]->push_back(celldat->siteEdge);
		if ((*siteedgeverts).find(celldat->siteEdge) != (*siteedgeverts).end()){
			(*siteedgeverts)[celldat->siteEdge]->push_back(celldat->intersection);
		}
		else {
			vor::Vertices * vertices = new Vertices();
			vertices->push_back(celldat->intersection);
			(*siteedgeverts)[celldat->siteEdge] = vertices; 
		}
		//(*siteedgeverts)[celldat->siteEdge]->push_back(celldat->intersection);
	} 
	if((*cells).find((celldat->LeftPSite)) != (*cells).end()){
		VoronoiCell * lpcell = (*cells)[celldat->LeftPSite];  //should be assignment to a pointer so may need & on the right
		lpcell->places->push_back(celldat->intersection);
		VertEdges * leftvertedges = lpcell->vertedges;
		EdgeVerts * leftedgeverts = lpcell->edgeverts;

		if ((*leftvertedges).find(celldat->intersection)!= (*leftvertedges).end()) {
			(*leftvertedges)[celldat->intersection]->push_back(celldat->LeftEdge);
			
		}
		else {
			vor::Edges * nedges = new Edges();
			nedges->push_back(celldat->LeftEdge);
			(*leftvertedges)[celldat->intersection] = nedges;
			
		}
		
		//(*sitevertedges)[celldat->intersection]->push_back(celldat->siteEdge);
		if ((*leftedgeverts).find(celldat->LeftEdge) != (*leftedgeverts).end()){
			(*leftedgeverts)[celldat->LeftEdge]->push_back(celldat->intersection);
		}
		else {
			vor::Vertices * vertices = new Vertices();
			vertices->push_back(celldat->intersection);
			(*leftedgeverts)[celldat->LeftEdge] = vertices; 
		}
		//(*leftvertedges)[celldat->intersection]->push_back(celldat->LeftEdge);
		//(*leftedgeverts)[celldat->LeftEdge]->push_back(celldat->intersection);
	} 

	if((*cells).find((celldat->RightPSite)) != (*cells).end()){
		VoronoiCell * rpcell = (*cells)[celldat->RightPSite];  //should be assignment to a pointer so may need & on the right
		rpcell->places->push_back(celldat->intersection);
		VertEdges * rightvertedges = rpcell->vertedges;
		EdgeVerts * rightedgeverts = rpcell->edgeverts;

		if ((*rightvertedges).find(celldat->intersection)!= (*rightvertedges).end()) {
			(*rightvertedges)[celldat->intersection]->push_back(celldat->RightEdge);
			
		}
		else {
			vor::Edges * nedges = new Edges();
			nedges->push_back(celldat->RightEdge);
			(*rightvertedges)[celldat->intersection] = nedges;
			
		}
		
		//(*sitevertedges)[celldat->intersection]->push_back(celldat->siteEdge);
		if ((*rightedgeverts).find(celldat->RightEdge) != (*rightedgeverts).end()){
			(*rightedgeverts)[celldat->RightEdge]->push_back(celldat->intersection);
		}
		else {
			vor::Vertices * vertices = new Vertices();
			vertices->push_back(celldat->intersection);
			(*rightedgeverts)[celldat->RightEdge] = vertices; 
		}
		//(*rightvertedges)[celldat->intersection]->push_back(celldat->RightEdge);
		//(*rightedgeverts)[celldat->RightEdge]->push_back(celldat->intersection);
	} 

	/*  So with appending the data, it occurred to me that the algorithm isn't 
	    much concerned with where the voronoi cells are in relation to the a given 
	    boundary for the entire map.
	*/
	xl->edge->end = p;
	xr->edge->end = p;
	
	VParabola * higher;
	VParabola * par = p1;
	while(par != root)
	{
		par = par->parent;
		if(par == xl) higher = xl;
		if(par == xr) higher = xr;
	}
	higher->edge = new VEdge(p, p0->site, p2->site);
	edges->push_back(higher->edge);

	/* 
		In deleting a given parabola a new edge is created in the process.  Appending this
		edge data to the Voronoi cell.  Presuming that vertex intersection data is already
		appended up above to the voronoi cell class, we just need to ensure that the new
		edge to vertex and vertex to edge relation is provisioned here.
	*/
	

	if((*cells).find((p0->site)) != (*cells).end()){
		VoronoiCell * p0cell = (*cells)[p0->site]; 
		VertEdges * p0vertedges = p0cell->vertedges;
		EdgeVerts * p0edgeverts = p0cell->edgeverts;
		if ((*p0vertedges).find(celldat->intersection)!= (*p0vertedges).end()) {
			(*p0vertedges)[celldat->intersection]->push_back(higher->edge);
			
		}
		else {
			vor::Edges * nedges = new Edges();
			nedges->push_back(higher->edge);
			(*p0vertedges)[celldat->intersection] = nedges;
			
		}
		if ((*p0edgeverts).find(higher->edge) != (*p0edgeverts).end()){
			(*p0edgeverts)[higher->edge]->push_back(celldat->intersection);
		}
		else {
			vor::Vertices * vertices = new Vertices();
			vertices->push_back(celldat->intersection);
			(*p0edgeverts)[higher->edge] = vertices; 
		}
	}

	if((*cells).find((p2->site)) != (*cells).end()){
		VoronoiCell * p2cell = (*cells)[p2->site]; 
		VertEdges * p2vertedges = p2cell->vertedges;
		EdgeVerts * p2edgeverts = p2cell->edgeverts;
		if ((*p2vertedges).find(celldat->intersection)!= (*p2vertedges).end()) {
			(*p2vertedges)[celldat->intersection]->push_back(higher->edge);
			
		}
		else {
			vor::Edges * nedges = new Edges();
			nedges->push_back(higher->edge);
			(*p2vertedges)[celldat->intersection] = nedges;
			
		}
		if ((*p2edgeverts).find(higher->edge) != (*p2edgeverts).end()){
			(*p2edgeverts)[higher->edge]->push_back(celldat->intersection);
		}
		else {
			vor::Vertices * vertices = new Vertices();
			vertices->push_back(celldat->intersection);
			(*p2edgeverts)[higher->edge] = vertices; 
		}
	}
	
	VParabola * gparent = p1->parent->parent;
	if(p1->parent->Left() == p1)
	{
		if(gparent->Left()  == p1->parent) gparent->SetLeft ( p1->parent->Right() );
		if(gparent->Right() == p1->parent) gparent->SetRight( p1->parent->Right() );
	}
	else
	{
		if(gparent->Left()  == p1->parent) gparent->SetLeft ( p1->parent->Left()  );
		if(gparent->Right() == p1->parent) gparent->SetRight( p1->parent->Left()  );
	}

	delete p1->parent;
	delete p1;

	CheckCircle(p0);
	CheckCircle(p2);
}

void	Voronoi::FinishEdge(VParabola * n)
{
	//This finishes all end point vertices for all parabolas recursively
	if(n->isLeaf) {delete n; return;}
	double mx;
	//I am seeing potentially a number of issues for vertices returned on a given voronoi edge using
	//this algorithm control point below.  A parabola that need terminate at the given voronoi map 
	//boundary is such to have a point residing as either a max width, or max height position,
	//but not solely max width.  The maximum boundary position then could be a problem where the 
	//maximum y is given and we solve the x and not solely solving for y every time.  
	//when I have browsed the set of finished vertices for a given voronoi node, these are not 
	//inside the defined boundaries of the voronoi map for finishing such edge...I am literally seeing
	//values that are well outside these (e.g., with a 10,000 x 10,000 map nodes that are at 120,000+ 
	//for a given y value or being negative ) which is not what we want!  

	//we can use an edge intersection on four different possible voronoi map edge boundaries,
	//toss the intersections which are clearly outside our defined map range, and chose 
	//the edge with minimum distance relative to the cell node.
	if(n->edge->direction->x > 0.0)	mx = std::max(width, n->edge->start->x + 10 );
	else mx = std::min(0.0, n->edge->start->x - 10);
	
	VPoint * end = new VPoint(mx, mx * n->edge->f + n->edge->g); 
	n->edge->end = end;
	points.push_back(end);
	if((*cells).find((n->site)) != (*cells).end()){
		VoronoiCell * lpcell = (*cells)[(n->site)];  //should be assignment to a pointer so may need & on the right
		//lpcell->places->push_back(end); //****************
	} 			
	FinishEdge(n->Left() );
	FinishEdge(n->Right());
	delete n;
}

double	Voronoi::GetXOfEdge(VParabola * par, double y)
{
	VParabola * left = VParabola::GetLeftChild(par);
	VParabola * right= VParabola::GetRightChild(par);

	VPoint * p = left->site;
	VPoint * r = right->site;
	//this formula makes use of two point intersection between 
	//two parabolas.  Or writing out both parabolic equations
	// and solving these simultaneously, then having solved the 
	//quadratic expression.  

	//solutions below need division by zero resolution.
	double dp = 2.0 * (p->y - y);
	//double a1,b1,c1,a2,b2,c2;
	//if (dp != 0.0){
		double a1 = 1.0 / dp;
		double b1 = -2.0 * p->x / dp;
		double c1 = y + dp / 4 + p->x * p->x / dp;
	/*
	}
	else{
		double a1 = 9999999999999;
		double b1 = -9999999999999*p->x;
		double c1 = 9999999999999;
	}
	*/
	//double b1 = -2.0 * p->x / dp;
	//double c1 = y + dp / 4 + p->x * p->x / dp;
			
	dp = 2.0 * (r->y - y);
	
	//if (dp != 0.0){
		double a2 = 1.0 / dp;
		double b2 = -2.0 * r->x / dp;
		double c2 = ly + dp / 4 + r->x * r->x / dp;
	/*
	}
	else{
	
		double a2 = 9999999999999;
		double b2 = -9999999999999*r->x;
		double c2 = 9999999999999;
	}
	*/
	//double a2 = 1.0 / dp;
	//double b2 = -2.0 * r->x/dp;
	//double c2 = ly + dp / 4 + r->x * r->x / dp;
			
	double a = a1 - a2;

	double b = b1 - b2;
	double c = c1 - c2;
			
	double disc = b*b - 4 * a * c;
	//double x1,x2;
	/*
	if (a == 0.0) {
		double x1 = (-b + std::sqrt(disc)) * 9999999999999;
		double x2 = (-b - std::sqrt(disc)) * 9999999999999;		
	}
	else{
	*/
		double x1 = (-b + std::sqrt(disc)) / (2*a);
		double x2 = (-b - std::sqrt(disc)) / (2*a);
	//}
	//double x1 = (-b + std::sqrt(disc)) / (2*a);
	//double x2 = (-b - std::sqrt(disc)) / (2*a);

	double ry;
	if(p->y < r->y ) ry =  std::max(x1, x2);
	else ry = std::min(x1, x2);

	return ry;
}

VParabola * Voronoi::GetParabolaByX(double xx)
{
	VParabola * par = root;
	double x = 0.0;

	while(!par->isLeaf) // projdu stromem dokud nenarazím na vhodný list
	{
		x = GetXOfEdge(par, ly);
		if(x>xx) par = par->Left();
		else par = par->Right();				
	}
	return par;
}

double	Voronoi::GetY(VPoint * p, double x) // ohnisko, x-souøadnice
{
	double dp = 2 * (p->y - ly);
	//double a1,b1,c1;
	/*
	if (dp == 0.0){
		double a1 = 9999999999999;
		double b1 = -2 * p->x *9999999999999;
		double c1 = 9999999999999;
	}
	else{
	*/
		double a1 = 1 / dp;
		double b1 = -2 * p->x / dp;
		double c1 = ly + dp / 4 + p->x * p->x / dp;
	//}
	
	return(a1*x*x + b1*x + c1);
}

void	Voronoi::CheckCircle(VParabola * b)
{
	VParabola * lp = VParabola::GetLeftParent (b);
	VParabola * rp = VParabola::GetRightParent(b);

	VParabola * a  = VParabola::GetLeftChild (lp);
	VParabola * c  = VParabola::GetRightChild(rp);

	if(!a || !c || a->site == c->site) return;

	VPoint * s = 0;
	s = GetEdgeIntersection(lp->edge, rp->edge);
	if(s == 0) return;

	double dx = a->site->x - s->x;
	double dy = a->site->y - s->y;

	double d = std::sqrt( (dx * dx) + (dy * dy) );

	if(s->y - d >= ly) { return;}
//at this point intersection is true can set edges and vertices to all necessary cells
//an edge will be set on one parent site from a parent parabola, while 
//all three sites from parabolas will have a vertex added.



//Technically circle test is not completed, or at least when appending the vertices this should
//be done at the circle event node (which confirms appending the vertices), or at least, if 
//a node is found inside the delaunay triangle, removes the added vertices to the given cells.
// This can be fixed by recording the sites and the intersection point to a given map,
//and then appending the vertices instead as given below on the delete parabola method call.

//I've created a preliminary circle event map data structure appending VPoint * data from 
//into a class object called VoronoiCellDat.  The VoronoiCellDat is the value of the Event *
// key of the map data structure called cellsdat.  This key value pairing makes data lookup retrieval
//easier.

  	VoronoiCellDat * celldat = new VoronoiCellDat(cell->sitePos);
	celldat->siteEdge = b->edge;
	celldat->LeftPSite = lp->site;
	celldat->LeftEdge = lp->edge;
	celldat->RightPSite = rp->site;
	celldat->RightEdge = rp->edge;
	celldat->intersection = s;
	/*
	cell->places->push_back(s);
	if((*cells).find((lp->site)) != (*cells).end()){
		VoronoiCell * lpcell = (*cells)[(lp->site)];  //should be assignment to a pointer so may need & on the right
		lpcell->places->push_back(s);
	} 

	if((*cells).find((rp->site)) != (*cells).end()){
		VoronoiCell * rpcell = (*cells)[(rp->site)];  //should be assignment to a pointer so may need & on the right
		rpcell->places->push_back(s);
	} 
	*/
	VEvent * e = new VEvent(new VPoint(s->x, s->y - d), false);
	(*cellsdat)[e] = celldat;
	points.push_back(e->point);
	b->cEvent = e;
	e->arch = b;
	queue.push(e);
}

VPoint * Voronoi::GetEdgeIntersection(VEdge * a, VEdge * b)
{
	//intersection of two lines
	//double x;
	/*
	if ((a->f - b->f) == 0.0){
		double x = (b->g-a->g)*9999999999999;
	}
	else{
	*/
		double x = (b->g-a->g) / (a->f - b->f);	
	//}
	//double x = (b->g-a->g) / (a->f - b->f);
	double y = a->f * x + a->g;

	if((x - a->start->x)/a->direction->x < 0) return 0;
	if((y - a->start->y)/a->direction->y < 0) return 0;
		
	if((x - b->start->x)/b->direction->x < 0) return 0;
	if((y - b->start->y)/b->direction->y < 0) return 0;	

	VPoint * p = new VPoint(x, y);		
	points.push_back(p);
	return p;
}

Cells * Voronoi::GetVoronoiCells(){
	return cells;
}


	
