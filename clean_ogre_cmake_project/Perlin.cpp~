#ifndef __Perlin_CPP
#define __Perlin_CPP
#include <vector>
using namespace std;
class Perlin{
	public:
		Perlin(void);
		Perlin(int gridsizex, int gridsizey);
		Perlin(int gridsizex, int gridsizey, int gridsizez);
		double getNoiseValue(float x, float y);
		void dnoise3f( vector<float>* vout, float x, float y, float z );
		double getNoiseValue(float x, float y, float z);
		vector<double> get3dNoisePartials(void);  //access to cNoisePartial3d method only
	private:
		double dotGridGradient(int ix, int iy, float x, float y);
		double dotGridGradient(int ix, int iy, int iz, float x, float y, float z);
		vector<vector<Ogre::Vector2> > Gradient;
		vector<vector<vector<Ogre::Vector3> > > Gradient3d;
		vector<double> cNoisePartials3d;  //temporary noise partial storage for 1 iteration point
		double lerp(float a0, float a1, float w); 
		Ogre::Vector2 randomPointUnitCircle(void);
		Ogre::Vector3 randomPointUnitSphere(void);
		
		float myRandomMagic(float x, float y, float z, int ix, int iy, int iz);
		void iGetIntegerAndFractional(float val, int* outi, float* outd);		
};

Perlin::Perlin(){
}

Perlin::Perlin(int gridsizex, int gridsizey){
	Gradient.resize(gridsizex);
	for (int i = 0; i < gridsizex; i++){
		Gradient[i].resize(gridsizey);
		for(int j = 0; j < gridsizey; j++){
			Gradient[i][j] = randomPointUnitCircle();
		}
	}
}

Perlin::Perlin(int gridsizex, int gridsizey, int gridsizez){
	Gradient3d.resize(gridsizex);
	cNoisePartials3d.resize(3);
	for (int i = 0; i < gridsizex; i++){
		Gradient3d[i].resize(gridsizey);
		//cNoisePartials3d[i].resize(gridsizey);
		for(int j = 0; j < gridsizey; j++){
			//cNoisePartials3d[i][j].resize(3);
			Gradient3d[i][j].resize(gridsizez);
			for (int k = 0; k < gridsizez; k++){
				Gradient3d[i][j][k] = randomPointUnitSphere();
			}
			//Gradient[i][j] = randomPointUnitCircle();
		}
	}
}

Ogre::Vector2 Perlin::randomPointUnitCircle(){
	int v1 = rand() % 360;         // v1 in the range 0 to 360 degrees
	Ogre::Real y = Ogre::Math::Sin(Ogre::Math::DegreesToRadians(v1));
	Ogre::Real x = Ogre::Math::Cos(Ogre::Math::DegreesToRadians(v1));
	return Ogre::Vector2(x,y);	
}

