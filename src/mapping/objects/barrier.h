#ifndef BARRIER_H
#define BARRIER_H

#include "objectssuperclass.h"

class barrier: public objectsSuperClass
{
	public:
		barrier(int ri, int rf);
		void addStringVector(mapStringVector strVec);
};

#endif