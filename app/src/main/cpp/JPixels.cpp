// JPixels.cpp
//
// Author: Jeron Moore
// Date: 11/12/16
// Purpose: This file implements the JPixels class used to encapsulate a list of nodes containing
//			pixel and row/column data, as well as other private data members used to track the
//			size of the list and running totals of the color values. It includes several accessor
//			methods, operator overrides, and methods to perform functions such as adding a single
//			pixel to the list, merging two separate lists together, and determing the average
//			color of all pixels in the list.


#include "JPixels.h"

// Constructors & Destructor

// Default Constructor
// Precondition: None
// Postcondition: A JPixels object with no Nodes is created with all variables set to 
//				their initial values
JPixels::JPixels() {
	head = nullptr;
	size = 0;
	totalRed = 0;
	totalBlue = 0;
	totalGreen = 0;
}

// Constructor using an input pixel and location
// Precondition: input is a valid pixel, row,col are valid integers
// Postcondition: A JPixels object is created with a starting Node filled with input,
//				row, and col. All other variables are initialized.
JPixels::JPixels(const pixel &input, int row, int col) {
	JPixels();
	addPixel(input, row, col);
}

// Copy constructor
// Precondition: input is a valid JPixels object
// Postcondition: A copy of input is created
JPixels::JPixels(const JPixels &input) {
	copyHelper(input);
}

// Destructor
// Precondition: None
// Postcondition: All nodes are deleted and head is set to nullptr
JPixels::~JPixels() {
	clear();
}



// Operator overrides

// Operator=
// Precondition: input is a valid JPixels object
// Postcondition: The nodes of this are cleared and set to copies of the nodes of 
//				input. All other private variables are set to equal that of input.
const JPixels& JPixels::operator=(const JPixels &input) {
	copyHelper(input);
	return *this;
}

// Operator==
// Precondition: input is a valid JPixels object
// Postconditon: A comparison is performed for the nodes and private variables of input and 
//				this, returning a bool value. If any of the nodes are different, or any of 
//				the private variables are not the same, False is returned, else True.
bool JPixels::operator==(const JPixels &input) const {
	if (input.size != size || input.totalRed != totalRed || input.totalBlue != totalBlue || 
		input.totalGreen != totalGreen) return false;
	Node *cur = head;
	Node *inputCur = input.head;
	while (cur != nullptr) {
		if (cur->p.red != inputCur->p.red || cur->p.blue != inputCur->p.blue || 
			cur->p.green != inputCur->p.green || cur->row != inputCur->row || 
			cur->col != inputCur->col) return false;
		cur = cur->next;
		inputCur = inputCur->next;
	}
	return true;
}

// Operator!=
// Precondition: input is a valid JPixels object
// Postcondition: A bool value of the opposite of the == operator is returned
bool JPixels::operator!=(const JPixels &input) const {
	return !(*this == input);
}



// Private helper methods

// copyHelper
// Precondition: input is a valid JPixels object
// Postcondition: A copy of input is created to assist methods such as the copy constructor
//				and the operator= overload. If this == input, no operations are performed.
void JPixels::copyHelper(const JPixels &input) {
	if (this == &input) return;
	
	clear();
	
	if (input.head != nullptr) {
		size = input.size;
		totalRed = input.totalRed;
		totalBlue = input.totalBlue;
		totalGreen = input.totalGreen;
		head = new Node;
		head->p = input.head->p;
		head->row = input.head->row;
		head->col = input.head->col;
		Node *cur = head;
		Node *inputCur = input.head->next;
		while (inputCur != nullptr) {
			cur->next = new Node;
			cur = cur->next;
			cur->p = inputCur->p;
			cur->row = inputCur->row;
			cur->col = inputCur->col;
			inputCur = inputCur->next;
		}
	}
}

// mergeHelper
// Precondition: inputHead is a valid Node pointer
// postCondition: The Nodes that inputHead point to are merged with the nodes of this
void JPixels::mergeHelper(Node *inputHead) {
	if (inputHead == nullptr) return;
	addPixel(inputHead->p, inputHead->row, inputHead->col);
	mergeHelper(inputHead->next);
}



// Accessor methods

// getSize
// Precondition: None
// Postcondition: An int is returned representing the current number of nodes in the list
int JPixels::getSize() const {
	return size;
}

// getFrontRow
// Precondition: None
// Postcondition: An int is returned representing the 'row' value in the first node in
//				the list. If the list is empty, -1 is returned.
int JPixels::getFrontRow() const {
	if (head == nullptr) return -1;
	return head->row;
}
	
// getFrontCol
// Precondition: None
// Postcondition: An int is returned representing the 'col' value in the first node in
//				the list. If the list is empty, -1 is returned.
int JPixels::getFrontCol() const {
	if (head == nullptr) return -1;
	return head->col;
}



// Other methods

// addPixel
// Precondition: input is a valid pixel; row, col are valid integers
// Postcondition: A new node is created at the front of the list and the data
//				is set to the input parameters. Size and totalRed/Green/Blue are updated.
void JPixels::addPixel(const pixel &iPixel, int row, int col) {
	Node *cur = head;
	head = new Node;
	head->p = iPixel;
	head->row = row;
	head->col = col;
	head->next = cur;
	totalRed += iPixel.red;
	totalBlue += iPixel.blue;
	totalGreen += iPixel.green;
	size++;
}

// Merge
// Precondition: input is a valid JPixels object
// Postcondition: The private variables are updated to represent the combined nodes and the 
//				mergeHelper function is called to merge the input list of nodes with this
void JPixels::merge(const JPixels &input) {
	mergeHelper(input.head);
}

// averageColor
// Precondition: None
// Postcondition: A pixel is returned with its red, blue, and green values representing
//				the averages of all pixels in the list
pixel JPixels::averageColor() const {
	pixel aColor;
	aColor.red = totalRed / size;
	aColor.blue = totalBlue / size;
	aColor.green = totalGreen / size;
	return aColor;
}

// removeFront
// Precondition: None
// Postcondition: The first node is removed from the list and size is decreased by 1. The
//				red, green, and blue values in the node's pixel are subrtracted from
//				totalRed/Green/Blue. If the list is empty, nothing happens.
void JPixels::removeFront() {
	if(size > 0) {
		Node *cur = head;
		head = head->next;
		totalRed -= cur->p.red;
		totalBlue -= cur->p.blue;
		totalGreen -= cur->p.green;
		delete cur;
		size--;
		if (size == 0) {
			head = nullptr;
		}
	}
}

// Clear all pixels
// Precondition: None
// Postcondition: All nodes are cleared from the list and size, totalRed, totalGreen,
//				and totalBlue are set to 0;
void JPixels::clear() {
	while(size > 0) {
		removeFront();
	}
}