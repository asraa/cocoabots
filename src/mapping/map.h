#ifndef MAP_H
#define MAP_H

#define RES_INIT 24 // inches
#define RES_FIN 1 // inches
#define ROBOT_SIZE 16 // max diag inches
#define MAPFILENAME "map.txt"
#define EMPTY 0
#define WALL 1
#define PLATFORM 2
#define STACK 3
#define HOMEBASE 4
#define STARTLOC 5
#define OUTSIDE 6
#define RAND_CONF_BIAS 5
#define RRT_NUM_ITERS 5000

#include "../configFile.h"
#include "objects/barrier.h"
#include "objects/stack.h"
#include "objects/homebase.h"
#include "objects/startlocation.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <random>

class map
{

	public:
		map(std::string filename);
		bool isPassable(struct mapPosition Pos);
		bool typeIsPassable(int type);
		mapPositionVector generateCircleAroundPoint(struct mapPosition Pos, int diameter);
        bool isWall(struct mapPosition pos);
        bool isWall(double x, double y);

		mapPositionVector generateSquareAroundPoint(struct mapPosition Pos, int diameter);
		void buildMap(std::string filename);
		void buildMapRRT();
		void printMapFileRRT(std::string filename);
		void parseMapFile(std::string filename);
		void parseObjects();
		void writeMapVector(struct mapPosition Pos, int type);
		void printMapFile(std::string filename);
		mapGridMap createZeroMap(int xDim, int yDim);
		struct mapPosition getMaxPos(mapPositionVector posVec);
		int sign(int num);
		int convertInd(int ind, int gridResInit, int gridResFin);
		double getDistance(struct mapPosition Pos1, struct mapPosition Pos2);
		struct mapPosition getClosestHomeBase(struct mapPosition currentPos);
		struct mapPosition getClosestStack(struct mapPosition currentPos);
		struct mapPosition getClosestItem(struct mapPosition currentPos, mapPositionVector posVec);
		struct mapCubeStack lookupStackOrder(struct mapPosition stackPos);
        struct mapPosition getEndPoint(struct mapPosition Pos1, int orientation);

        double getSonarReading(struct mapPosition Pos1, int orientation);
        double getSonarReading(double x, double y, int angle);
        double getSonarReadingFront(double x, double y, int angle);
        double getSonarReadingRight(double x, double y, int angle);
        double getSonarReadingLeft(double x, double y, int angle);
        double getSonarReadingBack(double x, double y, int angle);

        double getStartLocationX();
        double getStartLocationY();

		void removeStack(struct mapPosition stackPos);
		double indToInch(int ind);
		int inchToInd(double inch);
		void floodFillHomeBase();
		void floodFillOutside();
		struct mapPosition getAveragePosition(mapPositionVector posVec);
		void floodFill(struct mapPosition pos, int target, int replacement);

		// RRT begins
		mapNodeVector nodeVectorRRT;
		mapPositionVector findPathRRT(struct mapPosition startPos, struct mapPosition goalPos);
		void printNodes(mapNodeVector nodes, std::string filename);
		mapPositionVector buildPathRRT(mapNodeVector nodes);
		struct mapPosition randConfRRT(struct mapPosition goalPos, int bias);
		int nearestVertexIndiceRRT(struct mapPosition pos);
		int getParentAtIndiceRRT(int indice);
		struct mapPosition getPosAtIndiceRRT(int indice);
		bool isConnectableRRT(struct mapPosition startPos, struct mapPosition goalPos);
		double getThetaRRT(struct mapPosition Pos1, struct mapPosition Pos2);
		struct mapPosition stepFromToRRT(struct mapPosition Pos1, struct mapPosition Pos2, double stepSize);
		void getMostsRRT();
		void printMapPositionVector(mapPositionVector posVec);
		//mapPositionVector smoothTriangle(mapPosition currentPos, mapPosition middlePos, mapPosition finalPos);
		mapPositionVector smooth(mapPositionVector posVec);
		int getFarthestConnectablePoint(mapPositionVector posVec);

		// std::random_device rd;
		// std::mt19937 gen(rd());
		// std::uniform_real_distribution<> random(0,1);
		// RRT ends

		int LEFTMOST, RIGHTMOST, UPMOST, DOWNMOST;
		mapPosition MAX_POS;

		barrier walls, platforms;
		stack stacks;
		homeBase homeBases;
		startLocation startLoc;

		std::string mapFilename;
		mapGridMap mapVector;
		mapPosition robotLoc;
};

#endif
