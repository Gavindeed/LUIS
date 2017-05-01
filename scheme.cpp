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

BasicScheme::BasicScheme(int num, int sketchType, int d, int w, int b, uint *encode)
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
		sketchPos = new MemberSketch(d, w+1, b);
		sketchNeg = new MemberSketch(d, w, b);
	}
	codeList = new uint[num];
	for(int i = 0; i < num; i++)
	{
		codeList[i] = encode[i];
	}
}

void BasicScheme::Insert(cuc *str, uint type)
{
	sketchPos->Insert(str, codeList[type]);
	sketchNeg->Insert(str, codeList[numType-1-type]);
}

Bound BasicScheme::Query(cuc *str)
{
	uint resP = sketchPos->Query(str);
	if(resP == 0)
	{
		return Bound(-1, -1);
	}
	uint up = 0;
	int left = 0, right = numType - 1;
	int mid = right / 2;
	while(left < right)
	{
		if(codeList[mid] < resP)
		{
			left = mid + 1;
			mid = (left + right) / 2;
		}
		else
		{
			right = mid;
			mid = (left + right) / 2;
		}
	}
	if(left != right)
	{
		printf("error!\n");
		exit(0);
	}
	if(left + 1 < numType && codeList[left+1] <= resP)
	{
		up = left + 1;
	}
	else if(codeList[left] > resP)
	{
		up = left - 1;
	}
	else
	{
		up = left;
	}
	uint resN = sketchNeg->Query(str);
	if(resN == 0)
	{
		return Bound(-1, -1);
	}
	uint down = numType - 1;
	left = 0, right = numType - 1;
	mid = right / 2;
	while(left < right)
	{
		if(codeList[mid] < resN)
		{
			left = mid + 1;
			mid = (left + right) / 2;
		}
		else
		{
			right = mid;
			mid = (left + right) / 2;
		}
	}
	if(left != right)
	{
		printf("error!\n");
		exit(0);
	}
	if(left + 1 < numType && codeList[left+1] <= resN)
	{
		down = numType - left - 2;
	}
	else if(codeList[left] > resN)
	{
		down = numType - left;
	}
	else
	{
		down = numType - 1 - left;
	}
	if(up < down)
	{
		return Bound(-1, -1);
	}
	return Bound(up, down);
}

OptimizedScheme::OptimizedScheme(int num, int d, int w, int b, int f)
{
	numType = num;
	int ratio = 1; 
	sketchPos = new CoveringSketch(d, w*2/(ratio+1), b, f);
	sketchNeg = new CoveringSketch(d, w*2*ratio/(ratio+1)+1, b, f);
	this->f = f;
	hash = new HashFunction();
}

void OptimizedScheme::Insert(cuc *str, uint type)
{
	bool ok = sketchPos->Insert(str, type);
	sketchNeg->Insert(str, numType-type);
}

int OptimizedScheme::Query(cuc *str)
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
