// JPixels.h
//
// Author: Jeron Moore
// Date: 11/12/2016
// Purpose: This file describes the JPixels class used to encapsulate a list of nodes containing
//			pixel and row/column data, as well as other private data members used to track the
//			size of the list and running totals of the color values. It includes several accessor
//			methods, operator overrides, and methods to perform functions such as adding a single
//			pixel to the list, merging two separate lists together, and determing the average
//			color of all pixels in the list.


#pragma once

#include "ImageLib.h"
#include <iostream>
using namespace std;

class JPixels {


private:
	// Node struct for containing a pixel and row/col info
	struct Node {
		pixel p;
		int row;
		int col;
		Node *next;
	};

	// Private variables
	Node *head;
	int size;
	int totalRed;
	int totalBlue;
	int totalGreen;

	// Private helper methods
	
	// copyHelper
	// Precondition: input is a valid JPixels object
	// Postcondition: A copy of input is created to assist methods such as the copy constructor
	//				and the operator= overload. If this == input, no operations are performed.
	void copyHelper(const JPixels &input);

	// mergeHelper
	// Precondition: inputHead is a valid Node pointer
	// postCondition: The Nodes that inputHead point to are merged with the nodes of this
	void mergeHelper(Node *inputHead);



public:

	// Constructors & Destructor

	// Default Constructor
	// Precondition: None
	// Postcondition: A JPixels object with no Nodes is created with all variables set to 
	//				their initial values
	JPixels();

	// Constructor using an input pixel and location
	// Precondition: input is a valid pixel, row,col are valid integers
	// Postcondition: A JPixels object is created with a starting Node filled with input,
	//				row, and col. All other variables are initialized.
	JPixels(const pixel &input, int row, int col);

	// Copy constructor
	// Precondition: input is a valid JPixels object
	// Postcondition: A copy of input is created
	JPixels(const JPixels &input);

	// Destructor
	// Precondition: None
	// Postcondition: All nodes are deleted and head is set to nullptr
	~JPixels();



	// Operator overrides

	// Operator=
	// Precondition: input is a valid JPixels object
	// Postcondition: The nodes of this are cleared and set to copies of the nodes of 
	//				input. All other private variables are set to equal that of input.
	const JPixels& operator=(const JPixels &input);

	// Operator==
	// Precondition: input is a valid JPixels object
	// Postconditon: A comparison is performed for the nodes and private variables of input and 
	//				this, returning a bool value. If any of the nodes are different, or any of 
	//				the private variables are not the same, False is returned, else True.
	bool operator==(const JPixels &input) const;

	// Operator!=
	// Precondition: input is a valid JPixels object
	// Postcondition: A bool value of the opposite of the == operator is returned
	bool operator!=(const JPixels &input) const;

	// Accessor methods

	// getSize
	// Precondition: None
	// Postcondition: An int is returned representing the current number of nodes in the list
	int getSize() const;

	// getFrontRow
	// Precondition: None
	// Postcondition: An int is returned representing the 'row' value in the first node in
	//				the list. If the list is empty, -1 is returned.
	int getFrontRow() const;

	// getFrontCol
	// Precondition: None
	// Postcondition: An int is returned representing the 'col' value in the first node in
	//				the list. If the list is empty, -1 is returned.
	int getFrontCol() const;



	// Other methods

	// addPixel
	// Precondition: input is a valid pixel; row, col are valid integers
	// Postcondition: A new node is created at the front of the list and the data
	//				is set to the input parameters. Size and totalRed/Green/Blue are updated.
	void addPixel(const pixel &input, int row, int col);

	// Merge
	// Precondition: input is a valid JPixels object
	// Postcondition: The private variables are updated to represent the combined nodes and the 
	//				mergeHelper function is called to merge the input list of nodes with this
	void merge(const JPixels &input);

	// averageColor
	// Precondition: None
	// Postcondition: A pixel is returned with its red, blue, and green values representing
	//				the averages of all pixels in the list
	pixel averageColor() const;

	// removeFront
	// Precondition: None
	// Postcondition: The first node is removed from the list and size is decreased by 1. The
	//				red, green, and blue values in the node's pixel are subrtracted from
	//				totalRed/Green/Blue. If the list is empty, nothing happens.
	void removeFront();

	// Clear all pixels
	// Precondition: None
	// Postcondition: All nodes are cleared from the list and size, totalRed, totalGreen,
	//				and totalBlue are set to 0;
	void clear();
};