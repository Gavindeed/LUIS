#ifndef _SCHEME_H
#define _SCHEME_H

#include "BloomFilter.h"
#include "CMSketch.h"
#include "CUSketch.h"
#include "MemberSketch.h"
#include "FingerSketch.h"
#include "CoveringSketch.h"
#include "HashTable.h"

class BuffaloScheme
{
public:
	BuffaloScheme(int num, int w, int k);
	void Insert(cuc *str, uint type);
	uint Query(cuc *str);
	uint GetMemory()
	{
		uint ma = 0;
		for(int i = 0; i < numType; i++)
		{
			ma += filters[i]->GetMemory();
		}
		return ma;
	}
	
private:
	int numType;
	BloomFilter **filters;
};

struct Bound
{
	uint up;
	uint down;
	Bound(uint u, uint d)
	{
		up = u;
		down = d;
	}	
};

class BasicScheme
{
public:
	BasicScheme(int num, int sketchType, int d, int w, int b, uint *encode);
	void Insert(cuc *str, uint type);
	Bound Query(cuc *str);
	uint GetMemory()
	{
		return sketchPos->GetMemory() + sketchNeg->GetMemory();
	}
	
private:
	int numType;
	Sketch *sketchPos, *sketchNeg;
	uint *codeList;
};

class OptimizedScheme
{
public:
	OptimizedScheme(int num, int d, int w, int b, int f);
	void Insert(cuc *str, uint type);
	int Query(cuc *str);
	void Insert2(cuc *str, uint type);
	
	uint GetMemory()
	{
		return sketchPos->GetMemory() + sketchNeg->GetMemory();
	}
	
	
private:
	int FindMS(uint number)
	{
		for(int i = 31; i >= 0; i--)
		{
			if(((number >> i) & 1) == 1)
			{
				return i;
			}
		}
		return -1;
	}
	int numType;
	CoveringSketch *sketchPos, *sketchNeg; 
	int f;
	HashFunction *hash;
};

#endif
