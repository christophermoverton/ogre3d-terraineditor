
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

	if(root->isLeaf && root->site->y - p->y < 1) // degenerovan� p��pad - ob� spodn� m�sta ve stejn� v��ce
	{
		VPoint * fp = root->site;
		root->isLeaf = false;
		root->SetLeft( new VParabola(fp) );
		root->SetRight(new VParabola(p)  );
		VPoint * s = new VPoint((p->x + fp->x)/2, height); // za��tek hrany uprost�ed m�st
		points.push_back(s);
		if(p->x > fp->x) {
			root->edge = new VEdge(s, fp, p);
			cell -> edges->push_back(root->edge);
		} // rozhodnu, kter� vlevo, kter� vpravo
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

	// p�estavuju strom .. vkl�d�m novou parabolu
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

	if(p0 == p2) std::cout << "chyba - prav� a lev� parabola m� stejn� ohnisko!\n";

	if(p0->cEvent){ deleted.insert(p0->cEvent); p0->cEvent = 0; }
	if(p2->cEvent){ deleted.insert(p2->cEvent); p2->cEvent = 0; }

	VPoint * p = new VPoint(e->point->x, GetY(p1->site, e->point->x));
	points.push_back(p);

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

	while(!par->isLeaf) // projdu stromem dokud nenaraz�m na vhodn� list
	{
		x = GetXOfEdge(par, ly);
		if(x>xx) par = par->Left();
		else par = par->Right();				
	}
	return par;
}

double	Voronoi::GetY(VPoint * p, double x) // ohnisko, x-sou�adnice
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

//I am again seeing absurd results being pushed back for cell vertices on intersection.
//Again not certain where this is exactly coming.  Technically if this is arising from 
//false positives on (push back of vertices from intersection testing)?  Or if there 
// is an issue say occurring because of the edge chosen in terms of coefficients stored.
// I am not certain if circle test failure is occurring, for instance, because a site is within
//the delaunay triangle's circle for three sites?!  Hence why an intersection is producing, 
//potentially a not valid vertex?! 

//Technically circle test is not completed, or at least when appending the vertices this should
//be done at the circle event node (which confirms appending the vertices), or at least, if 
//a node is found inside the delaunay triangle, removes the added vertices to the given cells.
// This can be fixed by recording the sites and the intersection point to a given map,
//and then appending the vertices instead as given below on the delete parabola method call.
  
	cell->places->push_back(s);
	if((*cells).find((lp->site)) != (*cells).end()){
		VoronoiCell * lpcell = (*cells)[(lp->site)];  //should be assignment to a pointer so may need & on the right
		lpcell->places->push_back(s);
	} 

	if((*cells).find((rp->site)) != (*cells).end()){
		VoronoiCell * rpcell = (*cells)[(rp->site)];  //should be assignment to a pointer so may need & on the right
		rpcell->places->push_back(s);
	} 
	VEvent * e = new VEvent(new VPoint(s->x, s->y - d), false);
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


	