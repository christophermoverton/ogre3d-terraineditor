#ifndef __PerlinGuiClass_CPP
#define __PerlinGuiClass_CPP
#include "PerlinTest.cpp"

class PerlinGuiClass{
	public:
		PerlinGuiClass(void);
		void update(void);
	private:
		
};

PerlinGuiClass::PerlinGuiClass(){
	CEGUI::Window updatebutton = newWindow ->getChild("UpdateButton");
	
	updatebutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PerlinGuiClass::update, this));
}

PerlinGuiClass::update(){
}
#endif
