#include "map.h"
#include "ctime"
#include <cstdio>

template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F>
    static typename TimeT::rep execution(F const &func)
    {
        auto start = std::chrono::system_clock::now();
        func();
        auto duration = std::chrono::duration_cast< TimeT>(
            std::chrono::system_clock::now() - start);
        return duration.count();
    }
};

map::map(std::string filename):
walls(RES_INIT, RES_FIN),
platforms(RES_INIT, RES_FIN),
stacks(RES_INIT, RES_FIN),
homeBases(RES_INIT, RES_FIN),
startLoc(RES_INIT, RES_FIN) {

	MAX_POS = {0,0};
	mapFilename = filename;
	buildMap(mapFilename);
}

bool map::isPassable(struct mapPosition Pos) {
	int x, y;
	int range = floor(ROBOT_SIZE / RES_FIN);
	mapPositionVector posVec = generateSquareAroundPoint(Pos, range);
	for (int i = 0; i < posVec.size(); ++i) {
		x = fmax(0,fmin(posVec[i].x,MAX_POS.x));
		y = fmax(0,fmin(posVec[i].y,MAX_POS.y));
		if (!typeIsPassable(mapVector[x][y])) {
			return false;
		}
	}
	return true;;
}

struct mapPosition map::mostValuableStack(int currentGreen, int currentRed) {
	int value, green, red;
	int maxGain = 0;
	struct mapPosition stackPos, maxStackPos;
	struct mapCubeStack cubeStack;

	mapPositionVector stackPositions = stacks.getPositions();
	for (int i = 0; i < stackPositions.size(); i++) {
		value = 0;
		stackPos = stackPositions[i];
		cubeStack = stacks.getCubeStack(stackPos);
		red = 3 - (cubeStack.bot + cubeStack.mid + cubeStack.top);
		green = 3 - red;
		value += pow(green+currentGreen,2) - pow(currentGreen,2);
		value += pow(red+currentRed,2) - pow(currentRed,2);
		if (value > maxGain) {
			maxGain = value;
			maxStackPos = stackPos;
		}
		if (value == maxGain && value != 0) {
			
		}


	}
}

bool map::isWall(struct mapPosition pos){
    int sizex= mapVector.size();
    if (pos.x >=sizex ||pos.x<=0)
        return true;
    int sizey = mapVector[pos.x].size();
    if (pos.y>=sizey ||pos.y<=0)
        return true;
    int type = mapVector[pos.x][pos.y];
    switch(type) {
        case WALL : return true;
        case PLATFORM : return true;
        case OUTSIDE : return true;
        default: return false;
    }
}

bool map::isWall(double x, double y){
    mapPosition tempPos;
    int indX = inchToInd(x);
    int indY = inchToInd(y);
    tempPos.x=indX;
    tempPos.y=indY;
    return isWall(tempPos);
}

bool map::typeIsPassable(int type) {
	bool isPassable = false;
	switch(type) {
		case 0 : isPassable = true;
		case 3 : isPassable = true;
		case 4 : isPassable = true;
		case 5 : isPassable = true;
	}
	return isPassable;
}

mapPositionVector map::generateCircleAroundPoint(struct mapPosition Pos, int diameter) {
	mapPositionVector tempVec;
	mapPosition tempPos;
	int radius = ceil(diameter/2);
	for (int theta = 0; theta <= 360; theta += 5) {
		tempPos.x = round(Pos.x + radius*cos(theta));
		tempPos.y = round(Pos.y + radius*sin(theta));
		tempVec.push_back(tempPos);
	}
	return tempVec;
}

mapPositionVector map::generateSquareAroundPoint(struct mapPosition Pos, int diameter) {
	mapPositionVector tempVec;
	mapPosition tempPos1, tempPos2;
	int radius = ceil(diameter/2);
	for (int offx = -radius; offx <= radius; ++offx) {
		tempPos1 = {Pos.x+offx,Pos.y-radius};
		tempPos2 = {Pos.x+offx,Pos.y+radius};
		tempVec.push_back(tempPos1);
		tempVec.push_back(tempPos2);
	}
	for (int offy = -radius; offy <= radius; ++offy) {
		tempPos1 = {Pos.x-radius,Pos.y+offy};
		tempPos2 = {Pos.x+radius,Pos.y+offy};
		tempVec.push_back(tempPos1);
		tempVec.push_back(tempPos2);
	}
	return tempVec;
}

