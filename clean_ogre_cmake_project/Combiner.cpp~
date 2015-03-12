#ifndef __Combiner_cpp
#define __Combiner_cpp
#include <vector>
#include <algorithm>
#include "TPoint3.h"
#include "TerrainStruct.h"

using namespace std;
enum COMBINER_TYPE {ADD, MULTIPLY, DIVIDE, DIFFERENCE
};

class Combiner{
	public:

		Combiner(COMBINER_TYPE type);
		Combiner(int type);
		//COMBINER_TYPE ctype;
		vector<vector<vector<double> > > Combine(vector<vector<vector<double> > > val1,
							float weight1,
							vector<vector<vector<double> > > val2,
							float weight2);
		vector<vector<vector<double> > > Combine(vector<vector<vector<double> > > val1,
							float weight1,
							vector<vector<vector<double> > > val2,
							float weight2,
							vector<vector<vector<double> > > val3,
							float weight3);
		vector<vector<vector<double> > > Combine(vector<vector<vector<vector<double> > > > vals,
							vector<float> weights);
		terr::T3dCPointsMap * Combine(terr::T3dCPointsMaps * vals, vector<float> weights);
		terr::CPointsMap * Combine(terr::CPointsMaps * vals, vector<float> weights, bool repeat);
	private:
		COMBINER_TYPE ctype;
};

Combiner::Combiner(COMBINER_TYPE type){
	ctype = type;
}

Combiner::Combiner(int type){
	if (type == 1){
		ctype = ADD;
	}
	else if (type == 2) {
		ctype = MULTIPLY;
	}
	else if (type == 3) {
		ctype = DIFFERENCE;
	}
	else if (type == 4) {
		ctype = DIVIDE;
	}
}

vector<vector<vector<double> > > Combiner::Combine(vector<vector<vector<double> > > vals1,
						   float weight1,
						   vector<vector<vector<double> > > vals2,
						   float weight2){
	vector<vector<vector<double> > > combinevals;
	double maxval = (double) 0.0f;
	double minval = (double) 0.0f;
	for (int i = 0; i < vals1.size(); i++){
		combinevals.resize(vals1.size());
		for(int j = 0; j < vals1[i].size(); j++){
			combinevals[i].resize(vals1[i].size());
			for(int k = 0; k < vals1[i][j].size()-1; k++){
				combinevals[i][j].resize(vals1[i][j].size());
				double val1 = vals1[i][j][k];
				double val2 = vals2[i][j][k];
				double tval;
				if (ctype == ADD) {
					//combinevals[i][j][k] = weight1*val1 + weight2*val2;
					tval = weight1*val1 + weight2*val2;
				}
				else if (ctype == MULTIPLY){
					//no weighting since order of weights is commutative 
					tval = val1*val2;
				}
				else if (ctype == DIFFERENCE){
					tval = weight1*val1-weight2*val2;
				}
				else if (ctype == DIVIDE){
					tval = val1/val2;
				}
				if (tval < minval){
					minval = tval;
				}
				else if (tval > maxval){
					maxval = tval;
				}
				combinevals[i][j][k] = tval;
			} //for k
		} //for j
	} //for i
	double scalediv;
	if (abs(minval) < abs(maxval)){
		scalediv = abs(maxval);
	}
	else{
		scalediv = abs(minval);
	}
	for (int i = 0; i<combinevals.size(); i++){
		for(int j = 0; j < combinevals[i].size(); j++){
			for(int k = 0; k < combinevals[i][j].size()-1; k++){
				combinevals[i][j][k] = combinevals[i][j][k]/scalediv;
			}
		}
	}
	return combinevals;
}

