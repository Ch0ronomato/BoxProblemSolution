/* This solution is combination of all files presented at the following url: https://github.com/IanSchweer/BoxProblemSolution */
/* The project utilizes visual c++ 2011 and visual studio 2012, so please use this version as it will not compile else wise*/
/* This submission comes from Ian Schweer, email: Schweerian33@gmail.com*/
#include <iostream>
#include <map>
#include <string> 
#include <list>
#include <iostream>
#include <utility>
#include <ostream>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <vector>
#include <stdlib.h>
#include <ctype.h>
using std::map;
using std::string;
using std::list;
using std::pair;
using std::vector;
using std::copy;
using std::sort;
using std::cout;
using std::cin;
using std::endl;

// temporary type defs
typedef class Box;

// function defintions
unsigned int getNBoxes();
list<Box> getBoxProperties(unsigned int);
void getBoxFromString(string, list<Box>*, int);
bool sortVectorPair (pair<int, int>, pair<int, int>);

// constants
const int BOX_PROPERTIES_COUNT = 3;
const int MAX_BOX_COUNT = 20;
const int MIN_BOX_COUNT = 1;

enum BoxRotationType
{
	RollBox = 0x000,
	YawBox = 0x001,
	PitchBox = 0x002,
};

class Box
{
private:
	int height, width, length, id;

	// public function to yaw a box.
	void yawBox();

	// public function to roll a box.
	void rollBox();

	// public function to pitch a box.
	void pitchBox();

public:
	// constructor
	Box(int, int, int, int);

	// getter for the height attribute.
	int getHeight() { return height; }
	
	// getter for the width attribute.
	int getWidth() { return width; }

	// getter for the length attribute.
	int getLength() { return length; }

	// getter for the id
	int getId() { return id; }

	// get all properties as a map.
	map<string, int> getProperties();

	// public function to rotate box by constrant
	bool transformBox(BoxRotationType);

	// public function to caclulate the area of a box.
	int calculateArea();

	// public function to calculate the volume of a box.
	int calculateVolume();

	bool operator==(const Box A);

	void __debug();
};

void Box::__debug()
{
	cout << "Height: " << height << " width: " << width << " length: " << length << endl;
}

Box::Box(int _id, int _length, int _width, int _height)
{
	id = _id;
	height = _height;
	length = _length;
	width = _width;
}

/**
 * According to the prompt, we are allowed a 
 * basic set of 3D transformations. 
 * These are defined as Yaw, Roll, and Pitch.
 *
 * Although, these names are inaccurate,
 * assume that when we preform these transformations
 * we are effectively doing rotation at a 90 degree
 * angle. That will allow for a simple transformation.
 */

// void Box::yawBox
//
// "Yaw" the box. Essentially, just switch the length
// with the width.
void Box::yawBox()
{
	 int t = width;
	 width = length;
	 length = t;
}
// void Box::rollBox
//
// "Roll" the box. Essentially, just switch the length
// with the height
void Box::rollBox()
{
	int t = height;
	height = length;
	length = t;
}

// void Box::pitchBox
// 
// "Pitch" the box. Essentially, just switch the width
// with the height
void Box::pitchBox()
{
	int t = width;
	width = height;
	height = t;
}

// std::map<std::string, int> Box::getProperties
//
// get properties will return a map of all 
// properties.
map<string, int> Box::getProperties()
{
	map<string, int> properties;

	properties["height"] = height;
	properties["width"] = width;
	properties["length"] = length;

	return properties;
}

// bool Box::transformBox
//
// Given a rotation direction, execute
// the proper function to rotate the box.
//
// @param enum BoxRotationType
//		The direction to flip the box.
// @return bool success
//		A boolean to indicate if the operation was
//		successful.
bool Box::transformBox(BoxRotationType rot)
{
	switch(rot)
	{
		case RollBox:
			{
				rollBox();
			}
			break;
		case YawBox:
			{
				yawBox();
			}
			break;
		case PitchBox:
			{
				pitchBox();
			}
			break;
	}

	return true;
}

bool Box::operator==(Box A)
{
	return A.getId() == id;
}

int Box::calculateArea()
{
	return length * width;
}

int Box::calculateVolume()
{
	return length * width * height;
}

template <class T>
class Architect
{
private:
	list<T> Boxes;	

public:
	// constructor
	Architect(list<T>);

	// transform boxes to make height largest.
	void makeLargestHeight();

	// determine the best order of the boxes.
	void stack();

	// function checks boxes stackability
	Box checkStackability(Box, Box);

	list<T> getBoxes() { return Boxes; }
};

template <class T> Architect<T>::Architect(list<T> _boxes) { Boxes = _boxes; }
template <> Architect<Box>::Architect(list<Box> _boxes) { Boxes = _boxes; }

