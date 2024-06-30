#pragma once
#include <string>
#include <iostream>
#include <stack>
#include <functional>

using namespace std;

class AVL_Tree {
private:
	// STRUCTURE FROM ONLINEGDB BST FILE
	struct Node {
		string name;
		string id;
		int height;

		Node* left;
		Node* right;
		Node(string name, string id) : name(name), id(id), height(1), left(nullptr), right(nullptr) {}
	};
	Node* root = nullptr;
	AVL_Tree::Node* helper_insert(Node* helpRoot, string name, string id);
	void helperInorder(Node* helpRoot, bool& first);
	void helperPreorder(Node* helpRoot, bool& first);
	void helperPostorder(Node* helpRoot, bool& first);
	int height(Node* node);
	void updateHeights(Node* node);
	int balanceFactor(Node* node);
	Node* rotateLeft(Node* node);
	Node* rotateRight(Node* node);
	Node* rotateLeftRight(Node* node);
	Node* rotateRightLeft(Node* node);
	Node* helperRemove(Node* helpRoot, string id);
	Node* removeInorder(Node* helpRoot, int& n);
	bool helpSearchID(Node* helpRoot, string id);
	bool suppressedSearchID(Node* helpRoot, string id);
	bool helpSearchName(Node* helpRoot, string name);
	Node* findMin(Node* node);
	int countNodes(Node* helpRoot);
	//void inorderStack(Node* helpRoot, stack<Node*> inorderNodes);
public:
	void insert(string name, string id);
	void inorder();
	void preorder();
	void postorder();
	void remove(string id);
	void rmInorder(int n);
	bool searchID(string id);
	bool IDverify(string id);
	void searchName(string name);
	void printLevelCount();
};

// inserts and rebalances if necessary
AVL_Tree::Node* AVL_Tree::helper_insert(Node* helpRoot, string name, string id) {
	if (helpRoot == nullptr)
		return new Node(name, id);
	else if (id < helpRoot->id)
		helpRoot->left = helper_insert(helpRoot->left, name, id);
	else
		helpRoot->right = helper_insert(helpRoot->right, name, id);

	updateHeights(helpRoot);

	if (balanceFactor(helpRoot) <= -2) {
		if (balanceFactor(helpRoot->right) >= 1) {
			helpRoot = rotateRightLeft(helpRoot);
			updateHeights(helpRoot);
		}
		else {
			helpRoot = rotateLeft(helpRoot);
			updateHeights(helpRoot);
		}
	}
	else if (balanceFactor(helpRoot) >= 2) {
		if (balanceFactor(helpRoot->left) <= -1) {
			helpRoot = rotateLeftRight(helpRoot);
			updateHeights(helpRoot);
		}
		else {
			helpRoot = rotateRight(helpRoot);
			updateHeights(helpRoot);
		}
	}

	return helpRoot;
}

void AVL_Tree::insert(string name, string id) {
	this->root = helper_insert(this->root, name, id);
}

int AVL_Tree::height(Node* node) {
	if (node == nullptr)
		return 0;
	else {
		return node->height;
	}
}

void AVL_Tree::updateHeights(Node* node) {
	if (node->left != nullptr) {
		updateHeights(node->left);
	}
	if (node->right != nullptr) {
		updateHeights(node->right);
	}
	node->height = max(height(node->left), height(node->right)) + 1;
}

// calculates the difference between left and right subtree heights
int AVL_Tree::balanceFactor(Node* node) {
	int leftBal, rightBal;
	if (node->left != nullptr)
		leftBal = node->left->height;
	else
		leftBal = 0;
	if (node->right != nullptr)
		rightBal = node->right->height;
	else
		rightBal = 0;
	return leftBal - rightBal;
}


// structure taken from slides
AVL_Tree::Node* AVL_Tree::rotateLeft(Node* node) {
	Node* grandchild = node->right->left;
	Node* newParent = node->right;
	newParent->left = node;
	node->right = grandchild;
	return newParent;
}
AVL_Tree::Node* AVL_Tree::rotateRight(Node* node) {
	Node* grandchild = node->left->right;
	Node* newParent = node->left;
	newParent->right = node;
	node->left = grandchild;
	return newParent;
}
AVL_Tree::Node* AVL_Tree::rotateLeftRight(Node* node) {
	node->left = rotateLeft(node->left);
	return rotateRight(node);
}
AVL_Tree::Node* AVL_Tree::rotateRightLeft(Node* node) {
	node->right = rotateRight(node->right);
	return rotateLeft(node);
}

// https://stackoverflow.com/questions/68040513/c-remove-last-comma-when-printing-avl-tree-elements to help with ending comma
void AVL_Tree::helperInorder(Node* helpRoot, bool& first) {
	if (helpRoot == nullptr)
		cout << "";
	else {
		helperInorder(helpRoot->left, first);
		if (first)
			first = false;
		else
			cout << ", ";
		cout << helpRoot->name;
		helperInorder(helpRoot->right, first);
	}
}

void AVL_Tree::helperPreorder(Node* helpRoot, bool& first) {
	if (helpRoot == nullptr)
		cout << "";
	else {
		if (first)
			first = false;
		else
			cout << ", ";
		cout << helpRoot->name;
		helperPreorder(helpRoot->left, first);
		helperPreorder(helpRoot->right, first);
	}
}

void AVL_Tree::helperPostorder(Node* helpRoot, bool& first) {
	if (helpRoot == nullptr)
		cout << "";
	else {
		helperPostorder(helpRoot->left, first);
		helperPostorder(helpRoot->right, first);
		if (first)
			first = false;
		else
			cout << ", ";
		cout << helpRoot->name;
	}
}

