#include <iostream>
#include <queue> //For level order traversal
using namespace std;

/*
Program that runs a simulation of a Red-Black Binary Search Tree
*/

template <class T> class BST;	//Class Prototype

								//Tree Node class, templated
template <class T>
class BSTNode {
private:
	T data;	//Data variable
	BSTNode<T>* parent;	//Parent of node
	BSTNode<T>* left;	//Left child of node
	BSTNode<T>* right;	//Right child of node
	bool color;	//RB color (RED = 1, BLACK = 0)
public:
	BSTNode(T newdata = T(), BSTNode<T>* newParent = nullptr,	//Constructor for node
		BSTNode<T>* newLeft = nullptr, BSTNode<T>* newRight = nullptr, bool newcolor = 1)
		:data(newdata), parent(newParent), left(newLeft), right(newRight), color(newcolor) {}

	//Returns the data values
	T get_Data() const { return data; }

	friend class BST<T>;	//Friend Tree
	int getSize() const;	//Returns the size of the node + children
};

//Binary Search Tree, templated
template <class T>
class BST {
private:
	BSTNode<T>* root;	//Pointer to root node
public:
	BST(BSTNode<T>* newRoot = nullptr) :root(newRoot) {}	//Constructor

	BSTNode<T>* get_Root() const { return root; }	//Returns the root node

	BSTNode<T>* find_Node(T item) const;	//Returns the node of a given number

	void insert(T item);	//Inserts a new item into the tree
	void remove(BSTNode<T>*& temp);	//Removes an item from the tree
	void printInOrder(BSTNode<T>* node); //Prints the elements of the tree in order
	void printLevelOrder();

	void rotateRight(BSTNode<T>*& temp);	//Rotates a node to the right
	void rotateLeft(BSTNode<T>*& temp);	//Rotates a node to the left

	void balanceRB(BSTNode<T>*& temp);	//Balances the tree according to Red-Black rules
	void removeRB(BSTNode<T>*& temp);
	void doubleBlackBalance(BSTNode<T>*& par, bool direction);	//Attempted Red-Black Deletion balance
};


void createRBSimulation();	//Function Prototype


int main() {

	createRBSimulation();	//Calls Red-Black Tree simulation

	return 0;
}


//Function that interacts with a user to create a Red-Black Tree
void createRBSimulation() {
	BST<int> RBTree;	//Red-Black Binary Search Tree

	cout << "Welcome to the Red-Black Tree Simulation!" << endl;
	cout << "Please enter a positive sequence of unique numbers, pressing enter after each entry:" << endl;
	cout << "(Enter a -1 when the sequence is over)" << endl;

	int entry;	//User entry
	cin >> entry;	//Input
	while (entry >= 0) {	//While the numbers are greater than -1
		RBTree.insert(entry);	//Inserts the number into the tree
		cin >> entry;	//Asks for further entry
	}

	cout << "Here is the sequence of numbers you inputted, in order:" << endl;
	RBTree.printInOrder(RBTree.get_Root());	//Prints all numbers in order
	cout << endl;

	cout << "Here is the tree in level order (with colors R/B):" << endl;
	RBTree.printLevelOrder();	//Prints the tree in level order, with colors, for ease of visualizing
	cout << endl;
}


//Returns the size of the node
template <class T>
int BSTNode<T>::getSize() const {
	int count = 1;	//Initial count
	if (left != nullptr)
		count += left->getSize();	//Gets the total size of the left nodes
	if (right != nullptr)
		count += right->getSize();	//Gets the total size of the right nodes
	return count;	//Returns total
}

//Returns the location of a requested item
template <class T>
BSTNode<T>* BST<T>::find_Node(T item) const {
	BSTNode<T>* temp = root;
	while (temp != nullptr) {
		if (item == temp->data)
			return temp;
		else if (item < temp->data)
			temp = temp->left;
		else
			temp = temp->right;
	}
	cout << "COULD NOT FIND NUMBER" << endl;
	return nullptr;
}

