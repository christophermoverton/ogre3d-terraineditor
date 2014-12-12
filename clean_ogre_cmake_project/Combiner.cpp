#include <vector>

using namespace std;
enum class COMBINER_TYPE {ADD, MULTIPLY, DIVIDE, DIFFERENCE
};

class Combiner{
	public:

		Combiner(COMBINER_TYPE type);
		COMBINER_TYPE ctype;
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
		vector<vector<vector<double> > > Combine(vector<vector<vector<vector<double> vals,
							vector<float> weights);
	private:
		COMBINER_TYPE ctype;
};

Combiner::Combiner(COMBINER_TYPE type){
	ctype = type;
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
				if ctype == COMBINER_TYPE::ADD {
					//combinevals[i][j][k] = weight1*val1 + weight2*val2;
					tval = weight1*val1 + weight2*val2;
				}
				else if (ctype == COMBINER_TYPE::MULTIPLY){
					//no weighting since order of weights is commutative 
					tval = val1*val2;
				}
				else if (ctype == COMBINER_TYPE::DIFFERENCE){
					tval = weight1*val1-weight2*val2;
				}
				else if (ctype == COMBINER_TYPE::DIVIDE){
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

vector<vector<vector<double> > > Combiner::Combine(vector<vector<vector<vector<double> vals,
							vector<float> weights){

}
