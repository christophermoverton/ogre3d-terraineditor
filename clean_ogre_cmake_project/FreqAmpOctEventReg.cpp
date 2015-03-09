#ifndef __FreqAmpOctEventReg_CPP
#define __FreqAmpOctEventReg_CPP

#include "PerlinTest.cpp"
#include "BuildVoronoi.cpp"
#include "BuildSimplex.cpp"
#include "BuildFbm.cpp"
#include "DiamondSquare.cpp"
#include "Musgrave.cpp"
#include "BuildNoise.cpp"
#include <iostream>
#include <string>
#include <sstream>
#include <CEGUI/CEGUI.h>
//#include <CEGUISchemeManager.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/RendererModules/Ogre/Texture.h>
//#include <OgreTexture.h>
#include "BaseApplication.h"
#include "LoadHeightMap.cpp"
#include "ScaleHeights.cpp"
#include "TranslateHeights.cpp"
#include "TerrainTexturesNode.cpp" 
#include "TerrainTexturesSettings.cpp" 
#include "Combiner.cpp"
#include "Voronoi.cpp"
#include <map>
#include <string>
#include <iostream>

typedef std::map<std::string, CEGUI::Window *> EditboxMap;
typedef std::map<std::string, float> ConfSets;
typedef std::map<int,ConfSets> Configs;
typedef std::map<std::string, Configs> TextureConfigs;
using namespace std;
//The following are interchangable configurations...Depth/H, Frequency/Lacunarity
class FreqAmpOctEventReg{
	public:
		FreqAmpOctEventReg(Ogre::Terrain* terrain, Ogre::Camera* Camera, Ogre::Rectangle2D* mMiniScreen);
		void updateF(const CEGUI::EventArgs &e);  //Frequency
		void updateFE(const CEGUI::EventArgs &e);  //Frequency Editbox
		void updateA(const CEGUI::EventArgs &e);   //Amplitude
		void updateAE(const CEGUI::EventArgs &e);  //Amplitude Editbox
		void updateO(const CEGUI::EventArgs &e);   //Octaves
		void updateOE(const CEGUI::EventArgs &e);  //Octaves Editbox
		void updateB(const CEGUI::EventArgs &e);   //Update Button 1rst
		void updateG(const CEGUI::EventArgs &e);   //Gain Button
		void updateGE(const CEGUI::EventArgs &e);  //Gain Button Editbox
		void updateS(const CEGUI::EventArgs &e);   //Scales
		void updateSE(const CEGUI::EventArgs &e);  //Scales Editbox
		void updateM(const CEGUI::EventArgs &e);
		void updateNTB(const CEGUI::EventArgs &e);
		void updateRLT(const CEGUI::EventArgs &e);  //rlt slider
		void updateBB(const CEGUI::EventArgs &e);
		void updateBBB(const CEGUI::EventArgs &e);  //update button 3
		void updateT(const CEGUI::EventArgs &e);
		void updateTD(const CEGUI::EventArgs &e);
		void updateTES(const CEGUI::EventArgs &e);  //Update Texture Size Slider
		void updateTEB(const CEGUI::EventArgs &e);  //Update Texture Size Edit Box
		void updateNS(const CEGUI::EventArgs &e);  //Update Normal Slider
		void updateNB(const CEGUI::EventArgs &e);  //Update Normal Edit Box
		void updateWS(const CEGUI::EventArgs &e);  //Update World Size Slider
		void updateWB(const CEGUI::EventArgs &e);  //Update World Size Edit Box
		void updateW(const CEGUI::EventArgs &e);  //Weight Slider
		void updateN(const CEGUI::EventArgs &e);
		void updateFD(const CEGUI::EventArgs &e);
		void updateD(const CEGUI::EventArgs &e);
		void updateDE(const CEGUI::EventArgs &e);
		void updateMHS(const CEGUI::EventArgs &e);  //mountain height slider
		void updateMHE(const CEGUI::EventArgs &e);  //mountain height edit box
		void updateWHS(const CEGUI::EventArgs &e);  //Water height slider
		void updateWHE(const CEGUI::EventArgs &e);  //Water height edit box
		void updateTILB(const CEGUI::EventArgs &e);  //TextureItemListBox
	private:
		CEGUI::Window *cnewWindow;
		CEGUI::Window *tcnewWindow;
		CEGUI::Window *DefaultWindow;
		CEGUI::Window *TabControlWindow;
		CEGUI::Window *HydraulicWindow;
		CEGUI::Window *MarbleWindow;
		CEGUI::Window *TerrainSettingsWindow;
		CEGUI::Window *ShowDataWindow;
		CEGUI::Window *TerrainTexturesWindow;
		CEGUI::Window *Frequencyslider;
		CEGUI::Window *FrequencyEditbox;
		CEGUI::Window *Amplitudeslider;
                CEGUI::Window *AmpEditbox;
		CEGUI::Window *Octavesslider;
		CEGUI::Window *OctavesEditbox;
		CEGUI::Window *Gainslider;
		CEGUI::Window *GainEditbox;
		CEGUI::Window *Scaleslider;
		CEGUI::Window *ScaleEditbox;
		CEGUI::Window *Freqdivslider;
		CEGUI::Window *Maxheightslider;
		CEGUI::Window *MountainHslider;
		CEGUI::Window *MountainHEditbox;
		CEGUI::Window *RLTslider;
		CEGUI::Window *UpdateButton;
		CEGUI::Window *UpdateButton2;
		CEGUI::Window *UpdateButton3;
		CEGUI::Window *Terrainbox;
		CEGUI::Window *FilterTypebox;
		CEGUI::Window *Weightslider;
		CEGUI::Window *WaterHslider;
		CEGUI::Window *WaterHEditbox;
		CEGUI::Window *NoiseTypebox;
		//CEGUI::Window *NoiseBasisbox;
		CEGUI::Window *Depthslider;
		CEGUI::Window *DepthEditbox;
		CEGUI::Window *Biasbox;
		CEGUI::Window *Shapebox;
		CEGUI::Window *Sharpbox;
		CEGUI::Window *NoiseBasisbox;
		CEGUI::Window *TextureItemListbox;
		CEGUI::Window *Texturesizeslider;
		CEGUI::Window *TextureSizeEditbox;
		CEGUI::Window *Normalslider;  //textures tab normal 
		CEGUI::Window *NormalEditbox;
		CEGUI::Window *Worldsizeslider;
		CEGUI::Window *WorldSizeEditbox;
		//CEGUI::Window *TextureItemEditbox1;
		CEGUI::Window *TextureImageWindow;
		EditboxMap * Textureeditboxmaps; //typedef above. Is used for CEGUI list boxes
		//CEGUI::OgreRenderer* mRenderer;
		Ogre::Terrain* cterrain;
		Ogre::Camera* mCamera;
		Ogre::Rectangle2D* cmMiniScreen;
		Ogre::TerrainGroup* cmTerrainGroup;
		map<int,CEGUI::String> csel;
		Configs config; 
		//map<int,map<string, float> > config; 
		TextureConfigs texconfigs;
		void addconfig(int nameID);
		void updateConfig(void);
		void loadConfig(void);
		void updateNoiseTypesettings();
		std::string TextureSelection;
};