//Inserts data into a tree
template <class T>
void BST<T>::insert(T item) {
	if (root == nullptr) {	//Root node case, empty tree
		root = new BSTNode<T>(item);
		root->color = 0;	//Colors root black
		return;	//Exits
	}
	BSTNode<T>* temp = root;	//Temporary finder node
	BSTNode<T>* prev = root;	//Temporary node for previous of temp
	while (temp != nullptr) {	//Searches for the correct empty location
		prev = temp;
		if (item == prev->data)	//Rejects all numbers already in tree
			return;
		else if (item < temp->data)
			temp = temp->left;
		else
			temp = temp->right;
	}
	if (item < prev->data) {	//If new data is less than the parent
		prev->left = new BSTNode<T>(item, prev);	//Sets as left child
		temp = prev->left;
	}
	else {	//If data is greater than the parent
		prev->right = new BSTNode<T>(item, prev);	//Sets as right child
		temp = prev->right;
	}

	balanceRB(temp);	//Balances the red-black tree
}

//Removes a node from a tree
template <class T>
void BST<T>::remove(BSTNode<T>*& temp) {
	if ((temp->left == nullptr) && (temp->right == nullptr)) {	//If there are no children
		if (temp->parent == nullptr)	//Root case
			root = nullptr;
		else if (temp->parent->left == temp)	//Left child
			temp->parent->left = nullptr;
		else
			temp->parent->right = nullptr;	//Right child
		delete temp;	//Deletes node
	}
	else if (temp->left == nullptr) {	//If there is one child to the right
		BSTNode<T>* toDelete = temp->right;	//Temporary deletion variable
		temp->data = toDelete->data;	//Transfers all data and children
		temp->left = toDelete->left;
		if (temp->left != nullptr)	//Moves parent data
			temp->left->parent = temp;
		temp->right = toDelete->right;
		if (temp->right != nullptr)
			temp->right->parent = temp;
		delete toDelete;	//Deletes node
	}
	else if (temp->right == nullptr) {	//If there is one child to the left
		BSTNode<T>* toDelete = temp->left;	//Mirror of right case
		temp->data = toDelete->data;
		temp->left = toDelete->left;
		if (temp->left != nullptr)
			temp->left->parent = temp;
		temp->right = toDelete->right;
		if (temp->right != nullptr)
			temp->right->parent;
		delete toDelete;
	}
	else {	//If there are two children
		BSTNode<T>* minOfRight = temp->right;	//Finds minimum of right subtree
		while (minOfRight->left != nullptr)
			minOfRight = minOfRight->left;
		temp->data = minOfRight->data;	//Moves data from minimum to "root"
		remove(minOfRight);	//Removes the minimum element
	}
}

//Prints the tree's elements in order
template <class T>
void BST<T>::printInOrder(BSTNode<T>* node) {
	if (node != nullptr) {	//As long as not a null leaf
		printInOrder(node->left);	//Prints all elements to the left
		cout << node->data << "\t";	//Prints the node
		printInOrder(node->right);	//Prints all elements to the right
	}
}

//Prints the tree in level order
template <class T>
void BST<T>::printLevelOrder() {
	queue<BSTNode<T>*> q;	//Queue to hold elements
	q.push(root);	//Adds the root to the queue
	while (!q.empty()) {	//Goes through all elements of the tree
		BSTNode<T>* temp = q.front();	//The current element
		q.pop();
		cout << temp->data;	//Prints out the next element
		if (temp->color == 1)	//Prints out the color of each element
			cout << "(R)" << "\t";
		else
			cout << "(B)" << "\t";
		if (temp->left != nullptr)	//Pushes the siblings and cousins into the queue before children
			q.push(temp->left);
		if (temp->right != nullptr)
			q.push(temp->right);
	}
}

