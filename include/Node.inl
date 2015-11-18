inline float heuristic(float d1, float d2){
	return d1 + d2;
}

template<typename T, typename V>
Node<T,V>::Node(const Node<T,V>&) = default;

template<typename T, typename V>
Node<T, V>& Node<T, V>::operator=(const Node<T, V>&) = default;

template<typename T, typename V>
Node<T,V>::Node(V pos, T& w) : world(w), position(pos) {}

template<typename T, typename V>
Node<T,V>::Node(Node<T,V>&& other) : world(std::move(other.world)), position(std::move(other.position)),
G(std::move(other.G)), H(std::move(other.H)), F(std::move(other.F)), parent(std::move(other.parent)) {}

template<typename T, typename V>
Node<T,V>& Node<T,V>::operator=(Node<T,V>&& other) {
	position = std::move(other.position);
	G = std::move(other.G);
	F = std::move(other.F);
	H = std::move(other.H);
	parent = std::move(other.parent);

	return *this;
}

template<typename T, typename V>
std::vector<Node<T,V>> Node<T,V>::successors(int tileSize, bool diagonal) const{
	std::vector<Node<T,V>> ret;
	for (auto y = -tileSize; y <= tileSize; y += tileSize){
		for (auto x = -tileSize; x <= tileSize; x += tileSize){
			if (x == 0 && y == 0) continue;
			V temp(static_cast<float>(x), static_cast<float>(y));
			Node n(position + temp, world);
			if (world.get().isOutOfBounds(n.position) ||
				!world.get().isWalkable(n.position) ||
				(!diagonal & (abs(x) == abs(y)))) {
				continue;
			}
			ret.push_back(n);
		}
	}
	return ret;
}