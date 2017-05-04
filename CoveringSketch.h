/*
* This file contains the codes of the Covering sketch
*/

#ifndef _COVERINGSKETCH_H
#define _COVERINGSKETCH_H

#include "common.h"
#include "sketch.h"

class CoveringSketch : public Sketch
{
public:
	CoveringSketch(int d, int w, int b);
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
