/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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

 
#include "ITutorial02.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
//#include "TextRenderer.cpp"
#include "Terrainheightselection.cpp"
#include "cspline.cpp"
#include "Falloffinterpolate.cpp"
#include "PlaneIntersection.cpp"
//#include "Imagestuff.cpp"
#include "PerlinTest.cpp"
#include "LoadHeightMap.cpp"
#include "FreqAmpOctEventReg.cpp"
#include "BuildVoronoi.cpp"
#include "BuildSimplex.cpp"
 
//-------------------------------------------------------------------------------------
ITutorial02::ITutorial02(void)

    :   mTerrainGlobals(0),
    mTerrainGroup(0),
    mTerrainsImported(false),
    mInfoLabel(0)
{
// Set the default state
bRobotMode = true;
selectMode = 0;
falloffwidth = 20.0;
}
//-------------------------------------------------------------------------------------
ITutorial02::~ITutorial02(void)
{
}
void ITutorial02::destroyScene(void)
{
    OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainGlobals; 
}
void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
    img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (flipX)
        img.flipAroundY();
    if (flipY)
        img.flipAroundX();
  
}
//-------------------------------------------------------------------------------------
void ITutorial02::defineTerrain(long x, long y)
{
    Ogre::String filename = mTerrainGroup->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
    {
        mTerrainGroup->defineTerrain(x, y);
    }
    else
    {
        Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        mTerrainGroup->defineTerrain(x, y, &img);
	//mTerrainGroup->defineTerrain(x, y, 0.0f);
        mTerrainsImported = true;
    } 
}
//-------------------------------------------------------------------------------------
void ITutorial02::initBlendMaps(Ogre::Terrain* terrain)
{
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;
 
            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;
 
            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update(); 
}
//-------------------------------------------------------------------------------------
void ITutorial02::configureTerrainDefaults(Ogre::Light* light)
{
    // Configure global
    mTerrainGlobals->setMaxPixelError(8);
    // testing composite map
    mTerrainGlobals->setCompositeMapDistance(3000);
 
    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
 
    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = 513;
    defaultimp.worldSize = 12000.0f;
    defaultimp.inputScale = 600;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;
    // textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds"); 
} 
//-------------------------------------------------------------------------------------
/*
string bool_cast(const bool b) {
    ostringstream ss;
    ss << boolalpha << b;
    return ss.str();
}
*/
void ITutorial02::createScene(void)
{
    mCamera->setPosition(Ogre::Vector3(1683, 50, 2116));
    mCamera->lookAt(Ogre::Vector3(1963, 50, 1660));
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(50000);
 
    if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }
    mGUIRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

// Mouse
    //CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    //CEGUI::MouseCursor::getSingleton().setImage("TaharezLook", "MouseArrow");


    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
    CEGUI::Window* myRoot = CEGUI::WindowManager::getSingleton().createWindow( "DefaultWindow", "_MasterRoot" );
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow( myRoot );
    //CEGUI::Window *myImageWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage","PrettyWindow" );
   // myImageWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,0),CEGUI::UDim(0.5,0)));
    //myImageWindow->setSize(CEGUI::USize(CEGUI::UDim(0,150),CEGUI::UDim(0,100)));
    //myImageWindow->setProperty("Image","TaharezLook/full_image");
    //myImageWindow->setText("Hello World!");
    //CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(myImageWindow);
    CEGUI::Window *newWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("TreeDemoTaharez2.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(newWindow);
    newWindow ->setAlpha(.5);
    cMultiLineEditbox = newWindow ->getChild("MultiLineEditbox");
    cMultiLineEditbox ->setText("Hello World!");
    CEGUI::Window *editbox = newWindow ->getChild("Editbox");
    editbox ->setText("Hello World!");
    
    
// Play with startup Texture Filtering options
// Note: Pressing T on runtime will discarde those settings
//  Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
//  Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);
 
    Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();
 
    Ogre::Light* light = mSceneMgr->createLight("tstLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
 
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
 
    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
 
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f); //513  12000.0f default
    mTerrainGroup->setFilenameConvention(Ogre::String("BasicTutorial3Terrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
 
    configureTerrainDefaults(light);
 
    for (long x = 0; x <= 0; ++x)
        for (long y = 0; y <= 0; ++y)
            defineTerrain(x, y);
 
    // sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);
 
    if (mTerrainsImported)
    {
        Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }
 
    mTerrainGroup->freeTemporaryResources();
 
    Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);
    mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 10, 1200);
    mWindow->getViewport(0)->setBackgroundColour(fadeColour);
 
    Ogre::Plane plane;
    plane.d = 100;
    plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
 
    //mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8, 500);
    mSceneMgr->setSkyPlane(true, plane, "Examples/CloudyNoonSkyBox", 500, 20, true, 0.5, 150, 150);

    // adding miniscreen lower right corner
    mMiniScreen = new Ogre::Rectangle2D(true);
    mMiniScreen->setCorners(0.5f, -0.25f, 1.0f, -1.0f);
    mMiniScreen->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);

    Ogre::SceneNode* miniScreenNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode");
    miniScreenNode->attachObject(mMiniScreen);
    // done adding miniscreen

    //add material to miniscreen
