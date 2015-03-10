#ifndef __BumpMap_CPP
#define __BumpMap_CPP
#include "TerrainStruct.h"
#include "TPoint3.h"
#include "Imagestuff.cpp"
#include "TerrainTexturesNode.cpp"
#include "TerrainTexturesSettings.cpp"
#include <string>
// unsigned char h(x, y) returns the height map value at x, y.
// the map is of size width*height
// Vector3 normal[width*height] will contain the calculated normals.
//
// The height map has x, y axes with (0, 0) being the top left corner of the map.
// The resulting mesh is assumed to be in a left hand system with x right, z into the screen
// and y up (i.e. as in DirectX).
//
// yScale denotes the scale of mapping heights to final y values in model space
// (i.e. a height difference of 1 in the height map results in a height difference
// of yScale in the vertex coordinate).
// xzScale denotes the same for the x, z axes. If you have different scale factors
// for x, z then the formula becomes
// normal[y*width+x].set(-sx*yScale, 2*xScale, xScalesy*xScale*yScale/zScale);
TPoint3 * Lx = new TPoint3(0.0f,0.0f,1.0f);  //Light Color RGB
TPoint3 * Ax = new TPoint3(0.0f,0.0f,1.0f);//Ax = ambient color 
TPoint3 * Dx = new TPoint3(0.0f,0.0f,1.0f);//Dx = diffuse color 
TPoint3 * Sx = new TPoint3(0.0f,0.0f,1.0f);//Sx = specular color 
double Ka = 0.1f, Kd = 0.0f, Ks = 1.0f, Att = 1.0f, n = 1.0f; //phong coefficients

TPoint3 reflection(TPoint3 vec1, TPoint3 normal);
double dotproduct(TPoint3 vec1, TPoint3 vec2);
TPoint3  phongillum(TPoint3 N, TPoint3 L, TPoint3 V);
TPoint3  phongillum(TPoint3 N, TPoint3 L, TPoint3 V, double Ka, double Kd);
double clamp(double val, double min, double max);

double dotproduct(TPoint3 vec1, TPoint3 vec2){
	return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
}

double clamp(double val, double min, double max){
	if(val < min){
		return min;
	}
	else if (val > max) {
		return max;
	}
	else{
		return val;
	}
}

TPoint3 reflection(TPoint3 vec1, TPoint3 n){
   //u = (v · n / n · n) n 
   //w = v − u  2N (N dot L) - L 
   double val = dotproduct(vec1,n);
   TPoint3 * u = new TPoint3(2*val*n.x - vec1.x, 2*val*n.y-vec1.y, 2*val*n.z -vec1.z );
   //TPoint3 * w = new TPoint3(vec1.x - n.x, vec1.y-n.y, vec1.z-n.z);
   //v′ = w − u  v' is reflection
   return  (*u); //TPoint3((*w).x-(*u).x,(*w).y-(*u).y, (*w).z-(*u).z);
}

void bump(double xzScale, double yScale, terr::CPointsMap * heightmap, TerrainTexturesSettings texsets);//std::string TextureName); //finite differences method 
void bump(double xzScale, double yScale, Ogre::Terrain* terrain, TerrainTexturesSettings texsets);
double lerp(double a0, double a1, double w);

double lerp(double a0, double a1, double w) {
     return (1.0 - w)*a0 + w*a1;
 }

