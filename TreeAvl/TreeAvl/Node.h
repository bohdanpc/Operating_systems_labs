#pragma once

template<class dataType>
struct Node {
	Node() : left(nullptr), right(nullptr), height(1) {};
	Node(dataType val) : left(nullptr), right(nullptr), height(1), key(val) {};

	unsigned char getHeight(const Node *pNode) const {
		return pNode ? pNode->height : 0;
	}

	char getBalanceFactor() const {
		return getHeight(right) - getHeight(left);
	}

	void fixHeight() {
		unsigned char heightLeft = getHeight(left);
		unsigned char heightRight = getHeight(right);
		this->height = (heightLeft > heightRight ? heightLeft : heightRight) + 1;
	}

	dataType key;
	unsigned char height;
	Node *left;
	Node *right;
};