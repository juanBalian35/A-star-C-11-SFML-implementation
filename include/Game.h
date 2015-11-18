#pragma once

#include "Solver.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

class Game{
	sf::RenderWindow renderWindow;
	World world;
	Solver solver;
	bool solveFinished, isDiagonalEnabled, movingStart, movingEnd,
		startExists, endExists;
	sf::Font font;
	sf::Text operationsText, timeText, diagonalEnabledText,
		pathFoundText, noTileText;
	sf::Clock solveTime;

	void processInput();
	void processMouseInput();
	void processKeyboardInput(sf::Keyboard::Key& evt);
	void render();
	void loadTexts(int height);

	void solvePath();

	void setDiagonalTextString();
public:
	Game(int height, int width, int tileSize, sf::Vector2f startPos, sf::Vector2f endPos);

	void run();
};