FreqAmpOctEventReg::FreqAmpOctEventReg(Ogre::Terrain* terrain, Ogre::Camera* Camera, Ogre::Rectangle2D* mMiniScreen){
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().createLog("ScaleHeight.log");
	Ogre::Log* slog = Ogre::LogManager::getSingleton().createLog("Voronoi.log");
	cterrain = terrain;
	mCamera = Camera;
	cmMiniScreen = mMiniScreen;
	Textureeditboxmaps = new EditboxMap(); 
	//cnewWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("DemoWindow"); default keep 
        tcnewWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("TerrainWindow");
	cnewWindow = tcnewWindow->getChild("TerrainEditWindow");
	HydraulicWindow = tcnewWindow->getChild("HydraulicWindow");
	MarbleWindow = tcnewWindow->getChild("MarbleWindow");
	TabControlWindow = tcnewWindow->getChild("DefaultWindow")->getChild("TabControl");
	DefaultWindow = tcnewWindow->getChild("DefaultWindow");
	ShowDataWindow = tcnewWindow->getChild("ShowDataWindow");
	TerrainSettingsWindow = tcnewWindow->getChild("TerrainSettingsWindow");
	TerrainTexturesWindow = tcnewWindow->getChild("TerrainTexturesWindow");
	TextureItemListbox = TerrainTexturesWindow->getChild("TextureItemListbox");
	//TextureItemEditbox1 = TextureItemListbox->getChild("TextureItemEditbox1"); 
	TextureImageWindow = TerrainTexturesWindow->getChild("StaticImage");
	Texturesizeslider = TerrainTexturesWindow->getChild("TextureSizeSlider");
	TextureSizeEditbox = TerrainTexturesWindow->getChild("TextureSizeEditbox");
	Normalslider = TerrainTexturesWindow->getChild("NormalSlider");
	NormalEditbox = TerrainTexturesWindow->getChild("NormalEditbox");
	Worldsizeslider = TerrainTexturesWindow->getChild("WorldSizeSlider");
	WorldSizeEditbox = TerrainTexturesWindow->getChild("WorldSizeEditbox");
	CEGUI::TabControl * tabcontrol = static_cast<CEGUI::TabControl*>(TabControlWindow);
	tabcontrol->addTab(cnewWindow);
	tabcontrol->addTab(HydraulicWindow);
	tabcontrol->addTab(MarbleWindow);
	tabcontrol->addTab(ShowDataWindow);
	tabcontrol->addTab(TerrainSettingsWindow);
	tabcontrol->addTab(TerrainTexturesWindow);
	Frequencyslider = cnewWindow ->getChild("FrequencySlider");
	FrequencyEditbox = cnewWindow ->getChild("FreqEditbox");
	Amplitudeslider = cnewWindow ->getChild("AmplitudeSlider");
	AmpEditbox = cnewWindow ->getChild("AmpEditbox");
	Octavesslider = cnewWindow ->getChild("OctavesSlider");
	OctavesEditbox = cnewWindow ->getChild("OctavesEditbox");
	UpdateButton = DefaultWindow ->getChild("UpdateButton");
	Gainslider = cnewWindow ->getChild("GainSlider");
	GainEditbox = cnewWindow ->getChild("GainEditbox");
	Scaleslider = cnewWindow ->getChild("ScaleSlider");
	ScaleEditbox = cnewWindow ->getChild("ScaleEditbox");
	Freqdivslider = cnewWindow ->getChild("FreqDivSlider");
	Depthslider = cnewWindow ->getChild("DepthSlider");
	DepthEditbox = cnewWindow ->getChild("DepthEditbox");
	Maxheightslider = DefaultWindow ->getChild("MaxHeightSlider");
	Weightslider = DefaultWindow ->getChild("WeightSlider");
	WaterHslider = TerrainSettingsWindow->getChild("WaterHeightSlider");
	WaterHEditbox = TerrainSettingsWindow->getChild("WaterHeightEditbox");
	MountainHslider = TerrainSettingsWindow->getChild("MountainHeightSlider");
	MountainHEditbox = TerrainSettingsWindow->getChild("MountainHeightEditbox");
	UpdateButton2 = DefaultWindow ->getChild("UpdateButton2");
	UpdateButton3 = DefaultWindow ->getChild("UpdateButton3");
	RLTslider = DefaultWindow ->getChild("RLTSlider");
	Terrainbox = cnewWindow ->getChild("Terrainbox");
	FilterTypebox = cnewWindow ->getChild("FilterTypebox");
	NoiseTypebox = cnewWindow ->getChild("NoiseTypebox");
	NoiseBasisbox = cnewWindow ->getChild("NoiseBasisbox");
	Sharpbox = MarbleWindow ->getChild("Sharpbox");
	Shapebox = MarbleWindow ->getChild("Shapebox");
	Biasbox = MarbleWindow ->getChild("Biasbox");
	
//	std::ostringstream ss5;
//	ss5 << "Terrain1";
//	CEGUI::ListboxItem listboxitem = new CEGUI::ListboxItem(ss5.str(), 0);
//	ss5.str(std::string());
//	ss5 << "Terrain2";
//	CEGUI::ListboxItem listboxitem2 = new CEGUI::ListboxItem(ss5.str(),0);
//	listboxitem = new CEGUI::ListboxItem(ss5.str(),0);
//	ss5.str(std::string());
//	ss5 << "Terrain2";
//	listboxitem2 = new CEGUI::ListboxItem(ss5.str(),1);
	std::ostringstream ss5;
	Ogre::TexturePtr texture1 = cterrain->getLayerBlendTexture(cterrain->getLayerBlendTextureIndex(1).first);
	ss5<< texture1->getName();
	//TextureItemEditbox1->setText(ss5.str());
	//textureData * a = new textureData();
	//std::string textureName = "maTexture";  textureData & textureoutData = *a;	
	//*textureoutData = {};
	//if(TerrainTexturesNode::Instance()->getTextureNode(textureName, textureoutData)){
	textureNodeMap terraintexnds = TerrainTexturesNode::Instance()->getTextureNodes(); //textureNodeMap defined in TerrainTexturesNode.cpp
	textureNodeMap::iterator i = terraintexnds.begin();  //set by default first texture
	CEGUI::Listbox* listbox = static_cast<CEGUI::Listbox*>(TextureItemListbox);
	CEGUI::ListboxTextItem * itemListbox;
	if( i != terraintexnds.end()){
		std::string texName = (*i).first;
		itemListbox = new CEGUI::ListboxTextItem(CEGUI::String(texName), 1);
		itemListbox->setSelectionColours(CEGUI::Colour(1.0f,.5f,.5f,.5f));
		//itemListbox->setSelectionBrushImage("MultiListSelectionBrush");	
		//TextureItemEditbox1->setText(texName);
		//(*Textureeditboxmaps)[texName] = TextureItemEditbox1;
		listbox->addItem(itemListbox); 
		listbox->setItemSelectState(itemListbox,true); 
		Ogre::TexturePtr pResource3 = Ogre::TextureManager::getSingleton().getByName(texName);
		CEGUI::System& ceguiSystem = CEGUI::System::getSingleton();
		CEGUI::Renderer* gui_renderer(ceguiSystem.getRenderer());
		CEGUI::Texture& texture = gui_renderer->createTexture("MyCEGUITextureName1");
		CEGUI::OgreTexture & rendererTexture =  static_cast<CEGUI::OgreTexture&>(texture);
	
		rendererTexture.setOgreTexture(pResource3, false);
		const CEGUI::Rectf rect(CEGUI::Vector2f(0.0f, 0.0f), rendererTexture.getOriginalDataSize());
		CEGUI::BasicImage* image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage", texName));
   		image->setTexture(&rendererTexture);
   		image->setArea(rect);
   		image->setAutoScaled(CEGUI::ASM_Both);
	//*/
		TextureImageWindow->setProperty("Image", texName);
		i++;
	}
	int p = 1;
       ///*
	while (i != terraintexnds.end()){
		/*
		CEGUI::Window *si = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Editbox", "RTTWindow");
		//si->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), {{0.346846, 128}, {0, 32}}
		si->setSize(CEGUI::USize(CEGUI::UDim(0.346846f, 128.0f),
        	                    CEGUI::UDim(0.0f, 32.0f)));
		//{0.0225225, -2}, {0, 0}
		si->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0225225f, -2.0f),
                 	               CEGUI::UDim(0.0f, 0.0f + p*32.0f)));
		si->setText((*i).first);
		TextureItemListbox->addChild(si);
		(*Textureeditboxmaps)[(*i).first] = si;
		*/
		Ogre::TexturePtr pResource3 = Ogre::TextureManager::getSingleton().getByName((*i).first);
		CEGUI::System& ceguiSystem = CEGUI::System::getSingleton();
		CEGUI::Renderer* gui_renderer(ceguiSystem.getRenderer());
		std::ostringstream ss5;
		ss5<<p+1;
		CEGUI::Texture& texture = gui_renderer->createTexture("MyCEGUITextureName"+ss5.str());
		CEGUI::OgreTexture & rendererTexture =  static_cast<CEGUI::OgreTexture&>(texture);
	
		rendererTexture.setOgreTexture(pResource3, false);
		const CEGUI::Rectf rect(CEGUI::Vector2f(0.0f, 0.0f), rendererTexture.getOriginalDataSize());
		CEGUI::BasicImage* image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage", (*i).first));
   		image->setTexture(&rendererTexture);
   		image->setArea(rect);
   		image->setAutoScaled(CEGUI::ASM_Both);
	//*/
		//TextureImageWindow->setProperty("Image", "RTTImage");
		itemListbox = new CEGUI::ListboxTextItem(CEGUI::String((*i).first), p+1);
		itemListbox->setSelectionColours(CEGUI::Colour(1.0f,1.0f,1.0f,1.0f));
		//itemListbox->setSelectionBrushImage("MultiListSelectionBrush");	
		listbox->addItem(itemListbox); 
		i++;
		p++;
	}
	//*/
        /*
	Ogre::TexturePtr pResource3 = Ogre::TextureManager::getSingleton().getByName("maTexture3");
	CEGUI::System& ceguiSystem = CEGUI::System::getSingleton();
	CEGUI::Renderer* gui_renderer(ceguiSystem.getRenderer());
	CEGUI::Texture& texture = gui_renderer->createTexture("MyCEGUITextureName");
	CEGUI::OgreTexture & rendererTexture =  static_cast<CEGUI::OgreTexture&>(texture);
	//rendererTexture->createEmptyOgreTexture ();
	rendererTexture.setOgreTexture(pResource3, false);
	*/
	//Ogre::RenderTexture *rtex = pResource3->getBuffer()->getRenderTarget();
	//CEGUI::ImageManager::getSingleton().addFromImageFile ("ImageForTextureImageWindow", "test10a.png");	
	//CEGUI::DefaultWindow*  textureimagewindow = static_cast<CEGUI::DefaultWindow*>(TextureImageWindow);
	///*
	//CEGUI::Texture &guiTex = mRenderer->createTexture("textname", pResource3);
	///*
        /*
	const CEGUI::Rectf rect(CEGUI::Vector2f(0.0f, 0.0f), rendererTexture.getOriginalDataSize());
	CEGUI::BasicImage* image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage", "RTTImage"));
   	image->setTexture(&rendererTexture);
   	image->setArea(rect);
   	image->setAutoScaled(CEGUI::ASM_Both);
	//*/
	//TextureImageWindow->setProperty("Image", "RTTImage");
	
	CEGUI::Slider* WHSlider = static_cast<CEGUI::Slider*>(WaterHslider);
	WHSlider->setCurrentValue(.25f);
	CEGUI::Slider* MHSlider = static_cast<CEGUI::Slider*>(MountainHslider);
	MHSlider->setCurrentValue(.8f);
	CEGUI::ListboxTextItem* itemCombobox = new CEGUI::ListboxTextItem("Terrain1", 1);
	csel[1] = CEGUI::String("Terrain1");
	CEGUI::Combobox* combobox = static_cast<CEGUI::Combobox*>(Terrainbox);
	combobox->addItem(itemCombobox);
	addconfig(1);
	combobox->setItemSelectState 	(itemCombobox,true);  	
	itemCombobox = new CEGUI::ListboxTextItem("Terrain2", 2);
	combobox->addItem(itemCombobox);
	addconfig(2);
	csel[2] = CEGUI::String("Terrain2");
	itemCombobox = new CEGUI::ListboxTextItem("ADD", 1);
	combobox = static_cast<CEGUI::Combobox*>(FilterTypebox);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("MULTIPLY", 2);
	//CEGUI::Combobox* combobox = static_cast<CEGUI::Combobox*>(FilterTypebox);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("DIFFERENCE", 3);
	//CEGUI::Combobox* combobox = static_cast<CEGUI::Combobox*>(FilterTypebox);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("DIVIDE", 4);
	//CEGUI::Combobox* combobox = static_cast<CEGUI::Combobox*>(FilterTypebox);
	combobox->addItem(itemCombobox);
//	Terrainbox->addItem(listboxitem2);
	combobox = static_cast<CEGUI::Combobox*>(NoiseTypebox);
	itemCombobox = new CEGUI::ListboxTextItem("Turbulent Noise fBm", 1);
	combobox->addItem(itemCombobox);
	combobox->setItemSelectState 	(itemCombobox,true); 
	itemCombobox = new CEGUI::ListboxTextItem("General Noise", 2);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Simplex", 3);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("SteinFbm", 4);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("DiamondSquare", 5);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("MmultiFractal", 6);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("MHetero", 7);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("MHybridmultiFractal", 8);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("MRigedFractal", 9);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Marble", 10);
	combobox->addItem(itemCombobox);
	combobox = static_cast<CEGUI::Combobox*>(NoiseBasisbox);
	itemCombobox = new CEGUI::ListboxTextItem("Blender Original", 1);
	combobox->addItem(itemCombobox);
	combobox->setItemSelectState 	(itemCombobox,true); 
	itemCombobox = new CEGUI::ListboxTextItem("Original Perlin", 2);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("New Perlin", 3);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Voronoi F1", 4);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Voronoi F2", 5);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Voronoi F3", 6);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Voronoi F4", 7);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Voronoi F1/F2", 8);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Voronoi CR", 9);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Cell Noise", 10);
	combobox->addItem(itemCombobox);

	combobox = static_cast<CEGUI::Combobox*>(Shapebox);
	itemCombobox = new CEGUI::ListboxTextItem("Marble", 1);
	combobox->addItem(itemCombobox);
	combobox->setItemSelectState 	(itemCombobox,true); 
	itemCombobox = new CEGUI::ListboxTextItem("Ring", 2);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Swirl", 3);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Bumps", 4);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Y Gradient", 5);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("X Gradient", 6);
	combobox->addItem(itemCombobox);


	combobox = static_cast<CEGUI::Combobox*>(Biasbox);
	itemCombobox = new CEGUI::ListboxTextItem("Sine", 1);
	combobox->addItem(itemCombobox);
	combobox->setItemSelectState 	(itemCombobox,true); 
	itemCombobox = new CEGUI::ListboxTextItem("Saw", 2);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Triangle", 3);
	combobox->addItem(itemCombobox);

	combobox = static_cast<CEGUI::Combobox*>(Sharpbox);
	itemCombobox = new CEGUI::ListboxTextItem("Soft", 1);
	combobox->addItem(itemCombobox);
	combobox->setItemSelectState 	(itemCombobox,true); 
	itemCombobox = new CEGUI::ListboxTextItem("Sharp", 2);
	combobox->addItem(itemCombobox);
	itemCombobox = new CEGUI::ListboxTextItem("Sharper", 3);
	combobox->addItem(itemCombobox);

	Frequencyslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateF, this));
	FrequencyEditbox->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateFE, this));
	Amplitudeslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateA, this));
	AmpEditbox->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateAE, this));
	Octavesslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateO, this));
	OctavesEditbox->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateOE, this));
	Gainslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateG, this));
	GainEditbox->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateGE, this));
	Scaleslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateS, this));
	ScaleEditbox->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateSE, this));
	Texturesizeslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateTES, this));
	TextureSizeEditbox->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateTEB, this));
	Normalslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateNS, this));
	NormalEditbox->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateNB, this));
	Worldsizeslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateWS, this));
	WorldSizeEditbox->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateWB, this));
	Freqdivslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateFD, this));
	Depthslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateD, this));
	DepthEditbox->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateDE, this));
	Maxheightslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateM, this));
	RLTslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateRLT, this));
	Weightslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateW, this));
	WaterHslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateWHS, this));
	MountainHslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateMHS, this));
	WaterHEditbox->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateWHE, this));
	MountainHEditbox->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateMHE, this));
	Terrainbox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateT, this));	//EventDropListDisplayed
        NoiseTypebox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateNTB, this));
        TextureItemListbox->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateTILB, this));
	Terrainbox->subscribeEvent(CEGUI::Combobox::EventDropListDisplayed, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateTD, this));
	 UpdateButton->subscribeEvent(CEGUI::PushButton::EventClicked,
    CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateB, this));
	 UpdateButton2->subscribeEvent(CEGUI::PushButton::EventClicked,
    CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateBB, this));
	 UpdateButton3->subscribeEvent(CEGUI::PushButton::EventClicked,
    CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateBBB, this));
}

