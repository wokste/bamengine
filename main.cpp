#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <memory>

#include "src/game.h"
#include "src/gui/bamgui.h"
#include "src/util/assert.h"

void setSize(sf::RenderTarget& target, float width, float height);

class Program{
	constexpr static float updatesPerSecond = 25;
	constexpr static float maxFrameskip = 5;

	sf::View gameView;
	sf::View guiView;
	sf::RenderWindow app;
	tgui::Gui gui;
	std::unique_ptr<IGame> game;
	std::unique_ptr<Gui::IBamGui> bamGui;

public:
	Program() : app(sf::VideoMode(800, 600), "BAM Engine"), gui(app){
		gui.setGlobalFont("arial.ttf");

		game = IGame::factory();
		bamGui = Gui::IBamGui::factory(gui, *game);

		setSize(app, 800, 600);
	}

	void run(){
		sf::Time next_game_tick;
		sf::Clock clock;
		int loops;

		// Start the game loop
		while (app.isOpen())
		{
			// Process events
			sf::Event event;
			while (app.pollEvent(event)){
				// Close window : exit
				if (event.type == sf::Event::Closed){
					app.close();
				}
				if (event.type == sf::Event::Resized){
					setSize(app, event.size.width, event.size.height);
				}
				gui.handleEvent(event, false);
			}

			while( clock.getElapsedTime() > next_game_tick && loops < maxFrameskip) {
				game->logic();

				next_game_tick += sf::seconds(1.0 / updatesPerSecond);
				loops++;
			}
			loops = 0;

			// Show the frame
			float interpolation = (clock.getElapsedTime() + sf::seconds(1.0 / updatesPerSecond) - next_game_tick).asSeconds() * updatesPerSecond;

			app.setView(gameView);
			game->display(app, interpolation);

			app.setView(guiView);
			gui.draw(false);

			app.display();
		}
	}

	void setSize(sf::RenderTarget& target, float width, float height){
		float zoom = 2;
		gameView.setSize(width / zoom, height / zoom);
		guiView.setSize(width, height);
	}
};

int main(){
	Program program;
#ifdef NDEBUG
	try{
		program.run();
	} catch (Exception& ex){
		std::cout << ex.describe();
	}
#else
	program.run();
#endif

	return 0;
}
