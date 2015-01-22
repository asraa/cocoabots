#ifndef STARTLOCATION_H
#define STARTLOCATION_H

#include "objectssuperclass.h"

class startLocation: public objectsSuperClass
{
	public:
		startLocation(int ri, int rf);
		void addStringVector(stringVector strVec);

};

#endif