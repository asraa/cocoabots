#include "homebase.h"

homeBase::homeBase(int ri, int rf): objectsSuperClass(ri,rf) {
	// do nothin
}

void homeBase::addStringVector(mapStringVector strVec) {
	mapPositionVector tempPosList, tempPosList2;
	std::string tempString;
	const char* tempChar;
	std::vector<int> tempInts;
	int tempInt;
	mapPosition Pos1, Pos2;

	for (int i = 0; i < strVec.size(); ++i) {
		tempString = strVec[i];
		tempChar = tempString.c_str();
		if (i != 0) {
			tempInt = convertInd(std::stoi(tempChar));
		}
		else {
			tempInt = std::stoi(tempChar);
		}
		tempInts.push_back(tempInt);
	}

	int N = tempInts[0];
	tempInts.erase(tempInts.begin());

	for (int i = 0; i < N; ++i) {
		int x = tempInts[2*i];
		int y = tempInts[2*i+1];
		mapPosition tempPos = {x,y};
		tempPosList.push_back(tempPos);
	}

	for (int i = 0; i < tempPosList.size(); ++i) {
		Pos1 = tempPosList[i];
		if (i != tempPosList.size()-1) {
			Pos2 = tempPosList[i+1];
		}
		else {
			Pos2 = tempPosList[0];
		}

		tempPosList2 = createLine(Pos1, Pos2);
		for (int i = 0; i < tempPosList2.size(); ++i) {
			addPosition(tempPosList2[i]);
		}
	}
	//fillInPolygon(getPositions());
}

// void homeBase::fillInPolygon(mapPositionVector posVec) {
// 	mapPosition Pos;
// 	mapPosition maxPos = getMaxPos(posVec);
// 	for (int x = 0; x <= maxPos.x; ++x) {
// 		for (int y = 0; y <= maxPos.y; ++y) {
// 			if (checkIfEnclosed(x,y,posVec,maxPos)) {
// 				Pos = {x,y};
// 				addPosition(Pos);
// 			}
// 		}
// 	}
// }