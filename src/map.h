#ifndef MAP_H
#define MAP_H

#define RESOLUTION_INIT 24 // inches
#define RESOLUTION_FIN 2 // inches
#define ROBOT_SIZE 29 // max diag inches
#define MAPFILENAME "../map.txt"
#define EMTPY 0
#define WALL 1
#define PLATFORM 2
#define STACK 3
#define HOMEBASE 4
#define STARTLOC 5


#include "configFile.h"
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

typedef std::vector<int> vecInt;
typedef std::vector< std::vector<int> > gridMap;
typedef std::tuple<int,int> intTuple;

typedef std::vector<std::tuple<int,int,int,int>> wallItems;
typedef std::vector<std::tuple<int,int,int,int>> platformItems;
typedef std::vector<std::tuple<int,int,int,int,int>> stackItems;
typedef std::vector<std::tuple<int,int>> homeBaseItems;
typedef std::tuple<int,int> startLocItem;

class map
{
	public:
		map(std::string filename);
		void buildMap(std::string filename);
		void addWalls();
		void addPlatforms();
		void addStacks();
		void addHomeBases();
		void addStartLoc();
		void updateWalls(std::vector<std::string> tempVector);
		void updatePlatforms(std::vector<std::string> tempVector);
		void updateStacks(std::vector<std::string> tempVector);
		void updateHomeBases(std::vector<std::string> tempVector);
		void updateStartLoc(std::vector<std::string> tempVector);
		void updateMaxX(int x);
		void updateMaxY(int y);
		gridMap createZeroMap(int xDim, int yDim);
		bool isPassable(intTuple pos);

		int maxX, maxY;

		std::string mapFilename;
		gridMap mapVector;
		intTuple robotLoc;
		int res;
		int convFactor;

		wallItems walls;
		platformItems platforms;
		stackItems stacks;
		homeBaseItems homeBases;
		startLocItem startLoc;
};

#endif
