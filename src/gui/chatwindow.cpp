#include "chatwindow.h"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace Gui{
	ChatWindow::ChatWindow(tgui::Gui& gui, IGame& game) : mWindow(gui), mChatbox(*mWindow), mEditbox(*mWindow) {
		init();
	}

	void ChatWindow::processLine(){
		auto text = mEditbox->getText();
		if (text != ""){
			mChatbox->addLine(text);
			mEditbox->setText("");
		}
	}

	void ChatWindow::init(){
		const std::string stlye = "data/widgets/black.conf";

		mWindow->load(stlye);
		mWindow->setSize(300, 130);
		mWindow->setPosition(300, 360);
		mWindow->setTitle("Console");

		mChatbox->load(stlye);
		mChatbox->setSize(300, 98);
		mChatbox->setTextSize(14);
		mChatbox->setPosition(0, 0);

		//This line crashes
		mEditbox->load(stlye);
		mEditbox->setPosition(0, 100);
		mEditbox->setSize(300, 30);
		mEditbox->bindCallback(&ChatWindow::processLine, this, tgui::EditBox::ReturnKeyPressed);
	}
}
