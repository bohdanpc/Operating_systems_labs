#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>
#include <queue>
#include "Node.h"

template<class dataType>
class TreeAvl {
public:
	enum {
		eIdMaxLen = 2,
	};

	TreeAvl() : pRoot(nullptr) {};
	~TreeAvl() { clear(); }

	void insert(const dataType &key) {
		pRoot = insert(pRoot, key);
	}

	void clear() {
		clearSubTree(pRoot);
		pRoot = nullptr;
	}
	bool findKey(const dataType &key) {
		Node<dataType> *currNode = pRoot;
		findPath.clear();

		while (currNode) {
			findPath.push_back(currNode->key);
			if (currNode->key == key)
				return true;
			if (currNode->key > key)
				currNode = currNode->left;
			else
				currNode = currNode->right;
		}
		return false;
	}

	void printLastKeyWay() {
		std::string strPath;
		for (auto elem : findPath) {
			strPath += elem + "->";
		}
		std::cout << strPath.substr(0, strPath.size() - 2) << std::endl;
	}

	void levelOrderTraversal() {
		Node<dataType> *currNode = pRoot;
		std::queue<Node<dataType> *> queueNodes;
		while (currNode) {
			std::cout << currNode->key << " ";
			if (currNode->left)
				queueNodes.push(currNode->left);
			if (currNode->right)
				queueNodes.push(currNode->right);
			if (queueNodes.size())
			{
				currNode = queueNodes.front();
				queueNodes.pop();
			}
			else
				currNode = nullptr;
		}
	}

	void printTree() {
		if (pRoot) {

			for (int i = 1; i <= pRoot->height; i++) {
				std::ostringstream myOut;
				for (int j = 0; j < eIdMaxLen * (int)pow(2, pRoot->height); j++)
					myOut << " ";
				
				printGivenLevel(myOut, pRoot, i, (int)pow(2, pRoot->height - 1));
				std::cout << myOut.str();
				std::cout << std::endl;
			}
		}
		else
			std::cout << "ERR: The tree is empty...\n";
	}

	void remove(const dataType &key) {
		pRoot = removeNode(pRoot, key);
	}

private:
	Node<dataType> *pRoot;
	std::list<dataType> findPath;

	Node<dataType> *rotateRight(Node<dataType> *nodeToRotate) {
		Node<dataType> *newNode = nodeToRotate->left;
		nodeToRotate->left = newNode->right;
		newNode->right = nodeToRotate;
		nodeToRotate->fixHeight();
		newNode->fixHeight();
		return newNode;
	}

	Node<dataType> *rotateLeft(Node<dataType> *nodeToRotate) {
		Node<dataType> *newNode = nodeToRotate->right;
		nodeToRotate->right = newNode->left;
		newNode->left = nodeToRotate;
		nodeToRotate->fixHeight();
		newNode->fixHeight();
		return newNode;
	}

	Node<dataType> *balance(Node<dataType> *nodeToBalance) {
		nodeToBalance->fixHeight();
		if (nodeToBalance->getBalanceFactor() == 2) { //right Case
			if (nodeToBalance->right->getBalanceFactor() < 0)
				nodeToBalance->right = rotateRight(nodeToBalance->right);
			return rotateLeft(nodeToBalance);
		}
		if (nodeToBalance->getBalanceFactor() == -2) { //left Case
			if (nodeToBalance->left->getBalanceFactor() > 0)
				nodeToBalance->left = rotateLeft(nodeToBalance->left);
			return rotateRight(nodeToBalance);
		}
		return nodeToBalance;
	}

	Node<dataType> *insert(Node<dataType> *node, const dataType key) {
		if (!node)
			return new Node<dataType>(key);
		if (key < node->key)
			node->left = insert(node->left, key);
		else
			node->right = insert(node->right, key);
		return balance(node);
	}

	Node<dataType> *findMin(Node<dataType> *node) {
		return node->left ? findMin(node->left) : node;
	}
	
	Node<dataType> *removeMin(Node<dataType> *node) {
		if (!node->left)
			return node->right;
		node->left = removeMin(node->left);
		return balance(node);
	}

	Node<dataType> *removeNode(Node<dataType> *node, const dataType &key) {
		if (!node)
			return nullptr;
		if (key < node->key)
			node->left = removeNode(node->left, key);
		else
			if (key > node->key)
				node->right = removeNode(node->right, key);
			else {	// key = node->key
				Node<dataType> *left = node->left;
				Node<dataType> *right = node->right;
				delete node;
				if (!right)
					return left;
				Node<dataType> *min = findMin(right);
				min->right = removeMin(right);
				min->left = left;
				return balance(min);
			}
		return balance(node);
	}


	void printGivenLevel(std::ostringstream &out, Node<dataType> *node, int level, int pos) {
		if (!node)
			return;

		if (level == 1) {
			out.seekp(eIdMaxLen * pos);
			out << node->key;
		}
		else if (level > 1) {
			printGivenLevel(out, node->left, level - 1, pos - (int)pow(2, node->height - 2));
			printGivenLevel(out, node->right, level - 1, pos + (int)pow(2, node->height - 2));
		}
	}

	void clearSubTree(Node<dataType> *node) {
		if (node) {
			clearSubTree(node->left);
			clearSubTree(node->right);
			delete node;
		}
	}
};