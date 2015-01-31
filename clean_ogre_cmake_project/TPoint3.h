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
};

#endif
