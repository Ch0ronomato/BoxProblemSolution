#ifndef BOX_H
#define BOX_H

#include "../BoxApplication/Enums.cpp"
#include<map>
#include<string> 

using std::map;
using std::string;
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
};
#endif