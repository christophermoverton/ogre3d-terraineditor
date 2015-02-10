/****************************************************************************
    Copyright (C) 2012 Adrian Blumer (blumer.adrian@gmail.com)
    Copyright (C) 2012 Pascal Spörri (pascal.spoerri@gmail.com)
    Copyright (C) 2012 Sabina Schellenberg (sabina.schellenberg@gmail.com)

    All Rights Reserved.

    You may use, distribute and modify this code under the terms of the
    MIT license (http://opensource.org/licenses/MIT).
*****************************************************************************/

#ifndef TERRAINFLUIDSIMULATION_CPP
#define TERRAINFLUIDSIMULATION_CPP

#include "Simulation/FluidSimulation.h"
#include "SimulationState.h"
#include <memory>
//#include <chrono>
#include <iostream>
#include "platform_includes.h"
#include "TerrainStruct.h"
#include "Imagestuff.cpp"

using namespace glm;

class TerrainFluidSimulation
{
public:
    TerrainFluidSimulation(uint dim=513);
    TerrainFluidSimulation(Ogre::Terrain* mterrain, uint dim=513);
protected:

    /// Starts simulation main loop.
    void runMainloop();
    /// Advances physics by timestep dt (in milliseconds).
    void updatePhysics(double dt);
    void Run();
    void Stop();

protected:
    bool _finished;
    Ogre::Terrain * _mterrain;
    bool _rain;
    bool _flood;
    uint _dim;
    glm::vec2 _rainPos;

    SimulationState * _simulationState;
    Simulation::FluidSimulation * _simulation;
    //terr::CPointsMap *	        	     _shaderManager;
    terr::CPointsMap *		       _terrainHeightBuffer;
    terr::CPointsMap *                   _waterHeightBuffer;
    terr::CPointsMap *                     _gridCoordBuffer;
    //Graphics::IndexBuffer               _gridIndexBuffer;
    terr::CPointsMap *                      _sedimentBuffer;
    //terr::CPointsMap *                        _normalBuffer;

};

void TerrainFluidSimulation::Run()
{
    //init();
    runMainloop();
}

void TerrainFluidSimulation::Stop()
{
    _finished = true;
}

TerrainFluidSimulation::TerrainFluidSimulation(uint dim)
{
    _simulationState = new SimulationState(dim,dim);
    _simulation = new Simulation::FluidSimulation((*_simulationState));
    _rain = false;
    _rainPos = vec2(dim/2,dim/2);
    _flood = false;
    _dim = dim;
    _terrainHeightBuffer = new terr::CPointsMap();
    _waterHeightBuffer   = new terr::CPointsMap();
    _sedimentBuffer      = new terr::CPointsMap();
    Run();
}

TerrainFluidSimulation::TerrainFluidSimulation(Ogre::Terrain* mterrain, uint dim)
{
    _rain = false;
    _rainPos = vec2(dim/2,dim/2);
    _flood = false;
    _mterrain = mterrain;
    _dim = dim;
    _terrainHeightBuffer = new terr::CPointsMap();
    
    for (uint x = 0; x < _dim; x++){
	for (uint y = 0; y < _dim; y++){
		terr::Coordpair * coordpair = new terr::Coordpair(x,y);
		double heightpos = (double) mterrain->getHeightAtPoint(x,y);
		(*_terrainHeightBuffer)[(*coordpair)] = heightpos;

	}
    }
    /*
    if (abs(maxval) > abs(minval)){scalediv = abs(maxval);}
    else {scalediv = abs(minval);}

    for (uint x = 0; x < _dim; x++){
	for (uint y = 0; y < _dim; y++){
		terr::Coordpair * coordpair = new terr::Coordpair(x,y);
		//double heightpos = (double) mterrain->getHeightAtPoint(x,y);
		(*_terrainHeightBuffer)[(*coordpair)] /= scalediv;
	}
    }
    //*/

    _simulationState = new SimulationState(dim,dim, _terrainHeightBuffer);
    _simulation = new Simulation::FluidSimulation((*_simulationState));
   
    _waterHeightBuffer   = new terr::CPointsMap();
    _sedimentBuffer      = new terr::CPointsMap();
    Run();
}


void TerrainFluidSimulation::updatePhysics(double dt)
{
    // Run simulation
    _simulation->update(dt,_rain,_flood);
// Copy data to GPU
    ImageBuffer buffer((double)_dim);
    FillColour* fill = new FillColour (&buffer);
    double maxval = 0.0f, minval = 10000000.0f, scalediv;
    for (int i = 0; i < _dim; i++){
    	for (int j = 0; j < _dim; j++){
		terr::Coordpair * coordpair = new terr::Coordpair(i,j);	
		double colval = _simulationState->terrain(i,j);
		(*_terrainHeightBuffer)[(*coordpair)] = _simulationState->terrain(i,j);
		(*_waterHeightBuffer)[(*coordpair)] = _simulationState->water(i,j);
		(*_sedimentBuffer)[(*coordpair)] = _simulationState->suspendedSediment(i,j);

	        if (colval > maxval){maxval = colval;}
		if (colval < minval) {minval = colval;}
		_mterrain->setHeightAtPoint((long) i, (long) j, colval);
	}
    }

    if (abs(maxval) > abs(minval)){scalediv = abs(maxval);}
    else {scalediv = abs(minval);}

    for (uint x = 0; x < _dim; x++){
	for (uint y = 0; y < _dim; y++){
		terr::Coordpair * coordpair = new terr::Coordpair(x,y);
		//double heightpos = (double) mterrain->getHeightAtPoint(x,y);
		double colval = (*_terrainHeightBuffer)[(*coordpair)]/scalediv;
		Ogre::ColourValue col = Ogre::ColourValue(colval,colval,colval);
		fill->setPixl((size_t)x, (size_t)y, col);
		//(*_terrainHeightBuffer)[(*coordpair)] /= scalediv;
	}
    }
    buffer.saveImage("test8.png");
    //_terrainHeightBuffer.SetData(_simulationState.terrain);
    //_waterHeightBuffer.SetData(_simulationState.water);
    //_sedimentBuffer.SetData(_simulationState.suspendedSediment);
    //_normalBuffer.SetData(_simulationState.surfaceNormals);
}

void TerrainFluidSimulation::runMainloop()
{
    //using namespace std::chrono;
    // Settings
    double dt = 1000.0; //60.0f; // 60 fps physics simulation
    // Setup
    //high_resolution_clock clock;
    //high_resolution_clock::time_point currentTime, newTime;
    //high_resolution_clock::duration frameTime;
    //high_resolution_clock::duration accumulator(0);

    // count number of simulation steps
    ulong counterSim = 0;

    _finished = false;
    //currentTime = clock.now();
    updatePhysics(dt);

}


/*
void TerrainFluidSimulation::init()
{
    uint dimX = _simulationState.terrain.width();
    uint dimY = _simulationState.terrain.height();

    for (int i = 0; i < dimX;i++){
	for (int j = 0 ; j < dimY; j++){
	}
    }
}
*/
#endif