void map::buildMap(std::string filename) {
	parseMapFile(filename);
	parseObjects();
	floodFillOutside(); // fill the outside map with 6's
	floodFillHomeBase();
    //floodFill({100,100},0,1);
	printMapFile("map.txt");
	getMostsRRT();
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

double map::getDistanceInd(struct mapPosition Pos1, struct mapPosition Pos2) {
	return sqrt(pow(Pos2.x-Pos1.x,2)+pow(Pos2.y-Pos1.y,2));
}

struct mapPosition map::getEndPoint(struct mapPosition Pos1, int orientation){
    double deltaX = cos(orientation*3.1415/180.0);
    double deltaY = sin(orientation*3.1415/180.0);
    double newX = Pos1.x;
    double newY = Pos1.y;
    mapPosition tempPos = Pos1;
    while (!isWall(tempPos)){
        newX += deltaX;
        newY += deltaY;
        int tempX, tempY;
        tempX = ceil(newX);
        tempY = ceil(newY);
        tempPos = {tempX,tempY};
    }

    return tempPos;
}

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
#if MAP_DEBUG
    printf("x= %lf y = %lf\n", x, y);
    printMapFile("mapDebug.txt");
#endif
    return getSonarReading(myMapPosition,myAngle);
}


double map::getSonarReadingFront(double x, double y, int angle){
    double newX;
    double newY;
    angle=-angle;
    double cosAngle=cos(angle*PI/180);
    double sinAngle =sin(angle*PI/180);
    newX= x + cosAngle*MAP_ROBOT_DISTANCE_CENTER_FRONT_SENSOR_X - sinAngle*MAP_ROBOT_DISTANCE_CENTER_FRONT_SENSOR_Y;
    newY= y + sinAngle*MAP_ROBOT_DISTANCE_CENTER_FRONT_SENSOR_X + cosAngle* MAP_ROBOT_DISTANCE_CENTER_FRONT_SENSOR_Y;
#if MAP_DEBUG
    mapVector[inchToInd(newX)][inchToInd(newY)]=9;
#endif
    return getSonarReading(newX,newY,angle);
}

double map::getSonarReadingRight(double x, double y, int angle){
    double newX;
    double newY;
    angle+=90;
    angle=-angle;
    double cosAngle=cos(angle*PI/180);
    double sinAngle =sin(angle*PI/180);
    newX= x + cosAngle*MAP_ROBOT_DISTANCE_CENTER_RIGHT_SENSOR_X - sinAngle*MAP_ROBOT_DISTANCE_CENTER_RIGHT_SENSOR_Y;
    newY= y + sinAngle*MAP_ROBOT_DISTANCE_CENTER_RIGHT_SENSOR_X + cosAngle* MAP_ROBOT_DISTANCE_CENTER_RIGHT_SENSOR_Y;

#if MAP_DEBUG
    mapVector[inchToInd(newX)][inchToInd(newY)]=8;
#endif
    return getSonarReading(newX,newY,angle);
}


double map::getSonarReadingLeft(double x, double y, int angle){
    double newX;
    double newY;
    angle+=270;
    angle=-angle;
    double cosAngle=cos(angle*PI/180);
    double sinAngle =sin(angle*PI/180);
    newX= x + cosAngle*MAP_ROBOT_DISTANCE_CENTER_LEFT_SENSOR_X - sinAngle*MAP_ROBOT_DISTANCE_CENTER_LEFT_SENSOR_Y;
    newY= y + sinAngle*MAP_ROBOT_DISTANCE_CENTER_LEFT_SENSOR_X + cosAngle* MAP_ROBOT_DISTANCE_CENTER_LEFT_SENSOR_Y;
#if MAP_DEBUG
    mapVector[inchToInd(newX)][inchToInd(newY)]=7;
#endif
    return getSonarReading(newX,newY,angle);
}


double map::getSonarReadingBack(double x, double y, int angle){
    double newX;
    double newY;
    angle+=180;
    angle=-angle;
    newX= x + cos(angle*PI/180)*MAP_ROBOT_DISTANCE_CENTER_BACK_SENSOR_X;
    newY= y+ sin(angle*PI/180)*MAP_ROBOT_DISTANCE_CENTER_BACK_SENSOR_X;
#if MAP_DEBUG
    mapVector[inchToInd(x)][inchToInd(y)]=3;
    mapVector[inchToInd(newX)][inchToInd(newY)]=5;
#endif
    return getSonarReading(newX,newY,angle);}

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
	int ind = (int) round(inch/gridResDouble);
	return ind;
}


