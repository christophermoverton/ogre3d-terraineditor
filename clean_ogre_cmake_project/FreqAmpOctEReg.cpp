#ifndef __FreqAmpOctEReg_CPP
#define __FreqAmpOctEReg_CPP
//#include "PerlinTest.cpp"
#include <iostream>
#include <string>
#include <sstream>
#include <CEGUI/CEGUI.h>
//#include <CEGUISchemeManager.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

class FreqAmpOctEReg{
	public:
		FreqAmpOctEReg(void);
		void updateF(void);
		void updateA(void);
		void updateO(void);
	
	private:
		//CEGUI::Window* cnewWindow;
		CEGUI::Window Frequencyslider;
		CEGUI::Window Amplitudeslider;
		CEGUI::Window Ocatavesslider;
	
};

FreqAmpOctEReg::FreqAmpOctEReg(){
	cnewWindow = newwindow;
	Frequencyslider = cnewWindow ->getChild("FrequencySlider");
	Amplitudeslider = cnewWindow ->getChild("AmplitudeSlider");
	Ocatvesslider = cnewWindow ->getChild("OctavesSlider");
	Frequencyslider->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&FreqAmpOctEReg::updateF, this));
	Amplitudeslider->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&FreqAmpOctEReg::updateA, this));
	Octavesslider->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&FreqAmpOctEReg::updateO, this));
}

FreqAmpOctEReg::updateF(){
	CEGUI::Window flabel = cnewWindow ->getChild("Freqlabel");
	float val = Frequencyslider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	flabel->setText(ss5.str());
}

FreqAmpOctEReg::updateA(){
	CEGUI::Window alabel = cnewWindow ->getChild("Amplabel");
	float val = Amplitudeslider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	alabel->setText(ss5.str());
}

FreqAmpOctEReg::updateO(){
	CEGUI::Window olabel = cnewWindow ->getChild("Octaveslabel");
	float val = Frequencyslider->getCurrentValue();
	std::ostringstream ss5;
	ss5<<val;
	olabel->setText(ss5.str());
}
#endif