Ogre::Vector3 Perlin::randomPointUnitSphere(){
	int theta = rand() % 359; 
	int phi = rand() % 359;
	double dec1 = (rand() % 999)/1000;
	double dec2 = (rand() % 999)/1000;
	
	//Generated from Spherical Coordinates
	Ogre::Real y = Ogre::Math::Sin(Ogre::Math::DegreesToRadians(theta+dec1))*Ogre::Math::Sin(Ogre::Math::DegreesToRadians(phi));
	Ogre::Real x = Ogre::Math::Cos(Ogre::Math::DegreesToRadians(theta+dec1))*Ogre::Math::Sin(Ogre::Math::DegreesToRadians(phi));
	Ogre::Real z = Ogre::Math::Cos(Ogre::Math::DegreesToRadians(phi+dec2));
	return Ogre::Vector3(x,y,z);
}
 // Function to linearly interpolate between a0 and a1
 // Weight w should be in the range [0.0, 1.0]
 double Perlin::lerp(float a0, float a1, float w) {
     return (1.0 - w)*a0 + w*a1;
 }
 
 // Computes the dot product of the distance and gradient vectors.
 double Perlin::dotGridGradient(int ix, int iy, float x, float y) {
 
     // Precomputed (or otherwise) gradient vectors at each grid point X,Y
     //extern float Gradient[Y][X][2];
 
     // Compute the distance vector
     double dx = x - (double)ix;
     double dy = y - (double)iy;
 
     // Compute the dot-product
     return (dx*Gradient[ix][iy][0] + dy*Gradient[ix][iy][1]);
 }
 
 // Computes the dot product of the distance and gradient vectors.
 double Perlin::dotGridGradient(int ix, int iy, int iz, float x, float y, float z) {
 
     // Precomputed (or otherwise) gradient vectors at each grid point X,Y
     //extern float Gradient[Y][X][2];
 
     // Compute the distance vector
     double dx = x - (double)ix;
     double dy = y - (double)iy;
     double dz = z - (double)iz;
     // Compute the dot-product
     return (dx*(double)Gradient3d[ix][iy][iz][0] + dy*(double)Gradient3d[ix][iy][iz][1] +dz*(double)Gradient3d[ix][iy][iz][2]);
 }
 // Compute Perlin noise at coordinates x, y
 double Perlin::getNoiseValue(float x, float y) {
     //Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("Perlin.log");
     // Determine grid cell coordinates
     int x0 = (x > 0.0 ? (int)x : (int)x );
     int x1 = x0 + 1;
     int y0 = (y > 0.0 ? (int)y : (int)y );
     int y1 = y0 + 1;
 
     // Determine interpolation weights
     // Could also use higher order polynomial/s-curve here
     double sx = x - (double)x0;
     double sy = y - (double)y0;
     std::ostringstream ss5;
     //ss5<< "Test4" << "\n";
     //ss5<< x0<<","<<y0<< ","<< x<< ","<< y<<"\n";
     //tlog->logMessage(ss5.str());
     //ss5.str(std::string());
     //ss5<<"Gradient: " <<Gradient[x0][y0]<<"\n";
     //tlog->logMessage(ss5.str());
  
     // Interpolate between grid point gradients
     double n0, n1, ix0, ix1, value;
     n0 = dotGridGradient(x0, y0, x, y);
     n1 = dotGridGradient(x1, y0, x, y);
     //ss5.str(std::string());
     //ss5<< "Test5" << "\n";
     ix0 = lerp(n0, n1, sx);
     //ss5.str(std::string());
     //ss5<< "Test5" << "\n";
     //tlog->logMessage(ss5.str());
     n0 = dotGridGradient(x0, y1, x, y);
     n1 = dotGridGradient(x1, y1, x, y);
     ix1 = lerp(n0, n1, sx);
     value = lerp(ix0, ix1, sy);
 
     return value;
 }

 double Perlin::getNoiseValue(float x, float y, float z) {
     //Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("Perlin.log");
     // Determine grid cell coordinates
/*
On a periodic and cubic lattice, let x_d, y_d, and z_d be the differences between each of x, y, z and the smaller coordinate related, that is:

 \ x_d = (x - x_0)/(x_1 - x_0)
 \ y_d = (y - y_0)/(y_1 - y_0)
 \ z_d = (z - z_0)/(z_1 - z_0)
where  x_0  indicates the lattice point below  x , and   x_1  indicates the lattice point above  x  and similarly for y_0, y_1, z_0 and z_1.

First we interpolate along x (imagine we are pushing the front face of the cube to the back), giving:

 \ c_{00} = V[x_0,y_0, z_0]  (1 - x_d) + V[x_1, y_0, z_0] x_d 
 \ c_{10} = V[x_0,y_1, z_0]  (1 - x_d) + V[x_1, y_1, z_0] x_d 
 \ c_{01} = V[x_0,y_0, z_1]  (1 - x_d) + V[x_1, y_0, z_1] x_d 
 \ c_{11} = V[x_0,y_1, z_1]  (1 - x_d) + V[x_1, y_1, z_1] x_d 
Where V[x_0,y_0, z_0] means the function value of  (x_0,y_0,z_0).  Then we interpolate these values (along y, as we were pushing the top edge to the bottom), giving:

 \ c_0 = c_{00}(1 - y_d) + c_{10}y_d
 \ c_1 = c_{01}(1 - y_d) + c_{11}y_d
Finally we interpolate these values along z(walking through a line):

 \ c = c_0(1 - z_d) + c_1z_d .

*/
     int x0 = (x > 0.0 ? (int)x : (int)x );
     int x1 = x0 + 1;
     int y0 = (y > 0.0 ? (int)y : (int)y );
     int y1 = y0 + 1;
     int z0 = (int)z;
     int z1 = z0 + 1;
 
     // Determine interpolation weights
     // Could also use higher order polynomial/s-curve here
     double sx = x - (double)x0;
     double sy = y - (double)y0;
     double sz = z - (double)z0;
     double u = 6.0f*pow(sx,5) - 15*pow(sx,4)+10*pow(sx,3);
     double v = 6.0f*pow(sy,5) - 15*pow(sy,4)+10*pow(sy,3);
     double w = 6.0f*pow(sz,5) - 15*pow(sz,4)+10*pow(sz,3);

     double pu = 30.0f*pow(sx,4)-60.0f*pow(sx,3)+30.0f*pow(sx,2);
     double pv = 30.0f*pow(sy,4)-60.0f*pow(sy,3)+30.0f*pow(sy,2);
     double pw = 30.0f*pow(sz,4)-60.0f*pow(sz,3)+30.0f*pow(sz,2);
     std::ostringstream ss5;
     //ss5<< "Test4" << "\n";
     //ss5<< x0<<","<<y0<< ","<< x<< ","<< y<<"\n";
     //tlog->logMessage(ss5.str());
     //ss5.str(std::string());
     //ss5<<"Gradient: " <<Gradient[x0][y0]<<"\n";
     //tlog->logMessage(ss5.str());
  
     // Interpolate between grid point gradients
     double n000, n100, n010, n001, n110, n101, n011, n111 ,c00, c10, c01, c11, c0,c1,value;
     n000 = dotGridGradient(x0, y0, z0, x, y, z);
     n100 = dotGridGradient(x1, y0, z0, x, y, z);
     n010 = dotGridGradient(x0, y1, z0, x, y, z);
     n001 = dotGridGradient(x0, y0, z1, x, y, z);
     n110 = dotGridGradient(x1, y1, z0, x, y, z);
     n101 = dotGridGradient(x1, y0, z1, x, y, z);
     n011 = dotGridGradient(x0, y1, z1, x, y, z);
     n111 = dotGridGradient(x1, y1, z1, x, y, z);
     double k1 = n000;
     double k2 = n100 - n000;
     double k3 = n010;
     double k4 = n110 - n010;
     double k5 = n001;
     double k6 = n101 - n001;
     double k7 = n011;
     double k8 = n111 - n011;
     //ss5.str(std::string());
     //ss5<< "Test5" << "\n";
     c00 = lerp(n000, n100, u);
     c10 = lerp(n010, n110, u);
     c01 = lerp(n001, n101, u);
     c11 = lerp(n011, n111, u);
     c0 = lerp(c00, c10, v);
     c1 = lerp(c01, c11, v);
     value = lerp(c0,c1, w);
     //ss5.str(std::string());
     //ss5<< "Test5" << "\n";
     //tlog->logMessage(ss5.str());
     //n0 = dotGridGradient(x0, y1, x, y);
     //n1 = dotGridGradient(x1, y1, x, y);
     //ix1 = lerp(n0, n1, sx);
     //value = lerp(ix0, ix1, sy);
     double pNx = k2*pu -k2*pu*v + k4*pu*v +(k6-k2)*pu*w +(k2-k4-k6+k8)*pu*v*w;
     double pNy = -k1*pv-k2*u*pv+k3*pv+k4*u*pv +(k1-k3-k5+k7)*pv*w + (k2-k4-k6+k8)*u*pv*w;
     double pNz = (k5-k1)*pw + (k6-k2)*u*pw + (k1-k3-k5+k7)*v*pw + (k2-k4-k6+k8)*u*v*pw;
     cNoisePartials3d[0] = pNx;
     cNoisePartials3d[1] = pNy;
     cNoisePartials3d[2] = pNz;
     return value;
 }

