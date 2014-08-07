#pragma once
#include <memory>
#include <SFGUI/SFGUI.hpp>
#include "chatwindow.h"
class IGame;

namespace Gui{
	class BamGui {
		public:
			void buttonClick();
			void init(IGame& game);
			void display(sf::RenderWindow& target);
			void handleEvent(sf::Event& event);
			void logic();
			bool hasFocus();

		private:
			sfg::SFGUI mSfgui;
			sfg::Desktop mDesktop;
			ChatWindow::Ptr mChatWindow;
	};
}