void FreqAmpOctEventReg::updateNoiseTypesettings(){
	CEGUI::Combobox* ncombobox = static_cast<CEGUI::Combobox*>(NoiseTypebox);
	CEGUI::Slider* FSlider = static_cast<CEGUI::Slider*>(Frequencyslider);
	CEGUI::Window* freqtitlelabel = cnewWindow ->getChild("Label");
	CEGUI::Window* amptitlelabel = cnewWindow ->getChild("Label2");
	CEGUI::ListboxItem* nlitem = ncombobox->getSelectedItem();
	//CEGUI::Slider* DSlider = static_cast<CEGUI::Slider*>(Depthslider);
	CEGUI::Window* depthtitlelabel = cnewWindow ->getChild("Label8");
	float nameID = (float)nlitem->getID();
	//switch ((int)nameID){
		//case 5: {
		if ((int) nameID == 6){
			freqtitlelabel->setText("Lacunarity");
			FSlider->setMaxValue(3.0f);
			depthtitlelabel->setText("H (smoothing)");
		}
		//case 6: {
		else if ((int) nameID == 7){
			freqtitlelabel->setText("Lacunarity");
			FSlider->setMaxValue(3.0f);
			depthtitlelabel->setText("H (smoothing)");
			amptitlelabel->setText("Offset");
		}
		else if ((int) nameID == 8){
			freqtitlelabel->setText("Lacunarity");
			FSlider->setMaxValue(3.0f);
			depthtitlelabel->setText("H (smoothing)");
			amptitlelabel->setText("Offset");
		}
		else if ((int) nameID == 9){
			freqtitlelabel->setText("Lacunarity");
			FSlider->setMaxValue(3.0f);
			depthtitlelabel->setText("H (smoothing)");
			amptitlelabel->setText("Offset");
		}
		//default:{
		else {
			freqtitlelabel->setText("Frequency");
			FSlider->setMaxValue(1.0f);
			depthtitlelabel->setText("Depth");
			amptitlelabel->setText("Amplitude");
		}
	//}     
}
void FreqAmpOctEventReg::addconfig(int nameID){
	ConfSets settings;
	//map<string, float> settings;
	settings["Frequency"] = 1.0f/513.0f;
	settings["Amplitude"] = .5f;
	settings["Octaves"] = 6.0f;
	settings["Gain"] = .5f;
	settings["Scale"] = 1.0f/513.0f;
	settings["Freqdiv"] = 1.0f;
	settings["Maxheight"] = 1500.0f;
	settings["Weight"] = .5f;
	settings["NoiseType"] = 0.0f;
	settings["Depth"] = 0.75f;
	settings["Bias"] = 0.0f;
	settings["Shape"] = 0.0f;
	settings["Sharp"] = 0.0f;
	settings["NoiseBasis"] = 0.0f;
	//settings["Freqfloored"] = 0.0f;
	ConfSets settings2;
	settings2["TextureSize"] = 513.0f;
	settings2["WorldSize"] = 200.0f;
	settings2["Normal"] = 1.0f;
	config[nameID] = settings;
	config[3] = settings2;
}