template <class T> void Architect<T>::makeLargestHeight() { return; }
// specialization for the Box class.
template <> void Architect<Box>::makeLargestHeight()
{
	list<Box>::iterator begin = Boxes.begin();

	for(begin; begin != Boxes.end(); begin++)
	{
		// if length is heightest, than roll.
		if (begin->getLength() > begin->getWidth() && begin->getLength() > begin->getHeight())
		{
			begin->transformBox(RollBox);
		}

		// if the length is in the middle of the two values, than Pitch
		else if (begin->getLength() < begin->getWidth() && begin->getLength() > begin->getHeight())
		{
			begin->transformBox(PitchBox);
		}

		// if the width is larger, than pitch.
		else if (begin->getWidth() > begin->getHeight())
		{
			begin->transformBox(PitchBox);
		}
		begin->__debug();
	}
}

template <class T> void Architect<T>::stack() { return; }
template <> void Architect<Box>::stack()
{
	// Iterate over the boxes starting at 
	// box N, and box N + 1;.
	list<Box>::iterator begin = Boxes.begin(), beginNPlusOne;
	map<int, int> areaOfBoxes, volumeOfBoxes;
	vector< pair<int, int> > mapAsVector;

	// store area and volume and sort by the values.	
	for (int i = 0; begin != Boxes.end(); begin++, i++)
	{
		areaOfBoxes[i] = begin->calculateArea();
		volumeOfBoxes[i] = begin->calculateVolume();
	}

	// copy our map over to a vector of type pair for sorting.
	// NOTE: Why not just flip the map and let std keep the array sorted by key?
	// Simple, if the map was to encounter a collision, then we would lose data.
	copy(areaOfBoxes.begin(), areaOfBoxes.end(), 
		std::back_inserter< vector< pair<int, int> > >(mapAsVector));

	sort(mapAsVector.rbegin(), mapAsVector.rend(), sortVectorPair);

	// get index of boxes.
	vector<int> sortedBoxes;
	for(vector< pair<int, int> >::iterator iter = mapAsVector.begin(); iter != mapAsVector.end(); iter++) 
	{
		sortedBoxes.push_back(iter->first);
	}

	// finish sort and copy back to Boxes.
	vector<Box> unsortedBoxObjects(Boxes.begin(), Boxes.end()), sortedBoxObjects;
	for (vector<int>::iterator iter = sortedBoxes.begin(); iter != sortedBoxes.end(); iter++)
	{
		sortedBoxObjects.push_back(unsortedBoxObjects[*iter]);
	}

	// copy our now sorted box vector to out list.
	Boxes = list<Box>(sortedBoxObjects.begin(), sortedBoxObjects.end());

	// we now need to recursve over the box set to see if the boxes can be stacked together
	// or not. At the moment, the height of each box is the largets property, which will help
	// later on.

	// iterators
	begin = Boxes.begin();
	beginNPlusOne = Boxes.begin();
	beginNPlusOne++;

	// vector to get all the boxes in stackable order.
	vector<Box> orderedBoxes;

	// a copy of the boxes now, this will allow us to get the smallest valuee
	list<Box> BoxCopies = Boxes;

	// begin iteration.
	while (beginNPlusOne != Boxes.end())
	{
		// get the larger of the two boxes.
		const Box biggerBox = checkStackability(*begin, *beginNPlusOne);

		// remove the largest box from the copied list.
		BoxCopies.remove(biggerBox);
		
		// add to the stack.
		orderedBoxes.push_back(biggerBox);
		
		// remove duplicates
		orderedBoxes.erase(std::unique(orderedBoxes.begin(), orderedBoxes.end()), orderedBoxes.end());

		// increment iterators.
		begin++;
		beginNPlusOne++;
	}

	if (BoxCopies.size() > 1)
	{
		// so if we have a larger size, then we can't possible stack 
		// those two values on top of eachother. So lets try stacking
		// it somewhere else. This is where the performace will suffer
		// the most due to it's parabolic time complexity and space
		// complexity
		vector< pair<int, int> > addValuesAt;
		// Originally, I used C++ 11 for this. Essentially what I am doing here is declaring
		// a point to a member function.
		// Box (Architect::* checkStackabilityPtr)(Box, Box) = &Architect::checkStackability;
		int outsideIterator = 0;

		for(list<Box>::iterator iter = BoxCopies.begin(); iter != BoxCopies.end(); iter++) 
		{
			int insideIterator = 0;
			vector<Box>::iterator begin = orderedBoxes.begin();
			for(; begin != orderedBoxes.end(); begin++)
			{
				if (checkStackability(*iter, *begin) == *begin)
				{
					pair<int, int> location(insideIterator, outsideIterator);
					addValuesAt.push_back(location);
				}

				else
				{
					// continue over the set of blocks.
					continue;
				}
				insideIterator++;
			}
			outsideIterator++;
		}
		
		// we have some new locations, lets place these boxes.
		// iterate over the pairs of values.
		int i = 0;
		vector< pair<int, int> >::iterator iter;
		for (iter = addValuesAt.begin(); iter != addValuesAt.end(); iter++, i++)
		{
			if (iter->first == i)
			{
				// iterate over the ordered boxes now.
				int j = 0;
				vector<Box>::iterator boxIter = orderedBoxes.begin();
				for(; boxIter != orderedBoxes.end(); boxIter++, j++)
				{
					if (iter->second == j)
					{
						break;
					}
				}
				continue;
			}
		}
		orderedBoxes.erase(std::unique(orderedBoxes.begin(), orderedBoxes.end()), orderedBoxes.end());
		std::cout << "";
	}
	else
	{
		// get last value, and store.
		orderedBoxes.push_back(*BoxCopies.begin());
	}
	Boxes = list<Box>(orderedBoxes.begin(), orderedBoxes.end());

	// print.
	int totalHeight = 0;
	for(list<Box>::iterator box = Boxes.begin(); box != Boxes.end(); box++)
	{
		// std::cout << "Place box " << box->getId() << " with base (" << box->getWidth() << ", " << box->getLength() << ") with height " << box->getHeight() << std::endl;
		totalHeight += box->getHeight();
	}

	std::cout << totalHeight << std::endl;
}

