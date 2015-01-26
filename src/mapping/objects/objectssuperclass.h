#ifndef OBJECTSSUPERCLASS_H
#define OBJECTSSUPERCLASS_H

#include "../maputils.h"

class objectsSuperClass
{	
	public:
		objectsSuperClass(int res_init, int res_fin);
		virtual void addPosition(struct mapPosition Pos);
		mapPositionVector getPositions();
		virtual void addStringVector(mapStringVector strVec)=0;
		struct mapPosition getMaxPos(mapPositionVector posVec);
		void updateMaxPos();
		mapPositionVector createLine(struct mapPosition Pos1, struct mapPosition Pos2);
		// bool checkIfEnclosed(int x, int y, mapPositionVector posVec, struct mapPosition maxPos);

		int sign(int num);
		int convertInd(int ind);

		int res_init, res_fin;
		mapPosition MAX_POS;
		mapPositionVector posList;
		mapPositionVectorIterator posListIt;
		// virtual datatype function=what
};

#endif // OBJECTSSUPERCLASS_H
