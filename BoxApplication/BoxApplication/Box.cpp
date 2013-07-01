#include <map>
#include <string>
#include "Box.h"

using std::map;
using std::string;

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