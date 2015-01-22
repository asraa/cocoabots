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
    typedef std::vector< position > positionVector;
    typedef positionVector::iterator positionVectorIterator;
    typedef std::vector< std::string > stringVector;
		map(std::string filename);
		void buildMap(std::string filename);
		void parseMapFile(std::string filename);
		void parseObjects();
		void writeMapVector(position Pos, int type);
		void printMapFile(std::string filename);
		gridMap createZeroMap(int xDim, int yDim);
		position getMaxPos(positionVector posVec);
		int sign(int num);
		int convertInd(int ind, int gridResInit, int gridResFin);
		double indToInch(int ind);
		int inchToInd(double inch);

		position MAX_POS;

		barrier walls, platforms;
		stack stacks;
		homeBase homeBases;
		startLocation startLoc;

		std::string mapFilename;
		gridMap mapVector;
		position robotLoc;
};

#endif