//Rotates a node to the right
template <class T>
void BST<T>::rotateRight(BSTNode<T>*& temp) {
	BSTNode<T>* replace = temp->left;	//Defines the rpelacing node
	temp->left = replace->right;	//Moves the new element's right node to the original's left node
	replace->right = temp;	//Sets the new node	as the parent node
	replace->parent = temp->parent;	//Sets the original parent as the new node's parent
	replace->right->parent = replace;	//Sets the original node's parent as the new node
	if (replace->right->left != nullptr)	//Moved child node is set to identify original node as parent
		replace->right->left->parent = replace->right;

	if (replace->parent == nullptr)	//Resets the root node if needed
		root = replace;
	else if (replace->parent->left == replace->right)	//Resets the new parent's identifer for child
		replace->parent->left = replace;
	else
		replace->parent->right = replace;
}

//Rotates a node to the left
template <class T>
void BST<T>::rotateLeft(BSTNode<T>*& temp) {
	BSTNode<T>* replace = temp->right;	//Defines the replacing node
	temp->right = replace->left;	//Moves the new element's left node to the original's right node
	replace->left = temp;	//Sets the new node as the parent node
	replace->parent = temp->parent;	//Sets the original parent as the new node's parent
	replace->left->parent = replace;	//Sets the original node's parent as the new node
	if (replace->left->right != nullptr)	//Moved child node is set to identify original node as parent
		replace->left->right->parent = replace->left;

	if (replace->parent == nullptr)	//Resets the root node if needed
		root = replace;
	else if (replace->parent->left == replace->left)	//Resets the new parent's identifier for child
		replace->parent->left = replace;
	else
		replace->parent->right = replace;
}

//Function that balances a tree using Red Black logic
template <class T>
void BST<T>::balanceRB(BSTNode<T>*& temp) {
	while ((temp != root) && (temp->parent->color == 1)) {
		if (temp->parent == temp->parent->parent->right) {	//parent is the right child
			BSTNode<T>* uncle = temp->parent->parent->left;	//uncle is the left child
			if ((uncle != nullptr) && (uncle->color == 1)) {	//uncle is red
				uncle->color = 0;	//Recolors the uncle to black
				temp->parent->color = 0;	//parent to black
				temp->parent->parent->color = 1;	//Grandparent to red
				temp = temp->parent->parent;	//Restarts loop with grandparent
			}
			else {	//uncle is black
				if (temp == temp->parent->right) {	//temp is right child of right child (right-right)
					rotateLeft(temp->parent->parent);	//Rotates the grandparent to the left
					temp->parent->color = 0;	//Recolors parent to black
					temp->parent->left->color = 1;	//Original grandparent to red
				}
				else {								//temp is left child of right child (right-left)
					rotateRight(temp->parent);	//Double rotation
					rotateLeft(temp->parent);
					temp->left->color = 1;	//Original grandparent to red
					temp->color = 0;	//Node to black
				}
			}
		}
		else {													//parent is the left child
			BSTNode<T>* uncle = temp->parent->parent->right;	//uncle is the right child
			if ((uncle != nullptr) && (uncle->color == 1)) {
				uncle->color = 0;	//Recolors the uncle to black
				temp->parent->color = 0;	//parent to black
				temp->parent->parent->color = 1;	//grandparent to red
				temp = temp->parent->parent;	//restarts loop with grandparent
			}
			else {
				if (temp == temp->parent->left) {	//temp is left child of left child (left-left)
					rotateRight(temp->parent->parent);	//Rotates grandparent to right
					temp->parent->color = 0;	//parent to black
					temp->parent->right->color = 1;	//original grandparent to red
				}
				else {								//temp is right child of left child (left-right)
					rotateLeft(temp->parent);	//Double rotation
					rotateRight(temp->parent);
					temp->right->color = 1;	//Original grandparent to red
					temp->color = 0;	//Node to black
				}
			}
		}
	}
	root->color = 0;	//Root is always colored black
}