void bump(double xzScale, double yScale, terr::CPointsMap * heightmap, TerrainTexturesSettings * texsets){//std::string TextureName){
    double size = xzScale;
    double normfac = texsets->normal;
    std::string TextureName = texsets->name;
    ImageBuffer buffer(size);
    ImageBuffer buffr(size);
    FillColour* fill = new FillColour (&buffer);
    FillColour* filltwo = new FillColour (&buffr);
    terr::CVectorMaps * normal = new terr::CVectorMaps();
    textureNodeMap terraintexnds = TerrainTexturesNode::Instance()->getTextureNodes(); //textureNodeMap defined in TerrainTexturesNode.cpp
    std::string * normaltexname = terraintexnds[TextureName].normaltextureName;
    //CVectorMaps * bump = new 
    TPoint3 * V = new TPoint3(0.0f,-1.0f,0.0f); TPoint3 * L = new TPoint3(0.55, -0.3, 0.75);
    (*L) = (*L).normalize();
    double sy, sx; double x0 = xzScale-1; double y0 = xzScale-1;
    for (int x = 0; x < xzScale; x++){
	for (int y = 0; y < xzScale; y++){
	    //computing surface normals (forward difference equation) y' ~ delta f (x_i + 1)/( delta (x_i+1 - x_i) = 1) - f(x_i) 
            //gradient is (f'(x), f'(y), f'(z)) = (h(x+1,y)-h(x), constant (shown below), h(x,y+1)-h(y)) 
           
	    double sx = (*heightmap)[terr::Coordpair(x<xzScale-1 ? (double)(x+1) : (double)x, (double)y)] - 
				(*heightmap)[terr::Coordpair(x == x0 ? (double)(x-1) : (double)x, (double)y)];
	    if (x == 0 || x == xzScale - 1){ sx *= 2;}
	    double sy = (*heightmap)[terr::Coordpair(x, y < xzScale-1 ? (double)(y+1) : (double)y)] - 
				(*heightmap)[terr::Coordpair(x, y == y0 ? (double)(y-1) : (double)y)];
	    if (y == 0 || y == xzScale -1) {sy *= 2;}
	    TPoint3 * vec = new TPoint3(-1.0f*normfac*sx, -1.0f*normfac*sy, 0.1f);
	    //TPoint3 * vec = new TPoint3(sx*yScale, 0.0f, sy*yScale);  //changing -sx, 2*xzScale mid term ??!
            (*vec) = (*vec).normalize();
            TPoint3 vec2 = (*vec)/2.0f + .5f;
	    vec2 = vec2.normalize();
	    (*normal)[terr::Coordpair(x,y)] = vec2;
	    //(*normal)[terr::Coordpair(x,y)] = TPoint3(lerp((*vec).x, 0, 0.25f), lerp((*vec).y, 1, 0.25f),
	    //					      lerp((*vec).z, 0, 0.25f));
	    //the specialized surface normal is intended for producing the bump map using
	    //a linear interpolation between a finite difference 'actual' approximated
	    //surface normal mixed with its ideal planar 'geometric' form.
	    //(*normal)[terr::Coordpair(x,y)] = (*normal)[terr::Coordpair(x,y)].normalize();
	    Ka = 0.2f; Kd = 1.0f;
	    //TPoint3 rgb = phongillum((*normal)[terr::Coordpair(x,y)], (*L), (*V), Ka, Kd);
	    //Ogre::ColourValue col = Ogre::ColourValue(rgb.x,rgb.y,rgb.z, (*heightmap)[terr::Coordpair(x, y)]);
	    //Ogre::ColourValue col = Ogre::ColourValue((*normal)[terr::Coordpair(x,y)].x,(*normal)[terr::Coordpair(x,y)].y,
	    //						(*normal)[terr::Coordpair(x,y)].z, (*heightmap)[terr::Coordpair(x, y)]);
	    Ogre::ColourValue col = Ogre::ColourValue(vec2.x,vec2.y,vec2.z, (*heightmap)[terr::Coordpair(x, y)]);
	    //col = Ogre::ColourValue(0.0f,0.0f,0.5f, 1.0f);
	    //compute phong specular intensity
            TPoint3 R = reflection((*L), (*normal)[terr::Coordpair(x,y)]);
	    R = R.normalize();
	    n = 1.0f;
	    double pdRVn = pow(dotproduct(R,(*V)),n);
            n = 1.0f;
	    //TPoint3 rgbtwo = phongillum((*normal)[terr::Coordpair(x,y)], (*L), (*V), Ka, Kd);
	    Ogre::ColourValue coltwo = Ogre::ColourValue(pdRVn,pdRVn,pdRVn);
	    fill->setPixl((size_t)x, (size_t)y, col);
	    filltwo->setPixl((size_t)x, (size_t)y, coltwo);	    
	}
    }
//    buffer.saveImage("../media/materials/textures/test9.png");
//    buffr.saveImage("../media/materials/textures/test9spec.png");
    //if (!terraintexnds[TextureName].normalimageName){
        buffer.saveImage("../media/materials/textures/"+ TextureName+"normal.png");
        buffr.saveImage("../media/materials/textures/"+TextureName+"spec.png");
     texsets->normals = *normal;
        //terraintexnds[TextureName].normalHeightMapData = normal;
        //std::string nimagename = *normaltexname +".png";
        //terraintexnds[TextureName].normalimageName = &nimagename;
    /*
    }
    else{
        buffer.saveImage("../media/materials/textures/"+(*(terraintexnds[TextureName].normalimageName)));
        buffr.saveImage("../media/materials/textures/"+TextureName+"spec.png");
    }
    */
}

