#include "chatwindow.h"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "../game.h"

namespace Gui{
	ChatWindow::ChatWindow(tgui::Gui& gui, IGame& game) : mGame(game), mWindow(gui), mChatbox(*mWindow), mEditbox(*mWindow) {
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

	void ChatWindow::resizeScreen(int width, int height){

	}
}
