/*
* This file contains the codes of the CM sketch
*/

#ifndef _CMSKETCH_H
#define _CMSKETCH_H

#include "common.h"
#include "sketch.h"

class CMSketch : public Sketch
{
public:
	CMSketch(int d, int w, int b);
	void Insert(cuc *str, uint num);
	void Remove(cuc *str, uint num);
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
