#pragma once
#include <TGUI/TGUI.hpp>

class IGame;

namespace Gui{
	class ChatWindow{
		tgui::ChildWindow::Ptr mWindow;
		tgui::ChatBox::Ptr mChatbox;
		tgui::TextBox::Ptr mTextbox;
	public:
		ChatWindow(tgui::Gui& gui, IGame& game);
		void processLine();
		void init();
	};
}
