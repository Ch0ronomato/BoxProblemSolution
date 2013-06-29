#include <iostream>
#include <string>
#include <map>
#include <list>
#include "Box.h"
#include "Architect.h"
using std::cout;
using std::cin;
using std::endl;
using std::map;
using std::string;
using std::list;

// function defintions
unsigned int getNBoxes();
list<Box> getBoxProperties(unsigned int);
void getBoxFromString(string, list<Box>*);

// constants
const int BOX_PROPERTIES_COUNT = 3;
const int MAX_BOX_COUNT = 20;
const int MIN_BOX_COUNT = 1;

int main() 
{
	// get number of boxes.
	unsigned int N = getNBoxes();

	// get box properties
	list<Box> boxes = getBoxProperties(N);

	// initalize the architect
	Architect<Box> architect(boxes);

	// rotate boxes to get largest height.
	architect.makeLargestHeight();

	// stack the objects.
	architect.stack();
	
	return 0;
}

// getNBoxes
//
// get a number of boxes "N" from 
// stdin.
unsigned int getNBoxes()
{
	int N = 0;
	bool ready = false;

	while (!ready)
	{
		cout << "Please enter the number of boxes: ";
		cin >> N;

		if (N < MIN_BOX_COUNT || N > MAX_BOX_COUNT)
		{
			ready = false;
		}
		else
		{
			ready = true;
		}
	}

	return N;
}

// list<Box> getBoxProperties
//
// Function will read in properties
// from standard input, and construct
// a list of boxes.
//
// @param int N
//		The number of boxes. Assume 1 to allow for
//		SOMETHING to be created.
list<Box> getBoxProperties(unsigned int N = 1)
{
	list<Box> boxes;
	string input;

	// until we reach N, propmpt the user for
	// properties of box. In order of 
	// height, width, length.
	// complexity: O(N^2);
	for (unsigned int i = 0; i < N; i++)
	{
		cout << "Please enter the properties of box (length, width, height): ";

		// read values in as string.
		cin >> input;

		getBoxFromString(input, &boxes);
	}

	return boxes;
}

// void getBoxFromString
//
// Function will parse string by commas to set
// properties of a box.
// @param string input
//		The input string.
//
// @param list<Box>* boxes
//		A pointer to our list. We take this as 
//		a pointer because if we have a bad
//		input string, we just want to 
//		skip over the box and continue on.
void getBoxFromString(string input, list<Box>* boxes)
{
	int values[BOX_PROPERTIES_COUNT], valuesLength = 0, lastCommaIndex = 0;
	string::size_type st;

	// parse string.
	// Note. The complexity of this is O(N), where
	// N is the length of the input string. However, 
	// since we are already in a for loop, this 
	// doubles the previous complexity. 
	// Making the whole loop O(N^2) =(
	for (unsigned int i = 0; i < input.length(); i++)
	{
		// if not a comma, or if end of string
		if (input[i] != ',' && (i+1) < input.length())
		{
			continue;
		}

		try
		{
			// if we are at a comma, store value.
			if ((i + 1) == input.length())
			{
				values[valuesLength] = std::stoi(input.substr(lastCommaIndex + 1, i), &st);
			}
			else if (lastCommaIndex > 0)
			{
				values[valuesLength] = std::stoi(input.substr(lastCommaIndex + 1, (i - (lastCommaIndex + 1))), &st);
			}
			else
			{
				values[valuesLength] = std::stoi(input.substr(lastCommaIndex, i), &st);
			}
			valuesLength++;
			lastCommaIndex = i;
		}
		catch(...)
		{
			break;
		}
	}

	// create a box, and add to list.
	Box box(values[0], values[1], values[2]);

	boxes->push_back(box);
}