void bump(double xzScale, double yScale, Ogre::Terrain* terrain, TerrainTexturesSettings * texsets){//std::string TextureName){
    double size = xzScale;
    double normfac = texsets->normal;
    std::string TextureName = texsets->name;
    ImageBuffer buffer(size);
    ImageBuffer buffr(size);
    FillColour* fill = new FillColour (&buffer);
    FillColour* filltwo = new FillColour (&buffr);
    terr::CVectorMaps * normal = new terr::CVectorMaps();
    textureNodeMap terraintexnds = TerrainTexturesNode::Instance()->getTextureNodes(); //textureNodeMap defined in TerrainTexturesNode.cpp
    std::string * normaltexname = terraintexnds[TextureName].normaltextureName;
    //CVectorMaps * bump = new 
    TPoint3 * V = new TPoint3(0.0f,-1.0f,0.0f); TPoint3 * L = new TPoint3(0.55, -0.3, 0.75);
    (*L) = (*L).normalize();
    double sy, sx; double x0 = xzScale-1; double y0 = xzScale-1;
    double diff = (double)abs(terrain->getMinHeight() - terrain->getMaxHeight());
    for (int x = 0; x < xzScale; x++){
	for (int y = 0; y < xzScale; y++){
	    //computing surface normals (forward difference equation) y' ~ delta f (x_i + 1)/( delta (x_i+1 - x_i) = 1) - f(x_i) 
            //gradient is (f'(x), f'(y), f'(z)) = (h(x+1,y)-h(x), constant (shown below), h(x,y+1)-h(y)) 
	    double posx = x/size; double posy = y/size;
	    double posx1p = (x+1)/size; double posy1p = (y+1)/size;
	    double posx1m = (x-1)/size; double posy1m = (y-1)/size; 
	    double height = (double)terrain->getHeightAtTerrainPosition(posx,posy);
	    
	    //double heightx1y = (double)terrain->getHeightAtTerrainPosition(posx,posy);
	    double sx = ((double)terrain->getHeightAtTerrainPosition(x<xzScale-1 ? posx1p : posx, posy) - 
				(double)terrain->getHeightAtTerrainPosition(x == x0 ? posx1m : posx, posy))/diff;
	    if (x == 0 || x == xzScale - 1){ sx *= 2;}
	    double sy = ((double)terrain->getHeightAtTerrainPosition(posx, y < xzScale-1 ? posy1p : posy) - 
				(double)terrain->getHeightAtTerrainPosition(posx, y == y0 ? posy1m : posy))/diff;
	    if (y == 0 || y == xzScale -1) {sy *= 2;}
	    TPoint3 * vec = new TPoint3(-1.0f*normfac*sx, -1.0f*normfac*sy, 0.1f);  //changing -sx, 2*xzScale mid term ??!
            (*vec) = (*vec).normalize();
            TPoint3 vec2 = (*vec)/2.0f + .5f;
	    (*normal)[terr::Coordpair(x,y)] = vec2;
	    //(*normal)[terr::Coordpair(x,y)] = TPoint3(lerp((*vec).x, 0, 0.25f), lerp((*vec).y, 1, 0.25f),
	    //					      lerp((*vec).z, 0, 0.25f));
	    //the specialized surface normal is intended for producing the bump map using
	    //a linear interpolation between a finite difference 'actual' approximated
	    //surface normal mixed with its ideal planar 'geometric' form.
	    //(*normal)[terr::Coordpair(x,y)] = (*normal)[terr::Coordpair(x,y)].normalize();
	    Ka = 0.2f; Kd = 1.0f;
	    //TPoint3 rgb = phongillum((*normal)[terr::Coordpair(x,y)], (*L), (*V), Ka, Kd);
	    //Ogre::ColourValue col = Ogre::ColourValue(rgb.x,rgb.y,rgb.z, (*heightmap)[terr::Coordpair(x, y)]);
	    //Ogre::ColourValue col = Ogre::ColourValue((*normal)[terr::Coordpair(x,y)].x,(*normal)[terr::Coordpair(x,y)].y,
	    //						(*normal)[terr::Coordpair(x,y)].z, (*heightmap)[terr::Coordpair(x, y)]);
	    Ogre::ColourValue col = Ogre::ColourValue(vec2.x,vec2.y,vec2.z, height/diff);
	    //col = Ogre::ColourValue(0.0f,0.0f,0.5f, 1.0f);
	    //compute phong specular intensity
            TPoint3 R = reflection((*L), (*normal)[terr::Coordpair(x,y)]);
	    R = R.normalize();
	    n = 1.0f;
	    double pdRVn = pow(dotproduct(R,(*V)),n);
            n = 1.0f;
	    //TPoint3 rgbtwo = phongillum((*normal)[terr::Coordpair(x,y)], (*L), (*V), Ka, Kd);
	    Ogre::ColourValue coltwo = Ogre::ColourValue(pdRVn,pdRVn,pdRVn);
	    fill->setPixl((size_t)x, (size_t)y, col);
	    filltwo->setPixl((size_t)x, (size_t)y, coltwo);	    
	}
    }
//    buffer.saveImage("../media/materials/textures/test9.png");
//    buffr.saveImage("../media/materials/textures/test9spec.png");
    //if (!terraintexnds[TextureName].normalimageName){
        buffer.saveImage("../media/materials/textures/"+ TextureName+"normal.png");
        buffr.saveImage("../media/materials/textures/"+TextureName+"spec.png");
        terraintexnds[TextureName].normalHeightMapData = normal;
        std::string nimagename = TextureName +"normal.png";
        terraintexnds[TextureName].normalimageName = &nimagename;
	texsets->normals = *normal;
    /*
    }
    else{
        buffer.saveImage("../media/materials/textures/"+(*(terraintexnds[TextureName].normalimageName)));
        buffr.saveImage("../media/materials/textures/"+TextureName+"spec.png");
    }
    */
}
//for terrain 'geometric' surface normals are actually given by a normal on a flat plane which 
// is in our case assuming the y axis is actually the heightfield position  [0,1,0] or [0,-1,0]
/*
for (unsigned int y = 0; y<height; ++y)
{
    for (unsigned int x = 0; x<width; ++x)
    {
        // The ? : and ifs are necessary for the border cases.

        float sx = h(x<width-1 ? x+1 : x, y) - h(x0 ? x-1 : x, y);
        if (x == 0 || x == width-1)
            sx *= 2;

        float sy = h(x, y<height-1 ? y+1 : y) - h(x, y0 ?  y-1 : y);
        if (y == 0 || y == height -1)
            sy *= 2;

        normal[y*width+x].set(-sx*yScale, 2*xzScale, sy*yScale);
        normal[y*width+x].normalize();
    }
} 
*/