vector<vector<vector<double> > > Combiner::Combine(vector<vector<vector<vector<double> > > > vals,
							vector<float> weights){

	vector<vector<vector<double> > > combinevals;
	double maxval = (double) 0.0f;
	double minval = (double) 0.0f;
	
	for (int i = 0; i < vals[0].size(); i++){
		combinevals.resize(vals[0].size());
		for(int j = 0; j < vals[0][i].size(); j++){
			combinevals[i].resize(vals[0][i].size());
			for(int k = 0; k < vals[0][i][j].size()-1; k++){
				combinevals[i][j].resize(vals[0][i][j].size());
				double val1 = vals[0][i][j][k];
				float weight1 = weights[0];
				for(int l = 1; l < vals.size(); l++){
				if (l>1){
					double val1 = combinevals[i][j][k];
					float weight1 = 1.0f;
					}
				double val2 = vals[l][i][j][k];
				float weight2 = weights[l];
				double tval;
				if (ctype == ADD) {
					//combinevals[i][j][k] = weight1*val1 + weight2*val2;
					tval = weight1*val1 + weight2*val2;
				}
				else if (ctype == MULTIPLY){
					//no weighting since order of weights is commutative 
					tval = val1*val2;
				}
				else if (ctype == DIFFERENCE){
					tval = weight1*val1-weight2*val2;
				}
				else if (ctype == DIVIDE){
					tval = val1/val2;
				}
				if (tval < minval){
					minval = tval;
				}
				else if (tval > maxval){
					maxval = tval;
				}
				combinevals[i][j][k] = tval;
				}//for l
			} //for k
		} //for j
	} //for i
	double scalediv; bool negvals = false;
	if (abs(minval) < abs(maxval)){
		scalediv = abs(maxval);
	}
	else{
		scalediv = abs(minval);
	}
        if (minval < 0){
		scalediv *= 2.0f;  negvals = true;
        }
	for (int i = 0; i<combinevals.size(); i++){
		for(int j = 0; j < combinevals[i].size(); j++){
			for(int k = 0; k < combinevals[i][j].size()-1; k++){
				combinevals[i][j][k] = combinevals[i][j][k]/scalediv;
			}
		}
	}
	return combinevals;
}

terr::T3dCPointsMap * Combiner::Combine(terr::T3dCPointsMaps * vals, vector<float> weights){

	terr::T3dCPointsMap * combinevals = new terr::T3dCPointsMap();
	double maxval = (double) 0.0f;
	double minval = (double) 1000000.0f;
	for (terr::T3dCPointsMaps::iterator i = vals->begin(); i != vals->end(); i++) {
		terr::T3dCPointsMap * pointmap = (*i).second;
		int weightparam = (*i).first; 
		for (terr::T3dCPointsMap::iterator j = pointmap->begin(); j != pointmap->end(); j++){
			TPoint3 * coord = (*j).first;
			double val = (*j).second;
			if ((*combinevals).find(coord) != (*combinevals).end()){
				if (ctype == ADD){
					(*combinevals)[coord] += val*weights[weightparam];
				}
				else if (ctype == MULTIPLY){
					(*combinevals)[coord] *= val;
				}
				else if (ctype == DIFFERENCE){
					(*combinevals)[coord] -= val*weights[weightparam];
				}
				else if (ctype == DIVIDE){
					(*combinevals)[coord] /= val;
				}
				double tval = (*combinevals)[coord]; 
				if (tval < minval){
					minval = tval;
				}
				else if (tval > maxval){
					maxval = tval;
				}
			}
			else {
				if (ctype == ADD){
					(*combinevals)[coord] = val*weights[weightparam];
				}
				else if (ctype == MULTIPLY){
					(*combinevals)[coord] = val;
				}
				else if (ctype == DIFFERENCE){
					(*combinevals)[coord] = val*weights[weightparam];
				}
				else if (ctype == DIVIDE){
					(*combinevals)[coord] = val;
				}
				double tval = (*combinevals)[coord]; 
				if (tval < minval){
					minval = tval;
				}
				else if (tval > maxval){
					maxval = tval;
				}
			}
		}
	}
	double scalediv;
	if (abs(minval) < abs(maxval)){
		scalediv = abs(maxval);
	}
	else{
		scalediv = abs(minval);
	}

	for (terr::T3dCPointsMap::iterator i = combinevals->begin(); i != combinevals->end(); i++){
		(*i).second /= scalediv;
	}

	return combinevals;
}

