#include "chatwindow.h"

#include <SFML/Graphics.hpp>
#include <sstream>

#include "../game.h"

namespace Gui{
	ChatWindow::Ptr ChatWindow::Create(sfg::Desktop& desktop, IGame& game, std::uint8_t style){
		ChatWindow::Ptr window( new ChatWindow( desktop, game, style ) );
		window->init(desktop);
		window->RequestResize();
		return window;
	}

	ChatWindow::ChatWindow(sfg::Desktop& desktop, IGame& game, std::uint8_t style) : sfg::Window(style), mGame(game){
		SetTitle( "Hello world!" );
	}

	void ChatWindow::init(sfg::Desktop& desktop){
/*		SetRequisition(sf::Vector2f(50,50));
		SetPosition(sf::Vector2f(50,50));

		auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 1.0f);
		Add(box);
		mChatbox = sfg::Label::Create("Hello World");
		mTextBox = sfg::Entry::Create("Hi there");

		box->Pack(mChatbox);
		box->Pack(mTextBox);*/
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
}

