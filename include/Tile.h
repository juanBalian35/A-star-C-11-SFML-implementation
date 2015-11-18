#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace tile{
	enum ID{
		Start, End, Path, Empty, nonWalkable
	};
}

class Tile : public sf::Drawable, public sf::Transformable{
	sf::RectangleShape rect;
	tile::ID id;

	static sf::Color IDtoColor(tile::ID id) ;
public:
	explicit Tile(sf::IntRect intrect, tile::ID id = tile::Empty);
	Tile();	

	void setID(tile::ID id);
	tile::ID getID() const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};