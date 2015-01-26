#include "barrier.h"

barrier::barrier(int ri, int rf): objectsSuperClass(ri, rf) {
	// do shit
}

void barrier::addStringVector(mapStringVector strVec) {
	std::string tempString;
	const char* tempChar;
	std::vector<int> tempInts;
	int tempInt;

	for (int i = 0; i < 4; ++i) {
		tempString = strVec[i];
		tempChar = tempString.c_str();
		tempInt = convertInd(std::stoi(tempChar));
		tempInts.push_back(tempInt);
	}

	mapPosition Pos1 = {tempInts[0],tempInts[1]};
	mapPosition Pos2 = {tempInts[2],tempInts[3]};

	mapPositionVector PosVec = createLine(Pos1, Pos2);

	for (int i = 0; i < PosVec.size(); ++i) {
		addPosition(PosVec[i]);
	}
}