#include "map.h"

map::map(std::string filename):
walls(RES_INIT, RES_FIN),
platforms(RES_INIT, RES_FIN),
stacks(RES_INIT, RES_FIN),
homeBases(RES_INIT, RES_FIN),
startLoc(RES_INIT, RES_FIN) {
//	assert((RES_INIT/RES_FIN)%2==0 || RES_INIT==RES_FIN);

	MAX_POS = std::make_tuple(0,0);
	mapFilename = filename;
	buildMap(mapFilename);
}

void map::buildMap(std::string filename) {
	parseMapFile(filename);
	parseObjects();
	printMapFile("map.txt");
}

void map::parseMapFile(std::string mapFilename) {
	std::string currentLine;
	std::vector< std::string > tempVector;

	std::ifstream mapFile;
	mapFile.open(mapFilename);

	while (!mapFile.eof()) {
		tempVector.clear();
		getline(mapFile,currentLine);
		std::istringstream iss(currentLine);
		std::string token;
		while (getline(iss,token,',')) {
			tempVector.push_back(token);
		}
		if (!tempVector.empty()) {
			std::string id = tempVector[0];
			tempVector.erase(tempVector.begin());

			if (id.compare("W") == 0) {
				walls.addStringVector(tempVector);
				walls.updateMaxPos();
			}
			else if (id.compare("P") == 0) {
				platforms.addStringVector(tempVector);
				platforms.updateMaxPos();
			}
			else if (id.compare("S") == 0) {
				stacks.addStringVector(tempVector);
				stacks.updateMaxPos();
			}
			else if (id.compare("H") == 0) {
				homeBases.addStringVector(tempVector);
				homeBases.updateMaxPos();
			}
			else if (id.compare("S") == 0) {
				startLoc.addStringVector(tempVector);
				startLoc.updateMaxPos();
			}
		}
	}
}

void map::parseObjects() {
	positionVector tempVec;
	tempVec.push_back(walls.MAX_POS);
	tempVec.push_back(platforms.MAX_POS);
	tempVec.push_back(stacks.MAX_POS);
	tempVec.push_back(homeBases.MAX_POS);
	tempVec.push_back(startLoc.MAX_POS);
	MAX_POS = getMaxPos(tempVec);
	mapVector = createZeroMap(std::get<0>(MAX_POS),std::get<1>(MAX_POS));

	tempVec.clear();
	tempVec = walls.getPositions();
	for (int i = 0; i < tempVec.size(); ++i) {
		writeMapVector(tempVec[i],WALL);
	}

	tempVec.clear();
	tempVec = platforms.getPositions();
	for (int i = 0; i < tempVec.size(); ++i) {
		writeMapVector(tempVec[i],PLATFORM);
	}

	tempVec.clear();
	tempVec = stacks.getPositions();
	for (int i = 0; i < tempVec.size(); ++i) {
		writeMapVector(tempVec[i],STACK);
	}

	tempVec.clear();
	tempVec = homeBases.getPositions();
	for (int i = 0; i < tempVec.size(); ++i) {
		writeMapVector(tempVec[i],HOMEBASE);
	}

	tempVec.clear();
	tempVec = startLoc.getPositions();
	for (int i = 0; i < tempVec.size(); ++i) {
		writeMapVector(tempVec[i],STARTLOC);
	}
}

void map::writeMapVector(position Pos, int type) {
	// 0 = empty
	// 1 = wall
	// 2 = platform
	// 3 = stack
	// 4 = homebase
	// 5 = startloc
	int x = std::get<0>(Pos);
	int y = std::get<1>(Pos);
	int current = mapVector[x][y];
	switch(current) {
		case 0 : mapVector[x][y] = type;
		case 1 : if (type == 2 || type == 3 || type == 5) {
			mapVector[x][y] = type;
		}
		case 2 : if (type == 3 || type == 5) {
			mapVector[x][y] = type;
		}
		case 4 : if (type == 1 || type == 2 || type == 3 || type == 5) {
			mapVector[x][y] = type;
		}
	}
}

gridMap map::createZeroMap(int xDim, int yDim) {
	std::vector<int> tempVector;
	gridMap tempMap;

	for (int x = 0; x <= xDim; ++x) {
		tempVector.clear();
		for (int y = 0; y <= yDim; ++y) {
			tempVector.push_back(EMPTY);
		}
		tempMap.push_back(tempVector);
	}
	return tempMap;
}

position map::getMaxPos(positionVector posVec) {
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

void map::printMapFile(std::string filename) {
	std::ofstream outFile;
	outFile.open(filename);

	for (int y = 0; y <= std::get<1>(MAX_POS); ++y) {
		for (int x = 0; x <= std::get<0>(MAX_POS); ++x) {
			outFile << mapVector[x][y];
		}
		outFile << std::endl;
	}

	outFile.close();
}

int map::sign(int num) {
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


int map::convertInd(int ind, int gridResInit, int gridResFin) {
	double indDouble = (double) ind;
	double gridResInitDouble = (double) gridResInit;
	double gridResFinDouble = (double) gridResFin;
	int convertedInd = (int) ((indDouble + 0.5)*gridResInitDouble/gridResFinDouble);
	return convertedInd;
}

double map::indToInch(int ind) {
	return (double) ind*RES_FIN;
}

int map::inchToInd(double inch) {
	double gridResDouble = (double) RES_FIN;
	int ind = (int) inch/gridResDouble;
	return ind;
}

int main() {
	map myMap("green_map.txt");
}
