#include "chatwindow.h"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "../game.h"

namespace Gui{
	ChatWindow::ChatWindow(tgui::Gui& gui, IGame& game) : mGame(game), mPanel(gui), mChatbox(*mPanel), mEditbox(*mPanel) {
		init();
	}

	void ChatWindow::processLine(){
		auto text = mEditbox->getText();
		if (text != ""){
			std::string stdText = text.toAnsiString();
			if (stdText[0] == '/'){
				processCheat(stdText);
			} else {
				mChatbox->addLine(text);
			}
			mEditbox->setText("");
		}
	}

	void ChatWindow::init(){
		const std::string style = "data/widgets/black.conf";
		mPanel->load(style);
		mPanel->setSize(mWidth, mHeight);
		mPanel->setPosition(500, 500);

		mChatbox->load(style);
		mChatbox->setSize(mWidth, mHeight - mEditboxHeight - 2);
		mChatbox->setTextSize(14);
		mChatbox->setPosition(0, 0);

		//This line crashes
		mEditbox->load(style);
		mEditbox->setPosition(0, mHeight - mEditboxHeight);
		mEditbox->setSize(mWidth, mEditboxHeight);
		mEditbox->bindCallback(&ChatWindow::processLine, this, tgui::EditBox::ReturnKeyPressed);
	}

	void ChatWindow::processCheat(const std::string& line){
		std::stringstream stream(line);
		std::string cheat;
		stream >> cheat;
		if (cheat == "/load"){
			std::string mapname = "";
			stream >> mapname;
			mGame.load(mapname);
		} else if (cheat == "/save"){
			std::string mapname = "";
			stream >> mapname;
			mGame.save(mapname);
		} else if (cheat == "/mapgen"){
			std::string biome = "";
			int seed = 0;
			stream >> biome >> seed;
			mGame.generate(biome, seed);
		}
	}

	void ChatWindow::resizeScreen(int screenWidth, int screenHeight){
		int halfScreenWidth = screenWidth / 2;
		int halfScreenHeight = screenHeight / 2;

		//I have no idea how this technically works, but it seems to move the widget to the bottem-right location.
		auto size = mPanel->getFullSize();
		mPanel->setPosition(500 - halfScreenWidth, 500 + halfScreenHeight - size.y);
	}
}