/*
    Ogre::MaterialPtr renderMaterial = Ogre::MaterialManager::getSingleton().create("RttMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    Ogre::TextureUnitState* lTextureUnit = renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState();
    lTextureUnit->setTextureName("test6.png", Ogre::TEX_TYPE_2D);
    lTextureUnit->setTextureCoordSet(0);
    mMiniScreen->setMaterial("RttMat");
*/
    Ogre::Image imageOgre;
    imageOgre.load("test6.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    Ogre::TexturePtr m_texture;

                // Texture creation 1
    m_texture = Ogre::TextureManager::getSingletonPtr()->createManual(
                "maTexture",
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                Ogre::TEX_TYPE_2D,
                imageOgre.getWidth(),
                imageOgre.getHeight(),
                0,
                Ogre::PF_R8G8B8A8, Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
    Ogre::MaterialPtr renderMaterial = Ogre::MaterialManager::getSingleton().create("RttMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	
        renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	m_texture->loadImage(imageOgre);
        renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("maTexture");
        //lTextureUnit->setTextureName("test6.png", Ogre::TEX_TYPE_2D);
        //lTextureUnit->setTextureCoordSet(0);
        mMiniScreen->setMaterial("RttMat");
    //lTextureUnit->addListener(this);
    // finished material add

    Ogre::Log* tlog = Ogre::LogManager::getSingleton().createLog("test.log");
    tlog->logMessage("blahblah");
    std::vector<double> twopoints(2);
    twopoints[0] = 0; 
    twopoints[1] = 1;
    cspline* cs = new cspline(twopoints,twopoints);
    double tan1 = (double)Ogre::Math::Tan(Ogre::Math::DegreesToRadians(30));
    falloffcoeff = cs->compute2nodecspline (tan1, tan1);
    //Ogre::Vector3 trheight = planeint->getHeight();
    std::ostringstream ss5;
    Ogre::Terrain* cterrain = mTerrainGroup->getTerrain(0,0);
    FreqAmpOctEventReg* reg = new FreqAmpOctEventReg(cterrain, mCamera,  mMiniScreen); //registering event handlers
    ss5<<"Terrain Size: "<< cterrain->getSize()<<"\n";
    ss5<<"Maximum Height: "<< cterrain->getMaxHeight()<<"\n";
    ss5<<"World Size: " << cterrain->getWorldSize()<<"\n";
    for (int i=0; i<250; i++ ){
	for (int j=0; j<250; j++){
	   ss5 << "Terrain position("<<i<<","<<j<<")"<<cterrain->getHeightAtPoint(i,j)<<"\n" ;
	   cterrain->setHeightAtPoint(i,j,cterrain->getHeightAtPoint(i,j)+100);
	}
    }
    ss5 <<"Get terrain position (30.5,30.5): "<<cterrain->getHeightAtPoint(30.5,30.5)<<"\n";
    ss5 <<"Get terrain position (30,30): "<<cterrain->getHeightAtPoint(30,30)<<"\n";
    ss5 <<"Get terrain position (31,31): "<<cterrain->getHeightAtPoint(31,31)<<"\n";
    Ogre::Vector3 wsvec;
    Ogre::Vector3 tsvec;
    cterrain->getPoint(30,30,&wsvec);
    cterrain->getTerrainPosition(wsvec,&tsvec);
    
    ss5 <<"Terrain world space coordinate for (30,30): " << wsvec << "\n";
    ss5 <<"Terrain local space coordinate for (30,30): " << Ogre::Vector3(513.0*tsvec.x,513.0*tsvec.y,tsvec.z) << "\n";
    ss5 <<"Falloff coefficients: "<< falloffcoeff<<"\n";
    cterrain->update();
    tlog->logMessage(ss5.str());
    ss5.str(std::string());
    Terrainheightselection* a = new Terrainheightselection(mTerrainGroup, Ogre::Vector3(100,100,100),
							   20.0);
    //ImageBuffer buffer(256);
    //FillColour (&buffer).setColour(Ogre::ColourValue(0.0f, 1.0f, 1.0f)).process();
    //buffer.saveImage("test1.png");
    //PerlinTest* test = new PerlinTest(1026.0f,20.0f, 2.0f,0.001949318f, .90f, 7.0f, (int)3.0f); //keep by default third arg 2.0f..don't tweak won't work for higher or lower vals
    //new LoadHeightMap(cterrain, Ogre::String("test3.png"), 3.0f);
    //new BuildVoronoi();
    new BuildSimplex();
    //CEGUI::Window *newWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("DemoWindow");
    //cMultiLineEditbox = newWindow ->getChild("MultiLineEditbox");
}
void ITutorial02::createFrameListener(void)
{
    BaseApplication::createFrameListener();
 
    mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);
// Setup default variables
    mCount = 0;
    mCurrentObject = NULL;
    mLMouseDown = false;
    mRMouseDown = false;
 
         // Reduce rotate speed
    mRotateSpeed =.1;
// Create RaySceneQuery
    mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());

// We created the query, and we are also responsible for deleting it.
    mSceneMgr->destroyQuery(mRaySceneQuery);
}
/*
void ITutorial02::chooseSceneManager(void)
{
	// Use the terrain scene manager.
	//mSceneMgr = mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
}
*/
bool ITutorial02::frameRenderingQueued(const Ogre::FrameEvent &evt)
{    
    bool ret = BaseApplication::frameRenderingQueued(evt);
// Process the base frame listener code.  Since we are going to be
         // manipulating the translate vector, we need this to happen first.
    if (!BaseApplication::frameRenderingQueued(evt))
        return false;
///*
// Setup the scene query
        Ogre::Vector3 camPos = mCamera->getPosition();
        Ogre::Ray cameraRay(Ogre::Vector3(camPos.x, 5000.0f, camPos.z), Ogre::Vector3::NEGATIVE_UNIT_Y);
        float theight = mTerrainGroup->getHeightAtWorldPosition (camPos); 	
// Create RaySceneQuery
        mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
        //mRaySceneQuery ->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION );
        mRaySceneQuery->setRay(cameraRay);
// Perform the scene query
        Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
        Ogre::RaySceneQueryResult::iterator itr = result.begin();
// Get the results, set the camera height
///*
         std::ostringstream ss, ss2;
         bool val;
         bool val2;
         float x1,x2;
         x1 = itr ->distance;
         ss << "Camera Height: " << std::showpoint << camPos.y << "\n";
         ss << "Terrain Height: " << std::showpoint << theight << "\n";
         //val = 1 == 5;
         //val = (itr == result.end()); 
         //ss << std::boolalpha << val ;
         //val2 = (itr -> worldFragment); 
         //ss2 << std::boolalpha << val2 ;
/*
	 CEGUI::Window *newWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("DemoWindow");
         CEGUI::Window *MultiLineEditbox = newWindow ->getChild("MultiLineEditbox");
         //MultiLineEditbox ->setText("Hello World!");
         
	MultiLineEditbox ->setText(ss.str());
        // MultiLineEditbox ->appendText("\n");
        // MultiLineEditbox ->appendText(ss2.str());
        for (itr = result.begin(); itr != result.end(); itr++) {
           std::ostringstream ss, ss2, ss3;
           x1 = itr ->distance;
           ss << std::showpoint << x1;
           ss2 << itr->movable->getName();
           //ss2 << "Test";
           ss3 << std::boolalpha << itr ->movable->isAttached();
           MultiLineEditbox ->appendText("\n");
           MultiLineEditbox ->appendText(ss.str());
           //MultiLineEditbox ->appendText("\n");
           //MultiLineEditbox ->appendText(ss2.str());
           MultiLineEditbox ->appendText("\n");
           MultiLineEditbox ->appendText(ss3.str());
        //if (itr→movable→getName().compare(“Mech”)!=0 && itr→distance<mMechLength/2) {

          //  return true;

        }
*/
//         if (itr != result.end() && itr->worldFragment)
        if (camPos.y < theight + 10.0f)
         {
	 //    Ogre::Real terrainHeight = itr->worldFragment->singleIntersection.y;
         //    if ((terrainHeight + 10.0f) < camPos.y)
                 mCamera->setPosition( camPos.x, theight+10.0f, camPos.z );
         }
//*/
//*/ 
/*
    if (mTerrainGroup->isDerivedDataUpdateInProgress())
    {
        mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
        mInfoLabel->show();
        if (mTerrainsImported)
        {
            mInfoLabel->setCaption("Building terrain, please wait...");
        }
        else
        {
            mInfoLabel->setCaption("Updating textures, patience...");
        }
    }
    else
    {
        mTrayMgr->removeWidgetFromTray(mInfoLabel);
        mInfoLabel->hide();
        if (mTerrainsImported)
        {
            mTerrainGroup->saveAllTerrains(true);
            mTerrainsImported = false;
        }
    }
*/

 
    return ret;
}
bool ITutorial02::mouseMoved(const OIS::MouseEvent &arg)
{
     CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
// If we are dragging the left mouse button.
        if (mLMouseDown)
        {
            if(mCurrentObject)
            {
                mCurrentObject->showBoundingBox(false);
            }
            CEGUI::Vector2<float> mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
            Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.d_x/float(arg.state.width), mousePos.d_y/float(arg.state.height));
            //mRaySceneQuery->setRay(mouseRay);

            //Ogre::Vector3 ovec = mouseRay.getOrigin();
            //I believe this next function always clamps to a position on the terrain even if the viewport
            //mouse ray origin has no clear corresponding points bounded over the terrain.
            //float theight = mTerrainGroup->getHeightAtWorldPosition (ovec);

            mRaySceneQuery->setRay(mouseRay);
            mRaySceneQuery->setSortByDistance(true);
            mRaySceneQuery->setQueryMask(bRobotMode ? ROBOT_MASK : NINJA_MASK);
            // Execute query
            Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
            Ogre::RaySceneQueryResult::iterator itr = result.begin();
            Ogre::Vector3 ovec = mouseRay.getOrigin();
            //I believe this next function always clamps to a position on the terrain even if the viewport
            //mouse ray origin has no clear corresponding points bounded over the terrain.
            float theight = mTerrainGroup->getHeightAtWorldPosition (ovec);
	 CEGUI::Window *newWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("DemoWindow");
         CEGUI::Window *MultiLineEditbox = newWindow ->getChild("MultiLineEditbox");
         //MultiLineEditbox ->setText("Hello World!");
         
	//MultiLineEditbox ->setText(ss.str());
        // MultiLineEditbox ->appendText("\n");
        // MultiLineEditbox ->appendText(ss2.str());
	std::ostringstream ss4;
	if (selectMode == 1){
		ss4 << "Tool: Select Mode\n";
	}
	else if (selectMode == 0){
		ss4 << "Tool: Add Object\n";
	}
	MultiLineEditbox ->setText(ss4.str());
        for (itr; itr != result.end(); itr++) {
           std::ostringstream ss, ss2, ss3;
           float x1 = itr ->distance;
           ss <<"Distance: "<< std::showpoint << x1 << "\n";
           ss2 <<"Name: "<< itr->movable->getName() << "\n";
           //ss2 << "Test";
           //ss3 << std::boolalpha << itr ->movable->isAttached();
	   ss3 << "Direction: "<<mouseRay.getDirection()<<"\n";
           //MultiLineEditbox ->appendText("\n");
           MultiLineEditbox ->appendText(ss.str());
           //MultiLineEditbox ->appendText("\n");
           MultiLineEditbox ->appendText(ss2.str());
           //MultiLineEditbox ->appendText("\n");
           MultiLineEditbox ->appendText(ss3.str());
        //if (itr→movable→getName().compare(“Mech”)!=0
           if (itr->movable && itr->movable->getName() != "")
           {
                //mCurrentObject = itr->movable->getParentSceneNode();
                break;
           } // if
        }
        if (selectMode==1){
            for(itr = result.begin(); itr != result.end(); itr++){
                if (itr->movable && itr->movable->getName() != "")
                {
                mCurrentObject = itr->movable->getParentSceneNode();
                break;
                } // if
            }
        }

	else if (selectMode == 2){

		PlaneIntersection* planeint = new PlaneIntersection(rlterrVector, mouseRay);
		if (planeint->getTest()){
			Ogre::Vector3 trheight = planeint->getHeight();
			Ogre::Terrain* cterrain = mTerrainGroup->getTerrain(0,0);
			cterrain->dirty();
			Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("test.log");
			std::ostringstream ss5;
			Falloffinterpolate fint = Falloffinterpolate(falloffSelection, crlterrVectorts, falloffwidth, falloffcoeff, (double) trheight.y);
				
			std::vector<Ogre::Vector3> newpos1s = fint.getIpoints();
			for (int i = 0;i < falloffSelection.size();i++){
				Ogre::Vector3 pos1 = falloffSelection[i];
				//ss5<<"Pre Interpolated position: "<< pos1<<"\n";
		
//				Falloffinterpolate fint = Falloffinterpolate(pos1, rlterrVector, 20.0, falloffcoeff, (double) trheight.y);
				
//				Ogre::Vector3 newpos1 = fint.getIpoint();
				//ss5<<"Interpolated position: "<< newpos1s[i] <<"\n";
				Ogre::Vector3 outvec(0,0,0);
				Ogre::Vector3* outvecp = &outvec;
				//const Ogre::Vector3 invec = Ogre::Vector3(pos1.x, newpos1s[i],pos1.z);
				//const Ogre::Vector3& invecp = &invec;
				//cterrain->getTerrainPosition(invec,&outvec);
				cterrain->setHeightAtPoint (newpos1s[i].x, newpos1s[i].z, newpos1s[i].y);
			}
			ss5<<"Translate Height: "<< trheight.y<<"\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			cterrain->update();
			mTerrainGroup->update();
		//Falloffinterpolate(rlterrVector, rlterrVector, 20.0, falloffcoeff, 0.0);
		//Ogre::Vector3 getIpoint();			
		}
	}
        else
        {
            for (int i = 0; i < 4000 ; i ++){
               Ogre::Vector3 ovec1 = mouseRay.getPoint(i);
	       float theight1 = mTerrainGroup->getHeightAtWorldPosition (ovec1);
               if (theight1 > ovec1.y){
                   ovec = ovec1;
                   theight = theight1;
                   break;
               }
            }
            mCurrentObject->setPosition(Ogre::Vector3(ovec.x,theight,ovec.z));
        }
            if(mCurrentObject)
            {
                mCurrentObject->showBoundingBox(true);
            }
        } // if
 
        // If we are dragging the right mouse button.
        else if (mRMouseDown)
        {
            mCamera->yaw(Ogre::Degree(-arg.state.X.rel * mRotateSpeed));
            mCamera->pitch(Ogre::Degree(-arg.state.Y.rel * mRotateSpeed));
        } // else if
     return true;
}

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
 
    default:
        return CEGUI::LeftButton;
    }
}

