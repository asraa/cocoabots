#include "map.h"
#include <typeinfo>

map::map(std::string filename) {
	maxX = 0;
	maxY = 0;
	res = RESOLUTION_FIN;
	convFactor = RESOLUTION_INIT / RESOLUTION_FIN;

	mapFilename = filename;
	buildMap(mapFilename);
	// do shit
}

void map::buildMap(std::string filename) {
	std::string currentLine;
	std::vector<std::string> tempVector;

	std::ifstream mapFile;
	mapFile.open(filename);

	while (!mapFile.eof()) {
		tempVector.clear();
		getline(mapFile,currentLine);
		std::istringstream iss(currentLine);
		std::string token;
		while (getline(iss, token, ',')) {
			tempVector.push_back(token);
		}

		std::string id = tempVector[0];
		tempVector.erase(tempVector.begin()); // remove 1st elt

		if (id.compare("W") == 0) { 		// WALL
			updateWalls(tempVector);
		}
		else if (id.compare("P") == 0) {	// PLATFORM
			updatePlatforms(tempVector);
		}
		else if (id.compare("S") == 0) {	// STACK
			updateStacks(tempVector);
		}
		else if (id.compare("H") == 0) {	// HOMEBASE
			updateHomeBases(tempVector);
		}
		else if (id.compare("L") == 0) {	// STARTLOC
			updateStartLoc(tempVector);
		}
	}
	mapFile.close();

	mapVector = createZeroMap(maxX, maxY);

	addWalls();
	addPlatforms();
	addStacks();
	addHomeBases();
	addStartLoc();

	for (int x = 0; x <= maxX; ++x) {
		for (int y = 0; y <= maxY; ++y) {
			std::cout << mapVector[x][y];
		}
		std::cout << std::endl;
	}
}

// typedef std::vector<std::tuple<int,int,int,int>> wallItems
// typedef std::vector<std::tuple<int,int,int,int>> platformItems
// typedef std::vector<std::tuple<int,int,int,int,int>> stackItems
// typedef std::vector<std::tuple<int,int>> homeBaseItems;
// typedef std::tuple<int,int> startLocItem

void map::addWalls() {
	for (int item = 0; item < walls.size(); ++item) {
		int x1 = std::get<0>(walls[item]);
		int y1 = std::get<1>(walls[item]);
		int x2 = std::get<2>(walls[item]);
		int y2 = std::get<3>(walls[item]);
		int deltaX = (x2 - x1) / convFactor;
		int deltaY = (y2 - y1) / convFactor;
		for (int i = 0; i <= convFactor; ++i) {
			int xCoord = x1 + deltaX*i;
			int yCoord = y1 + deltaY*i;
			mapVector[xCoord][yCoord] = 1;
		}
	}
}

void map::addPlatforms() {
	for (int item = 0; item < platforms.size(); ++item) {
		int x1 = std::get<0>(platforms[item]);
		int y1 = std::get<1>(platforms[item]);
		int x2 = std::get<2>(platforms[item]);
		int y2 = std::get<3>(platforms[item]);
		int deltaX = (x2 - x1) / convFactor;
		int deltaY = (y2 - y1) / convFactor;
		for (int i = 0; i <= convFactor; ++i) {
			int xCoord = x1 + deltaX*i;
			int yCoord = y1 + deltaY*i;
			mapVector[xCoord][yCoord] = 2;
		}
	}
}

void map::addStacks() {
	for (int item = 0; item < stacks.size(); ++item) {
		int x1 = std::get<0>(stacks[item]);
		int y1 = std::get<1>(stacks[item]);
		int xCoord = ceil(x1 + ceil(convFactor/2));
		int yCoord = ceil(y1 + ceil(convFactor/2));
		mapVector[xCoord][yCoord] = 3;
	}
}

void map::addHomeBases() {
	for (int item = 0; item < homeBases.size(); ++item) {
		int x1 = std::get<0>(homeBases[item]);
		int y1 = std::get<1>(homeBases[item]);
		for (int i = 0; i < convFactor; ++i) {
			int xCoord = x1 + i;
			for (int j = 0; j < convFactor; ++j) {
				int yCoord = y1 + j;
				mapVector[xCoord][yCoord] = 4;
			}
		}
	}
}

void map::addStartLoc() {
	int x1 = std::get<0>(startLoc);
	int y1 = std::get<1>(startLoc);	
	mapVector[x1][y1] = 5;
}

