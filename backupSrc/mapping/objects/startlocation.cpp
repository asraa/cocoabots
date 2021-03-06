#include "startlocation.h"

startLocation::startLocation(int ri, int rf): objectsSuperClass(ri, rf) {
	// do nothin
}

void startLocation::addStringVector(mapStringVector strVec) {
	std::string tempString;
	const char* tempChar;
	std::vector<int> tempInts;
	int tempInt;

	for (int i = 0; i < 2; ++i) {
		tempString = strVec[i];
		tempChar = tempString.c_str();
		tempInt = convertInd(std::stoi(tempChar));
		tempInts.push_back(tempInt);
	}

	mapPosition Pos1 = {tempInts[0],tempInts[1]};
	addPosition(Pos1);
}