/*
Here is the phong model for illumination 
Ix = result color 
Lx = light color 
Ax = ambient color 
Dx = diffuse color 
Sx = specular color 
Ka = ambient coefficient 
Kd = diffuse coefficient 
Ks = specular coefficient 
Att = attenuation coefficient 
n = usually referred to as "shine" or "roughness" 
N = surface normal 
L = light vector 
R = reflection vector 
V = view vector 

Given those variables, here's the Phong model (modified to account for specular color): 

Ix = AxKaDx + AttLx [KdDx(N dot L) + KsSx(R dot V)^n] 

For default purposes I'll likely leave the view vector in the direction of the light vector which 
is directly overhead pointed down [0,-1,0] and a reflection vector which need be computed as in 
the surface normal given.

What is controlling the specularity maps... alpha full and zero are producing high specularity?!  :D


*/

TPoint3  phongillum(TPoint3 N, TPoint3 L, TPoint3 V){
        TPoint3 R = reflection(L, N);// reflection vector
	double dNL = dotproduct(N,L); double pdRVn = pow(dotproduct(R,V),n);
	double Ir = (*Ax).x*Ka*(*Dx).x + Att*(*Lx).x *(Kd*(*Dx).x*dNL + Ks*(*Sx).x*pdRVn);
        double Ig = (*Ax).y*Ka*(*Dx).y + Att*(*Lx).y *(Kd*(*Dx).y*dNL + Ks*(*Sx).y*pdRVn);
	double Ib = (*Ax).z*Ka*(*Dx).z + Att*(*Lx).z *(Kd*(*Dx).z*dNL + Ks*(*Sx).z*pdRVn);
	return TPoint3(Ir,Ig,Ib);
}

TPoint3  phongillum(TPoint3 N, TPoint3 L, TPoint3 V, double Ka, double Kd){
        TPoint3 R = reflection(L, N);// reflection vector
	double dNL = dotproduct(N,L); double pdRVn = pow(dotproduct(R,V),n);
	double Ir = (*Ax).x*Ka*(*Dx).x + Att*(*Lx).x *(Kd*(*Dx).x*dNL + Ks*(*Sx).x*pdRVn);
        double Ig = (*Ax).y*Ka*(*Dx).y + Att*(*Lx).y *(Kd*(*Dx).y*dNL + Ks*(*Sx).y*pdRVn);
	double Ib = (*Ax).z*Ka*(*Dx).z + Att*(*Lx).z *(Kd*(*Dx).z*dNL + Ks*(*Sx).z*pdRVn);
	return TPoint3(Ir,Ig,Ib);
}
#endif
