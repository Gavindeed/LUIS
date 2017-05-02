#ifndef _GCRCOVERINGSKETCH_H
#define _GCRCOVERINGSKETCH_H

#include "common.h"
#include "sketch.h"

class GCRCoveringSketch
{
public:
	GCRCoveringSketch(int d, int w, int b);
	bool Insert(cuc *str, uint num);
	uint Query(cuc *str);
	uint GetMemory() {return memoryAccess;}
	
private:
	int depth;
	int width;
	int bits;
	HashFunction *hash;
	int **sketch;
	uint MAXV;	
	uint memoryAccess;
};

#endif
