#pragma once
#include "dockingwindow.h"

class IGame;

namespace Gui{
	class ChatWindow : public DockingWindow{
		static constexpr int mWidth = 300;
		static constexpr int mHeight = 130;
		static constexpr int mTextboxHeight = 30;

		sfg::Label::Ptr mChatbox;
		sfg::Entry::Ptr mTextbox;

		IGame& mGame;
	public:
		typedef std::shared_ptr<ChatWindow> Ptr;

		static ChatWindow::Ptr Create(IGame& game, std::uint8_t style = Style::BACKGROUND);
		ChatWindow(IGame& game, std::uint8_t style);
		~ChatWindow() override;
		void processLine();
		void processCheat(const std::string& line);
		void init();
	};
}

