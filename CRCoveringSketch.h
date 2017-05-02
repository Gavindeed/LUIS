#ifndef _CRCOVERINGSKETCH_H
#define _CRCOVERINGSKETCH_H

#include "common.h"
#include "sketch.h"

class CRCoveringSketch
{
public:
	CRCoveringSketch(int d, int w, int b, int f);
	bool Insert(cuc *str, uint num);
	uint Query(cuc *str);
	uint GetMemory() {return memoryAccess;}
	
private:
	int depth;
	int width;
	int bits;
	int finger;
	HashFunction *hash;
	int **sketch;
	uint MAXV;	
	uint memoryAccess;
};

#endif
