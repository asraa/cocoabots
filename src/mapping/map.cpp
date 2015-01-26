#include "map.h"

map::map(std::string filename):
walls(RES_INIT, RES_FIN),
platforms(RES_INIT, RES_FIN),
stacks(RES_INIT, RES_FIN),
homeBases(RES_INIT, RES_FIN),
startLoc(RES_INIT, RES_FIN) {
//	assert((RES_INIT/RES_FIN)%2==0 || RES_INIT==RES_FIN);
    
    //want to add: getSonarReading(position currentPos, angle orientation)

	MAX_POS = {0,0};
	mapFilename = filename;
	buildMap(mapFilename);
}

bool map::isPassable(struct mapPosition Pos) {
	bool isPassable = true;
	int x, y;
	int range = ceil(ROBOT_SIZE / RES_FIN);
	mapPositionVector posVec = generateSquareAroundPoint(Pos, range);
	for (int i = 0; i < posVec.size(); ++i) {
		x = fmax(0,fmin(posVec[i].x,MAX_POS.x));
		y = fmax(0,fmin(posVec[i].y,MAX_POS.y));
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

mapPositionVector map::generateSquareAroundPoint(struct mapPosition Pos, int diameter) {
	mapPositionVector tempVec;
	mapPosition tempPos;
	int radius = ceil(diameter/2);
	for (int offx = -radius; offx <= radius; ++offx) {
		for (int offy = -radius; offy <= radius; ++offy) {
			tempPos = {Pos.x+offx,Pos.y+offy};
			tempVec.push_back(tempPos);
		}
	}
	return tempVec;
}

void map::buildMap(std::string filename) {
	parseMapFile(filename);
	parseObjects();
	floodFillOutside(); // fill the outside map with 6's
	floodFillHomeBase();
	printMapFile("map.txt");
}

void map::parseMapFile(std::string mapFilename) {
	std::string currentLine;
	mapStringVector tempVector;

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
	mapPositionVector tempVec;
	tempVec.push_back(walls.MAX_POS);
	tempVec.push_back(platforms.MAX_POS);
	tempVec.push_back(stacks.MAX_POS);
	tempVec.push_back(homeBases.MAX_POS);
	tempVec.push_back(startLoc.MAX_POS);
	MAX_POS = getMaxPos(tempVec);

	mapVector = createZeroMap(MAX_POS.x,MAX_POS.y);

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

void map::writeMapVector(struct mapPosition Pos, int type) {
	// 0 = empty
	// 1 = wall
	// 2 = platform
	// 3 = stack
	// 4 = homebase
	// 5 = startloc
	int current = mapVector[Pos.x][Pos.y];
	switch(current) {
		case 0 : mapVector[Pos.x][Pos.y] = type;
		case 1 : if (type == 2 || type == 3 || type == 5) {
			mapVector[Pos.x][Pos.y] = type;
		}
		case 2 : if (type == 3 || type == 5) {
			mapVector[Pos.x][Pos.y] = type;
		}
		case 4 : if (type == 1 || type == 2 || type == 3 || type == 5) {
			mapVector[Pos.x][Pos.y] = type;
		}
	}
}

mapGridMap map::createZeroMap(int xDim, int yDim) {
	std::vector<int> tempVector;
	mapGridMap tempMap;

	for (int x = 0; x <= xDim; ++x) {
		tempVector.clear();
		for (int y = 0; y <= yDim; ++y) {
			tempVector.push_back(EMPTY);
		}
		tempMap.push_back(tempVector);
	}
	return tempMap;
}

struct mapPosition map::getMaxPos(mapPositionVector posVec) {
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

void map::printMapFile(std::string filename) {
	std::ofstream outFile;
	outFile.open(filename);

	for (int y=MAX_POS.y; y >= 0; --y) {
		for (int x = 0; x <= MAX_POS.x; ++x) {
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

double map::getDistance(struct mapPosition Pos1, struct mapPosition Pos2) {
	double dx1, dy1, dx2, dy2;

	dx1 = indToInch(Pos1.x);
	dy1 = indToInch(Pos1.y);
	dx2 = indToInch(Pos2.x);
	dy2 = indToInch(Pos2.y);

	double answer = sqrt(pow(dx2-dx1,2)+pow(dy2-dy1,2));
	return answer;
}

//returns a position vector for a line of sight
struct mapPosition map::getEndPoint(struct mapPosition Pos1, int orientation){
    double deltaX = cos(orientation*3.1415/180.0);
    double deltaY = sin(orientation*3.1415/180.0);
    double newX = Pos1.x;
    double newY = Pos1.y;
    mapPosition tempPos = Pos1;
    while (isPassable(tempPos)){
        newX += deltaX;
        newY += deltaY;
        int tempX, tempY;
        tempX = ceil(newX);
        tempY = ceil(newY);
        tempPos = {tempX,tempY};
    }

    return tempPos; //first blocked pos
}

//returns an ideal "sonar" reading for a position and vector
double map::getSonarReading(struct mapPosition Pos1, int orientation){
    mapPosition imped = getEndPoint(Pos1, orientation);
    return getDistance(Pos1, imped);
}

double map::getSonarReading(double x, double y, int angle){
    mapPosition myMapPosition;
    int myX;
    int myY;
    int myAngle=angle;
    myX=inchToInd(x);
    myY=inchToInd(y);
    myMapPosition.x=myX;
    myMapPosition.y=myY;

    return getSonarReading(myMapPosition,myAngle);
}


double map::getSonarReadingFront(double x, double y, int angle){
    return getSonarReading(x,y,-angle);
}

double map::getSonarReadingRight(double x, double y, int angle){
    return getSonarReading(x,y,angle+90);
}


double map::getSonarReadingLeft(double x, double y, int angle){
    return getSonarReading(x,y,angle+270);
}


double map::getSonarReadingBack(double x, double y, int angle){
    return getSonarReading(x,y,angle+180);
}

struct mapPosition map::getClosestHomeBase(struct mapPosition currentPos) {
	return getClosestItem(currentPos, homeBases.getPositions());
}

struct mapPosition map::getClosestStack(struct mapPosition currentPos) {
	return getClosestItem(currentPos, stacks.getPositions());
}

struct mapPosition map::getClosestItem(struct mapPosition currentPos, mapPositionVector posVec) {
	mapPosition minPos = {0,0};
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

struct mapCubeStack map::lookupStackOrder(struct mapPosition stackPos) {
	return stacks.getCubeStack(stackPos);
}

void map::removeStack(struct mapPosition stackPos) {
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

void map::floodFillOutside() {
	int x, y;
	mapPosition pos;
	int target = EMPTY;
	int replacement = OUTSIDE;
	for (x = 0; x <= MAX_POS.x; ++x) {
		pos = {x,0};
		floodFill(pos, target, replacement);
		pos = {x,MAX_POS.y};
		floodFill(pos, target, replacement);
	}
	for (y = 0; y <= MAX_POS.y; ++y) {
		pos = {0,y};
		floodFill(pos, target, replacement);
		pos = {MAX_POS.x,y};
		floodFill(pos, target, replacement);
	}
}

void map::floodFillHomeBase() {
	mapPosition pos = getAveragePosition(homeBases.getPositions());
	int target = EMPTY;
	int replacement = HOMEBASE;
	floodFill(pos,target,replacement);
}

void map::floodFill(struct mapPosition pos, int target, int replacement) {
	int x = fmax(0,fmin(MAX_POS.x,pos.x));
	int y = fmax(0,fmin(MAX_POS.y,pos.y));
	pos = {x,y};
	int color = mapVector[x][y];
	if (target == replacement) {
	}
	else if (color == target) {
		writeMapVector(pos,replacement);
		mapPosition west = {x-1,y};
		mapPosition east = {x+1,y};
		mapPosition north = {x,y+1};
		mapPosition south = {x,y-1};
		floodFill(west,target,replacement);
		floodFill(east,target,replacement);
		floodFill(north,target,replacement);
		floodFill(south,target,replacement);
	}
}

struct mapPosition map::getAveragePosition(mapPositionVector posVec) {
	int x, y;
	int xTot = 0;
	int yTot = 0;
	for (int i = 0; i < posVec.size(); i++) {
		xTot += posVec[i].x;
		yTot += posVec[i].y;
	}
	x = round(xTot/posVec.size());
	y = round(yTot/posVec.size());
	mapPosition Pos = {x,y};
	return Pos;
}

int main_map() {
    map myMap("practice_map.txt");
}

//map myMap("green_map.txt");
//
// 0 = empty
// 1 = wall
// 2 = platform
// 3 = stack
// 4 = homebase
// 5 = startloc
// 6 = voidspace