void AVL_Tree::inorder() {
	bool first = true;
	helperInorder(this->root, first);
	cout << endl;
}

void AVL_Tree::preorder() {
	bool first = true;
	helperPreorder(this->root, first);
	cout << endl;
}

void AVL_Tree::postorder() {
	bool first = true;
	helperPostorder(this->root, first);
	cout << endl;
}

// removes a node and returns new root, geeksforgeeks resource used
AVL_Tree::Node* AVL_Tree::helperRemove(Node* helpRoot, string id) {
	if (helpRoot == nullptr) {
		return helpRoot;
	}

	// recursively search for node
	if (helpRoot->id > id) {
		helpRoot->left = helperRemove(helpRoot->left, id);
	}
	else if (helpRoot->id < id) {
		helpRoot->right = helperRemove(helpRoot->right, id);
	}
	else {
		// we found the node to be deleted
		if (helpRoot->left == nullptr) {
			Node* temp = helpRoot->right;
			delete helpRoot;
			return temp;
		}
		else if (helpRoot->right == nullptr) {
			Node* temp = helpRoot->left;
			delete helpRoot;
			return temp;
		}

		// if the node has two children, we need to find the inorder successor
		Node* temp = findMin(helpRoot->right);

		helpRoot->id = temp->id;
		helpRoot->name = temp->name;

		helpRoot->right = helperRemove(helpRoot->right, temp->id);
	}

	updateHeights(helpRoot);
	return helpRoot;
}

void AVL_Tree::remove(string id) {
	if (suppressedSearchID(this->root, id)) {
		this->root = helperRemove(this->root, id);
		cout << "successful" << endl;
	}
	else {
		cout << "unsuccessful" << endl;
	}
}

//helper function for finding inorder successor
AVL_Tree::Node* AVL_Tree::findMin(Node* node) {
	while (node->left != nullptr) {
		node = node->left;
	}
	return node;
}

AVL_Tree::Node* AVL_Tree::removeInorder(Node* helpRoot, int& n) {
	if (helpRoot == nullptr) {
		return helpRoot;
	}
	
	// use a functor to perform an inorder traversal and get the nth id
	string targetID = "";
	function<void(Node*)> inorder = [&](Node* node) {
		if (node == nullptr) {
			return;
		}
		inorder(node->left);
		if (--n == 0) {
			targetID = node->id; // found the n-th node, store its id
		}
		inorder(node->right);
	};

	inorder(helpRoot);

	// if we found a valid target key, delete the  node
	if (targetID != "") {
		root = helperRemove(helpRoot, targetID);
	}

	return helpRoot;
}
/*
void AVL_Tree::inorderStack(Node* helpRoot, stack<Node*> inorderNodes) {
	if (helpRoot == nullptr) {
		return;
	}
	else {
		inorderStack(helpRoot->left, inorderNodes);
		inorderNodes.push(helpRoot);
		inorderStack(helpRoot->right, inorderNodes);
	}
}*/

void AVL_Tree::rmInorder(int n) {
	int nodes = countNodes(this->root);
	if (n < 0 || n > nodes - 1) {
		cout << "unsuccessful" << endl;
		return;
	}
	else {
		n = n + 1;
		this->root = removeInorder(this->root, n);
		cout << "successful" << endl;
	}
}

// count the total nodes
int AVL_Tree::countNodes(Node* helpRoot) {
	if (helpRoot == nullptr) {
		return 0;
	}
	return 1 + countNodes(helpRoot->left) + countNodes(helpRoot->right);
}

// search for an id, printing the names if they exist
bool AVL_Tree::helpSearchID(Node* helpRoot, string id) {

	Node* curr = helpRoot;

	while (curr != nullptr) {
		if (curr->id == id) {
			cout << curr->name << endl;
			return true;
		}

		if (curr->id < id)
			curr = curr->right;
		else
			curr = curr->left;
	}
	cout << "unsuccessful" << endl;
	return false;
 }

bool AVL_Tree::searchID(string id) {
	return helpSearchID(this->root, id);
}

bool AVL_Tree::IDverify(string id) {
	Node* curr = this->root;

	while (curr != nullptr) {
		if (curr->id == id) {
			return false;
		}

		if (curr->id < id)
			curr = curr->right;
		else
			curr = curr->left;
	}
	return true;
}

// a modified search to be used by other functions, not printing to cout
bool AVL_Tree::suppressedSearchID(Node* helpRoot, string id) {

	Node* curr = helpRoot;

	while (curr != nullptr) {
		if (curr->id == id) {
			return true;
		}

		if (curr->id < id)
			curr = curr->right;
		else
			curr = curr->left;
	}
	return false;
}

// search for the name and return true if is found
bool AVL_Tree::helpSearchName(Node* helpRoot, string name) {
	bool foundL = false;
	bool foundR = false;

	if (helpRoot != nullptr) {
		if (helpRoot->name == name) {
			cout << helpRoot->id << endl;
		}
		foundL = helpSearchName(helpRoot->left, name);
		foundR = helpSearchName(helpRoot->right, name);
		if (helpRoot->name == name) {
			return true;
		}
	}

	return foundL || foundR;
}

void AVL_Tree::searchName(string name) {
	bool found = helpSearchName(this->root, name);
	if (!found) {
		cout << "unsuccessful" << endl;
	}
}

void AVL_Tree::printLevelCount() {
	if (this->root == nullptr) {
		cout << 0 << endl;
	}
	else {
		cout << this->root->height << endl;
	}
}
	