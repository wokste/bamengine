#pragma once
#include <memory>

class IGame;

namespace tgui{
	class Gui;
}

namespace Gui{
	class IBamGui{
		public:
			static std::unique_ptr<IBamGui> factory(tgui::Gui& gui, IGame& game);
			IBamGui() = default;
			virtual ~IBamGui() = default;
			virtual void resize(int width, int height) = 0;
	};
}
