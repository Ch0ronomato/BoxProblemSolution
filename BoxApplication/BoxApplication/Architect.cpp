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
	list<Box>::iterator begin = Boxes.begin(), beginNPlusOne;
	map<int, int> areaOfBoxes, volumeOfBoxes;
	vector<pair<int, int>> mapAsVector;

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
		vector<pair<int, int>> addValuesAt;
		Box (Architect::* checkStackabilityPtr)(Box, Box) = &Architect::checkStackability;
		Architect* _this = this;
		int outsideIterator = 0;

		std::for_each(BoxCopies.begin(), BoxCopies.end(), 
			[_this, &outsideIterator, &addValuesAt, checkStackabilityPtr, &orderedBoxes] (Box box) 
		{
			int insideIterator = 0;
			vector<Box>::iterator begin = orderedBoxes.begin();
			Architect arch = *_this;
			for(; begin != orderedBoxes.end(); begin++)
			{
				if ((arch.*checkStackabilityPtr)(box, *begin) == *begin)
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
		});
		// we have some new locations, lets place these boxes.
		// iterate over the pairs of values.
		int i = 0;
		for (auto iter = addValuesAt.begin(); iter != addValuesAt.end(); iter++, i++)
		{
			if (iter->first == i)
			{
				// iterate over the ordered boxes now.
				int j = 0;
				auto boxIter = orderedBoxes.begin();
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

	/*
	// print.
	int totalHeight = 0;
	for(auto box = Boxes.begin(); box != Boxes.end(); box++)
	{
		std::cout << "Place box " << box->getId() << " with base (" << box->getWidth() << ", " << box->getLength() << " with height " << box->getHeight() << std::endl;
		totalHeight += box->getHeight();
	}

	std::cout << "This will result in a maximum height of " << totalHeight << std::endl;
	return; */
}

template<> Box Architect<Box>::checkStackability(Box boxOne, Box boxTwo)
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
		}

		// box two is better to be stacked on top of.
		else
		{
			return boxTwo;
		}
	}
}