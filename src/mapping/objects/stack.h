#ifndef STACK_H
#define STACK_H

#include "objectssuperclass.h"

class stack: public objectsSuperClass
{
	public:
		stack(int ri, int rf);
		void addStringVector(mapStringVector strVec);
		void addStack(mapPosition Pos, mapCubeStack cubeStack);
		void removeStack(mapPosition Pos);
		struct mapCubeStack getCubeStack(mapPosition Pos);

		mapStackDictionary stackDict;
};

#endif