//Function that balances the tree if a deleted node child is colored double black
template <class T>
void BST<T>::doubleBlackBalance(BSTNode<T>*& par, bool direction) {
	bool doubleBlack = true;
	while (doubleBlack == true) {
		if (par == nullptr) {	//Node becomes root case
			root->color = 0;
			return;
		}

		BSTNode<T>* sibling = nullptr;
		if (direction == 0)
			sibling = par->right;
		else
			sibling = par->left;

		if (sibling->color == 0) {	//sibling color is black
			if ((sibling->left != nullptr) && (sibling->left->color == 1)) {	//Left child is red
				if (direction == 1) { //Left-Left Case
					rotateRight(par);
					sibling->left->color = 0;
				}
				else {	//Right-Left Case
					rotateRight(sibling);
					rotateLeft(par);
					sibling->right->color = 0;
				}
				doubleBlack = false;
			}
			else if ((sibling->right != nullptr) && (sibling->right->color == 1)) {	//Right child is red
				if (direction == 0) { //Right-Right Case
					rotateLeft(par);
					sibling->right->color = 0;
				}
				else {	//Left-Right Case
					rotateLeft(sibling);
					rotateRight(par);
					sibling->left->color = 0;
				}
				doubleBlack = false;
			}
			else {	//Neither child is red
				sibling->color = 1;
				if (par->color == 0) {	//If the parent is black
					par = par->parent;
					if (direction == 0)
						direction = 1;
					else
						direction = 0;
				}
				else {
					par->color = 0;
					doubleBlack = false;
				}
			}
		}
		else {	//sibling color is red
			if (direction == 0)	//sibling is right child
				rotateLeft(par);
			else	//sibling is left child
				rotateRight(par);
			par->color = 1;
			sibling->color = 0;
		}
	}
}

//Function that removes a node from a Red-Black Tree
template <class T>
void BST<T>::removeRB(BSTNode<T>*& temp) {
	if ((temp->left == nullptr) && (temp->right == nullptr)) {	//If there are no children
		bool direction = 0;	//Direction of parent to temp, 0 is left, 1 is right
		if (temp->parent == nullptr)	//Root case
			root = nullptr;
		else if (temp->parent->left == temp)	//Left child
			temp->parent->left = nullptr;
		else {
			temp->parent->right = nullptr;	//Right child
			direction = 1;
		}

		if (temp->color == 0)
			doubleBlackBalance(temp->parent, direction);

		delete temp;	//Deletes node
	}
	else if (temp->left == nullptr) {	//If there is one child to the right
		BSTNode<T>* toDelete = temp->right;	//Temporary deletion variable
		bool flag;
		if ((temp->color == 1) || (toDelete->color == 1))
			flag = true;
		temp->data = toDelete->data;	//Transfers all data and children

		temp->left = toDelete->left;
		if (temp->left != nullptr)	//Moves parent data
			temp->left->parent = temp;

		temp->right = toDelete->right;
		if (temp->right != nullptr)
			temp->right->parent = temp;

		delete toDelete;	//Deletes node

		if (flag)
			temp->color = 0;
		else {
			bool direction = 0;
			if (temp == temp->parent->right)
				direction = 1;
			doubleBlackBalance(temp->parent, direction);
		}
	}
	else if (temp->right == nullptr) {	//If there is one child to the left
		BSTNode<T>* toDelete = temp->left;	//Mirror of right case
		bool flag;
		if ((temp->color == 1) || (toDelete->color == 1))
			flag = true;
		temp->data = toDelete->data;

		temp->left = toDelete->left;
		if (temp->left != nullptr)
			temp->left->parent = temp;

		temp->right = toDelete->right;
		if (temp->right != nullptr)
			temp->right->parent;

		delete toDelete;

		if (flag)
			temp->color = 0;
		else {
			bool direction = 0;
			if (temp == temp->parent->right)
				direction = 1;
			doubleBlackBalance(temp->parent, direction);
		}
	}
	else {	//If there are two children
		BSTNode<T>* minOfRight = temp->right;	//Finds minimum of right subtree
		while (minOfRight->left != nullptr)
			minOfRight = minOfRight->left;
		temp->data = minOfRight->data;	//Moves data from minimum to "root"
		remove(minOfRight);	//Removes the minimum element
	}
}