void FreqAmpOctEventReg::updateConfig(){
	updateNoiseTypesettings();
	//map<string, float> settings = config[nameID];
	//float fval = settings["Frequency"];
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("ScaleHeight.log");
	std::ostringstream ss5;
	CEGUI::Slider* FSlider = static_cast<CEGUI::Slider*>(Frequencyslider);
	//FSlider->setCurrentValue(fval);
	float fval = FSlider->getCurrentValue();
	CEGUI::Slider* ASlider = static_cast<CEGUI::Slider*>(Amplitudeslider);
	float aval = ASlider->getCurrentValue();
	CEGUI::Slider* OSlider = static_cast<CEGUI::Slider*>(Octavesslider);
	float oval = OSlider->getCurrentValue();
	CEGUI::Slider* GSlider = static_cast<CEGUI::Slider*>(Gainslider);
	float gval = GSlider->getCurrentValue();
	CEGUI::Slider* SSlider = static_cast<CEGUI::Slider*>(Scaleslider);
	float sval = SSlider->getCurrentValue();
	CEGUI::Slider* FdSlider = static_cast<CEGUI::Slider*>(Freqdivslider);
	float fdval = FdSlider->getCurrentValue();
	CEGUI::Slider* DSlider = static_cast<CEGUI::Slider*>(Depthslider);
	float dval = DSlider->getCurrentValue();
	CEGUI::Slider* MSlider = static_cast<CEGUI::Slider*>(Maxheightslider);
	float mval = MSlider->getCurrentValue();
	CEGUI::Slider* WSlider = static_cast<CEGUI::Slider*>(Weightslider);
	float wval = WSlider->getCurrentValue();
	CEGUI::Combobox* combobox = static_cast<CEGUI::Combobox*>(Terrainbox);
	CEGUI::Combobox* ncombobox = static_cast<CEGUI::Combobox*>(NoiseTypebox);
	CEGUI::ListboxItem* nlitem = ncombobox->getSelectedItem();
	float nameID = (float)nlitem->getID();
	CEGUI::Combobox* nbcombobox = static_cast<CEGUI::Combobox*>(NoiseBasisbox);
	CEGUI::ListboxItem* nblitem = nbcombobox->getSelectedItem();
	float nbnameID = (float)nblitem->getID();
	CEGUI::Combobox* bcombobox = static_cast<CEGUI::Combobox*>(Biasbox);
	CEGUI::ListboxItem* blitem = bcombobox->getSelectedItem();
	float bnameID = (float)blitem->getID();
	CEGUI::Combobox* shcombobox = static_cast<CEGUI::Combobox*>(Shapebox);
	CEGUI::ListboxItem* shlitem = shcombobox->getSelectedItem();
	float shnameID = (float)shlitem->getID();
	CEGUI::Combobox* shpcombobox = static_cast<CEGUI::Combobox*>(Sharpbox);
	CEGUI::ListboxItem* shplitem = shpcombobox->getSelectedItem();
	float shpnameID = (float)shplitem->getID();
	CEGUI::Slider* TSizeSlider = static_cast<CEGUI::Slider*>(Texturesizeslider);
	float tsval = TSizeSlider->getCurrentValue();
	CEGUI::Slider* NormalSlider = static_cast<CEGUI::Slider*>(Normalslider);
	float normval = NormalSlider->getCurrentValue();
	CEGUI::Slider* WorldSizeSlider = static_cast<CEGUI::Slider*>(Worldsizeslider);
	float wsval = WorldSizeSlider->getCurrentValue();
	ConfSets settings, settings2;
	//map<string, float> settings;
	ss5 << "Test update config" << "\n";
	ss5 << "Noise type nameID: "<< nameID << "\n";
	tlog->logMessage(ss5.str());	
	settings["Frequency"] = fval;
	settings["Amplitude"] = aval;
	settings["Octaves"] = oval;
	settings["Gain"] = gval;
	settings["Scale"] = sval;
	if (fdval == 0.0f) {fdval = 1.0f;}
	settings["Freqdiv"] = fdval;
	if (dval == 0.0f){dval = 0.01f;}
	settings["Depth"] = dval;
	settings["Maxheight"] = mval;
	settings["Weight"] = wval;
	settings["NoiseType"] = nameID-1;
	settings["NoiseBasis"] = nbnameID-1;
	settings["Bias"] = bnameID-1;
	settings["Shape"] = shnameID-1;
	settings["Sharp"] = shpnameID-1;
	settings2["TextureSize"] = tsval;
	settings2["Normal"] = normval;
	settings2["WorldSize"] = wsval;
	ss5 << "Test update config" << "\n";
	ss5 << "updated noise type: " << nameID-1 << "\n";
	tlog->logMessage(ss5.str());
	CEGUI::ListboxItem* litem = combobox->getSelectedItem();
	ss5 << "Test update config2" << "\n";
	tlog->logMessage(ss5.str());
	int nameid = (int)litem->getID();
	ss5 << "Test update config" << "\n";
	tlog->logMessage(ss5.str());
	config[nameid] = settings;
	config[3] = settings2;
	texconfigs[TextureSelection] = config;
	ss5 << "Test update config settings updated" << "\n";
	tlog->logMessage(ss5.str());
	
}

