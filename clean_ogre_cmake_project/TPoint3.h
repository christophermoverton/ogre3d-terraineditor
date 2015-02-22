#ifndef TPoint3_h
#define TPoint3_h

struct TPoint3
{
	public:
	    
		double x, y, z;
	
		/*
			Constructor for structure; x, y, z - coordinates.  Like an Ogre::Vector3 class...more limited
			at the moment though...uses double instead of float for potentially greater numerical accuracy.
		*/

	    	TPoint3(double nx, double ny, double nz) 
		{
			x = nx; 
			y = ny;
			z = nz;
		}
		TPoint3(){
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

	        TPoint3 & operator=(const TPoint3 & a)
    		{
        		x=a.x;
        		y=a.y;
			z=a.z;
        		return *this;
    		}    		

		bool operator==(const TPoint3 & a) const
	        {
    	    		return (x == a.x && y == a.y && z == a.z);
    		}
		
		bool operator!=(const TPoint3 & a) const{
			return (x != a.x || y != a.y || z != a.z);
		}

		TPoint3 operator+(const TPoint3 & a) const
    		{
        		return TPoint3(a.x+x, a.y+y, a.z+z);
    		}
		TPoint3 operator+(const double a) const
    		{
        		return TPoint3(a+x, a+y, a+z);
    		}
		TPoint3 operator-(const double a) const
    		{
        		return TPoint3(a-x, a-y, a-z);
    		}
		TPoint3 operator/(const double a) const
    		{
        		return TPoint3(x/a, y/a, z/a);
    		}
		TPoint3 normalize(){
			double dist = pow((x*x + y*y + z*z),0.5f);
			return TPoint3(x/dist, y/dist, z/dist);
		}
};

#endif
