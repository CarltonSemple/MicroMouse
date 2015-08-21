#ifndef __NEIGHBORS_H__
#define __NEIGHBORS_H__

/**
* Simple template class to hold pointers to adjacent objects
*/
template <class X>
class Neighbors
{
public:
	Neighbors();
	~Neighbors();

	// Add the neighbor to the correct position in the collection according to given direction
	void setNeighbor(Direction _direction, X & new_neighbor);

	// Return the neighbor for the given direction
	X getNeighbor(Direction _direction);

	X *collection;
};

template <class X>
Neighbors<X>::Neighbors()
{
	collection = new X[4];
	for (int i = 0; i < 4; i++)
		collection[i] = nullptr;
}

template <class X>
Neighbors<X>::~Neighbors()
{
	delete[] collection;
}


template <class X>
/**
* Add the neighbor to the correct position in the collection according to given direction
*/
void Neighbors<X>::setNeighbor(Direction _direction, X & new_neighbor)
{
	switch (_direction)
	{
	case L:	collection[0] = new_neighbor;	// left
		break;
	case T:	collection[1] = new_neighbor;	// top
		break;
	case R:	collection[2] = new_neighbor;	// right
		break;
	case B:	collection[3] = new_neighbor;	// bottom
		break;
	default:
		break;
	}
}

template <class X>
/**
* Return the neighbor for the given direction
*/
X Neighbors<X>::getNeighbor(Direction _direction)
{
	switch (_direction)
	{
	case L:	return collection[0];
	case T:	return collection[1];
	case R:	return collection[2];
	case B:	return collection[3];
	}
}

#endif