void FreqAmpOctEventReg::loadConfig(){
	
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("ScaleHeight.log");
	std::ostringstream ss5;
	ss5 << "Test load config" << "\n";
	tlog->logMessage(ss5.str());
	CEGUI::Combobox* combobox = static_cast<CEGUI::Combobox*>(Terrainbox);
	CEGUI::ListboxItem* litem = combobox->getSelectedItem();
	ss5 << "Test load config at litem" << "\n";
	tlog->logMessage(ss5.str());
	int nameID = (int)litem->getID();
	ss5 << "Test load config at name" << "\n";
	tlog->logMessage(ss5.str());
	config = texconfigs[TextureSelection];
	ConfSets settings = config[nameID];
	//map<string, float> settings = config[nameID];
	float fval, aval, oval, gval, sval, fdval, mval, wval, nval, dval, nbval, bval, shapeval, sharpval,
	      tsval, normval, wsval;
	fval = settings["Frequency"];
	aval = settings["Amplitude"];
	oval = settings["Octaves"];
	gval = settings["Gain"];
	sval = settings["Scale"];
	fdval = settings["Freqdiv"];
	mval = settings["Maxheight"];
	wval = settings["Weight"];
	nval = settings["NoiseType"];
	dval = settings["Depth"];
	nbval = settings["NoiseBasis"];  //this and settings below apply to marble noise type only
	bval = settings["Bias"];  
	shapeval = settings["Shape"];
	sharpval = settings["Sharp"];
	ConfSets settings2 = config[3];
	tsval = settings2["TextureSize"];
	normval = settings2["Normal"];
	wsval = settings2["WorldSize"];
	ss5 << "Test load config" << "\n";
	tlog->logMessage(ss5.str());
	CEGUI::Combobox* ncombobox = static_cast<CEGUI::Combobox*>(NoiseTypebox);
	ncombobox->setItemSelectState 	((int)nval,true);
        updateNoiseTypesettings();

	CEGUI::Slider* FSlider = static_cast<CEGUI::Slider*>(Frequencyslider);
	FSlider->setCurrentValue(fval);
	CEGUI::Slider* ASlider = static_cast<CEGUI::Slider*>(Amplitudeslider);
	ASlider->setCurrentValue(aval);
	CEGUI::Slider* OSlider = static_cast<CEGUI::Slider*>(Octavesslider);
	OSlider->setCurrentValue(oval);
	CEGUI::Slider* GSlider = static_cast<CEGUI::Slider*>(Gainslider);
	GSlider->setCurrentValue(gval);
	CEGUI::Slider* SSlider = static_cast<CEGUI::Slider*>(Scaleslider);
	SSlider->setCurrentValue(sval);
	CEGUI::Slider* FdSlider = static_cast<CEGUI::Slider*>(Freqdivslider);
	FdSlider->setCurrentValue(fdval);
	CEGUI::Slider* DSlider = static_cast<CEGUI::Slider*>(Depthslider);
	DSlider->setCurrentValue(dval);
	CEGUI::Slider* MSlider = static_cast<CEGUI::Slider*>(Maxheightslider);
	MSlider->setCurrentValue(mval);
	CEGUI::Slider* WSlider = static_cast<CEGUI::Slider*>(Weightslider);
	WSlider->setCurrentValue(wval);	

	
	/*
	CEGUI::ListboxTextItem* itemCombobox;
	if ((int)nval == 1){
		itemCombobox = new CEGUI::ListboxTextItem("Advanced Perlin", 1);
	}
	else if ((int)nval == 2){
		itemCombobox = new CEGUI::ListboxTextItem("Voronoi", 2);
	}
	else if ((int)nval == 3){
		itemCombobox = new CEGUI::ListboxTextItem("Simplex", 3);
	}
	else if ((int)nval == 3){
		itemCombobox = new CEGUI::ListboxTextItem("SteinFbm", 4);
	}
	else if ((int)nval == 4){
		itemCombobox = new CEGUI::ListboxTextItem("DiamondSquare", 5);
	}
	else if ((int)nval == 5){
		itemCombobox = new CEGUI::ListboxTextItem("MmultiFractal", 6);
	}
	else if ((int)nval == 6){
		itemCombobox = new CEGUI::ListboxTextItem("MHetero", 7);
	}
	else if ((int)nval == 7){
		itemCombobox = new CEGUI::ListboxTextItem("MHybridmultiFractal", 8);
	}
	else if ((int)nval == 8){
		itemCombobox = new CEGUI::ListboxTextItem("MRigedFractal", 9);
	}
	else if ((int)nval == 9){
		itemCombobox = new CEGUI::ListboxTextItem("Marble", 10);
	}
        */
	//ncombobox->setItemSelectState 	((int)nval,true);

	ncombobox = static_cast<CEGUI::Combobox*>(NoiseBasisbox);
	ncombobox->setItemSelectState 	((int)nbval,true);

	ncombobox = static_cast<CEGUI::Combobox*>(Biasbox);
	ncombobox->setItemSelectState 	((int)bval,true);

	ncombobox = static_cast<CEGUI::Combobox*>(Shapebox);
	ncombobox->setItemSelectState 	((int)shapeval,true);

	ncombobox = static_cast<CEGUI::Combobox*>(Sharpbox);
	ncombobox->setItemSelectState 	((int)sharpval,true);

	CEGUI::Slider* TsSlider = static_cast<CEGUI::Slider*>(Texturesizeslider);
	TsSlider->setCurrentValue(tsval);
	CEGUI::Slider* NormSlider = static_cast<CEGUI::Slider*>(Normalslider);
	NormSlider->setCurrentValue(normval);
	CEGUI::Slider* WorldSizeSlider = static_cast<CEGUI::Slider*>(Worldsizeslider);
	WorldSizeSlider->setCurrentValue(wsval);		
	//updateConfig();
	//
        /*
	switch ((int)nbval) {
		case 2:
			itemCombobox = new CEGUI::ListboxTextItem("Original Perlin", 2);
		case 3:
			itemCombobox = new CEGUI::ListboxTextItem("New Perlin", 3);
		case 4:
			itemCombobox = new CEGUI::ListboxTextItem("Voronoi F1", 4);
		case 5:
			itemCombobox = new CEGUI::ListboxTextItem("Voronoi F2", 5);
		case 6:
			itemCombobox = new CEGUI::ListboxTextItem("Voronoi F3", 6);
		case 7:
			itemCombobox = new CEGUI::ListboxTextItem("Voronoi F4", 7);
		case 8:
			itemCombobox = new CEGUI::ListboxTextItem("Voronoi F1/F2", 8);
		case 9:
			itemCombobox = new CEGUI::ListboxTextItem("Voronoi CR", 9);
		case 10:
			itemCombobox = new CEGUI::ListboxTextItem("Cell Noise", 10);
		case 1:
		default:
		{
			itemCombobox = new CEGUI::ListboxTextItem("Blender Original", 1);
			
		}
	}
	*/
	
}

