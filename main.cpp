#include <SFML/Graphics.hpp>
#include <memory>

#include "src/game.h"

void setSize(sf::RenderTarget& target, float width, float height);

int main(){
	constexpr float updatesPerSecond = 25;
	constexpr float maxFrameskip = 5;

	// Create the main window
	sf::RenderWindow app(sf::VideoMode(800, 600), "BAM Engine");
	setSize(app, 800, 600);

	std::unique_ptr<IGame> game = IGame::factory();

	sf::Time next_game_tick;
	sf::Clock clock;
	int loops;
	int frames;

	// Start the game loop
	while (app.isOpen())
	{
		// Process events
		sf::Event event;
		while (app.pollEvent(event)){
			// Close window : exit
			if (event.type == sf::Event::Closed)
				app.close();
			if (event.type == sf::Event::Resized){
				setSize(app, event.size.width, event.size.height);
			}
		}

		while( clock.getElapsedTime() > next_game_tick && loops < maxFrameskip) {
			game->logic();

			next_game_tick += sf::seconds(1.0 / updatesPerSecond);
			loops++;
		}
		loops = 0;

		// Calculate fps
		frames++;

		// Show the frame
		float interpolation = (clock.getElapsedTime() + sf::seconds(1.0 / updatesPerSecond) - next_game_tick).asSeconds() * updatesPerSecond;
		app.clear();
		game->display(app, interpolation);
		app.display();
	}

	return EXIT_SUCCESS;
}

void setSize(sf::RenderTarget& target, float width, float height){
	float zoom = 2;
	sf::View v = target.getView();
	v.setSize(width / zoom, height / zoom);
	target.setView(v);
}
