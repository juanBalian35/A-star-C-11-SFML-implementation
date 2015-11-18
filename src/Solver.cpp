#include "Solver.h"

Solver::Solver(int _tileSize, int _width, int _height, 
	sNode startNode, sNode goalNode) :
	width(_width), height(_height), tileSize(_tileSize), operations(0),
goalFound(false), canMoveDiagonally(false), startNode(startNode),
goalNode(goalNode),lastAdded(startNode) {}


int Solver::getOperations() const{
	return operations;
}

std::pair<std::vector<sNode>,bool> Solver::path(bool isDiagonalEnabled, sf::Vector2f startPos, sf::Vector2f endPos){
	//Reset some values
	canMoveDiagonally = isDiagonalEnabled;
	startNode.position = startPos;
	goalNode.position = endPos;
	endPosToCalc.x = endPos.x / tileSize;
	endPosToCalc.y = endPos.y / tileSize;
	operations = 0;
	goalFound = false;

	//Reset containers
	openList.clear();
	closedList.clear();

	//Add start node to the openList
	startNode.H =
		heuristic(std::abs(startNode.position.x - goalNode.position.x), std::abs(startNode.position.y - goalNode.position.y));
	startNode.G = 0;
	startNode.F = startNode.G + startNode.H;

	openList.push_back(startNode);
	std::push_heap(std::begin(openList), std::end(openList), comp);

	while (!openList.empty() & !goalFound){
		step();
	}
	
	if ((openList.empty() & !goalFound ) | !goalFound){
		return std::make_pair(std::vector<sNode>(), false);
	}

	std::vector<sNode> ret;
	while (lastAdded != startNode){
		ret.push_back(lastAdded);
		lastAdded = *lastAdded.parent;
	}
	return std::make_pair(ret,true);
}

void Solver::step(){
	auto current = openList.front();
	lastAdded = *closedList.insert(current).first;
	std::pop_heap(std::begin(openList), std::end(openList), comp);
	openList.pop_back();
	if (current == goalNode) {
		goalFound = true;
		return;
	}

	auto neighbors = current.successors(tileSize, canMoveDiagonally);
	for (auto&& successor : neighbors){
		auto isDiagonal =
			!((successor.position.x == current.position.x) | (successor.position.y == current.position.y));

		if (closedList.find(successor) != closedList.end()) 
			continue;

		auto positionToCalc = successor.position;
		positionToCalc.x /= tileSize;
		positionToCalc.y /= tileSize;

		//tie breaking
		auto dx1 = positionToCalc.x - endPosToCalc.x, 
			 dy1 = positionToCalc.y - endPosToCalc.y,
			 dx2 = startNode.position.x - endPosToCalc.x, 
			 dy2 = startNode.position.y - endPosToCalc.y;
		auto cross = std::abs(dx1*dy2 - dx2*dy1);
		//calcule node values
		successor.parent = std::make_shared<sNode>(lastAdded);
		successor.H = heuristic(abs(positionToCalc.x - endPosToCalc.x), 
								abs(positionToCalc.y - endPosToCalc.y));
		successor.H += static_cast<float>(cross*0.001);
		successor.G = current.G + (isDiagonal & canMoveDiagonally ? SQRT2 : 1);
		successor.F = successor.G + successor.H;

		auto inOpen = std::find(openList.begin(), openList.end(), successor);
		if (inOpen == openList.end() || successor.G < inOpen->G) {
			/*if either the node is not in the open list or
			successor's G is lower than the found node in the open list's G
			that means it's a better path, add it to the priority queue
			and since the comparator will leave the one with the lower F
			the one with the greater F will be discarted later on, or simply
			will not be used */
			openList.push_back(successor);
			std::push_heap(std::begin(openList), std::end(openList), comp);
		}

	}
	operations++;
}