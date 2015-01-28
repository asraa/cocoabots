#ifndef MAPUTILS_H
#define MAPUTILS_H

#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <random>

struct mapPosition {
	int x;
	int y;

	bool operator==(const mapPosition& p)
	{
		return (x==p.x) && (y==p.y);
	}

	mapPosition():x(0),y(0){}
	mapPosition(int X, int Y):x(X),y(Y){}

};

struct mapCubeStack {
	int bot;
	int mid;
	int top;
	mapCubeStack():bot(0),mid(0),top(0){}
	mapCubeStack(int BOT, int MID, int TOP):bot(BOT),mid(MID),top(TOP){}
};

struct mapPositionComparator {
	bool operator()(const mapPosition& p1, const mapPosition& p2) const {
		return (p1.x<p2.x) || (p1.x==p2.x && p1.y<p2.y);
	}
};

struct mapNode {
	struct mapPosition pos;
	int parent;
};
typedef std::map<struct mapPosition, struct mapPosition, mapPositionComparator> mapPosPosDictionary;
typedef std::map<struct mapPosition, double, mapPositionComparator> mapPosDoubleDictionary;
typedef std::vector< std::vector<int> > mapGridMap;
typedef std::vector<struct mapPosition> mapPositionVector;
typedef mapPositionVector::iterator mapPositionVectorIterator;
typedef std::vector<std::string> mapStringVector;
typedef std::map<struct mapPosition, struct mapCubeStack, mapPositionComparator> mapStackDictionary;
typedef std::vector<struct mapNode> mapNodeVector;

class mapUtils {
	public:
		mapUtils();
};

#endif