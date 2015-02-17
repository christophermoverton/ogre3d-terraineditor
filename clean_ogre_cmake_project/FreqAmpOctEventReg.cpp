#ifndef __FreqAmpOctEventReg_CPP
#define __FreqAmpOctEventReg_CPP

#include "PerlinTest.cpp"
#include "BuildVoronoi.cpp"
#include "BuildSimplex.cpp"
#include "BuildFbm.cpp"
#include "DiamondSquare.cpp"
#include "Musgrave.cpp"
#include "BuildPerlin.cpp"
#include <iostream>
#include <string>
#include <sstream>
#include <CEGUI/CEGUI.h>
//#include <CEGUISchemeManager.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include "BaseApplication.h"
#include "LoadHeightMap.cpp"
#include "ScaleHeights.cpp"
#include "Combiner.cpp"
#include "Voronoi.cpp"
#include <map>
#include <string>
#include <iostream>

using namespace std;

class FreqAmpOctEventReg{
	public:
		FreqAmpOctEventReg(Ogre::Terrain* terrain, Ogre::Camera* Camera, Ogre::Rectangle2D* mMiniScreen);
		void updateF(const CEGUI::EventArgs &e);
		void updateA(const CEGUI::EventArgs &e);
		void updateO(const CEGUI::EventArgs &e);
		void updateB(const CEGUI::EventArgs &e);
		void updateG(const CEGUI::EventArgs &e);
		void updateS(const CEGUI::EventArgs &e);
		void updateM(const CEGUI::EventArgs &e);
		void updateBB(const CEGUI::EventArgs &e);
		void updateT(const CEGUI::EventArgs &e);
		void updateTD(const CEGUI::EventArgs &e);
		void updateW(const CEGUI::EventArgs &e);
		void updateN(const CEGUI::EventArgs &e);
		void updateFD(const CEGUI::EventArgs &e);
		void updateD(const CEGUI::EventArgs &e);
	private:
		CEGUI::Window *cnewWindow;
		CEGUI::Window *tcnewWindow;
		CEGUI::Window *DefaultWindow;
		CEGUI::Window *TabControlWindow;
		CEGUI::Window *HydraulicWindow;
		CEGUI::Window *MarbleWindow;
		CEGUI::Window *ShowDataWindow;
		CEGUI::Window *Frequencyslider;
		CEGUI::Window *Amplitudeslider;
		CEGUI::Window *Octavesslider;
		CEGUI::Window *Gainslider;
		CEGUI::Window *Scaleslider;
		CEGUI::Window *Freqdivslider;
		CEGUI::Window *Maxheightslider;
		CEGUI::Window *UpdateButton;
		CEGUI::Window *UpdateButton2;
		CEGUI::Window *Terrainbox;
		CEGUI::Window *FilterTypebox;
		CEGUI::Window *Weightslider;
		CEGUI::Window *NoiseTypebox;
		CEGUI::Window *Depthslider;
		CEGUI::Window *Biasbox;
		CEGUI::Window *Shapebox;
		CEGUI::Window *Sharpbox;
		CEGUI::Window *NoiseBasisbox;
		Ogre::Terrain* cterrain;
		Ogre::Camera* mCamera;
		Ogre::Rectangle2D* cmMiniScreen;
		map<int,CEGUI::String> csel;
		map<int,map<string, float> > config; 
		void addconfig(int nameID);
		void updateConfig(void);
		void loadConfig(void);
};



