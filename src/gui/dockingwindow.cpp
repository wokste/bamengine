#include "chatwindow.h"

#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>

#include "../game.h"

namespace Gui{
	DockingWindow::DockingWindow(std::uint8_t style) : sfg::Window(style){
		dock(0, 0);
	}

	DockingWindow::~DockingWindow(){

	}

	void DockingWindow::dock(float x, float y){
		mDockX = x;
		mDockY = y;
	}

	void DockingWindow::HandleEvent( const sf::Event& event ) {
		Container::HandleEvent(event);

		if (event.type == sf::Event::Resized)
		{
			 auto size = GetRequisition();
			 SetPosition(sf::Vector2f((event.size.width - size.x) * mDockX, (event.size.height - size.y) * mDockY));
		}
	}
}
