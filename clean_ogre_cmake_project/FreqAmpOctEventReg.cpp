#ifndef __FreqAmpOctEventReg_CPP
#define __FreqAmpOctEventReg_CPP

#include "PerlinTest.cpp"
#include <iostream>
#include <string>
#include <sstream>
#include <CEGUI/CEGUI.h>
//#include <CEGUISchemeManager.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include "BaseApplication.h"
#include "LoadHeightMap.cpp"
#include "ScaleHeights.cpp"
#include <map>
#include <string>

using namespace std;

class FreqAmpOctEventReg{
	public:
		FreqAmpOctEventReg(Ogre::Terrain* terrain, Ogre::Camera* Camera);
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
	private:
		CEGUI::Window *cnewWindow;
		CEGUI::Window *Frequencyslider;
		CEGUI::Window *Amplitudeslider;
		CEGUI::Window *Octavesslider;
		CEGUI::Window *Gainslider;
		CEGUI::Window *Scaleslider;
		CEGUI::Window *Maxheightslider;
		CEGUI::Window *UpdateButton;
		CEGUI::Window *UpdateButton2;
		CEGUI::Window *Terrainbox;
		Ogre::Terrain* cterrain;
		Ogre::Camera* mCamera;
		map<int,CEGUI::String> csel;
		map<int,map<string, float> > config; 
		void addconfig(int nameID);
		void updateConfig(void);
		void loadConfig(void);
};



FreqAmpOctEventReg::FreqAmpOctEventReg(Ogre::Terrain* terrain, Ogre::Camera* Camera){
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().createLog("ScaleHeight.log");
	cterrain = terrain;
	mCamera = Camera;
	cnewWindow = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("DemoWindow");
	Frequencyslider = cnewWindow ->getChild("FrequencySlider");
	Amplitudeslider = cnewWindow ->getChild("AmplitudeSlider");
	Octavesslider = cnewWindow ->getChild("OctavesSlider");
	UpdateButton = cnewWindow ->getChild("UpdateButton");
	Gainslider = cnewWindow ->getChild("GainSlider");
	Scaleslider = cnewWindow ->getChild("ScaleSlider");
	Maxheightslider = cnewWindow ->getChild("MaxHeightSlider");
	UpdateButton2 = cnewWindow ->getChild("UpdateButton2");
	Terrainbox = cnewWindow ->getChild("Terrainbox");
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
//	Terrainbox->addItem(listboxitem2);
	Frequencyslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateF, this));
	Amplitudeslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateA, this));
	Octavesslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateO, this));
	Gainslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateG, this));
	Scaleslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateS, this));
	Maxheightslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateM, this));
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
	settings["Octaves"] = 2.0f;
	settings["Gain"] = .5f;
	settings["Scale"] = 1.0f/513.0f;
	settings["Maxheight"] = 1500.0f;
	
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
	CEGUI::Slider* MSlider = static_cast<CEGUI::Slider*>(Maxheightslider);
	float mval = MSlider->getCurrentValue();
	CEGUI::Combobox* combobox = static_cast<CEGUI::Combobox*>(Terrainbox);
	map<string, float> settings;
	ss5 << "Test update config" << "\n";
	tlog->logMessage(ss5.str());	
	settings["Frequency"] = fval;
	settings["Amplitude"] = aval;
	settings["Octaves"] = oval;
	settings["Gain"] = gval;
	settings["Scale"] = sval;
	settings["Maxheight"] = mval;
	ss5 << "Test update config" << "\n";
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
	float fval, aval, oval, gval, sval, mval;
	fval = settings["Frequency"];
	aval = settings["Amplitude"];
	oval = settings["Octaves"];
	gval = settings["Gain"];
	sval = settings["Scale"];
	mval = settings["Maxheight"];
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
	CEGUI::Slider* MSlider = static_cast<CEGUI::Slider*>(Maxheightslider);
	MSlider->setCurrentValue(mval);
	
}

void FreqAmpOctEventReg::updateF(const CEGUI::EventArgs &e){
	CEGUI::Window* flabel = cnewWindow ->getChild("Freqlabel");
	CEGUI::Slider* FSlider = static_cast<CEGUI::Slider*>(Frequencyslider);
	float val = FSlider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
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
	CEGUI::Window* mlabel = cnewWindow ->getChild("MHeightlabel");
	CEGUI::Slider* MSlider = static_cast<CEGUI::Slider*>(Maxheightslider);
	float val = MSlider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	mlabel->setText(ss5.str());
	//if (val > 1.0f) {
	//	new ScaleHeights(cterrain, mCamera, val);
	//}
}

void FreqAmpOctEventReg::updateB(const CEGUI::EventArgs &e){
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
	PerlinTest* test = new PerlinTest(513.0f,sval, 2.0f, fval,aval,gval, int(3.0f)); //keep by default third arg 2.0f..don't tweak won't work for higher or lower vals
	vector<vector<vector<double> > > heightmapvalues = test->getNoisevalues();
	new LoadHeightMap(cterrain, 513.0f*3.0f, heightmapvalues);
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
