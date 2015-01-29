#ifndef BARRIER_H
#define BARRIER_H

#include "objectssuperclass.h"

class barrier: public objectsSuperClass
{
	public:
		barrier(int ri, int rf);
		void addStringVector(mapStringVector strVec);
		int maxBarrierX = 0;
		int maxBarrierY = 0; 
		int minBarrierX = 222222;
		int minBarrierY = 222222;
};

#endif