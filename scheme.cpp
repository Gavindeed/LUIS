#include "scheme.h"
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

BuffaloScheme::BuffaloScheme(int num, int w, int k)
{
	numType = num;
	filters = new BloomFilter*[num];
	for(int i = 0; i < num; i++)
	{
		filters[i] = new BloomFilter(w, k);
	}
}

void BuffaloScheme::Insert(cuc *str, uint type)
{
	filters[type]->Insert(str);
}

uint BuffaloScheme::Query(cuc *str)
{
	for(int i = numType-1; i >= 0; i--)
	{
		if(filters[i]->Query(str))
		{
			return i;
		}	
	}
	return -1;
}

BasicScheme::BasicScheme(int num, int sketchType, int d, int w, int b)
{
	numType = num;
	if(sketchType == 0)
	{
		sketchPos = new CMSketch(d, w+1, b);
		sketchNeg = new CMSketch(d, w, b);
	}
	else if(sketchType == 1)
	{
		sketchPos = new CUSketch(d, w+1, b);
		sketchNeg = new CUSketch(d, w, b);
	}
	else
	{
		sketchPos = new CoveringSketch(d, w+1, b);
		sketchNeg = new CoveringSketch(d, w, b);
	}
}

void BasicScheme::Insert(cuc *str, uint type)
{
	sketchPos->Insert(str, type);
	sketchNeg->Insert(str, numType-type);
}

Bound BasicScheme::Query(cuc *str)
{
	uint resP = sketchPos->Query(str);
	if(resP == 0)
	{
		return Bound(-1, -1);
	}
	uint resN = sketchNeg->Query(str);
	if(resN == 0)
	{
		return Bound(-1, -1);
	}
	resN = numType - resN;
	return Bound(resP, resN);
}


GCROptimizedScheme::GCROptimizedScheme(int num, int d, int w, int b)
{
	numType = num;
	sketchPos = new GCRCoveringSketch(d, w, b);
	sketchNeg = new GCRCoveringSketch(d, w+1, b);
	hash = new HashFunction();
}

void GCROptimizedScheme::Insert(cuc *str, uint type)
{
	sketchPos->Insert(str, type);
	sketchNeg->Insert(str, numType-type);
}

int GCROptimizedScheme::Query(cuc *str)
{
	int p = sketchPos->Query(str);
	int n = sketchNeg->Query(str); 
	uint vp = abs(p);
	uint vn = abs(n); 
	vn = numType - vn; 
	if(vp == vn)
	{
		return vp; 
	}
	if(p > 0)
	{
		return vp;
	}
	if(n > 0)
	{
		return vn;
	}
	if(p == 0 || n == 0 || vp < vn)
	{
		return 0;
	}
	
	return -numType;
}

CROptimizedScheme::CROptimizedScheme(int num, int d, int w, int b, int f)
{
	numType = num;
	sketchPos = new CRCoveringSketch(d, w, b, f);
	sketchNeg = new CRCoveringSketch(d, w+1, b, f);
	this->f = f;
	hash = new HashFunction();
}

void CROptimizedScheme::Insert(cuc *str, uint type)
{
	bool ok = sketchPos->Insert(str, type);
	sketchNeg->Insert(str, numType-type);
}

int CROptimizedScheme::Query(cuc *str)
{
	int p = sketchPos->Query(str);
	int n = sketchNeg->Query(str); 
	uint vp = (abs(p) >> f);
	uint vn = (abs(n) >> f); 
	vn = numType - vn; 
	if(vp == vn)
	{
		return vp; 
	}
	if(p > 0)
	{
		return vp;
	}
	if(n > 0)
	{
		return vn;
	}
	if(p == 0 || n == 0 || vp < vn)
	{
		return 0;
	}
	uint fp = (abs(p) & ((1 << f) - 1));
	uint fn = (abs(n) & ((1 << f) - 1));
	uint print = hash->Str2Int(str, 10, strlen((const char*)str)) % (1 << f);
	if(fp == print)
	{
		return -vp;
	}
	else if(fn == print)
	{
		return -vn;
	}
	else
	{
		return -numType;
	} 
}