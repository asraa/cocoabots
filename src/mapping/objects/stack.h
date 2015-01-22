#ifndef STACK_H
#define STACK_H

#include <map>
#include "objectssuperclass.h"

typedef std::tuple<int,int,int> cubeTuple;
typedef std::map<position,cubeTuple> stackDictionary;

class stack: public objectsSuperClass
{
	public:
		stack(int ri, int rf);
		void addStringVector(stringVector strVec);
		void addStack(position Pos, cubeTuple Cubes);
		void removeStack(position Pos);
		cubeTuple getCubeStack(position Pos);

		stackDictionary stackDict;
};

#endif