terr::CPointsMap * Combiner::Combine(terr::CPointsMaps * vals, vector<float> weights, bool repeat){

	terr::CPointsMap * combinevals = new terr::CPointsMap();
	double maxval = (double) 0.0f;
	double minval = (double) 1000000.0f;
	for (terr::CPointsMaps::iterator i = vals->begin(); i != vals->end(); i++) {
		terr::CPointsMap  pointmap = (*i).second;
		int weightparam = (*i).first; 
		for (terr::CPointsMap::iterator j = pointmap.begin(); j != pointmap.end(); j++){
			terr::Coordpair coord = (*j).first;
			double val = (*j).second;
			if ((*combinevals).find(coord) != (*combinevals).end()){
				if (ctype == ADD){
					(*combinevals)[coord] += val*weights[weightparam];
				}
				else if (ctype == MULTIPLY){
					(*combinevals)[coord] *= val;
				}
				else if (ctype == DIFFERENCE){
					(*combinevals)[coord] -= val*weights[weightparam];
				}
				else if (ctype == DIVIDE){
					(*combinevals)[coord] /= val;
				}
				double tval = (*combinevals)[coord]; 
				if (tval < minval){
					minval = tval;
				}
				else if (tval > maxval){
					maxval = tval;
				}
			}
			else {
				if (ctype == ADD){
					(*combinevals)[coord] = val*weights[weightparam];
				}
				else if (ctype == MULTIPLY){
					(*combinevals)[coord] = val;
				}
				else if (ctype == DIFFERENCE){
					(*combinevals)[coord] = val*weights[weightparam];
				}
				else if (ctype == DIVIDE){
					(*combinevals)[coord] = val;
				}
				double tval = (*combinevals)[coord]; 
				if (tval < minval){
					minval = tval;
				}
				else if (tval > maxval){
					maxval = tval;
				}
			}
		}
	}

	if (repeat){
		maxval = (double) 0.0f;
		minval = (double) 1000000.0f;
		terr::CPointsMap * rcombinevals = new terr::CPointsMap();
		for (terr::CPointsMap::iterator i = combinevals->begin(); i != combinevals->end(); i++){
/*
Ftileable(x, y) = ( 
       F(x, y) * (w - x) * (h - y) + 
       F(x - w, y) * (x) * (h - y) + 
       F(x - w, y - h) * (x) * (y) + 
       F(x, y - h) * (w - x) * (y)
) / (wh)
The problem with not applying this to the noise algorithm directly...hmm...it seems the problem occurs when 
the nearing proximate to the border on negative heightmap values (that in this context) are occuring due 
to periodicity with the modulus operator.  Using a tiled map, for instance, does not ensure values are
periodic unless the map is already periodic.  The algorithm may need a mirror flip on heightmaps.
So with mirrors the equation above changes.
F(x,y) = F1.  F(x-w,y) = F2.   F(x,y-h) = F3.  F(x-w,y-h) = F4.  
Mirrors are applied to F2,F3,and F4.  F2 = F1 LR  (left right flip) , F3 = F1 TB  ( top bottom flip)
F4 = F1 LRTB (left right top bottom flip).  In this case, flipping heightmaps in 3 additional states, 
and then grabbing the coordinate value. Basically the alternate, is supplying this formula directly 
to the noise algorithms direct, but the idea should be one in the same with translation.  
If the algorithm is correct for noise, then as one approaches the borders one should be proximately
similar to such heightmap values.

Ideally this method could be extended to non noise type generated textures in producing hopefully
seamless tiles.
180 rotation is top bottom flip with lr flip. (x,-y)
left right flip is -x,y
applying lrflip to tpbtm coord -xtb, ytb is tblr flip or pure 180 rotation is lrtb flip (-x,-y)
coord1 uses lr flip, coord2 uses tb flip, coord3 uses lrtb flip
*/
			terr::Coordpair hcoord = (*i).first;
			int xpos = hcoord.first;  int ypos = hcoord.second;
			int xposp = xpos-512;  int yposp = ypos - 512;
			//xposp = xposp % 513;//(xposp > 512 ? (xposp % 512)-1: xposp); 
			//yposp = yposp % 513;//(yposp > 512 ? (yposp % 512)-1: yposp);  //using modulus to determine repeat position 
			terr::Coordpair * hcoord1 = new terr::Coordpair(-1*xposp, ypos);
			terr::Coordpair * hcoord2 = new terr::Coordpair(xpos, -1*yposp);
			terr::Coordpair * hcoord3 = new terr::Coordpair(-1*xposp, -1*yposp);
			double val1 = (*combinevals)[hcoord]*((double)(512-xpos))*((double)(512-ypos));
			double val2 = (*combinevals)[(*hcoord1)]*((double)(xpos))*((double)(512-ypos));
			double val3 = (*combinevals)[(*hcoord2)]*((double)xpos)*((double)ypos);
			double val4 = (*combinevals)[(*hcoord3)]*((double)(512-xpos))*((double)ypos);
			double height = (val1+val2+val3+val4)/(512.0f*512.0f); 
			//if (!negvals){(*i).second /= scalediv;}
			//else{(*i).second /= scalediv; (*i).second += 0.5f;}
			if (height < minval){
				minval = height;
			}
			else if (height > maxval){
				maxval = height;
			}
			(*rcombinevals)[hcoord] = height;
		}
		*combinevals = *rcombinevals;		
	}

	double scalediv; bool negvals = false;
	if (abs(minval) < abs(maxval)){
		scalediv = abs(maxval);
	}
	else{
		scalediv = abs(minval);
	}
        if (minval < 0){
		scalediv *= 2.0f;  negvals = true;
        }
	for (terr::CPointsMap::iterator i = combinevals->begin(); i != combinevals->end(); i++){
		if (!negvals){(*i).second /= scalediv;}
		else{(*i).second /= scalediv; (*i).second += 0.5f;}
	}


	return combinevals;
}
#endif
