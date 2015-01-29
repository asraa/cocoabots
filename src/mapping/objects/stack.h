#ifndef STACK_H
#define STACK_H

#include "objectssuperclass.h"

class stack: public objectsSuperClass
{
	public:
		stack(int ri, int rf);
		void addStringVector(mapStringVector strVec);
		void addStack(struct mapPosition Pos, mapCubeStack cubeStack);
		void removeStack(struct mapPosition Pos);
		struct mapCubeStack getCubeStack(struct mapPosition Pos);

		mapStackDictionary stackDict;
};

#endif