# Introduction #

Important things you'll need to do get setup in terms of configuring Ogre3D for compilation on Linux.  Yes I've written this mostly around linux although you may find ways and means to compile in windows


# Details #

First you'll if you are using Linux I highly recommend my blog for a general reading on working with Ogre3d in addendum to following their forum guides for installation of Ogre on your system...my guide is written around linux here mostly here...

http://lotsofexpression.blogspot.com/2014/11/getting-started-with-ogre-experience.html

and in particular you'd need to follow the steps for installing cegui .
You don't have to configure any of the ogre files found in this repository, however, but you would need to ensure that the necessary cegui cmake files are installed, so that when you run cmake the compiler will know where to reference cegui (assuming you are using cmake for compilation...I've been using cmake for pre make compilation).

Here is the cegui installation procedure:

Cegui installation if using most recent mercurial clone,


When you use the mercurial clone call and build the source code libraries.  I also found that going to a newer fork of cegui library works (version .8x)  will present source library faulted on installation.
Thus for installation I used:


hg clone https://bitbucket.org/cegui/cegui
from terminal go to cegui library source root directory, then use the following command to update your source to the following version fork:
hg pull && hg update v0-8


Installation goes as follows:
Prerequisites:
Likely, as with any Ogre build dependencies that you'd have installed I recommend the following below additionally for cmake generation.
You may find an error on a typical cmake build with glew and glm libraries, just go to your distros app center and install the development glew and glm libraries, if these packages show missing.  Especially check for these libraries.  Also I installed
libboost-all-dev  package as well or at terminal
sudo apt-get install libboost-all-dev

while boost will show with the standard libboost package (used for ogre installation) cmake compilation will yield boost showing but not showing on some other things.  The libboost-all-dev package will include compilation of python bindings (which could technically be optional) but I believe owing to previous compilation build and installation issues some added packages to get cegui properly configured may be missing otherwise with the libboost as opposed to the libboost-all-dev package.


Thus installation as per cegui guide is typical or at terminal:
from terminal change directory to cegui source directory,
then
mkdir build
cd build
cmake ..


If everything looks good type in the same directory from terminal:


make
sudo make install

Otherwise build should be fine.

Next you'll need to install from
https://bitbucket.org/sumwars/sumwars-code/src/8526aee7d088/CMakeModules/

CompareVersionStrings.cmake, HandleLibraryTypes.cmake, DetermineVersion.cmake, FindCEGUIOGRE.cmake, FindCEGUI.cmake and FindPackageHandleAdvancedArgs.cmake

these will need to be placed into the directory
/usr/local/lib/OGRE/cmake/

## **I've also furnished all relevant CMake files in the CMakeModules folder of this program.  Please read the Read Me there for installation instructions (linux).** ##

at least for my linux distro

for
 wver1.1 branch**the following dependencies are needed.**

Secondly I am using for libfbm (fractional brownian motion) which auto correlates, for instance, 2 dimensional gaussian noise
http://www.fftw.org/download.html (Fast Fourier transform) libraries...for some reason I was having issues cloning and installing from git repositories so I installed downloaded latest official release given at the link above.

once decompressing the files into a given directory from terminal go to that install directory and you can use in terminal:

./configure
make
sudo make install

and this will install the fftw program into your systems directories.

FFTW libraries has a cmake file that I've furnished if you followed this instructions above you should be fine.

## **I've also furnished all relevant CMake files in the CMakeModules folder of this program.  Please read the Read Me there for installation instructions (linux).** ##

I've automatically adjusted the ogre apps main cmake files to use this cmake file.

Finally at the moment until I have extricated this dependency owing to a package you will need to install:
libglfw3-dev

or at console
sudo apt-get install libglfw3-dev
or find in Software center.