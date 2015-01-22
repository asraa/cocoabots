#include "objectssuperclass.h"

objectsSuperClass::objectsSuperClass(int ri, int rf)
{
	position max_pos = std::make_tuple(0,0);
	res_init = ri;
	res_fin = rf;
}

void objectsSuperClass::addPosition(position Pos) {
	posList.push_back(Pos);
}

positionVector objectsSuperClass::getPositions() {
	return posList;
}

position objectsSuperClass::getMaxPos(positionVector posVec) {
	int posVecMaxX, posVecMaxY = 0;
	for (int i = 0; i < posVec.size(); ++i) {
		int x = std::get<0>(posVec[i]);
		int y = std::get<1>(posVec[i]);
		if (x > posVecMaxX) {
			posVecMaxX = x;
		}
		if (y > posVecMaxY) {
			posVecMaxY = y;
		}
	}

	position Pos = std::make_tuple(posVecMaxX,posVecMaxY);
	return Pos;
}

void objectsSuperClass::updateMaxPos() {
	MAX_POS = getMaxPos(getPositions());
}

bool objectsSuperClass::checkIfEnclosed(int x, int y, positionVector posVec, position maxPos) {
	bool enclosedU, enclosedD, enclosedL, enclosedR = false;

	for (int yTemp = y; yTemp >= 0; --yTemp) {
		position Pos = std::make_tuple(x,yTemp);
		if (std::find(posVec.begin(), posVec.end(), Pos) != posVec.end()) {
			enclosedU = true;
		}
	}

	for (int yTemp = y; yTemp <= std::get<1>(maxPos); ++yTemp) {
		position Pos = std::make_tuple(x,yTemp);
		if (std::find(posVec.begin(), posVec.end(), Pos) != posVec.end()) {
			enclosedD = true;
		}
	}

	for (int xTemp = x; xTemp >= 0; --xTemp) {
		position Pos = std::make_tuple(xTemp,y);
		if (std::find(posVec.begin(), posVec.end(), Pos) != posVec.end()) {
			enclosedL = true;
		}
	}

	for (int xTemp = x; xTemp <= std::get<0>(maxPos); ++xTemp) {
		position Pos = std::make_tuple(xTemp,y);
		if (std::find(posVec.begin(), posVec.end(), Pos) != posVec.end()) {
			enclosedR = true;
		}
	}

	return (enclosedU && enclosedD && enclosedR && enclosedL);
}

positionVector objectsSuperClass::createLine(position Pos1, position Pos2) {
	positionVector PosVec;

	int x1 = std::get<0>(Pos1);
	int y1 = std::get<1>(Pos1);
	int x2 = std::get<0>(Pos2);
	int y2 = std::get<1>(Pos2);

	int xsgn, ysgn;
	int deltaX, deltaY, maxDelta;

	deltaX = (x2 - x1);
	deltaY = (y2 - y1);
	xsgn = sign(deltaX);
	ysgn = sign(deltaY);
	maxDelta = std::max(fabs(deltaX),fabs(deltaY));

	for (int i = 0; i <= maxDelta; ++i) {
		int xCoord = x1 + xsgn*i;
		int yCoord = y1 + ysgn*i;
		position Pos = std::make_tuple(xCoord,yCoord);
		PosVec.push_back(Pos);
	}

	return PosVec;
}

int objectsSuperClass::sign(int num) {
	if (num > 0) {
		return 1;
	}
	else if (num < 0) {
		return -1;
	}
	else {
		return 0;
	}
}

int objectsSuperClass::convertInd(int ind) {
	double indDouble = (double) ind;
	double gridResInitDouble = (double) res_init;
	double gridResFinDouble = (double) res_fin;
	int convertedInd = (int) ((indDouble + 0.5)*gridResInitDouble/gridResFinDouble);
	return convertedInd;
}