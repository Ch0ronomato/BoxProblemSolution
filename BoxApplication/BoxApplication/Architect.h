#ifndef ARCHITECT_H
#define ARCHITECT_H
#include<list>
using std::list;

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
	T checkStackability(T, T);
};
#endif	