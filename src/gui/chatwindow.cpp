#include "chatwindow.h"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace Gui{
	ChatWindow::ChatWindow(tgui::Gui& gui, IGame& game) : mWindow(gui), mChatbox(*mWindow), mTextbox(*mWindow) {
		init();
	}

	void ChatWindow::processLine(){
		auto text = mTextbox->getText();
		mChatbox->addLine(text);
		mTextbox->setText("");
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
		//mTextbox->load(stlye);
		mTextbox->setPosition(0, 100);
		mTextbox->setSize(300, 30);
		mTextbox->bindCallback(&ChatWindow::processLine, this, tgui::EditBox::ReturnKeyPressed);
	}
}
