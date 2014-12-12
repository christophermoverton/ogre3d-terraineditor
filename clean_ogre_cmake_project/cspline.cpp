//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.34014
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------
/*
using System;
using MathNet.Numerics.LinearAlgebra;
using MathNet.Numerics;
using MathNet.Numerics.LinearAlgebra.Double;
namespace linearalgpckg
{
*/
#include <vector>
#include <OgreMatrix4.h>
#include <OgreVector4.h>
using namespace std;

	class cspline
	{    
	   public:
		vector<double> xc;
		vector<double> yc;

		vector<double> ypc; //y prime boundary conditions
		//var Coeff =Ogre::Matrix4.Build(4,4);
		cspline (vector<double> x, vector<double> y);
		cspline (vector<double> x, vector<double> y, vector<double> yp);
		Ogre::Vector4 compute2nodecspline (double bc1, double bc2);
		vector<Ogre::Vector4> computecspline (void);
		vector<Ogre::Vector4> computecsplinebcs (void);
		Ogre::Matrix4 coeffmatrix(double x1, double x2, double y1, double y2);
		Ogre::Matrix4 coeffmatrixi(int iter1);
		Ogre::Vector4 simplesfuncvector(int iter1,int iter2, double bc1, double bc2);
		Ogre::Vector4 splinefuncvector(int iter1,int iter2);
		Ogre::Vector4 csplinecoeff(Ogre::Matrix4 M, Ogre::Vector4 y);
		vector<double> computeslope(int iter1, int iter2);

	};
		cspline::cspline (vector<double> x, vector<double> y)
		{	
/*	
				if (x.Length != y.Length)
				{
					throw new ArgumentException ("Vectors are not the same length!");
				}
				if (x.Length < 2)
				{
					throw new ArgumentException ("vector array needs to be of length greater than 1!");
				}
*/				
				//xc[sizeof(x)];
				//yc[sizeof(y)];
 				//std::copy(std::begin(x), std::end(x), std::begin(xc));
				//std::copy(std::begin(y), std::end(y), std::begin(yc));
				xc=x;
				yc=y;

		}
		cspline::cspline (vector<double> x, vector<double> y, vector<double> yp)
		{		
/*
			if (x.Length != y.Length || x.Length != yp.Length)
			{
				throw new ArgumentException ("Vectors are not the same length!");
			}
			if (x.Length < 2)
			{
				throw new ArgumentException ("vector array needs to be of length greater than 1!");
			}
*/
			//copy(std::begin(x), std::end(x), std::begin(xc));
			//copy(std::begin(y), std::end(y), std::begin(yc));
			//copy(std::begin(yp),std::end(yp),std::begin(ypc));
			xc=x;
			yc=y;
			ypc=yp;
			
		}
		Ogre::Vector4 cspline::compute2nodecspline (double bc1, double bc2){
			//Ogre::Vector4 vcont = Ogre::Vector4.Build.Dense (2);
			Ogre::Vector4 x = Ogre::Vector4(0,0,0,0);
			for (int i=0; i< sizeof(xc); i++) {
				if (i <sizeof(xc)-1){
					Ogre::Matrix4 M = coeffmatrix (xc[i],xc[i+1],yc[i],yc[i+1]);
					//Ogre::Vector4 y = splinefuncvector(i,i+1);
					Ogre::Vector4 y = simplesfuncvector(i,i+1,bc1, bc2);
					Ogre::Vector4 x = csplinecoeff(M,y);
					return x;
					//vcont = x;
				}
			}
			return x;

		}
		vector<Ogre::Vector4> cspline::computecspline (){
			//I compute first derviative boundary conditions.  You can modify this code for second or third
			//derivative boundary conditions, but would have to supply your methods for computing second 
			//or third derivatives on your own, likewise, you'd need to make adjustment to the coefficient
			//matrix according to the solution method indicated below...that is, with higher order derivatives
			//adjusting the matrix with zeros and appropriate derivative order on the interpolating row equation
			//f(x_i)=a+bx_i+cx_i^2+dx_i^3.

			//int iter1 = 0;
			vector<Ogre::Vector4> vcont(sizeof(xc));
			for (int i=0; i< sizeof(xc); i++) {
				if (i <sizeof(xc)-1){
					Ogre::Matrix4 M = coeffmatrixi (i);
					Ogre::Vector4 y = splinefuncvector(i,i+1);
					Ogre::Vector4 x = csplinecoeff(M,y);
					vcont[i] = x;
				}
			}
			return vcont;
		}
		vector<Ogre::Vector4> cspline::computecsplinebcs (){
			// First derviative boundary conditions supplied and assumed provided with constructor.  
			//You can modify this code for second or third
			//derivative boundary conditions, but would have to supply your methods for computing second 
			//or third derivatives on your own, likewise, you'd need to make adjustment to the coefficient
			//matrix according to the solution method indicated below...that is, with higher order derivatives
			//adjusting the matrix with zeros and appropriate derivative order on the interpolating row equation
			//f(x_i)=a+bx_i+cx_i^2+dx_i^3.
			
			//int iter1 = 0;
			vector<Ogre::Vector4> vcont(sizeof(xc));
			for (int i=0; i< sizeof(xc); i++) {
				if (i <sizeof(xc)-1){
					Ogre::Matrix4 M = coeffmatrixi (i);
					//Ogre::Vector4 y = splinefuncvector(i,i+1);
					Ogre::Vector4 y = simplesfuncvector(i,i+1, ypc[i], ypc[i+1]);
					Ogre::Vector4 x = csplinecoeff(M,y);
					vcont[i] = x;
				}
			}
			return vcont;
		}
		Ogre::Matrix4 cspline::coeffmatrix(double x1, double x2, double y1, double y2)
		{	
			//coefficients are determined for the cubic equation of the following order
			// f(x)= a + bx + cx^2+dx^3 and f'(x)=0+b+2cx+3dx^2
			// where f(x_i)=a+bx_i+cx_i^2+dx_i^3 and f'(x_i)=a*0+b+2cx_i+3dx_i^2 determine the rows of the 
			//coefficient matrix of the spline interpolation matrix between node points.

			double coeffvals[4][4];
			for (int i=0; i < 2; i++) {
				//double[] coeffrow= new double[4];
				for (int j=0; j < 4;j++){

					coeffvals[i][j]=pow(xc[i],j);
					//Console.WriteLine (coeffvals[i,j]);
				}
				//coeffvals[i,j]=coeffrow;
			}
			for (int k=0; k < 2; k++) {
				//double[] coeffrow= new double[4];
				//Console.WriteLine (i);
				for (int l=0; l < 4;l++){
					if (l==0){
						coeffvals[k+2][l]=0;
					}
					else {
						coeffvals[k+2][l]= l*pow(xc[k],l-1);
					}

				}
				//coeffvals[i]=coeffrow;
			}
			Ogre::Matrix4 M = Ogre::Matrix4(coeffvals[0][0], coeffvals[0][1], coeffvals[0][2],coeffvals[0][3],
							coeffvals[1][0],coeffvals[1][1],coeffvals[1][2],coeffvals[1][3],
							coeffvals[2][0],coeffvals[2][1],coeffvals[2][2],coeffvals[2][3],
							coeffvals[3][0],coeffvals[3][1],coeffvals[3][2],coeffvals[3][3]);
			return M;

		}
		Ogre::Matrix4 cspline::coeffmatrixi(int iter1)
		{	
			//coefficients are determined for the cubic equation of the following order
			// f(x)= a + bx + cx^2+dx^3 and f'(x)=0+b+2cx+3dx^2
			// where f(x_i)=a+bx_i+cx_i^2+dx_i^3 and f'(x_i)=a*0+b+2cx_i+3dx_i^2 determine the rows of the 
			//coefficient matrix of the spline interpolation matrix between node points.
			
			double coeffvals[4][4];
			for (int i=0; i < 2; i++) {
				//double[] coeffrow= new double[4];
				for (int j=0; j < 4;j++){
					
					coeffvals[i][j]=pow(xc[iter1+i],j);
					//Console.WriteLine (coeffvals[i,j]);
				}
				//coeffvals[i,j]=coeffrow;
			}
			for (int k=0; k < 2; k++) {
				//double[] coeffrow= new double[4];
				//Console.WriteLine (i);
				for (int l=0; l < 4;l++){
					if (l==0){
						coeffvals[k+2][l]=0;
					}
					else {
						coeffvals[k+2][l]= l*pow(xc[iter1+k],l-1);
					}
					
				}
				//coeffvals[i]=coeffrow;
			}
			Ogre::Matrix4 M = Ogre::Matrix4(coeffvals[0][0], coeffvals[0][1], coeffvals[0][2],coeffvals[0][3],
							coeffvals[1][0],coeffvals[1][1],coeffvals[1][2],coeffvals[1][3],
							coeffvals[2][0],coeffvals[2][1],coeffvals[2][2],coeffvals[2][3],
							coeffvals[3][0],coeffvals[3][1],coeffvals[3][2],coeffvals[3][3]);
			return M;
			
		}
		Ogre::Vector4 cspline::simplesfuncvector(int iter1,int iter2, double bc1, double bc2){
			//with supplied boundary conditions on a 2 node set
			double rvec[4];
			rvec [0] = yc [iter1];
			rvec [1] = yc [iter2];
			//double[] slopes = computeslope (iter1, iter2);
			rvec [2] = bc1;
			rvec [3] = bc2;
			Ogre::Vector4 V = Ogre::Vector4(rvec[0],rvec[1],rvec[2],rvec[3]);
			return V;
		}
		Ogre::Vector4 cspline::splinefuncvector(int iter1,int iter2){
			double rvec[4];
			rvec [0] = yc [iter1];
			rvec [1] = yc [iter2];
			vector<double> slopes = computeslope (iter1, iter2);
			rvec [2] = slopes [0];
			rvec [3] = slopes [1];
			//Ogre::Vector4 V = DenseVector.OfArray(rvec);
			Ogre::Vector4 V = Ogre::Vector4(rvec[0],rvec[1],rvec[2],rvec[3]);
			return V;
		}

		Ogre::Vector4 cspline::csplinecoeff(Ogre::Matrix4 M, Ogre::Vector4 y){
			Ogre::Matrix4 invmatrix = M.inverse();
			for (int i = 0; i < 4; i++){
				for(int j =0; j < 4; j++){
					if (abs(invmatrix[i][j])<1e-6){
						invmatrix[i][j] = 0;
					} 
				}
			}
			Ogre::Vector4 x =  invmatrix*y;
			return x;
		}

		vector<double> cspline::computeslope(int iter1, int iter2){
			//computing boundary condition first dervative slope conditions.  This is an average slope approaching from 
			// an exterior point...that is not from (y2-y1)/(x2-x1) but a previous iteration point
			//first we check to see if the point is on our node boundary of points, if so we assign a slope of zero
			double slope1;
			double slope2;
			vector<double> slopes(2);
			if (iter1 == 0) {
				slope1 = 0.0;
			} 
			else 
			{
				slope1 = (yc[iter1]-yc[iter1-1])/(xc[iter1]-xc[iter1-1]);
			}
			if (iter2 == sizeof(xc)-1) {
				slope2 = 0.0;
			} 
			else 
			{
				slope2 = (-yc[iter2]+yc[iter2+1])/(-xc[iter1]+xc[iter1+1]);
			}
			slopes [0] = slope1;
			slopes [1] = slope2;
			return slopes;

		}
//	}
//}

