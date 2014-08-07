#include "bamgui.h"
#include "chatwindow.h"

namespace Gui{
	void BamGui::buttonClick(){

	}

	void BamGui::init(IGame& game){
		// Create widgets
		mChatWindow = ChatWindow::Create(game);
		mDesktop.Add(mChatWindow);
	}

	void BamGui::handleEvent(sf::Event& event){
		mDesktop.HandleEvent(event);
	}

	void BamGui::display(sf::RenderWindow& target){
		target.pushGLStates();
		target.resetGLStates();
		mSfgui.Display(target);
		target.popGLStates();
	}

	void BamGui::logic(){
		mDesktop.Update( 0.04f );
	}

	bool BamGui::hasFocus(){
		return false;
	}
}