bool ITutorial02::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
// Left mouse button down
        if (id == OIS::MB_Left)
        {
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
	CEGUI::Window *newWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("DemoWindow");
	
	CEGUI::Vector2<float> mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
	if (newWindow->isHit(mousePos)){ 
	return true;
	}
	//float posx = mousePos.d_x/float(arg.state.width);
	//float posy = mousePos.d_y/float(arg.state.height);
	//float posx = arg.state.X.rel;
        //float posy = arg.state.Y.rel;
//show that the current object has been deselected by removing the bounding box visual
        if(mCurrentObject)
        {
	    mCurrentObject->showBoundingBox(false);
        }
        // Create RaySceneQuery
        mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
                // Setup the ray scene query, use CEGUI's mouse position
            //CEGUI::Vector2<float> mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
            Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.d_x/float(arg.state.width), mousePos.d_y/float(arg.state.height));
            mRaySceneQuery->setRay(mouseRay);
            mRaySceneQuery->setSortByDistance(true);
            mRaySceneQuery->setQueryMask(bRobotMode ? ROBOT_MASK : NINJA_MASK);
            // Execute query
            Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
            Ogre::RaySceneQueryResult::iterator itr = result.begin();
            Ogre::Vector3 ovec = mouseRay.getOrigin();
            //I believe this next function always clamps to a position on the terrain even if the viewport
            //mouse ray origin has no clear corresponding points bounded over the terrain.
            float theight = mTerrainGroup->getHeightAtWorldPosition (ovec);
