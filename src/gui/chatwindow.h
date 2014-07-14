#pragma once
#include <TGUI/TGUI.hpp>

class IGame;

namespace Gui{
	class ChatWindow{
		IGame& mGame;
		tgui::ChildWindow::Ptr mWindow;
		tgui::ChatBox::Ptr mChatbox;
		tgui::EditBox::Ptr mEditbox;
	public:
		ChatWindow(tgui::Gui& gui, IGame& game);
		void processLine();
		void processCheat(const std::string& line);
		void resizeScreen(int width, int height);
		void init();
	};
}
