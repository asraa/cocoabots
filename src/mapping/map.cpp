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

bool map::isPassable(position Pos) {
	bool isPassable = true;
	int x, y;
	int range = ceil(ROBOT_SIZE / RES_FIN);
	positionVector posVec = generateSquareAroundPoint(Pos, range);
	for (int i = 0; i < posVec.size(); ++i) {
		x = fmax(0,fmin(std::get<0>(posVec[i]),std::get<0>(MAX_POS)));
		y = fmax(0,fmin(std::get<1>(posVec[i]),std::get<1>(MAX_POS)));
		if (!typeIsPassable(mapVector[x][y])) {
			isPassable = false;
		}
	}
	return isPassable;
}

bool map::typeIsPassable(int type) {
	bool isPassable = false;
	switch(type) {
		case 0 : isPassable = true;
		case 4 : isPassable = true;
		case 5 : isPassable = true;
	}
	return isPassable;
}

map::positionVector map::generateSquareAroundPoint(position Pos, int diameter) {
	positionVector tempVec;
	position tempPos;
	int radius = ceil(diameter/2);
	int x = std::get<0>(Pos);
	int y = std::get<1>(Pos);
	for (int offx = -radius; offx <= radius; ++offx) {
		for (int offy = -radius; offy <= radius; ++offy) {
			tempPos = std::make_tuple(x+offx,y+offy);
			tempVec.push_back(tempPos);
		}
	}
	return tempVec;
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
			else if (id.compare("L") == 0) {
				startLoc.addStringVector(tempVector);
				startLoc.updateMaxPos();
			}
		}
	}

	mapFile.close();
}

void map::parseObjects() {
	positionVector tempVec;
	tempVec.push_back(walls.MAX_POS);
	tempVec.push_back(platforms.MAX_POS);
	tempVec.push_back(stacks.MAX_POS);
	tempVec.push_back(homeBases.MAX_POS);
	tempVec.push_back(startLoc.MAX_POS);
	MAX_POS = getMaxPos(tempVec);
	int MAX_POSX = std::get<0>(MAX_POS);
	int MAX_POSY = std::get<1>(MAX_POS);

	for (int i = 0; i < tempVec.size(); ++i) {
		position pos = tempVec[i];
		int posx = std::get<0>(pos);
		int posy = std::get<1>(pos);
	}

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

map::gridMap map::createZeroMap(int xDim, int yDim) {
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

map::position map::getMaxPos(positionVector posVec) {
	int posVecMaxX = 0;
	int posVecMaxY = 0;
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

double map::getDistance(position Pos1, position Pos2) {
	int x1, y1, x2, y2;
	double dx1, dy1, dx2, dy2;

	x1 = std::get<0>(Pos1);
	y1 = std::get<1>(Pos1);
	x2 = std::get<0>(Pos2);
	y2 = std::get<1>(Pos2);

	dx1 = indToInch(x1);
	dy1 = indToInch(y1);
	dx2 = indToInch(x2);
	dy2 = indToInch(y2);

	double answer = sqrt(pow(dx2-dx1,2)+pow(dy2-dy1,2));
	return answer;
}

map::position map::getClosestHomeBase(position currentPos) {
	return getClosestItem(currentPos, homeBases.getPositions());
}

map::position map::getClosestStack(position currentPos) {
	return getClosestItem(currentPos, stacks.getPositions());
}

map::position map::getClosestItem(position currentPos, positionVector posVec) {
	position minPos = std::make_tuple(0,0);
	double minDistance;
	if (!posVec.empty()) {
		minPos = posVec[0];
		minDistance = getDistance(currentPos,minPos);
		for (int i = 1; i < posVec.size(); ++i) {
			if (getDistance(posVec[i],currentPos) < minDistance) {
				minPos = posVec[i];
			}
		}
	}
	return minPos;
}

map::cubeTuple map::lookupStackOrder(position stackPos) {
	return stacks.getCubeStack(stackPos);
}

void map::removeStack(position stackPos) {
	stacks.removeStack(stackPos);
}

double map::indToInch(int ind) {
	return (double) ind*RES_FIN;
}

int map::inchToInd(double inch) {
	double gridResDouble = (double) RES_FIN;
	int ind = (int) inch/gridResDouble;
	return ind;
}