void FreqAmpOctEventReg::updateF(const CEGUI::EventArgs &e){
	CEGUI::Window* flabel = cnewWindow ->getChild("Freqlabel");
	CEGUI::Slider* FSlider = static_cast<CEGUI::Slider*>(Frequencyslider);
	float val = FSlider->getCurrentValue();
	CEGUI::Slider* FdSlider = static_cast<CEGUI::Slider*>(Freqdivslider);
	CEGUI::Editbox* FEbox = static_cast<CEGUI::Editbox*>(FrequencyEditbox);
	float dval = FdSlider->getCurrentValue();
	if (dval == 0.0f){
		dval = 1.0f;
	}
	std::ostringstream ss5;
	ss5<<val/dval;
	flabel->setText(ss5.str());
	FEbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateFE(const CEGUI::EventArgs &e){
	CEGUI::Window* flabel = cnewWindow ->getChild("Freqlabel");
	CEGUI::Slider* FSlider = static_cast<CEGUI::Slider*>(Frequencyslider);
	CEGUI::Editbox* FEbox = static_cast<CEGUI::Editbox*>(FrequencyEditbox);
	float val = atof(FEbox->getText().c_str());
	//float val = FSlider->getCurrentValue();
	CEGUI::Slider* FdSlider = static_cast<CEGUI::Slider*>(Freqdivslider);
	float dval = FdSlider->getCurrentValue();
	if (dval == 0.0f){
		dval = 1.0f;
	}
	std::ostringstream ss5;
	ss5<<val;
	flabel->setText(ss5.str());
	FSlider->setCurrentValue(val);
	FdSlider->setCurrentValue(1.0f);
}

void FreqAmpOctEventReg::updateD(const CEGUI::EventArgs &e){
	CEGUI::Window* dlabel = cnewWindow ->getChild("Depthlabel");
	CEGUI::Slider* DSlider = static_cast<CEGUI::Slider*>(Depthslider);
	CEGUI::Editbox* DEbox = static_cast<CEGUI::Editbox*>(DepthEditbox);
	float val = DSlider->getCurrentValue();

	if (val == 0.0f){
		val = .01f;
	}
	std::ostringstream ss5;
	ss5<<val;
	dlabel->setText(ss5.str());
	DEbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateDE(const CEGUI::EventArgs &e){
	CEGUI::Window* dlabel = cnewWindow ->getChild("Depthlabel");
	CEGUI::Slider* DSlider = static_cast<CEGUI::Slider*>(Depthslider);
	CEGUI::Editbox* DEbox = static_cast<CEGUI::Editbox*>(DepthEditbox);
	//float val = DSlider->getCurrentValue();
	float val = atof(DEbox->getText().c_str());

	if (val == 0.0f){
		val = .01f;
	}
	std::ostringstream ss5;
	ss5<<val;
	dlabel->setText(ss5.str());
	DSlider->setCurrentValue(val);
}

void FreqAmpOctEventReg::updateFD(const CEGUI::EventArgs &e){
	CEGUI::Window* flabel = cnewWindow ->getChild("Freqlabel");
	CEGUI::Slider* FSlider = static_cast<CEGUI::Slider*>(Frequencyslider);
	float val = FSlider->getCurrentValue();
	CEGUI::Slider* FdSlider = static_cast<CEGUI::Slider*>(Freqdivslider);
	CEGUI::Editbox* FEbox = static_cast<CEGUI::Editbox*>(FrequencyEditbox);
	float dval = FdSlider->getCurrentValue();
	if (dval == 0.0f){
		dval = 1.0f;
	}
	std::ostringstream ss5;
	ss5<<val/dval;
	flabel->setText(ss5.str());
	FEbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateA(const CEGUI::EventArgs &e){
	CEGUI::Window* alabel = cnewWindow ->getChild("Amplabel");
	CEGUI::Slider* ASlider = static_cast<CEGUI::Slider*>(Amplitudeslider);
	CEGUI::Editbox* AEbox = static_cast<CEGUI::Editbox*>(AmpEditbox);
	float val = ASlider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	alabel->setText(ss5.str());
	AEbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateAE(const CEGUI::EventArgs &e){
	CEGUI::Window* alabel = cnewWindow ->getChild("Amplabel");
	CEGUI::Slider* ASlider = static_cast<CEGUI::Slider*>(Amplitudeslider);
	CEGUI::Editbox* AEbox = static_cast<CEGUI::Editbox*>(AmpEditbox);
	float val = atof(AEbox->getText().c_str());
	std::ostringstream ss5;
	ss5<<val;
	alabel->setText(ss5.str());
        ASlider->setCurrentValue(val);
}

void FreqAmpOctEventReg::updateO(const CEGUI::EventArgs &e){
	CEGUI::Window* olabel = cnewWindow ->getChild("Octaveslabel");
	CEGUI::Slider* OSlider = static_cast<CEGUI::Slider*>(Octavesslider);
	CEGUI::Editbox* OEbox = static_cast<CEGUI::Editbox*>(OctavesEditbox);
	float val = OSlider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	olabel->setText(ss5.str());
	OEbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateOE(const CEGUI::EventArgs &e){
	CEGUI::Window* olabel = cnewWindow ->getChild("Octaveslabel");
	CEGUI::Slider* OSlider = static_cast<CEGUI::Slider*>(Octavesslider);
	CEGUI::Editbox* OEbox = static_cast<CEGUI::Editbox*>(OctavesEditbox);
	float val = atof(OEbox->getText().c_str());
	//float val = OSlider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	olabel->setText(ss5.str());
	OSlider->setCurrentValue(val);
}

void FreqAmpOctEventReg::updateG(const CEGUI::EventArgs &e){
	CEGUI::Window* glabel = cnewWindow ->getChild("Gainlabel");
	CEGUI::Slider* GSlider = static_cast<CEGUI::Slider*>(Gainslider);
	CEGUI::Editbox* GEbox = static_cast<CEGUI::Editbox*>(GainEditbox);
	float val = GSlider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	glabel->setText(ss5.str());
	GEbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateGE(const CEGUI::EventArgs &e){
	CEGUI::Window* glabel = cnewWindow ->getChild("Gainlabel");
	CEGUI::Slider* GSlider = static_cast<CEGUI::Slider*>(Gainslider);
	CEGUI::Editbox* GEbox = static_cast<CEGUI::Editbox*>(GainEditbox);
	float val = atof(GEbox->getText().c_str());
	//float val = GSlider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	glabel->setText(ss5.str());
	GSlider->setCurrentValue(val);
}

void FreqAmpOctEventReg::updateS(const CEGUI::EventArgs &e){
	CEGUI::Window* slabel = cnewWindow ->getChild("Scalelabel");
	CEGUI::Slider* SSlider = static_cast<CEGUI::Slider*>(Scaleslider);
	float val = SSlider->getCurrentValue();
	CEGUI::Editbox* SEbox = static_cast<CEGUI::Editbox*>(ScaleEditbox);
	std::ostringstream ss5;
	ss5<<val;
	slabel->setText(ss5.str());
	SEbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateSE(const CEGUI::EventArgs &e){
	CEGUI::Window* slabel = cnewWindow ->getChild("Scalelabel");
	CEGUI::Slider* SSlider = static_cast<CEGUI::Slider*>(Scaleslider);
	//float val = SSlider->getCurrentValue();
	CEGUI::Editbox* SEbox = static_cast<CEGUI::Editbox*>(ScaleEditbox);
	float val = atof(SEbox->getText().c_str());
	std::ostringstream ss5;
	ss5<<val;
	slabel->setText(ss5.str());
	SSlider->setCurrentValue(val);
}

void FreqAmpOctEventReg::updateWHS(const CEGUI::EventArgs &e){
	
	CEGUI::Slider* WHSlider = static_cast<CEGUI::Slider*>(WaterHslider);
	float val = WHSlider->getCurrentValue();
	CEGUI::Slider* MHSlider = static_cast<CEGUI::Slider*>(MountainHslider);
	float val2 = MHSlider->getCurrentValue();
	if (val > val2){val = val2; WHSlider->setCurrentValue(val);}
	CEGUI::Editbox* WHEbox = static_cast<CEGUI::Editbox*>(WaterHEditbox);
	std::ostringstream ss5;
	ss5<<val;
	WHEbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateWHE(const CEGUI::EventArgs &e){
	
	CEGUI::Slider* WHSlider = static_cast<CEGUI::Slider*>(WaterHslider);
	//float val = SSlider->getCurrentValue();
	CEGUI::Editbox* WHEbox = static_cast<CEGUI::Editbox*>(WaterHEditbox);
	float val = atof(WHEbox->getText().c_str());
	CEGUI::Slider* MHSlider = static_cast<CEGUI::Slider*>(MountainHslider);
	float val2 = MHSlider->getCurrentValue();
	std::ostringstream ss5;
	//ss5<<val;
	if (val > val2){
		val = val2; 
		ss5<<val;
		WHSlider->setCurrentValue(val);
		WHEbox->setText(ss5.str());
	}
	else{
		ss5<<val;
	}
	WHSlider->setCurrentValue(val);
}

void FreqAmpOctEventReg::updateMHS(const CEGUI::EventArgs &e){
	
	CEGUI::Slider* MHSlider = static_cast<CEGUI::Slider*>(MountainHslider);
	float val = MHSlider->getCurrentValue();
	CEGUI::Slider* WHSlider = static_cast<CEGUI::Slider*>(WaterHslider);
	float val2 = WHSlider->getCurrentValue();
	if (val < val2){val = val2; MHSlider->setCurrentValue(val);}
	CEGUI::Editbox* MHEbox = static_cast<CEGUI::Editbox*>(MountainHEditbox);
	std::ostringstream ss5;
	ss5<<val;
	MHEbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateMHE(const CEGUI::EventArgs &e){
	
	CEGUI::Slider* MHSlider = static_cast<CEGUI::Slider*>(MountainHslider);
	//float val = SSlider->getCurrentValue();
	CEGUI::Editbox* MHEbox = static_cast<CEGUI::Editbox*>(MountainHEditbox);
	float val = atof(MHEbox->getText().c_str());
	CEGUI::Slider* WHSlider = static_cast<CEGUI::Slider*>(WaterHslider);
	float val2 = WHSlider->getCurrentValue();
	std::ostringstream ss5;
	if (val < val2){
		val = val2; 
		ss5<<val;
		MHSlider->setCurrentValue(val);
		MHEbox->setText(ss5.str());
	}
	else{
		ss5<<val;
	}
	MHSlider->setCurrentValue(val);
}

void FreqAmpOctEventReg::updateM(const CEGUI::EventArgs &e){
	CEGUI::Window* mlabel = DefaultWindow ->getChild("MHeightlabel");
	CEGUI::Slider* MSlider = static_cast<CEGUI::Slider*>(Maxheightslider);
	float val = MSlider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	mlabel->setText(ss5.str());
	//if (val > 1.0f) {
	//	new ScaleHeights(cterrain, mCamera, val);
	//}
}

void FreqAmpOctEventReg::updateW(const CEGUI::EventArgs &e){
	CEGUI::Window* wlabel = DefaultWindow ->getChild("Weightlabel");
	CEGUI::Slider* WSlider = static_cast<CEGUI::Slider*>(Weightslider);
	float val = WSlider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	wlabel->setText(ss5.str());
}

void FreqAmpOctEventReg::updateRLT(const CEGUI::EventArgs &e){
	CEGUI::Window* rltlabel = DefaultWindow ->getChild("RLTlabel");
	CEGUI::Slider* rltSlider = static_cast<CEGUI::Slider*>(RLTslider);
	//CEGUI::Editbox* OEbox = static_cast<CEGUI::Editbox*>(OctavesEditbox);
	float val = rltSlider->getCurrentValue() - 6000.0f;
	std::ostringstream ss5;
	ss5<<val;
	rltlabel->setText(ss5.str());
	//OEbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateB(const CEGUI::EventArgs &e){
	updateConfig();
/*	
	CEGUI::Slider* FSlider = static_cast<CEGUI::Slider*>(Frequencyslider);
	float fval = FSlider->getCurrentValue();
	CEGUI::Slider* ASlider = static_cast<CEGUI::Slider*>(Amplitudeslider);
	float aval = ASlider->getCurrentValue();
	CEGUI::Slider* OSlider = static_cast<CEGUI::Slider*>(Octavesslider);
	float oval = OSlider->getCurrentValue();
	CEGUI::Slider* GSlider = static_cast<CEGUI::Slider*>(Gainslider);
	float gval = GSlider->getCurrentValue();
	CEGUI::Slider* SSlider = static_cast<CEGUI::Slider*>(Scaleslider);
	float sval = SSlider->getCurrentValue();
*/	
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("ScaleHeight.log");
	std::ostringstream ss5;
	vector<vector<vector<vector<double> > > > tnoisevals;
	terr::T3dCPointsMaps * tnoisevalsm = new terr::T3dCPointsMaps();
	terr::CPointsMaps * vnoisevalsm    = new terr::CPointsMaps();
	vector<float> weights(config.size());
	config = texconfigs[TextureSelection];
	tnoisevals.resize(config.size()); 
	ss5 << "Hitting update button!!!!!!!" << "\n";
	ss5 << "Second terrain noise type:  "<<config[1]["NoiseType"]<< "\n";
	//ss5 << "First terrain noise type: "<<config[0]["NoiseType"]<< "\n";
	ss5 << "Config size: "<< config.size() << "\n";
	tlog->logMessage(ss5.str());
	ss5.str(std::string());

	map<int, map<string,float> >::iterator miter;
	for (miter = config.begin(); miter != config.end(); miter++)
	{
    		ss5 << "Key: " << miter->first << endl << "Values:" << endl;
    		map<string,float>::iterator miter2;
    		for (miter2 = miter->second.begin(); miter2 != miter->second.end(); miter2++){
   			ss5 << " " << miter2->first << ","<< miter2->second << endl;
		}
	}
	tlog->logMessage(ss5.str());
	ss5.str(std::string());
	int i = 0;
	map<int, map<string,float> >:: iterator miter3;
	for (miter3 = config.begin(); miter3 != config.end(); miter3++){
		if (miter3->first == 3) {break;}
		ss5 << "Starting noise gen loop!!!!!!!" << "\n";
		ss5 << "NoiseType: " << miter3->second["NoiseType"] << "\n";
		tlog->logMessage(ss5.str());
		ss5.str(std::string());	
		ConfSets tc = miter3->second;	
		//map<string, float> tc = miter3->second;
		if (tc["Freqdiv"] == 0){tc["Freqdiv"] = 1.0f;}
		if ((int)tc["NoiseType"] == 0){
			//float size, float scale, float zdepth, float frequency, float amplitude, float octaves, float gain = .5f, int imagemap = (int)3.0f
			//PerlinTest* test = new PerlinTest(513.0f, tc["Scale"], 2.0f, tc["Frequency"]/tc["Freqdiv"], tc["Amplitude"], tc["Octaves"], int(3.0f));
			//tnoisevals[i] = test->getNoisevalues();
			//(*tnoisevalsm)[i] = test->getNoisevaluesT();
			//double size, double scale, double frequency, double amplitude, double octaves, double gain
			(*vnoisevalsm)[i] = pfbmbuild (513.0f, tc["Scale"], 1.0f, tc["Frequency"]/tc["Freqdiv"], tc["Amplitude"], tc["Octaves"],
                                                       tc["Gain"]);
			//(*vnoisevalsm)[i] = pfbmbuild (513.0f, tc["Scale"], tc["Depth"], tc["Frequency"], tc["Octaves"]);			
			ss5 << "Hitting update button!!!!!!!" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
		}
		else if ((int)tc["NoiseType"]==1){
			ss5 << "2 on noise !!!!!!!" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			// BLI_gNoise(float noisesize, float x, float y, float z, int hard, int noisebasis)
			//Voronoi* test = new Voronoi(tc["Scale"], 513);
			//BuildVoronoi * testm = new BuildVoronoi();  //fortune algorithim implementation (faster)
			ss5 << "21 on noise !!!!!!!" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			//tnoisevals[i] = test->getHeightMapradial(tc["Scale"]);//->getSimpleHeightMap();
			//(*tnoisevalsm)[i] = testm->getHeightMapradial();
			//(*vnoisevalsm)[i] = testm->getHeightMapradial2();
			(*vnoisevalsm)[i] =  BLI_gNoisebuild(513.0f, tc["Scale"], 1.0f,tc["Sharp"], tc["NoiseBasis"]);
		}
		else if ((int)tc["NoiseType"]==2){
			ss5 << "Simplex" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			//Voronoi* test = new Voronoi(tc["Scale"], 513);
			BuildSimplex * tests = new BuildSimplex(tc["Frequency"]/tc["Freqdiv"], tc["Amplitude"]);  //perlin derivative
			//ss5 << "!!!!!!!" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			//tnoisevals[i] = test->getHeightMapradial(tc["Scale"]);//->getSimpleHeightMap();
			(*tnoisevalsm)[i] = tests->getHeightMap();
			(*vnoisevalsm)[i] = tests->getHeightMap2();
		}
		else if ((int)tc["NoiseType"]==3){
			ss5 << "SteinFbm" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			//Voronoi* test = new Voronoi(tc["Scale"], 513);
			BuildFbm * testf = new BuildFbm(tc["Frequency"], tc["Amplitude"]);  
			//ss5 << "!!!!!!!" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			//tnoisevals[i] = test->getHeightMapradial(tc["Scale"]);//->getSimpleHeightMap();
			(*vnoisevalsm)[i] = testf->getHeightMap();
			//(*vnoisevalsm)[i] = testm->getHeightMap2();
		}
		else if ((int)tc["NoiseType"]==4){
			ss5 << "DiamondSquare" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			//Voronoi* test = new Voronoi(tc["Scale"], 513);
			DiamondSquare * testd = new DiamondSquare(); 
			//ss5 << "!!!!!!!" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			//tnoisevals[i] = test->getHeightMapradial(tc["Scale"]);//->getSimpleHeightMap();
			(*vnoisevalsm)[i] = testd->getHeightMap();
			//(*vnoisevalsm)[i] = testm->getHeightMap2();
		}
		else if ((int)tc["NoiseType"]==5){
			ss5 << "MmultiFractal" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			
			//multiFractalbuild (double size, double scale, double H, double, lucunarity, double octaves,
				//double offset)
			//(*vnoisevalsm)[i] = multiFractalbuild (513.0f, tc["Scale"], tc["Depth"], tc["Frequency"], tc["Octaves"],
			//	tc["Gain"]);
                        (*vnoisevalsm)[i] = mg_MultiFractalbuild(513.0f, tc["Scale"], 1.0f,tc["Depth"], tc["Frequency"], tc["Octaves"],
								 tc["NoiseBasis"]);
			//(*vnoisevalsm)[i] = testm->getHeightMap2();
		}
		else if ((int)tc["NoiseType"]==6){
			ss5 << "MHetero" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			//Heterobuild (double size, double scale, double H, double lucunarity, double octaves, 
                          //double offset)

			//(*vnoisevalsm)[i] = Heterobuild(513.0f, tc["Scale"], tc["Depth"], tc["Frequency"], tc["Octaves"],
			//	tc["Gain"]);
                        (*vnoisevalsm)[i] = mg_HeteroTerrainbuild(513.0f, tc["Scale"], 1.0f,tc["Depth"], tc["Frequency"], 
						tc["Octaves"], tc["Amplitude"], tc["NoiseBasis"]);
			//(*vnoisevalsm)[i] = testm->getHeightMap2();
		}
		else if ((int)tc["NoiseType"]==7){
			ss5 << "MHybrid" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());

			//HybridMultifractalbuild (double size, double scale, double H, double lucunarity, double octaves, 
                          //double offset)
			//(*vnoisevalsm)[i] = HybridMultifractalbuild(513.0f, tc["Scale"], tc["Depth"], tc["Frequency"], tc["Octaves"],
			//	tc["Gain"]);
			//terr::CPointsMap mg_HybridMultiFractalbuild(double size, double scale, float H, float lacunarity, float octaves, float offset, float gain, int noisebasis)
			(*vnoisevalsm)[i] = mg_HybridMultiFractalbuild(513.0f,tc["Scale"], 1.0f,tc["Depth"], tc["Frequency"], 
						tc["Octaves"], tc["Amplitude"], tc["Gain"], tc["NoiseBasis"]); 
			//(*vnoisevalsm)[i] = testm->getHeightMap2();
		}
		else if ((int)tc["NoiseType"]==8){
			ss5 << "MRiged" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());

			//RidgedMultifractalbuild (double size, double scale, double H, double lucunarity, double octaves, 
                          //double offset, double gain)
			//(*vnoisevalsm)[i] = RidgedMultifractalbuild(513.0f, tc["Scale"], tc["Depth"], tc["Frequency"], tc["Octaves"],
			//	tc["Gain"], tc["Gain"]*2);
			(*vnoisevalsm)[i] =  mg_RidgedMultiFractalbuild(513.0f,tc["Scale"], 1.0f, tc["Depth"], tc["Frequency"], 
						tc["Octaves"], tc["Amplitude"], tc["Gain"], tc["NoiseBasis"]); 
			//(*vnoisevalsm)[i] = testm->getHeightMap2();
		}
		else if ((int)tc["NoiseType"]==9){
			ss5 << "Marble" << "\n";
			ss5 << tc["Octaves"] << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());

                         //pmarblebuild (double size, double scale, double frequency, double amplitude, double octaves, int noisebasis,
				//int noisebasis, int bias, int shape, int sharp)
			(*vnoisevalsm)[i] = pmarblebuild (513.0f,tc["Scale"]/265.5f, 1.0f, tc["Frequency"]/tc["Freqdiv"], tc["Amplitude"], 
                                                           tc["Octaves"], (int)tc["NoiseBasis"] == 10 ? 14 : (int)tc["NoiseBasis"], 
							   (int)tc["Bias"], (int)tc["Shape"], (int)tc["Sharp"]);
			//(*vnoisevalsm)[i] = testm->getHeightMap2();
		}
		ss5 << "weights!!!!!!! " << tc["Weight"]<< "\n";
		tlog->logMessage(ss5.str());
		ss5.str(std::string());
		weights[i] = tc["Weight"];
		i++;
	}
	ss5 << "Update 3!!!!!!!" << "\n";
	tlog->logMessage(ss5.str());
	CEGUI::Combobox* combobox = static_cast<CEGUI::Combobox*>(FilterTypebox);
	CEGUI::ListboxItem* litem = combobox->getSelectedItem();
	ss5 << "Hitting update button!!!!!!!" << "\n";
	tlog->logMessage(ss5.str());
	int nameid = (int)litem->getID();
	Combiner* cmb = new Combiner(nameid);
	//vector<vector<vector<double> > > heightmapvalues = cmb -> Combine(tnoisevals, weights);
	//terr::T3dCPointsMap * heightmapvaluesm = cmb->Combine(tnoisevalsm,weights);
	terr::CPointsMap * cheightmapvaluesm = cmb->Combine(vnoisevalsm,weights);
//	PerlinTest* test = new PerlinTest(513.0f,sval, 2.0f, fval,aval,gval, int(3.0f)); //keep by default third arg 2.0f..don't tweak won't work for higher or lower vals
//	vector<vector<vector<double> > > heightmapvalues = test->getNoisevalues();
	//new LoadHeightMap(cterrain, 513.0f*1.0f, heightmapvalues);
	//new LoadHeightMap(cterrain, 513.0f*1.0f, heightmapvaluesm);
	CEGUI::Slider* WHSlider = static_cast<CEGUI::Slider*>(WaterHslider);
	float whval = WHSlider->getCurrentValue();
	CEGUI::Slider* MHSlider = static_cast<CEGUI::Slider*>(MountainHslider);
	float mhval = MHSlider->getCurrentValue();
	TerrainTexturesSettings * texsets = new TerrainTexturesSettings();
	texsets->size = config[3]["TextureSize"];
	texsets->normal = config[3]["Normal"];
	texsets->worldsize = config[3]["WorldSize"];
	texsets->name = TextureSelection;
	if (TextureSelection == "Terrain"){texsets->layerid = 1;}
	else{texsets->layerid = 2;}
	if (TextureSelection == "Terrain"){new LoadHeightMap(cterrain, cmMiniScreen,513.0f*1.0f, cheightmapvaluesm, 
					 		     whval, mhval,2.0f, true, *texsets);}
	else{new LoadHeightMap(cterrain, cmMiniScreen,513.0f*1.0f, cheightmapvaluesm, whval, mhval,2.0f, false, *texsets);}
	//new LoadHeightMap(cterrain, 513.0f*1.0f, heightmapvaluesm, 513.0f);
	//new LoadHeightMap(cterrain, 513.0f*1.0f, (*tnoisevalsm)[0]);
}

void FreqAmpOctEventReg::updateBB(const CEGUI::EventArgs &e){
	CEGUI::Slider* MSlider = static_cast<CEGUI::Slider*>(Maxheightslider);
	float val = MSlider->getCurrentValue();
	CEGUI::Slider* WHSlider = static_cast<CEGUI::Slider*>(WaterHslider);
	float whval = WHSlider->getCurrentValue();
	CEGUI::Slider* MHSlider = static_cast<CEGUI::Slider*>(MountainHslider);
	float mhval = MHSlider->getCurrentValue();
	//std::ostringstream ss5;
	//ss5<<val;
	//mlabel->setText(ss5.str());
	if (val > 513.0f) {
		new ScaleHeights(cterrain, cmMiniScreen,mCamera, val, whval, mhval,TextureSelection);
	}
}

void FreqAmpOctEventReg::updateBBB(const CEGUI::EventArgs &e){
	CEGUI::Slider* rltSlider = static_cast<CEGUI::Slider*>(RLTslider);
	float val = rltSlider->getCurrentValue();
	CEGUI::Slider* WHSlider = static_cast<CEGUI::Slider*>(WaterHslider);
	float whval = WHSlider->getCurrentValue();
	CEGUI::Slider* MHSlider = static_cast<CEGUI::Slider*>(MountainHslider);
	float mhval = MHSlider->getCurrentValue();
	//std::ostringstream ss5;
	//ss5<<val;
	//mlabel->setText(ss5.str());
	//if (val > 513.0f) {
		new TranslateHeights(cterrain, cmMiniScreen,mCamera, val-6000.0f, whval, mhval,TextureSelection);
	//}
}

void FreqAmpOctEventReg::updateT(const CEGUI::EventArgs &e){
	loadConfig();
}

void FreqAmpOctEventReg::updateNTB(const CEGUI::EventArgs &e){
	//loadConfig();
	//updateConfig();
	updateNoiseTypesettings();
}

void FreqAmpOctEventReg::updateTD(const CEGUI::EventArgs &e){
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("ScaleHeight.log");
	std::ostringstream ss5;
	ss5 << "Test update config" << "\n";
	tlog->logMessage(ss5.str());
	updateConfig();
}

void FreqAmpOctEventReg::updateTILB(const CEGUI::EventArgs &e){
	CEGUI::Listbox * titemlistbox = static_cast<CEGUI::Listbox*>(TextureItemListbox);
        CEGUI::ListboxItem * ie = titemlistbox->getFirstSelectedItem();
	std::ostringstream ss5;
	ss5<<ie->getText();
	//*/
	TextureImageWindow->setProperty("Image", ss5.str());
	updateConfig();
	TextureSelection = ss5.str();
	loadConfig();
}

void FreqAmpOctEventReg::updateTES(const CEGUI::EventArgs &e){
	
	CEGUI::Slider* TSlider = static_cast<CEGUI::Slider*>(Texturesizeslider);
	float val = TSlider->getCurrentValue();
	CEGUI::Editbox* TEbox = static_cast<CEGUI::Editbox*>(TextureSizeEditbox);
	std::ostringstream ss5;
	int ival = (int) val;
        ival = ival/513;
	ival *= 513;
	if (ival == 0) {ival = 513;}
	ss5<<ival;
	TSlider->setCurrentValue((float)ival);
	TEbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateTEB(const CEGUI::EventArgs &e){
	
	CEGUI::Slider* TSlider = static_cast<CEGUI::Slider*>(Texturesizeslider);
	CEGUI::Editbox* TEbox = static_cast<CEGUI::Editbox*>(TextureSizeEditbox);
	float val = atof(TEbox->getText().c_str());
	//float val = FSlider->getCurrentValue();
	
	std::ostringstream ss5;
	int ival = (int) val;
        ival = ival/513;
	ival *= 513;
	if (ival == 0) {ival = 513;}
	ss5<<ival;
	TSlider->setCurrentValue((float)ival);
	TEbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateNS(const CEGUI::EventArgs &e){
	
	CEGUI::Slider* NSlider = static_cast<CEGUI::Slider*>(Normalslider);
	float val = NSlider->getCurrentValue();
	CEGUI::Editbox* Nbox = static_cast<CEGUI::Editbox*>(NormalEditbox);
	std::ostringstream ss5;
	ss5<<val;
	Nbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateNB(const CEGUI::EventArgs &e){
	
	CEGUI::Slider* NSlider = static_cast<CEGUI::Slider*>(Normalslider);
	CEGUI::Editbox* Nbox = static_cast<CEGUI::Editbox*>(NormalEditbox);
	float val = atof(Nbox->getText().c_str());
	NSlider->setCurrentValue((float)val);
	
}

void FreqAmpOctEventReg::updateWS(const CEGUI::EventArgs &e){
	
	CEGUI::Slider* WSlider = static_cast<CEGUI::Slider*>(Worldsizeslider);
	float val = WSlider->getCurrentValue();
	CEGUI::Editbox* Wbox = static_cast<CEGUI::Editbox*>(WorldSizeEditbox);
	std::ostringstream ss5;
	ss5<<val;
	Wbox->setText(ss5.str());
}

void FreqAmpOctEventReg::updateWB(const CEGUI::EventArgs &e){
	
	CEGUI::Slider* WSlider = static_cast<CEGUI::Slider*>(Worldsizeslider);
	CEGUI::Editbox* Wbox = static_cast<CEGUI::Editbox*>(WorldSizeEditbox);
	float val = atof(Wbox->getText().c_str());
	WSlider->setCurrentValue((float)val);
	
}
#endif
