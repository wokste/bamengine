#include "chatwindow.h"

#include <SFML/Graphics.hpp>
#include <sstream>

#include "../game.h"

namespace Gui{
	ChatWindow::ChatWindow(sfg::Desktop& desktop, IGame& game) : mGame(game){
		init(desktop);
	}

	void ChatWindow::processLine(){
		std::string text = mTextBox->GetText();
		if (text != ""){
			if (text[0] == '/'){
				processCheat(text);
			} else {
				std::string lines = mChatbox->GetText();

				lines += "\n";
				lines += text;

				mChatbox->SetText(lines);
			}
			mTextBox->SetText("");
		}
	}

	void ChatWindow::init(sfg::Desktop& desktop){
		mWindow = sfg::Window::Create();
		mWindow->SetTitle( "Hello world!" );
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
	}
}

