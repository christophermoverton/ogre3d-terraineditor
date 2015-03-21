#ifndef __FluidErosionDReg_CPP
#define __FluidErosionDReg_CPP
#include "TerrainFluidSimulation.cpp"
#include <CEGUI/CEGUI.h>
//#include <CEGUISchemeManager.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

class FluidErosionDReg{
	public:
		FluidErosionDReg(Ogre::Terrain* terrain, Ogre::Camera* Camera, Ogre::Rectangle2D* mMiniScreen);
		void updateB(const CEGUI::EventArgs &e);
	private:
		CEGUI::Window *cnewWindow;
		CEGUI::Window *UpdateButton;
		Ogre::Terrain* cterrain;
		Ogre::Camera* mCamera;
		Ogre::Rectangle2D* cmMiniScreen;
};

FluidErosionDReg::FluidErosionDReg(Ogre::Terrain* terrain, Ogre::Camera* Camera, Ogre::Rectangle2D* mMiniScreen){
    cterrain = terrain;
    mCamera = Camera;
    cmMiniScreen = mMiniScreen;
    cnewWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("FluidErosionWindow");
    UpdateButton = cnewWindow ->getChild("UpdateButton");
    UpdateButton->subscribeEvent(CEGUI::PushButton::EventClicked,
    CEGUI::Event::Subscriber(&FluidErosionDReg::updateB, this));
}

void FluidErosionDReg::updateB(const CEGUI::EventArgs &e){
    new TerrainFluidSimulation(cterrain, cmMiniScreen, false);
}
#endif
