#pragma once
#include <SFGUI/SFGUI.hpp>

class IGame;

namespace Gui{
	class DockingWindow : public sfg::Window{
		float mDockX;
		float mDockY;
	public:
		DockingWindow(std::uint8_t style);
		~DockingWindow() override;
		void dock(float x, float y);

		void HandleEvent( const sf::Event& event ) override;
		void onScreenResize(int width, int height);
	};
}

