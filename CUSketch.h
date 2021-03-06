/*
* This file contains the codes of the CU sketch
*/

#ifndef _CUSKETCH_H
#define _CUSKETCH_H

#include "common.h"
#include "sketch.h"

class CUSketch : public Sketch
{
public:
	CUSketch(int d, int w, int b);
	void Insert(cuc *str, uint num);
	uint Query(cuc *str);
	
private:
	int depth;
	int width;
	int bits;
	HashFunction *hash;
	uint **sketch;
	uint MAXV;
};

#endif
