#ifndef __TerrainStruct_h
#define __TerrainStruct_h

#include <list>
#include <queue>
#include <set>
#include <tuple>

#include <functional>
#include <vector>
#include <map>
#include <utility>

namespace terr{
	typedef std::pair<int, int>                            	     Coordpair     ;
	typedef std::tuple< int, int, int>                            3dCoord      ;
	typedef std::map<Coordpair, double>                           CPointsMap   ;
	typedef std::map<3dCoord, double>                             3dCPointsMap ;
	typedef std::map<int, CPointsMap>                             CPointsMaps  ;
	typedef std::map<int, 3dCPointsMap>                           3dCPointsMaps;
}
#endif