FreqAmpOctEventReg::FreqAmpOctEventReg(Ogre::Terrain* terrain, Ogre::Camera* Camera, Ogre::Rectangle2D* mMiniScreen){
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().createLog("ScaleHeight.log");
	Ogre::Log* slog = Ogre::LogManager::getSingleton().createLog("Voronoi.log");
	cterrain = terrain;
	mCamera = Camera;
	cmMiniScreen = mMiniScreen;
	//cnewWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("DemoWindow"); default keep 
        tcnewWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("TerrainWindow");
	cnewWindow = tcnewWindow->getChild("TerrainEditWindow");
	HydraulicWindow = tcnewWindow->getChild("HydraulicWindow");
	MarbleWindow = tcnewWindow->getChild("MarbleWindow");
	TabControlWindow = tcnewWindow->getChild("DefaultWindow")->getChild("TabControl");
	DefaultWindow = tcnewWindow->getChild("DefaultWindow");
	ShowDataWindow = tcnewWindow->getChild("ShowDataWindow");
	CEGUI::TabControl * tabcontrol = static_cast<CEGUI::TabControl*>(TabControlWindow);
	tabcontrol->addTab(cnewWindow);
	tabcontrol->addTab(HydraulicWindow);
	tabcontrol->addTab(MarbleWindow);
	tabcontrol->addTab(ShowDataWindow);
	Frequencyslider = cnewWindow ->getChild("FrequencySlider");
	Amplitudeslider = cnewWindow ->getChild("AmplitudeSlider");
	Octavesslider = cnewWindow ->getChild("OctavesSlider");
	UpdateButton = DefaultWindow ->getChild("UpdateButton");
	Gainslider = cnewWindow ->getChild("GainSlider");
	Scaleslider = cnewWindow ->getChild("ScaleSlider");
	Freqdivslider = cnewWindow ->getChild("FreqDivSlider");
	Depthslider = cnewWindow ->getChild("DepthSlider");
	Maxheightslider = DefaultWindow ->getChild("MaxHeightSlider");
	Weightslider = DefaultWindow ->getChild("WeightSlider");
	UpdateButton2 = DefaultWindow ->getChild("UpdateButton2");
	Terrainbox = cnewWindow ->getChild("Terrainbox");
	FilterTypebox = cnewWindow ->getChild("FilterTypebox");
	NoiseTypebox = cnewWindow ->getChild("NoiseTypebox");
	NoiseBasisbox = MarbleWindow ->getChild("NoiseBasisbox");
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
	itemCombobox = new CEGUI::ListboxTextItem("Advanced Perlin", 1);
	combobox->addItem(itemCombobox);
	combobox->setItemSelectState 	(itemCombobox,true); 
	itemCombobox = new CEGUI::ListboxTextItem("Voronoi", 2);
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
	Amplitudeslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateA, this));
	Octavesslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateO, this));
	Gainslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateG, this));
	Scaleslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateS, this));
	Freqdivslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateFD, this));
	Depthslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateD, this));
	Maxheightslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateM, this));
	Weightslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateW, this));
	Terrainbox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateT, this));	//EventDropListDisplayed
	Terrainbox->subscribeEvent(CEGUI::Combobox::EventDropListDisplayed, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateTD, this));
	 UpdateButton->subscribeEvent(CEGUI::PushButton::EventClicked,
    CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateB, this));
	 UpdateButton2->subscribeEvent(CEGUI::PushButton::EventClicked,
    CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateBB, this));
}

void FreqAmpOctEventReg::addconfig(int nameID){
	map<string, float> settings;
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
	config[nameID] = settings;
}

void FreqAmpOctEventReg::updateConfig(){

	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("ScaleHeight.log");
	std::ostringstream ss5;
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
	map<string, float> settings;
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
	map<string, float> settings = config[nameID];
	float fval, aval, oval, gval, sval, fdval, mval, wval, nval, dval, nbval, bval, shapeval, sharpval;
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
	ss5 << "Test load config" << "\n";
	tlog->logMessage(ss5.str());
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

	CEGUI::Combobox* ncombobox = static_cast<CEGUI::Combobox*>(NoiseTypebox);
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
	ncombobox->setItemSelectState 	((int)nval,true);

	ncombobox = static_cast<CEGUI::Combobox*>(NoiseBasisbox);
	ncombobox->setItemSelectState 	((int)nbval,true);

	ncombobox = static_cast<CEGUI::Combobox*>(Biasbox);
	ncombobox->setItemSelectState 	((int)bval,true);

	ncombobox = static_cast<CEGUI::Combobox*>(Shapebox);
	ncombobox->setItemSelectState 	((int)shapeval,true);

	ncombobox = static_cast<CEGUI::Combobox*>(Sharpbox);
	ncombobox->setItemSelectState 	((int)sharpval,true);
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
	float dval = FdSlider->getCurrentValue();
	if (dval == 0.0f){
		dval = 1.0f;
	}
	std::ostringstream ss5;
	ss5<<val/dval;
	flabel->setText(ss5.str());
}

