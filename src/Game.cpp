#include "Game.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

Game::Game(int width, int height, int tileSize, sf::Vector2f startPos, sf::Vector2f endPos) : 
renderWindow(sf::VideoMode(width,height), "A star implementation"),
world(World(tileSize, width, height, startPos, endPos)), 
solver(tileSize, width, height, { { 0, 0 }, world }, { { 0, 0 }, world }),
solveFinished(true), isDiagonalEnabled(false), movingStart(false),
movingEnd(false), startExists(true), endExists(true){
	loadTexts(height);
}

void Game::run(){
	while (renderWindow.isOpen()){
		processInput();
		render();
	}
}


void Game::loadTexts(int height){
	//Load font
	font.loadFromFile("d:/SFMLpro/Sansation.ttf");

	const auto green = sf::Color(0, 140, 0);

	//set up texts
	operationsText.setPosition(20.f, height - 110.f);
	operationsText.setScale(0.5f, 0.5f);
	operationsText.setFont(font);
	operationsText.setColor(green);

	timeText.setPosition(20.f, height - 90.f);
	timeText.setScale(0.5f, 0.5f);
	timeText.setFont(font);
	timeText.setColor(green);

	diagonalEnabledText.setPosition(20.f, height - 70.f);
	diagonalEnabledText.setScale(0.5f, 0.5f);
	diagonalEnabledText.setFont(font);
	diagonalEnabledText.setColor(green);
	setDiagonalTextString();

	pathFoundText.setPosition(20.f, height - 50.f);
	pathFoundText.setScale(0.5f, 0.5f);
	pathFoundText.setFont(font);
	pathFoundText.setColor(green);

	noTileText.setPosition(20.f, height - 30.f);
	noTileText.setScale(0.7f, 0.7f);
	noTileText.setFont(font);
	noTileText.setColor(sf::Color::Red);
}

void Game::processMouseInput(){
	auto mousePos = sf::Mouse::getPosition(renderWindow);
	sf::Vector2f mousePosition(mousePos.x, mousePos.y);

	if (movingStart){
		world.setIDtoTile(mousePosition, tile::Start);
		movingStart = false;
		startExists = true;
		return;
	}
	if (movingEnd){
		world.setIDtoTile(mousePosition, tile::End);
		movingEnd = false;
		endExists = true;
		return;
	}

	auto tile = world.tileAt(mousePosition);
	switch (tile){
	case tile::nonWalkable:
		world.setIDtoTile(mousePosition, tile::Empty);
		break;
	case tile::Empty:
		world.setIDtoTile(mousePosition, tile::nonWalkable);
		break;
	case tile::Path:
		world.setIDtoTile(mousePosition, tile::nonWalkable);
		break;
	case tile::Start:
		world.setIDtoTile(mousePosition, tile::Empty);
		movingStart = true;
		startExists = false;
		break;
	case tile::End:
		world.setIDtoTile(mousePosition, tile::Empty);
		movingEnd = true;
		endExists = false;
	default: break;
	}
}

void Game::processKeyboardInput(sf::Keyboard::Key& key){
	switch (key){
	case sf::Keyboard::S:
		solvePath();
		break;
	case sf::Keyboard::D:
		isDiagonalEnabled = !isDiagonalEnabled;
		setDiagonalTextString();
		break;
	case sf::Keyboard::C:
		world.clearPath();
		break;
	case sf::Keyboard::R:
		world.randomize();
	default: break;
	}
}

void Game::solvePath(){
	solveFinished = false;
	world.clearPath();
	noTileText.setString("");
	solveTime.restart();
	if (startExists && endExists){
		auto path = solver.path(isDiagonalEnabled,
			world.getStart().getPosition(), world.getEnd().getPosition());
		if (path.second){
			pathFoundText.setString("");
			for (auto&& current : path.first){
				if (world.tileAt(current.position) == tile::Empty)
					world.setIDtoTile(current.position, tile::Path);
			}
		}
		else{
			pathFoundText.setString("There is no path");
		}

		timeText.setString("It took " +
			std::to_string(solveTime.getElapsedTime().asMilliseconds()) +
			" milliseconds to solve");
		operationsText.setString("It took " +
			std::to_string(solver.getOperations()) + " operations");
	}
	else{
		noTileText.setString("There is either no start or no end");
	}
}

#include <iostream>

void Game::processInput(){
	sf::Event event;
	while (renderWindow.pollEvent(event)){
		switch (event.type){
		case sf::Event::Closed:
			renderWindow.close();
			break;
		case sf::Event::MouseButtonPressed:
			processMouseInput();
			break;
		case sf::Event::KeyPressed:
			processKeyboardInput(event.key.code);
			break;
		default: break;
		}
	}
}

void Game::render(){
	renderWindow.clear();

	renderWindow.draw(world);
	renderWindow.draw(operationsText);
	renderWindow.draw(timeText);
	renderWindow.draw(diagonalEnabledText);
	renderWindow.draw(pathFoundText);
	renderWindow.draw(noTileText);

	renderWindow.display();
}

void Game::setDiagonalTextString(){
	std::string enabledOrDisabled = 
		isDiagonalEnabled ? "enabled" : "disabled";
	diagonalEnabledText.setString("Diagonal movement is " + enabledOrDisabled);
}