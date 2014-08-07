#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <SFGUI/SFGUI.hpp>

#include "src/game.h"
#include "src/gui/bamgui.h"
#include "src/util/assert.h"

void setSize(sf::RenderTarget& target, float width, float height);

class Program{
	constexpr static float updatesPerSecond = 25;
	constexpr static float maxFrameskip = 5;

	Gui::BamGui gui;
	sf::RenderWindow app;
	std::unique_ptr<IGame> game;

public:
	Program() : app(sf::VideoMode(1000, 600), "BAM Engine"){
		game = IGame::factory();
		gui.init(*game);
		resize(app, 1000, 600);
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
				gui.handleEvent(event);

				if (event.type == sf::Event::Closed){
					app.close();
				}

				if (event.type == sf::Event::Resized){
					resize(app, event.size.width, event.size.height);
				}
			}

			while( clock.getElapsedTime() > next_game_tick && loops < maxFrameskip) {
				game->logic(!gui.hasFocus());
				gui.logic();

				next_game_tick += sf::seconds(1.0 / updatesPerSecond);
				loops++;
			}
			loops = 0;

			// Show the frame
			float interpolation = (clock.getElapsedTime() + sf::seconds(1.0 / updatesPerSecond) - next_game_tick).asSeconds() * updatesPerSecond;

			app.clear();

			game->display(app, interpolation);

			gui.display(app);

			app.display();
		}
	}

	void resize(sf::RenderTarget& target, float width, float height){
		float zoom = 2;

		sf::View gameView;
		gameView.setSize(width / zoom, height / zoom);
		app.setView(gameView);
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
