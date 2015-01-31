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
