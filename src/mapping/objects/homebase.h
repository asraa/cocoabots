#ifndef HOMEBASE_H
#define HOMEBASE_H

#include "objectssuperclass.h"

class homeBase: public objectsSuperClass
{
	public:
		homeBase(int ri, int rf);
		void addStringVector(stringVector strVec);
		void fillInPolygon(positionVector posVec);

};

#endif