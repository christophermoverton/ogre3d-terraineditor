#ifndef __Imagestuff_CPP
#define __Imagestuff_CPP

class ImageBuffer
{
   friend class FillColour;
private:
   Ogre::uchar* mPixels;
   unsigned int mWidth;
   unsigned int mHeight;

private:
   void setPixel(size_t x, size_t y, Ogre::ColourValue colour);
   void setPixel(size_t x, size_t y, Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

public:
   ImageBuffer(unsigned int width_height);
   ImageBuffer(Ogre::uchar* Pixels, unsigned int width_height);
   Ogre::ColourValue getPixel(size_t x, size_t y); //with mPixels already assigned
   ~ImageBuffer();
   void saveImage(Ogre::String filename);
};

class FillColour
{
private:
   ImageBuffer* mBuffer;
   Ogre::ColourValue mColour;

public:
   FillColour(ImageBuffer* pBuffer)
      : mBuffer(pBuffer), mColour(Ogre::ColourValue::Black)
   {
   }

   /** Sets the colour of texture */
   inline FillColour & setColour(Ogre::ColourValue colour)
   {
      mColour = colour;
      return *this;
   }

   void process();
   void setPixl(size_t x, size_t y, Ogre::ColourValue color);
};

/*
class ImageData{
	public:
		ImageData(const String & filename);
		Ogre::Image & getImage(void);
	private:
		Ogre::Image img;
};

ImageData::ImageData(const String & filename){
	img = new Ogre::Image();
	img.load(filename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
}

Ogre::Image & ImageData::getImage{
	return img;
}
*/

ImageBuffer::ImageBuffer(unsigned int width_height)
   : mWidth(width_height), mHeight(width_height)
{
   mPixels = new Ogre::uchar[mWidth * mHeight * 4];

// set R=0, G=0, B=0, A=255
   for(size_t p = 0; p < (mWidth * mHeight * 4); p ++)
   {
      mPixels[p] = (p % 4 == 0 && p > 0) ? 255 : 0;
   }
}

ImageBuffer::ImageBuffer(Ogre::uchar* Pixels, unsigned int width_height)
   : mPixels(Pixels), mWidth(width_height), mHeight(width_height)
{
/*
   mPixels = new Ogre::uchar[mWidth * mHeight * 4];

// set R=0, G=0, B=0, A=255
   for(size_t p = 0; p < (mWidth * mHeight * 4); p ++)
   {
      mPixels[p] = (p % 4 == 0 && p > 0) ? 255 : 0;
   }
*/
}

ImageBuffer::~ImageBuffer()
{
   delete mPixels;
}

void ImageBuffer::setPixel(size_t x, size_t y, Ogre::ColourValue colour)
{
   setPixel(x, y, colour.r, colour.g, colour.b, colour.a);
}

Ogre::ColourValue ImageBuffer::getPixel(size_t x, size_t y){
   float red = (float) mPixels[y * mWidth * 4 + x * 4 + 3];  //= (Ogre::uchar)(red * 255.0f);
   float green = (float) mPixels[y * mWidth * 4 + x * 4 + 2];// = (Ogre::uchar)(green * 255.0f);
   float blue = (float) mPixels[y * mWidth * 4 + x * 4 + 1]; // = (Ogre::uchar)(blue * 255.0f);
   float alpha = (float) mPixels[y * mWidth * 4 + x * 4 + 0];// = (Ogre::uchar)(alpha * 255.0f);
   return Ogre::ColourValue(red,green,blue,alpha);;
}

void ImageBuffer::setPixel(size_t x, size_t y, Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
   mPixels[y * mWidth * 4 + x * 4 + 3] = (Ogre::uchar)(red * 255.0f);
   mPixels[y * mWidth * 4 + x * 4 + 2] = (Ogre::uchar)(green * 255.0f);
   mPixels[y * mWidth * 4 + x * 4 + 1] = (Ogre::uchar)(blue * 255.0f);
   mPixels[y * mWidth * 4 + x * 4 + 0] = (Ogre::uchar)(alpha * 255.0f);
}

void ImageBuffer::saveImage(Ogre::String filename)
{
   Ogre::Image* image = new Ogre::Image();
   image->loadDynamicImage(mPixels, mWidth, mHeight, 1, Ogre::PF_R8G8B8A8);
   image->save(filename);
   delete image;
}

void FillColour::process()
{
   for(size_t y = 0; y < mBuffer->mHeight; y++)
   {
      for(size_t x = 0; x < mBuffer->mWidth; x++)
      {
         mBuffer->setPixel(x, y, mColour);
      }
   }
}

void FillColour::setPixl(size_t x, size_t y, Ogre::ColourValue color){
	mBuffer->setPixel(x,y, color);
}
#endif
