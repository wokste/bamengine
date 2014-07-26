#pragma once
#include <string>
#include <SFGUI/SFGUI.hpp>

class IGame;

namespace Gui{
	class ChatWindow{
		static constexpr int mWidth = 300;
		static constexpr int mHeight = 130;
		static constexpr int mEditboxHeight = 30;

		sfg::Window::Ptr mWindow;
		sfg::Label::Ptr mChatbox;
		sfg::Entry::Ptr mTextBox;

		IGame& mGame;
	public:
		ChatWindow(sfg::Desktop& desktop, IGame& game);
		void processLine();
		void processCheat(const std::string& line);
		void resizeScreen(int width, int height);
		void init(sfg::Desktop& desktop);
	};
}

