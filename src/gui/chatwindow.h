#pragma once
#include <string>
#include <SFGUI/SFGUI.hpp>

class IGame;

namespace Gui{
	class ChatWindow : public sfg::Window{
		static constexpr int mWidth = 300;
		static constexpr int mHeight = 130;
		static constexpr int mEditboxHeight = 30;

		sfg::Label::Ptr mChatbox;
		sfg::Entry::Ptr mTextBox;

		IGame& mGame;
	public:
		typedef std::shared_ptr<ChatWindow> Ptr;

		static ChatWindow::Ptr Create(sfg::Desktop& desktop, IGame& game, std::uint8_t style = Style::TOPLEVEL);
		ChatWindow(sfg::Desktop& desktop, IGame& game, std::uint8_t style);
		void processLine();
		void processCheat(const std::string& line);
		void init(sfg::Desktop& desktop);
	};
}

