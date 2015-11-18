#pragma once

#include <memory>
#include <vector>
#include <SFML/System/Vector2.hpp>

template<typename T, typename V>
class Node{
	std::reference_wrapper<T> world;
public:
	V position;
	float G, H, F;
	std::shared_ptr<Node> parent;

	Node(V pos, T& w);
	Node(const Node& other);
	Node(Node&& other);
	Node& operator=(Node&& other);
	Node& operator=(const Node& other);

	std::vector<Node> successors(int tileSize, bool diagonal) const;
		
	friend bool operator==(const Node<T, V>& first, const Node<T, V>& other){
		return first.position == other.position;
	}

	friend bool operator!=(const Node<T, V>& first, const Node<T, V>& other){
		return !(first == other);
	}

	friend bool operator<(const Node<T, V>& first, const Node<T, V>& other){
		if (first.position.x != other.position.x)
			return first.position.x < other.position.x;
		return first.position.y < other.position.y;
	}
};

float heuristic(float d1, float d2);

#include "Node.inl"