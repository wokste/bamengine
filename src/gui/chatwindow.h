#pragma once
#include <string>

class IGame;

namespace Gui{
	class ChatWindow{
		static constexpr int mWidth = 300;
		static constexpr int mHeight = 130;
		static constexpr int mEditboxHeight = 30;

		IGame& mGame;
	public:
		ChatWindow(IGame& game);
		void processLine();
		void processCheat(const std::string& line);
		void resizeScreen(int width, int height);
		void init();
	};
}
