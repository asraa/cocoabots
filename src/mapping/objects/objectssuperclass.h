#ifndef OBJECTSSUPERCLASS_H
#define OBJECTSSUPERCLASS_H

#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <algorithm>
#include <math.h>


class objectsSuperClass
{
public:
    typedef std::tuple<int,int> position;
    typedef std::vector< position > positionVector;
    typedef positionVector::iterator positionVectorIterator;
    typedef std::vector< std::string > stringVector;
	objectsSuperClass(int res_init, int res_fin);
	virtual void addPosition(position Pos);
	positionVector getPositions();
	virtual void addStringVector(stringVector strVec)=0;
	position getMaxPos(positionVector posVec);
	void updateMaxPos();
	positionVector createLine(position Pos1, position Pos2);
	bool checkIfEnclosed(int x, int y, positionVector posVec, position maxPos);

	int sign(int num);
	int convertInd(int ind);

	int res_init, res_fin;
	position MAX_POS;
	positionVector posList;
	positionVectorIterator posListIt;
	// virtual datatype function=what
};

#endif // OBJECTSSUPERCLASS_H