//	 CEGUI::Window *newWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("DemoWindow");
         CEGUI::Window *MultiLineEditbox = newWindow ->getChild("MultiLineEditbox");
         //MultiLineEditbox ->setText("Hello World!");
         
	//MultiLineEditbox ->setText(ss.str());
        // MultiLineEditbox ->appendText("\n");
        // MultiLineEditbox ->appendText(ss2.str());
        for (itr; itr != result.end(); itr++) {
           std::ostringstream ss, ss2, ss3;
           float x1 = itr ->distance;
           ss <<"Distance: "<< std::showpoint << x1 << "\n";
           ss2 <<"Name: "<< itr->movable->getName() << "\n";
           //ss2 << "Test";
           //ss3 << std::boolalpha << itr ->movable->isAttached();
	   ss3 << "Direction: "<<mouseRay.getDirection()<<"\n";
           //MultiLineEditbox ->appendText("\n");
           MultiLineEditbox ->setText(ss.str());
           //MultiLineEditbox ->appendText("\n");
           MultiLineEditbox ->appendText(ss2.str());
           //MultiLineEditbox ->appendText("\n");
           MultiLineEditbox ->appendText(ss3.str());
        //if (itr→movable→getName().compare(“Mech”)!=0
        }
        if (selectMode==1){
            for(itr = result.begin(); itr != result.end(); itr++){
                if (itr->movable && itr->movable->getName() != "")
                {
                mCurrentObject = itr->movable->getParentSceneNode();
                break;
                } // if
            }
        }

	else if (selectMode == 2){
                for (int i = 0; i < 4000 ; i ++){
                   Ogre::Vector3 ovec1 = mouseRay.getPoint(i);
	           float theight1 = mTerrainGroup->getHeightAtWorldPosition (ovec1);
                   if (theight1 > ovec1.y){
                       ovec = ovec1;
                       theight = theight1;
                       break;
                   }
                }
		rlterrVector = Ogre::Vector3(ovec.x,theight,ovec.z);
		//conver rlterrVector to terrainspace vector
		Ogre::Vector3 rlterrVectorts;
		Ogre::Terrain* cterrain = mTerrainGroup->getTerrain(0,0);
		cterrain->getTerrainPosition(rlterrVector,&rlterrVectorts);
		//terrain space coordinate stored like world space
		crlterrVectorts = Ogre::Vector3(513*rlterrVectorts.x,rlterrVectorts.z,513*rlterrVectorts.y); 
		Terrainheightselection* a = new Terrainheightselection(mTerrainGroup, crlterrVectorts,
							   falloffwidth);
		falloffSelection = a->getSelectionVerts();
		//Falloffinterpolate(rlterrVector, rlterrVector, 20.0, falloffcoeff, 0.0);
		//Ogre::Vector3 getIpoint();
		
	}
        else{   
                for (int i = 0; i < 4000 ; i ++){
                   Ogre::Vector3 ovec1 = mouseRay.getPoint(i);
	           float theight1 = mTerrainGroup->getHeightAtWorldPosition (ovec1);
                   if (theight1 > ovec1.y){
                       ovec = ovec1;
                       theight = theight1;
                       break;
                   }
                }
                Ogre::Entity* ent;
                char name[16];
 
                if (bRobotMode)
                {
                    sprintf(name, "Robot%d", mCount++);
                    ent = mSceneMgr->createEntity(name, "robot.mesh");
                    ent->setQueryFlags(ROBOT_MASK);
                } // if
                else
                {
                    sprintf(name, "Ninja%d", mCount++);
                    ent = mSceneMgr->createEntity(name, "ninja.mesh");
                    ent->setQueryFlags(NINJA_MASK);
                } // else
                mCurrentObject = mSceneMgr->getRootSceneNode()->createChildSceneNode(std::string(name) + "Node", Ogre::Vector3(ovec.x,theight,ovec.z));
                mCurrentObject->attachObject(ent);
            } //end else on selectMode
            //mCurrentObject->setScale(0.1f, 0.1f, 0.1f);