template<class T> Box Architect<T>::checkStackability(Box boxOne, Box boxTwo)
{
	// from the prompt.
	// For structural integrity reasons, you must also not place a box that has 
	// a larger footprint on top of a box with a smaller footprint i.e a box can 
	// be kept on the top of another box only if the Length of the upper box is 
	// not more than the Length of box below and the same for Width. 

	if (boxOne.getWidth() > boxTwo.getWidth())
	{
		// box one is wider.
		if (boxOne.getLength() > boxTwo.getLength())
		{
			// box one is wider and longer. 
			return boxOne;
		}

		// box one is wider but not longer
		else
		{
			// box two is longer. lets see if we can alter it to be stacked
			if (boxOne.getHeight() > boxTwo.getLength())
			{
				boxOne.transformBox(RollBox);
				return checkStackability(boxOne, boxTwo);
			}

			// check if we can exchange the width with the length;
			else if (boxOne.getWidth() > boxTwo.getLength())
			{
				boxOne.transformBox(YawBox);
				return checkStackability(boxOne, boxTwo);
			}

			else
			{
				return boxTwo;
			}
		}
	}
	else
	{
		if (boxOne.getLength() > boxTwo.getLength())
		{
			// box one is longer than box two, but box two is wider.
			if (boxOne.getHeight() > boxTwo.getWidth()) 
			{
				boxOne.transformBox(PitchBox);
				return checkStackability(boxOne, boxTwo);
			}
			// lets see if we can exchange the length with the width
			else if (boxOne.getLength() > boxTwo.getWidth() && boxOne.getWidth() > boxTwo.getLength())
			{
				boxOne.transformBox(YawBox);
				return checkStackability(boxOne, boxTwo);
			}

			else if (boxOne.getLength() > boxTwo.getWidth() && boxOne.getHeight() > boxTwo.getLength())
			{
				// exchange the length with the height, then the width the height.
				boxOne.transformBox(RollBox);
				boxOne.transformBox(PitchBox);
				return checkStackability(boxOne, boxTwo);
			}

			// no way to stack the boxes, return boxTwo.
			else
			{
				return boxTwo;
			}
		}

		// box two is better to be stacked on top of.
		else
		{
			return boxTwo;
		}
	}
}

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

	cout << "Please enter the number of boxes: ";
	cin >> N;
	cin.ignore();

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
		getline(cin, input);

		getBoxFromString(input, &boxes, i);
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
void getBoxFromString(string input, list<Box>* boxes, int boxCount)
{
	int values[BOX_PROPERTIES_COUNT], valuesLength = 0, lastCommaIndex = 0;

	// parse string.
	// Note. The complexity of this is O(N), where
	// N is the length of the input string. However, 
	// since we are already in a for loop, this 
	// doubles the previous complexity. 
	// Making the whole loop O(N^2) =(
	for (unsigned int i = 0; i < input.length(); i++)
	{
		// if not a space, or if end of string
		if (!isspace(input[i]) && (i+1) < input.length())
		{
			continue;
		}

		try
		{
			// if we are at a comma, store value.
			if ((i + 1) == input.length())
			{
				const char *inputParsed = input.substr(lastCommaIndex + 1, i).c_str();
				values[valuesLength] = atoi(inputParsed);
			}
			else if (lastCommaIndex > 0)
			{
				const char *inputParsed = input.substr(lastCommaIndex + 1, (i - (lastCommaIndex + 1))).c_str();
				values[valuesLength] = atoi(inputParsed);
			}
			else
			{
				const char *inputParsed = input.substr(lastCommaIndex, i).c_str();
				values[valuesLength] = atoi(inputParsed);
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
	Box box(boxCount, values[0], values[1], values[2]);

	boxes->push_back(box);
}

bool sortVectorPair (pair<int, int> left, pair<int, int> right) {
	return left.second > right.second;
}
