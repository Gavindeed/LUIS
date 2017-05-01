#ifndef _CoveringSketch_H
#define _CoveringSketch_H

#include "common.h"
#include "sketch.h"

class CoveringSketch
{
public:
	CoveringSketch(int d, int w, int b, int f);
	bool Insert(cuc *str, uint num);
	void Remove(cuc *str, uint num);
	uint Query(cuc *str);
	uint QueryO(cuc *str);
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
