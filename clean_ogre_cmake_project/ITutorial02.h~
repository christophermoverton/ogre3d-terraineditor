/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __ITutorial02_h_
#define __ITutorial02_h_
#include <CEGUI/CEGUI.h>
//#include <CEGUISchemeManager.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include "BaseApplication.h"
#include <vector>
 
class ITutorial02 : public BaseApplication
{
enum QueryFlags
{
        NINJA_MASK = 1<<0,
        ROBOT_MASK = 1<<1
};
private:
    Ogre::TerrainGlobalOptions* mTerrainGlobals;
    Ogre::TerrainGroup* mTerrainGroup;
    bool mTerrainsImported;
    OgreBites::Label* mInfoLabel;
    CEGUI::Window *cMultiLineEditbox;
    Ogre::Vector3 rlterrVector;  //position vector for a raise/lower selection spot
    Ogre::Vector4 falloffcoeff;  //falloff coefficients for raise/lower terrain tool
    Ogre::Vector3 crlterrVectorts;
    std::vector<Ogre::Vector3> falloffSelection;
    double falloffwidth;
 
    void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
public:
    ITutorial02(void);
    virtual ~ITutorial02(void);
 
protected:
    virtual void createScene(void);
    //virtual void chooseSceneManager(void);
    virtual void createFrameListener(void);
    virtual void destroyScene(void);
    //frame listener    
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
    //mouse listener
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg,OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg,OIS::MouseButtonID id);
    virtual bool keyPressed(const OIS::KeyEvent& arg);
 
 
protected:
    Ogre::RaySceneQuery *mRaySceneQuery;// The ray scene query pointer
    bool mLMouseDown, mRMouseDown;		// True if the mouse buttons are down
    int mCount;							// The number of robots on the screen
    bool bRobotMode;        // The current state
    int selectMode;
    Ogre::SceneNode *mCurrentObject;	// The newly created object
    CEGUI::OgreRenderer* mGUIRenderer;
    //CEGUI::Renderer *mGUIRenderer;		// CEGUI renderer
    float mRotateSpeed;		
    Ogre::Rectangle2D* mMiniScreen; 
 
};
 
#endif // #ifndef __ITutorial02_h_
