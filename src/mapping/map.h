#ifndef MAP_H
#define MAP_H

#define RES_INIT 24 // inches
#define RES_FIN 3 // inches
#define ROBOT_SIZE 29 // max diag inches
#define MAPFILENAME "map.txt"
#define EMPTY 0
#define WALL 1
#define PLATFORM 2
#define STACK 3
#define HOMEBASE 4
#define STARTLOC 5
#define OUTSIDE 6

#include "../configFile.h"
#include "objects/barrier.h"
#include "objects/stack.h"
#include "objects/homebase.h"
#include "objects/startlocation.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <stdexcept>

class map
{

	public:

		typedef std::vector< std::vector<int> > gridMap;
		typedef std::tuple<int,int> position;
        int orientation; //orientation with angle between 0 and 360
		typedef std::vector< position > positionVector;
		typedef positionVector::iterator positionVectorIterator;
		typedef std::vector< std::string > stringVector;
		typedef std::tuple<int,int,int> cubeTuple;
		typedef std::map<position,cubeTuple> stackDictionary;

		map(std::string filename);
		bool isPassable(position Pos);
		bool typeIsPassable(int type);
		map::positionVector generateSquareAroundPoint(position Pos, int diameter);
		void buildMap(std::string filename);
		void parseMapFile(std::string filename);
		void parseObjects();
		void writeMapVector(position Pos, int type);
		void printMapFile(std::string filename);
		map::gridMap createZeroMap(int xDim, int yDim);
		map::position getMaxPos(positionVector posVec);
		int sign(int num);
		int convertInd(int ind, int gridResInit, int gridResFin);
		double getDistance(position Pos1, position Pos2);
		map::position getClosestHomeBase(position currentPos);
		map::position getClosestStack(position currentPos);
		map::position getClosestItem(position currentPos, positionVector posVec);
		map::cubeTuple lookupStackOrder(position stackPos);
        map::position getEndPoint(position Pos1, int orientation);
        double getSonarReading(position Pos1, int orientation);
		void removeStack(position stackPos);
		double indToInch(int ind);
		int inchToInd(double inch);
		void floodFillOutside();
		void floodFill(position pos, int target, int replacement);

		map::position MAX_POS;

		barrier walls, platforms;
		stack stacks;
		homeBase homeBases;
		startLocation startLoc;

		std::string mapFilename;
		map::gridMap mapVector;
		map::position robotLoc;
};

#endif
