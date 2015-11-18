#pragma once

#include "Node.h"
#include "World.h"
#include <set>

typedef Node<World, sf::Vector2f> sNode;
static const auto comp = [](const sNode& n1, const sNode& n2) {return n1.F > n2.F; };
static const auto SQRT2 = 1.41421356f;

class Solver{
	int width, height, tileSize, operations;
	bool goalFound, canMoveDiagonally;
	sNode startNode, goalNode, lastAdded;
	std::vector<sNode> openList;
	std::set<sNode> closedList;
	sf::Vector2f endPosToCalc;

	void step();

public:
	Solver(int tileSize, int width, int height, 
		sNode startNode, sNode goalNode);

	//method that returns the path and whether it was found or not.
	std::pair<std::vector<sNode>,bool> path(bool canMoveDiagonally, sf::Vector2f startPos, sf::Vector2f endPos);

	int getOperations() const;
};
