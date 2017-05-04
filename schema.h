/*
* This file contains the codes of all schemas
* Each schema has several functions: Insert, Query, and GetMemory
*/

#ifndef _SCHEMA_H
#define _SCHEMA_H

#include "BloomFilter.h"
#include "CMSketch.h"
#include "CUSketch.h"
#include "CoveringSketch.h"
#include "GCRCoveringSketch.h"
#include "CRCoveringSketch.h"

class BuffaloSchema
{
public:
	BuffaloSchema(int num, int w, int k);
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

class BasicSchema
{
public:
	BasicSchema(int num, int sketchType, int d, int w, int b);
	void Insert(cuc *str, uint type);
	Bound Query(cuc *str);
	uint GetMemory()
	{
		return sketchPos->GetMemory() + sketchNeg->GetMemory();
	}
	
private:
	int numType;
	Sketch *sketchPos, *sketchNeg;
};

class GCROptimizedSchema
{
public:
	GCROptimizedSchema(int num, int d, int w, int b);
	void Insert(cuc *str, uint type);
	int Query(cuc *str);
	
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
	GCRCoveringSketch *sketchPos, *sketchNeg; 
	HashFunction *hash;
};

class CROptimizedSchema
{
public:
	CROptimizedSchema(int num, int d, int w, int b, int f);
	void Insert(cuc *str, uint type);
	int Query(cuc *str);
	
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
	CRCoveringSketch *sketchPos, *sketchNeg; 
	int f;
	HashFunction *hash;
};

#endif
