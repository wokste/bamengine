#include "chatwindow.h"

#include <SFML/Graphics.hpp>
#include <sstream>

#include "../game.h"

namespace Gui{
	ChatWindow::Ptr ChatWindow::Create(IGame& game, std::uint8_t style){
		ChatWindow::Ptr window( new ChatWindow( game, style ) );
		window->init();
		window->RequestResize();
		return window;
	}

	ChatWindow::ChatWindow(IGame& game, std::uint8_t style) : DockingWindow(style), mGame(game){
		SetTitle( "Console" );
	}

	ChatWindow::~ChatWindow(){
	}

	void ChatWindow::init(){
		SetRequisition(sf::Vector2f(mWidth,mHeight));
		dock(0, 1);

		auto fixed = sfg::Fixed::Create();
		mChatbox = sfg::Label::Create("Hello World");
		mChatbox->SetRequisition(sf::Vector2f(mWidth,mHeight - mTextboxHeight));
		mTextbox = sfg::Entry::Create("Hi there");
		mTextbox->SetRequisition(sf::Vector2f(mWidth - 100,mTextboxHeight));
		auto button = sfg::Button::Create("ok");
		button->SetRequisition(sf::Vector2f(100, mTextboxHeight));
		button->GetSignal( sfg::Button::OnLeftClick ).Connect( std::bind( &ChatWindow::processLine, this ));

		fixed->Put(mChatbox, sf::Vector2f( 0.f, 0.f ));
		fixed->Put(mTextbox, sf::Vector2f( 0.f, mHeight - mTextboxHeight ));
		fixed->Put(button, sf::Vector2f(mWidth - 100, mHeight - mTextboxHeight ));
		Add(fixed);
	}

	void ChatWindow::processLine(){
		std::string text = mTextbox->GetText();
		if (text != ""){
			if (text[0] == '/'){
				processCheat(text);
			} else {
				std::string lines = mChatbox->GetText();
				lines += "\n";
				lines += text;
				mChatbox->SetText(lines);
			}
			mTextbox->SetText("");
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