//*/
            mLMouseDown = true;
            //now we show the bounding box so the user can see that this object is selected
            if(mCurrentObject)
            {
                mCurrentObject->showBoundingBox(true);
            }
        } // if
 
        // Right mouse button down
        else if (id == OIS::MB_Right)
        {
            //CEGUI::MouseCursor::getSingleton().getDefaultGUIContext().hide();
            mRMouseDown = true;
        } // else if


return true;
}
bool ITutorial02::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
// Left mouse button up
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
        if (id == OIS::MB_Left)
        {
            mLMouseDown = false;
        } // if
 
        // Right mouse button up
        else if (id == OIS::MB_Right)
        {
            //CEGUI::MouseCursor::getSingleton().getDefaultGUIContext().show();
            mRMouseDown = false;
        } // else if
return true;
}

bool ITutorial02::keyPressed(const OIS::KeyEvent& arg)
{
	//check and see if the spacebar was hit, and this will switch which mesh is spawned
	if(arg.key == OIS::KC_SPACE)
	{
		bRobotMode = !bRobotMode;
	}
	else if (arg.key == OIS::KC_F1){
		std::ostringstream ss4;
		falloffwidth += 10;
		ss4 << "Falloff width: " << falloffwidth << "\n";
		cMultiLineEditbox ->setText(ss4.str());
	}
	else if (arg.key == OIS::KC_F2){
		std::ostringstream ss4;
		falloffwidth -= 10;
		ss4 << "Falloff width: " << falloffwidth << "\n";
		cMultiLineEditbox ->setText(ss4.str());
	}
        else if(arg.key == OIS::KC_C){
            //swap(mPrimarySceneMgr, mSecondarySceneMgr);
            if (selectMode==1)
               selectMode = 2;
	    else if (selectMode == 2){
	       selectMode = 0;
	    }
            else 
               selectMode = 1;
	    std::ostringstream ss4;
	    if (selectMode == 1){
		ss4 << "Tool: Select Mode\n";
		}
	    else if (selectMode == 0){
		ss4 << "Tool: Add Object\n";
	    }
	    else if (selectMode == 2){
		ss4 << "Tool: Raise/Lower Terrain Tool\n";
	    }
	    cMultiLineEditbox ->setText(ss4.str());
/* 
    if (mDual)
	dualViewport(mPrimarySceneMgr, mSecondarySceneMgr);
    else
	setupViewport(mPrimarySceneMgr);
        
*/
        } 

	//then we return the base app keyPressed function so that we get all of the functionality
	//and the return value in one line
	return BaseApplication::keyPressed(arg);
}
 
 
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        ITutorial02 app;
 
        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
 
        return 0;
    }
 
#ifdef __cplusplus
}
#endif