void FreqAmpOctEventReg::updateD(const CEGUI::EventArgs &e){
	CEGUI::Window* dlabel = cnewWindow ->getChild("Depthlabel");
	CEGUI::Slider* DSlider = static_cast<CEGUI::Slider*>(Depthslider);
	float val = DSlider->getCurrentValue();

	if (val == 0.0f){
		val = .01f;
	}
	std::ostringstream ss5;
	ss5<<val;
	dlabel->setText(ss5.str());
}

void FreqAmpOctEventReg::updateFD(const CEGUI::EventArgs &e){
	CEGUI::Window* flabel = cnewWindow ->getChild("Freqlabel");
	CEGUI::Slider* FSlider = static_cast<CEGUI::Slider*>(Frequencyslider);
	float val = FSlider->getCurrentValue();
	CEGUI::Slider* FdSlider = static_cast<CEGUI::Slider*>(Freqdivslider);
	float dval = FdSlider->getCurrentValue();
	if (dval == 0.0f){
		dval = 1.0f;
	}
	std::ostringstream ss5;
	ss5<<val/dval;
	flabel->setText(ss5.str());
}

void FreqAmpOctEventReg::updateA(const CEGUI::EventArgs &e){
	CEGUI::Window* alabel = cnewWindow ->getChild("Amplabel");
	CEGUI::Slider* ASlider = static_cast<CEGUI::Slider*>(Amplitudeslider);
	float val = ASlider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	alabel->setText(ss5.str());
}

void FreqAmpOctEventReg::updateO(const CEGUI::EventArgs &e){
	CEGUI::Window* olabel = cnewWindow ->getChild("Octaveslabel");
	CEGUI::Slider* OSlider = static_cast<CEGUI::Slider*>(Octavesslider);
	float val = OSlider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	olabel->setText(ss5.str());
}

void FreqAmpOctEventReg::updateG(const CEGUI::EventArgs &e){
	CEGUI::Window* glabel = cnewWindow ->getChild("Gainlabel");
	CEGUI::Slider* GSlider = static_cast<CEGUI::Slider*>(Gainslider);
	float val = GSlider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	glabel->setText(ss5.str());
}