void Perlin::iGetIntegerAndFractional(float val, int* outi, float* outd){
    int outu;
    float outv;
    outu = (int)val;
    outv = (float)(val - (int)val);
    outi = &outu;
    outd = &outv;
}

float Perlin::myRandomMagic(float x, float y, float z, int ix, int iy, int iz){
//	float val = dotGridGradient(ix, iy, iz, x, y, z);
	//float returnval;
	float val = Gradient3d[ix][iy][iz][0];
	return val;
}

void Perlin::dnoise3f( vector<float>* vout, float x, float y, float z )
{
    int   i, j, k;
    float u, v, w;

    iGetIntegerAndFractional( x, &i, &u );
    iGetIntegerAndFractional( y, &j, &v );
    iGetIntegerAndFractional( z, &k, &w );

    const float du = 30.0f*u*u*(u*(u-2.0f)+1.0f);
    const float dv = 30.0f*v*v*(v*(v-2.0f)+1.0f);
    const float dw = 30.0f*w*w*(w*(w-2.0f)+1.0f);

    u = u*u*u*(u*(u*6.0f-15.0f)+10.0f);
    v = v*v*v*(v*(v*6.0f-15.0f)+10.0f);
    w = w*w*w*(w*(w*6.0f-15.0f)+10.0f);

    const float a = myRandomMagic( x,y,z,i+0, j+0, k+0 );
    const float b = myRandomMagic( x,y,z,i+1, j+0, k+0 );
    const float c = myRandomMagic( x,y,z,i+0, j+1, k+0 );
    const float d = myRandomMagic( x,y,z,i+1, j+1, k+0 );
    const float e = myRandomMagic( x,y,z,i+0, j+0, k+1 );
    const float f = myRandomMagic( x,y,z,i+1, j+0, k+1 );
    const float g = myRandomMagic( x,y,z,i+0, j+1, k+1 );
    const float h = myRandomMagic( x,y,z,i+1, j+1, k+1 );

    const float k0 =   a;
    const float k1 =   b - a;
    const float k2 =   c - a;
    const float k3 =   e - a;
    const float k4 =   a - b - c + d;
    const float k5 =   a - c - e + g;
    const float k6 =   a - b - e + f;
    const float k7 = - a + b + c - d + e - f - g + h;
    vector<float> pvout(4);

    pvout[0] = k0 + k1*u + k2*v + k3*w + k4*u*v + k5*v*w + k6*w*u + k7*u*v*w;
    pvout[1] = du * (k1 + k4*v + k6*w + k7*v*w);
    pvout[2] = dv * (k2 + k5*w + k4*u + k7*w*u);
    pvout[3] = dw * (k3 + k6*u + k5*v + k7*u*v);
    vout = &pvout;
}

vector<double> Perlin::get3dNoisePartials(){
	return cNoisePartials3d;
}

#endif
