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
		// TODO: This can cause some minor problems
		onScreenResize(800, 600);
	}

	void DockingWindow::HandleEvent( const sf::Event& event ) {
		Container::HandleEvent(event);

		if (event.type == sf::Event::Resized)
		{
			 onScreenResize(event.size.width, event.size.height);
		}
	}

	void DockingWindow::onScreenResize(int width, int height){
		auto size = GetRequisition();
		SetPosition(sf::Vector2f((width - size.x) * mDockX, (height - size.y) * mDockY));
	}
}