void FreqAmpOctEventReg::updateS(const CEGUI::EventArgs &e){
	CEGUI::Window* slabel = cnewWindow ->getChild("Scalelabel");
	CEGUI::Slider* SSlider = static_cast<CEGUI::Slider*>(Scaleslider);
	float val = SSlider->getCurrentValue();

	std::ostringstream ss5;
	ss5<<val;
	slabel->setText(ss5.str());
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
		ss5 << "Starting noise gen loop!!!!!!!" << "\n";
		ss5 << "NoiseType: " << miter3->second["NoiseType"] << "\n";
		tlog->logMessage(ss5.str());
		ss5.str(std::string());		
		map<string, float> tc = miter3->second;
		if (tc["Freqdiv"] == 0){tc["Freqdiv"] = 1.0f;}
		if ((int)tc["NoiseType"] == 0){
			//float size, float scale, float zdepth, float frequency, float amplitude, float octaves, float gain = .5f, int imagemap = (int)3.0f
			//PerlinTest* test = new PerlinTest(513.0f, tc["Scale"], 2.0f, tc["Frequency"]/tc["Freqdiv"], tc["Amplitude"], tc["Octaves"], int(3.0f));
			//tnoisevals[i] = test->getNoisevalues();
			//(*tnoisevalsm)[i] = test->getNoisevaluesT();
			//double size, double scale, double frequency, double amplitude, double octaves, double gain
			(*vnoisevalsm)[i] = pfbmbuild (513.0f, tc["Scale"], tc["Frequency"]/tc["Freqdiv"], tc["Amplitude"], tc["Octaves"],
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
			//Voronoi* test = new Voronoi(tc["Scale"], 513);
			BuildVoronoi * testm = new BuildVoronoi();  //fortune algorithim implementation (faster)
			ss5 << "21 on noise !!!!!!!" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			//tnoisevals[i] = test->getHeightMapradial(tc["Scale"]);//->getSimpleHeightMap();
			(*tnoisevalsm)[i] = testm->getHeightMapradial();
			(*vnoisevalsm)[i] = testm->getHeightMapradial2();
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
			(*vnoisevalsm)[i] = multiFractalbuild (513.0f, tc["Scale"], tc["Depth"], tc["Frequency"], tc["Octaves"],
				tc["Gain"]);
			//(*vnoisevalsm)[i] = testm->getHeightMap2();
		}
		else if ((int)tc["NoiseType"]==6){
			ss5 << "MHetero" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			//Heterobuild (double size, double scale, double H, double lucunarity, double octaves, 
                          //double offset)

			(*vnoisevalsm)[i] = Heterobuild(513.0f, tc["Scale"], tc["Depth"], tc["Frequency"], tc["Octaves"],
				tc["Gain"]);
			//(*vnoisevalsm)[i] = testm->getHeightMap2();
		}
		else if ((int)tc["NoiseType"]==7){
			ss5 << "MHybrid" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());

			//HybridMultifractalbuild (double size, double scale, double H, double lucunarity, double octaves, 
                          //double offset)
			(*vnoisevalsm)[i] = HybridMultifractalbuild(513.0f, tc["Scale"], tc["Depth"], tc["Frequency"], tc["Octaves"],
				tc["Gain"]);
			//(*vnoisevalsm)[i] = testm->getHeightMap2();
		}
		else if ((int)tc["NoiseType"]==8){
			ss5 << "MRiged" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());

			//RidgedMultifractalbuild (double size, double scale, double H, double lucunarity, double octaves, 
                          //double offset, double gain)
			(*vnoisevalsm)[i] = RidgedMultifractalbuild(513.0f, tc["Scale"], tc["Depth"], tc["Frequency"], tc["Octaves"],
				tc["Gain"], tc["Gain"]*2);
			//(*vnoisevalsm)[i] = testm->getHeightMap2();
		}
		else if ((int)tc["NoiseType"]==9){
			ss5 << "Marble" << "\n";
			ss5 << tc["Octaves"] << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());

                         //pmarblebuild (double size, double scale, double frequency, double amplitude, double octaves, int noisebasis,
				//int noisebasis, int bias, int shape, int sharp)
			(*vnoisevalsm)[i] = pmarblebuild (513.0f, tc["Scale"]/265.5f, tc["Frequency"]/tc["Freqdiv"], tc["Amplitude"], 
                                                           tc["Octaves"], (int)tc["NoiseBasis"], (int)tc["Bias"], (int)tc["Shape"],
							   (int)tc["Sharp"]);
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
	new LoadHeightMap(cterrain, cmMiniScreen,513.0f*1.0f, cheightmapvaluesm);
	//new LoadHeightMap(cterrain, 513.0f*1.0f, heightmapvaluesm, 513.0f);
	//new LoadHeightMap(cterrain, 513.0f*1.0f, (*tnoisevalsm)[0]);
}

void FreqAmpOctEventReg::updateBB(const CEGUI::EventArgs &e){
	CEGUI::Slider* MSlider = static_cast<CEGUI::Slider*>(Maxheightslider);
	float val = MSlider->getCurrentValue();
	//std::ostringstream ss5;
	//ss5<<val;
	//mlabel->setText(ss5.str());
	if (val > 513.0f) {
		new ScaleHeights(cterrain, mCamera, val);
	}
}

void FreqAmpOctEventReg::updateT(const CEGUI::EventArgs &e){
	loadConfig();
}

void FreqAmpOctEventReg::updateTD(const CEGUI::EventArgs &e){
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().getLog("ScaleHeight.log");
	std::ostringstream ss5;
	ss5 << "Test update config" << "\n";
	tlog->logMessage(ss5.str());
	updateConfig();
}
#endif
