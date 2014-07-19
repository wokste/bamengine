#include "bamgui.h"
#include "chatwindow.h"

namespace Gui{
	class BamGui : public IBamGui{
		ChatWindow chatWindow;

	public:
		BamGui(tgui::Gui& gui, IGame& game)
			: chatWindow(gui, game)
		{
		}

		virtual void resize(int width, int height){
			chatWindow.resizeScreen(width, height);
		}
	};

	std::unique_ptr<IBamGui> IBamGui::factory(tgui::Gui& gui, IGame& game){
		return std::unique_ptr<IBamGui>(new BamGui(gui, game));
	}
}
