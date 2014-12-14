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
#include "Combiner.cpp"
#include "Voronoi.cpp"
#include <map>
#include <string>
#include <iostream>

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
		void updateW(const CEGUI::EventArgs &e);
		void updateN(const CEGUI::EventArgs &e);
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
		CEGUI::Window *FilterTypebox;
		CEGUI::Window *Weightslider;
		CEGUI::Window *NoiseTypebox;
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
	Ogre::Log* tlog = Ogre::LogManager::getSingleton().createLog("Voronoi.log");
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
	Weightslider = cnewWindow ->getChild("WeightSlider");
	UpdateButton2 = cnewWindow ->getChild("UpdateButton2");
	Terrainbox = cnewWindow ->getChild("Terrainbox");
	FilterTypebox = cnewWindow ->getChild("FilterTypebox");
	NoiseTypebox = cnewWindow ->getChild("NoiseTypebox");
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

	Frequencyslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateF, this));
	Amplitudeslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateA, this));
	Octavesslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateO, this));
	Gainslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateG, this));
	Scaleslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&FreqAmpOctEventReg::updateS, this));
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
	settings["Octaves"] = 2.0f;
	settings["Gain"] = .5f;
	settings["Scale"] = 1.0f/513.0f;
	settings["Maxheight"] = 1500.0f;
	settings["Weight"] = .5f;
	settings["NoiseType"] = 0.0f;
	
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
	CEGUI::Slider* WSlider = static_cast<CEGUI::Slider*>(Weightslider);
	float wval = WSlider->getCurrentValue();
	CEGUI::Combobox* combobox = static_cast<CEGUI::Combobox*>(Terrainbox);
	CEGUI::Combobox* ncombobox = static_cast<CEGUI::Combobox*>(NoiseTypebox);
	CEGUI::ListboxItem* nlitem = ncombobox->getSelectedItem();
	float nameID = (float)nlitem->getID();
	map<string, float> settings;
	ss5 << "Test update config" << "\n";
	ss5 << "Noise type nameID: "<< nameID << "\n";
	tlog->logMessage(ss5.str());	
	settings["Frequency"] = fval;
	settings["Amplitude"] = aval;
	settings["Octaves"] = oval;
	settings["Gain"] = gval;
	settings["Scale"] = sval;
	settings["Maxheight"] = mval;
	settings["Weight"] = wval;
	settings["NoiseType"] = nameID-1;
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
	float fval, aval, oval, gval, sval, mval, wval, nval;
	fval = settings["Frequency"];
	aval = settings["Amplitude"];
	oval = settings["Octaves"];
	gval = settings["Gain"];
	sval = settings["Scale"];
	mval = settings["Maxheight"];
	wval = settings["Weight"];
	nval = settings["NoiseType"];
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
	CEGUI::Slider* WSlider = static_cast<CEGUI::Slider*>(Weightslider);
	WSlider->setCurrentValue(wval);	

	CEGUI::Combobox* ncombobox = static_cast<CEGUI::Combobox*>(NoiseTypebox);
	CEGUI::ListboxTextItem* itemCombobox;
	if ((int)nval == 1){
		itemCombobox = new CEGUI::ListboxTextItem("Advanced Perlin", 1);
	}
	else{
		itemCombobox = new CEGUI::ListboxTextItem("Voronoi", 2);
	}
	ncombobox->setItemSelectState 	((int)nval,true);
	
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

void FreqAmpOctEventReg::updateW(const CEGUI::EventArgs &e){
	CEGUI::Window* wlabel = cnewWindow ->getChild("Weightlabel");
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
		if ((int)tc["NoiseType"] == 0){
			
			PerlinTest* test = new PerlinTest(513.0f, tc["Scale"], 2.0f, tc["Frequency"], tc["Amplitude"], tc["Gain"], int(3.0f));
			tnoisevals[i] = test->getNoisevalues();
			ss5 << "Hitting update button!!!!!!!" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
		}
		else if ((int)tc["NoiseType"]==1){
			ss5 << "2 on noise !!!!!!!" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			Voronoi* test = new Voronoi(25, 513);
			ss5 << "21 on noise !!!!!!!" << "\n";
			tlog->logMessage(ss5.str());
			ss5.str(std::string());
			tnoisevals[i] = test->getHeightMapradial(tc["Scale"]);
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
	vector<vector<vector<double> > > heightmapvalues = cmb -> Combine(tnoisevals, weights);
//	PerlinTest* test = new PerlinTest(513.0f,sval, 2.0f, fval,aval,gval, int(3.0f)); //keep by default third arg 2.0f..don't tweak won't work for higher or lower vals
//	vector<vector<vector<double> > > heightmapvalues = test->getNoisevalues();
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
