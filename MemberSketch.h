#ifndef _MEMBERSKETCH_H
#define _MEMBERSKETCH_H

#include "common.h"
#include "sketch.h"

class MemberSketch : public Sketch
{
public:
	MemberSketch(int d, int w, int b);
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
