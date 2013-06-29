#include "Architect.h"
#include "Box.h"
#include <iostream>
#include <utility>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <vector>

using std::list;
using std::map;
using std::pair;
using std::vector;
using std::copy;
using std::sort;

template <class T>
Architect<T>::Architect(list<T> _boxes) { Boxes = _boxes; }
Architect<Box>::Architect(list<Box> _boxes) { Boxes = _boxes; }

template <class T>
void Architect<T>::makeLargestHeight() { return; }
// specialization for the Box class.
void Architect<Box>::makeLargestHeight()
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
	}
}

template <class T>
void Architect<T>::stack() { return; }
void Architect<Box>::stack()
{
	// Iterate over the boxes starting at 
	// box N, and box N + 1;.
	list<Box>::iterator begin = Boxes.begin(), beginNPlusOne = Boxes.begin();
	map<int, int> areaOfBoxes, volumeOfBoxes;
	vector<pair<int, int>> mapAsVector;
	beginNPlusOne++;

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
		std::back_inserter<vector<pair<int, int>>>(mapAsVector));

	sort(mapAsVector.rbegin(), mapAsVector.rend(), [] (pair<int, int> left, pair<int, int> right) {
		return left.second > right.second;
	});

	// get index of boxes.
	vector<int> sortedBoxes;
	std::for_each(mapAsVector.begin(), mapAsVector.end(), [&sortedBoxes] (pair<int, int> value) {
		sortedBoxes.push_back(value.first);
	});

	// finish sort and copy back to Boxes.
	vector<Box> unsortedBoxObjects(Boxes.begin(), Boxes.end()), sortedBoxObjects;
	for (vector<int>::iterator iter = sortedBoxes.begin(); iter != sortedBoxes.end(); iter++)
	{
		sortedBoxObjects.push_back(unsortedBoxObjects[*iter]);
	}

	Boxes = list<Box>(sortedBoxObjects.begin(), sortedBoxObjects.end());
}