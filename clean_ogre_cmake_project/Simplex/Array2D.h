#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <string>
#include <iostream>

template <class T>
class Array_2D{
protected:
        T* values;
        unsigned xgrid;
        unsigned ygrid;

public:
        Array_2D(unsigned width, unsigned height):xgrid(width),ygrid(height) {values = new T[ width * height ];}
        Array_2D(unsigned width, unsigned height, T default_value);
        Array_2D(const Array_2D<T>& rhs);
        
        T& operator()(int x, int y);
        const T& operator()(int x, int y) const;

        Array_2D<T>& operator=( const Array_2D<T>& rhs );
        virtual Array_2D<T>& operator+=( const Array_2D<T>& rhs );
        virtual Array_2D<T>& operator-=( const Array_2D<T>& rhs );
        virtual const Array_2D operator+( const Array_2D<T>& rhs ) const {return (Array_2D(*this)+=rhs);}
        virtual const Array_2D operator-( const Array_2D<T>& rhs ) const {return (Array_2D(*this)-=rhs);}
        
        unsigned width(){return xgrid;}
        unsigned height(){return ygrid;}
        
        ~Array_2D();
};

#endif