void map::updateWalls(std::vector<std::string> tempVector) {
	std::string tempString;
	const char* tempChar;
	std::vector<int> tempInts;


	for (int i = 0; i < tempVector.size(); ++i) {
		tempString = tempVector[i];
		tempChar = tempString.c_str();
		tempInts.push_back(convFactor*std::stoi(tempChar));
	}

	int x1 = tempInts[0];
	int y1 = tempInts[1];
	int x2 = tempInts[2];
	int y2 = tempInts[3];

	updateMaxX(x1);
	updateMaxX(x2);
	updateMaxY(y1);
	updateMaxY(y2);

	std::tuple<int,int,int,int> x1y1x2y2 = std::make_tuple(x1,y1,x2,y2);
	walls.push_back(x1y1x2y2);

}


void map::updatePlatforms(std::vector<std::string> tempVector) {
	std::string tempString;
	const char* tempChar;
	std::vector<int> tempInts;


	for (int i = 0; i < tempVector.size(); ++i) {
		tempString = tempVector[i];
		tempChar = tempString.c_str();
		tempInts.push_back(convFactor*std::stoi(tempChar));
	}

	int x1 = tempInts[0];
	int y1 = tempInts[1];
	int x2 = tempInts[2];
	int y2 = tempInts[3];

	updateMaxX(x1);
	updateMaxX(x2);
	updateMaxY(y1);
	updateMaxY(y2);

	std::tuple<int,int,int,int> x1y1x2y2 = std::make_tuple(x1,y1,x2,y2);
	platforms.push_back(x1y1x2y2);
}


void map::updateStacks(std::vector<std::string> tempVector) {
	std::string tempString;
	const char* tempChar;
	std::vector<int> tempInts;

	for (int i = 0; i < 2; ++i) {
		tempString = tempVector[i];
		tempChar = tempString.c_str();
		tempInts.push_back(convFactor*std::stoi(tempChar));
	}

	int x1 = tempInts[0];
	int y1 = tempInts[1];

	updateMaxX(x1);
	updateMaxY(y1);

	//   RED CUBE = 0
	// GREEN CUBE = 1
	//
	// C1->C2->C3
	// BOTTOM->MIDDLE->TOP

	int c1, c2, c3;

	if (tempVector[2].compare("R") == 0) {
		c1 = 0;
	}
	else {
		c1 = 1;
	}
	if (tempVector[3].compare("R") == 0) {
		c2 = 0;
	}
	else {
		c2 = 1;
	}
	if (tempVector[4].compare("R") == 0) {
		c3 = 0;
	}
	else {
		c3 = 1;
	}

	std::tuple<int,int,int,int,int> xyccc = std::make_tuple(x1,y1,c1,c2,c3);
	stacks.push_back(xyccc);
}


void map::updateHomeBases(std::vector<std::string> tempVector) {
	std::string tempString;
	const char* tempChar;
	std::vector<int> tempInts;


	for (int i = 0; i < tempVector.size(); ++i) {
		tempString = tempVector[i];
		tempChar = tempString.c_str();
		tempInts.push_back(std::stoi(tempChar));
	}

	int N = tempInts[0];
	tempInts.erase(tempInts.begin()); // remove 1st elt

	for (int i = 0; i < N; ++i) {
		int x = convFactor*tempInts[2*i];
		int y = convFactor*tempInts[2*i+1];
		updateMaxX(x);
		updateMaxY(y);
		std::tuple<int,int> tempTuple = std::make_tuple(x,y);
		homeBases.push_back(tempTuple);
	}
}


void map::updateStartLoc(std::vector<std::string> tempVector) {
	std::string tempString;
	const char* tempChar;
	std::vector<int> tempInts;


	for (int i = 0; i < 2; ++i) {
		tempString = tempVector[i];
		tempChar = tempString.c_str();
		tempInts.push_back(convFactor*std::stoi(tempChar));
	}

	int x = tempInts[0];
	int y = tempInts[1];
	updateMaxX(x);
	updateMaxY(y);

	std::tuple<int,int> loc = std::make_tuple(x,y);
	startLoc = loc;
}


void map::updateMaxX(int x) {
	if (x > maxX) {
		maxX = x;
	}
}


void map::updateMaxY(int y) {
	if (y > maxY) {
		maxY = y;
	}
}

gridMap map::createZeroMap(int xDim, int yDim) {
	std::vector<int> tempVector;
	gridMap tempMap;

	// intentionally creating a grid 0->xDim indexed
	// rather than 0->(xDim-1) indexed
	//
	// same for 0->yDim
	//
	// this is because of how staff is doing it.
	//

	for (int x = 0; x <= xDim; ++x) {
		tempVector.clear();
		for (int y = 0; y <= yDim; ++y) {
			tempVector.push_back(0);
		}
		tempMap.push_back(tempVector);
	}
	return tempMap;
}

bool map::isPassable(intTuple pos) {
	// return True || False
}

int main() {
	map myMap("red_map.txt");
}
