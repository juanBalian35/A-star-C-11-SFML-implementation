#include "World.h"
#include <stdexcept>

World::World(int tileSize, int width, int height, sf::Vector2f startPos, sf::Vector2f endPos) :
tileSize(tileSize), horizontalTiles(width / tileSize), width(width),height(height) {
	startTile = Tile(sf::IntRect(startPos.x,startPos.y,tileSize,tileSize),tile::Start);
	endTile = Tile(sf::IntRect(endPos.x, endPos.y, tileSize, tileSize), tile::End);
	for (auto y = 0; y <= height; y += tileSize){
		for (auto x = 0; x < width; x += tileSize){
			Tile t(sf::IntRect(x, y, tileSize, tileSize));
			tiles.push_back(t);
		}
	}
	(*this)[startTile.getPosition()] = startTile;
	(*this)[endTile.getPosition()] = endTile;
}

World::World(const World& world) = default;

World& World::operator=(const World& other) = default;

inline int World::indexAt(sf::Vector2f position) const{
	auto positionX = static_cast<int>(position.x),
		positionY = static_cast<int>(position.y);

	positionX = (positionX - (positionX % tileSize)) / tileSize;
	positionY = (positionY - (positionY % tileSize)) / tileSize;

	return (positionY*(horizontalTiles) + positionX);
}

void World::setEverythingToEmpty(){
	for (auto& tile : tiles){
		if (tile.getID() == tile::nonWalkable || tile.getID() == tile::Path){
			tile.setID(tile::Empty);
		}
	}
}

World::World() = default;

Tile& World::operator[](sf::Vector2f position){
	return tiles[indexAt(position)];
}

const Tile& World::operator[](sf::Vector2f position) const{
	return tiles[indexAt(position)];
}

void World::setIDtoTile(sf::Vector2f position, tile::ID id){
	return (*this)[position].setID(id);
}

void World::clearPath(){
	for (auto& tile : tiles){
		if (tile.getID() == tile::Path) tile.setID(tile::Empty);
	}
}

tile::ID World::tileAt(sf::Vector2f position) const{
	return (*this)[position].getID();

}

bool World::isOutOfBounds(sf::Vector2f position) const{
	return position.y < 0 || position.y >= height || position.x < 0 || position.x >= width ;
}

bool World::isWalkable(sf::Vector2f position) const{
	return (*this)[position].getID() != tile::nonWalkable;
}

const Tile& World::getStart() const {
	for (const auto& tile : tiles){
		if (tile.getID() == tile::Start){
			return tile;
		}
	}
	throw std::out_of_range("There is no start in this map");
}

const Tile& World::getEnd() const {
	for (const auto& tile : tiles){
		if (tile.getID() == tile::End){
			return tile;
		}
	}
	throw std::out_of_range("There is no end in this map");
}

void World::draw(sf::RenderTarget& target, sf::RenderStates ) const{
	for (const auto& tile : tiles)
		target.draw(tile);
}


void World::randomize(){
	static std::mt19937 engine(getRandomEngine());
	std::uniform_int_distribution<> distr(0, (tiles.size() / 2));
	auto numOfObstacles = distr(engine);

	std::uniform_int_distribution<> distrY(0, height);
	std::uniform_int_distribution<> distrX(0, width);
	setEverythingToEmpty();

	for (auto i = 0; i < numOfObstacles; ++i){
		sf::Vector2f pos(distrX(engine), distrY(engine));

		auto tile = (*this)[pos];
		if (tile.getID() != tile::Start && tile.getID() != tile::End && !isOutOfBounds(pos)){
			tile.setID(tile::nonWalkable);
			(*this)[pos] = tile;
		}
	}
}

inline std::mt19937& getRandomEngine(){
	std::mt19937 randomEngine(std::random_device{}());
	return randomEngine;
}
