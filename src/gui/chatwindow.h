#pragma once
#include <TGUI/TGUI.hpp>

class IGame;

namespace Gui{
	class ChatWindow{
		static constexpr int mWidth = 300;
		static constexpr int mHeight = 130;
		static constexpr int mEditboxHeight = 30;

		IGame& mGame;
		tgui::ChildWindow::Ptr mPanel;
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
