#ifndef STACK_H
#define STACK_H

#include <map>
#include "objectssuperclass.h"

class stack: public objectsSuperClass
{
	typedef std::tuple<int,int,int> cubeTuple;
	typedef std::map<position,cubeTuple> stackDictionary;
	
	public:
		stack(int ri, int rf);
		void addStringVector(stringVector strVec);
		void addStack(position Pos, cubeTuple Cubes);
		void removeStack(position Pos);
		stack::cubeTuple getCubeStack(position Pos);

		stack::stackDictionary stackDict;
};

#endif