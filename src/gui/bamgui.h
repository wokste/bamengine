#pragma once
#include <memory>
#include <SFGUI/SFGUI.hpp>
class IGame;

namespace Gui{
	class BamGui {
		public:
			void buttonClick();
			void init();
			void display(sf::RenderWindow& target);
			void handleEvent(sf::Event& event);
			void logic();
			bool hasFocus();

		private:
			sfg::SFGUI mSfgui;
			sfg::Desktop mDesktop;
			sfg::Entry::Ptr mEntry;
			sfg::Label::Ptr mLabel;
	};
}
