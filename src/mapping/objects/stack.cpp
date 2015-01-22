#include "stack.h"

stack::stack(int ri, int rf): objectsSuperClass(ri, rf) {
	// do shit
}

void stack::addStringVector(stringVector strVec) {
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

	position Pos1 = std::make_tuple(tempInts[0],tempInts[1]);

	int c1, c2, c3;

	if (strVec[2].compare("R") == 0) {
		c1 = 0;
	}
	else {
		c1 = 1;
	}
	if (strVec[3].compare("R") == 0) {
		c2 = 0;
	}
	else {
		c2 = 1;
	}
	if (strVec[4].compare("R") == 0) {
		c3 = 0;
	}
	else {
		c3 = 1;
	}

	cubeTuple ccc = std::make_tuple(c1,c2,c3);

	addStack(Pos1, ccc);
}

void stack::addStack(position Pos, cubeTuple Cubes) {
	stackDict[Pos] = Cubes;
	addPosition(Pos);
}

void stack::removeStack(position Pos) {
	stackDict.erase(stackDict.find(Pos));
	posListIt = std::find(posList.begin(), posList.end(), Pos);
	if (posListIt != posList.end()) {
		posList.erase(posListIt);
	}
}

stack::cubeTuple stack::getCubeStack(position Pos) {
	return stackDict[Pos];
}