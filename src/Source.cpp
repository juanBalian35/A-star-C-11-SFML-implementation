#include "Game.h"
#include <iostream>

int main()
{
	const auto width = 1280;
	const auto height = 720;
	const auto tileSize = 40;
	Game game(width, height, tileSize, { 0, 0 }, { 1240, 680 });

	try{
		game.run();
	}
	catch (std::exception& e){
		std::cerr << e.what();
	}
}