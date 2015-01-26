#include "objectssuperclass.h"

objectsSuperClass::objectsSuperClass(int ri, int rf)
{
	MAX_POS = {0,0};
	res_init = ri;
	res_fin = rf;
}

void objectsSuperClass::addPosition(struct mapPosition Pos) {
	posList.push_back(Pos);
}

mapPositionVector objectsSuperClass::getPositions() {
	return posList;
}

struct mapPosition objectsSuperClass::getMaxPos(mapPositionVector posVec) {
	int maxX = 0;
	int maxY = 0;
	for (int i = 0; i < posVec.size(); ++i) {
		if (posVec[i].x > maxX) {
			maxX = posVec[i].x;
		}
		if (posVec[i].y > maxY) {
			maxY = posVec[i].y;
		}
	}

	mapPosition Pos = {maxX,maxY};
	return Pos;
}

void objectsSuperClass::updateMaxPos() {
	MAX_POS = getMaxPos(getPositions());
}

// bool objectsSuperClass::checkIfEnclosed(int x, int y, mapPositionVector posVec, struct mapPosition maxPos) {
// 	bool enclosedU, enclosedD, enclosedL, enclosedR = false;

// 	for (int yTemp = y; yTemp >= 0; --yTemp) {
// 		mapPosition Pos = {x,yTemp};
// 		if (std::find(posVec.begin(), posVec.end(), Pos) != posVec.end()) {
// 			enclosedU = true;
// 		}
// 	}

// 	for (int yTemp = y; yTemp <= maxPos.y; ++yTemp) {
// 		mapPosition Pos = {x,yTemp};
// 		if (std::find(posVec.begin(), posVec.end(), Pos) != posVec.end()) {
// 			enclosedD = true;
// 		}
// 	}

// 	for (int xTemp = x; xTemp >= 0; --xTemp) {
// 		mapPosition Pos = {xTemp,y};
// 		if (std::find(posVec.begin(), posVec.end(), Pos) != posVec.end()) {
// 			enclosedL = true;
// 		}
// 	}

// 	for (int xTemp = x; xTemp <= maxPos.x; ++xTemp) {
// 		mapPosition Pos = {xTemp,y};
// 		if (std::find(posVec.begin(), posVec.end(), Pos) != posVec.end()) {
// 			enclosedR = true;
// 		}
// 	}

// 	return (enclosedU && enclosedD && enclosedR && enclosedL);
// }

mapPositionVector objectsSuperClass::createLine(struct mapPosition Pos1, struct mapPosition Pos2) {
	mapPositionVector PosVec;

	int xsgn, ysgn;
	int deltaX, deltaY, maxDelta;

	deltaX = (Pos2.x - Pos1.x);
	deltaY = (Pos2.y - Pos1.y);
	xsgn = sign(deltaX);
	ysgn = sign(deltaY);
	maxDelta = std::max(fabs(deltaX),fabs(deltaY));

	for (int i = 0; i <= maxDelta; ++i) {
		int xCoord = Pos1.x + xsgn*i;
		int yCoord = Pos1.y + ysgn*i;
		mapPosition Pos = {xCoord, yCoord};
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