double map::getStartLocationX(){
    mapPosition startPos = startLoc.getPositions()[0];
    double x = startPos.x;
    x= indToInch(x);
    return x;

}

double map::getStartLocationY(){
    mapPosition startPos = startLoc.getPositions()[0];
    double y = startPos.y;
    y=indToInch(y);
    return y;
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

/*

RRT IMPLEMENTATION

*/

mapPositionVector map::findPathRRT(struct mapPosition startPos, struct mapPosition goalPos) {
	nodeVectorRRT.clear();

	mapNode nextNode;
	mapNode startNode = {startPos,0};
	nodeVectorRRT.push_back(startNode);

	mapPosition nextPos, nearestPos, latestConnectedPos;
	latestConnectedPos = startPos;
	int nearestPosInd;

	int K = 0;

	while (!(latestConnectedPos == goalPos) && K < RRT_NUM_ITERS) {
		nextPos = randConfRRT(goalPos, RAND_CONF_BIAS);
		nearestPosInd = nearestVertexIndiceRRT(nextPos);
		nearestPos = getPosAtIndiceRRT(nearestPosInd);

		if (isConnectableRRT(nextPos,nearestPos)) {
			nextNode = {nextPos,nearestPosInd};
			nodeVectorRRT.push_back(nextNode);
			latestConnectedPos = nextNode.pos;
		}
		K++;
	}

	printNodes(nodeVectorRRT, "nodes.txt");
	printMapFile("map.txt");

	if (latestConnectedPos == goalPos) {
		return smooth(buildPathRRT(nodeVectorRRT));
	}
	else {
		return findPathRRT(startPos, goalPos);
	}
}

void map::printNodes(mapNodeVector nodes, std::string filename) {
	std::ofstream outFile;
	outFile.open(filename);

	for (int i = 0; i < nodes.size(); i++) {
		outFile << i << ": (" << nodes[i].pos.x << "," << nodes[i].pos.y << "), " << nodes[i].parent << std::endl;
	}

	outFile.close();
}

mapPositionVector map::buildPathRRT(mapNodeVector nodes) {
	mapPositionVector path;
	int parentInd = nodes.size()-1;

	path.emplace(path.begin(),getPosAtIndiceRRT(parentInd));

	while (parentInd != 0) {
		parentInd = getParentAtIndiceRRT(parentInd);
		path.emplace(path.begin(),getPosAtIndiceRRT(parentInd));
	}

	return path;
}

struct mapPosition map::randConfRRT(struct mapPosition goalPos, int bias) {
	if (std::rand() % 100 < bias) {
		return goalPos;
	}

	int x = (std::rand() % (RIGHTMOST-LEFTMOST)) + LEFTMOST;
	int y = (std::rand() % (UPMOST-DOWNMOST)) + DOWNMOST;
	mapPosition Pos = {x,y};

	while (!(isPassable(Pos))) {
		Pos.x = (std::rand() % (RIGHTMOST-LEFTMOST)) + LEFTMOST;
		Pos.y = (std::rand() % (UPMOST-DOWNMOST)) + DOWNMOST;
	}

	return Pos;
}

int map::nearestVertexIndiceRRT(struct mapPosition pos) {
	double distance;
	int nearestPosIndice = 0;
	double minDist = getDistance(pos, getPosAtIndiceRRT(nearestPosIndice));
	for (int i = 1; i < nodeVectorRRT.size(); i++) {
		distance = getDistance(pos, getPosAtIndiceRRT(i));
		if (distance < minDist) {
			minDist = distance;
			nearestPosIndice = i;
		}
	}
	return nearestPosIndice;
}

int map::getParentAtIndiceRRT(int indice) {
	mapNode tempNode = nodeVectorRRT[indice];
	int tempNodeParent = tempNode.parent;
	return tempNodeParent;
}

struct mapPosition map::getPosAtIndiceRRT(int indice) {
	mapNode tempNode = nodeVectorRRT[indice];
	mapPosition tempNodePos = tempNode.pos;
	return tempNodePos;
}

bool map::isConnectableRRT(struct mapPosition startPos, struct mapPosition goalPos) {
	double stepSize = ROBOT_SIZE;
	mapPosition Pos = startPos;

	if (!(isPassable(startPos)) || !(isPassable(goalPos))) {
		return false;
	}
	while (getDistance(Pos,goalPos) > stepSize) {
		Pos = stepFromToRRT(Pos, goalPos, stepSize);
		if (!(isPassable(Pos))) {
			return false;
		}
	}
	return true;
}

double map::getThetaRRT(struct mapPosition Pos1, struct mapPosition Pos2) {
	double x1, y1, x2, y2;
	x1 = indToInch(Pos1.x);
	y1 = indToInch(Pos1.y);
	x2 = indToInch(Pos2.x);
	y2 = indToInch(Pos2.y);
	return atan2(y2-y1,x2-x1);
}

struct mapPosition map::stepFromToRRT(struct mapPosition Pos1, struct mapPosition Pos2, double stepSize) {
	if (getDistance(Pos1, Pos2) < stepSize) {
		return Pos2;
	}
	else {
		double x1, y1;
		x1 = indToInch(Pos1.x);
		y1 = indToInch(Pos1.y);
		double theta = getThetaRRT(Pos1, Pos2);
		double xd = x1 + stepSize*cos(theta);
		double yd = y1 + stepSize*sin(theta);
		int x = inchToInd(xd);
		int y = inchToInd(yd);
		mapPosition newPos = {x,y};
		return newPos;
	}
}

void map::getMostsRRT() {
	LEFTMOST = fmin(walls.minBarrierX, platforms.minBarrierX);
	RIGHTMOST = fmax(walls.maxBarrierX, platforms.maxBarrierX);
	UPMOST = fmax(walls.maxBarrierY, platforms.maxBarrierY);
	DOWNMOST = fmin(walls.minBarrierY, platforms.minBarrierY);
}

mapPositionVector map::smooth(mapPositionVector posVec) {
	mapPositionVector tempVec;
	int ind = 0;

	while (posVec.size() > 1) {
		ind = getFarthestConnectablePoint(posVec);
		tempVec.push_back(posVec[0]);
		for (int i = 0; i < ind; i++) {
			posVec.erase(posVec.begin());
		}
	}
	tempVec.push_back(posVec[0]);

	return tempVec;
}

void map::printMapPositionVector(mapPositionVector posVec) {
	int size = posVec.size();
	for (int i = 0; i < size; i++) {
		std::cout << i << ": (" << posVec[i].x << "," << posVec[i].y << ")" << std::endl;
	}
}

int map::getFarthestConnectablePoint(mapPositionVector posVec) {
	int ind = 1;

	for (int i = 1; i < posVec.size(); i++) {
		if (isConnectableRRT(posVec[0],posVec[i])) {
			ind = i;
		}
	}
	
	return ind;
}

int main_map_simple() {
	map myMap("practice_map.txt");
    mapPosition startPos = myMap.startLoc.getPositions()[0];
    mapPosition goalPos = myMap.stacks.getPositions()[0];
    mapPositionVector path = myMap.findPathRRT(startPos,goalPos);
    myMap.printMapPositionVector(path);
}

int main_map() {
	int times = 500;

	map myMap("practice_map.txt");
    mapPosition startPos = myMap.startLoc.getPositions()[0];
    mapPosition goalPos = myMap.stacks.getPositions()[0];
    std::cout << "average time: ";
	std::cout << measure<>::execution( [&]() {
	for (int i = 0; i < times; i++) {
		mapPositionVector path = myMap.findPathRRT(startPos,goalPos);
	}})/times << "ms" << std::endl;

	std::cout << "jagged path" << std::endl;
	mapPositionVector path = myMap.findPathRRT(startPos,goalPos);
	for (int i = 0; i < path.size(); i++) {
		myMap.mapVector[path[i].x][path[i].y] = 7;
    	std::cout << i << ": (" << path[i].x << "," << path[i].y << ")" << std::endl;
    }

    mapPositionVector smoothPath = path;
    std::cout << "smooth path" << std::endl;
    smoothPath = myMap.smooth(smoothPath);

    for (int i = 0; i < smoothPath.size(); i++) {
    	myMap.mapVector[smoothPath[i].x][smoothPath[i].y] = 7;
    	std::cout << i << ": (" << smoothPath[i].x << "," << smoothPath[i].y << ")" << std::endl;
    }

    myMap.printMapFile("mapnoded.txt");
    std::cout << "the start was: (" << startPos.x << "," << startPos.y << ")" << std::endl;
    std::cout << "the goal was: (" << goalPos.x << "," << goalPos.y << ")" << std::endl;
    std::cout << "congrats you did it!" << std::endl;
}
