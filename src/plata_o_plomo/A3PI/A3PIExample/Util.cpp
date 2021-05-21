#include "Util.h"

int SquareDistance(int x1, int x2, int y1, int y2)
{ 
	return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

int SquareDistance(BWAPI::Position p1, BWAPI::Position p2)
{ 
	return (p1.x()-p2.x())*(p1.x()-p2.x()) + (p1.y()-p2.y())*(p1.y()-p2.y());
}