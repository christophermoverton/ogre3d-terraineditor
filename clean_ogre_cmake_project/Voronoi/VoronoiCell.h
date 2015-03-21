#ifndef __VoronoiCell_h
#define __VoronoiCell_h
#include "Voronoi.h"
#include <list>
#include <queue>
#include <set>

#include "VPoint.h"
#include "VEdge.h"
#include "VParabola.h"
#include "VEvent.h"
#include <functional>
#include <vector>
#include <map>
#include <utility>

//using namespace std;
namespace vor{
	/*
		Useful data containers for Vertices (places) and Edges of Voronoi diagram
	*/
 	typedef std::pair<VPoint *, VPoint *>			     Vertpair      ;
	typedef std::pair<int, int>                            	     Coordpair     ;
	typedef std::pair<double, double>                            dCoordpair    ;
	typedef std::list<VPoint *>		                     Vertices	   ;
	typedef std::list<VEdge *>				     Edges	   ;
	typedef std::map<Vertpair, VEdge *>                          VEdgemap      ;
	typedef std::map<VPoint *, Edges *>                           VertEdges     ;
	typedef std::map<VPoint *, VEdge * >                         VertEdge      ;
	typedef std::map<VEdge *, Vertices *>                         EdgeVerts     ;
	typedef std::map<VPoint *, double>                            PointsMap    ;
	typedef std::vector<double>                                     Dvalues      ;
	typedef std::map<Coordpair, double>                           CPointsMap   ;
	typedef std::map<Coordpair, VPoint *>                         CoordtoCellMap ;  
	typedef std::map<dCoordpair, Vertices*>                         CelltoCells   ;
	typedef std::pair<double, VPoint*>                              dPoint      ;

	/*
		Class for generating the Voronoi diagram
	*/
}

//using namespace vor;
	class VoronoiCell
	{
	public:
		
		/*
			Constructor - without any parameters
		*/

		

		/*
			The only public function for generating a diagram
			
			input:
				v		: Vertices - places for drawing a diagram
				w		: width  of the result (top left corner is (0, 0))
				h		: height of the result
			output:
				pointer to list of edges
			
			All the data structures are managed by this class
		*/

		vor::Edges *			GetEdges(void);
		vor::Vertices *              GetVertices(void);
		vor::Vertices *				places;
		vor::Edges *				 edges;
		VPoint *         		       sitePos;
		vor::VEdgemap *                        edgemap;
		vor::VertEdges *                       vertedges;
		vor::EdgeVerts *                       edgeverts;
		vor::Vertices *                   cellpolypoints;  //polygon vertices (ordered by neighboring relations).
		vor::Vertices *                       duplicates; //vertices may have 2 or 4 neighboring vertices.  Tracking 4.
		VPoint *                              centroid;
		vor::Vertices *                          ncells;   //neighborcells site pos
		VoronoiCell(VPoint * point){
			sitePos = point;
			places = new vor::Vertices();
			edges = new vor::Edges();
			edgemap = new vor::VEdgemap();
			vertedges = new vor::VertEdges();
			edgeverts = new vor::EdgeVerts();
			cellpolypoints = new vor::Vertices();
			duplicates = new vor::Vertices();
			ncells = new vor::Vertices();
			//centroid = new VPoint();
		}
	//private:

		/*
						places		: container of places with which we work
						edges		: container of edges which will be the result
						
						sitePos		: the position of the veronoi site.
						ly			: current "y" position of the line (see Fortune's algorithm)
		*/

	
	};
	
	class VoronoiCellDat
	{
		public:
			VPoint * 			intersection;
			VPoint *         		LeftPSite;
			VEdge  *                        LeftEdge;
			VPoint *         		RightPSite;
			VEdge  *                        RightEdge;
			VPoint *         		sitePos;
			VEdge  *                        siteEdge;
			VEdge  *                        siteEdge2;
			VoronoiCellDat(VPoint * spos){
				sitePos = spos;
				
			}
			
	};


#endif
