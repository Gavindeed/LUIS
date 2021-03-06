#include "CoveringSketch.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

CoveringSketch::CoveringSketch(int d, int w, int b)
{
	depth = d;
	width = w;
	bits = b;
	hash = new HashFunction();
	sketch = new uint*[d];
	for(int i = 0; i < d; i++)
	{
		sketch[i] = new uint[w];
		memset(sketch[i], 0, sizeof(uint)*w);
	}
	MAXV = (1 << b) - 1;
}

void CoveringSketch::Insert(cuc *str, uint num)
{
	for(int i = 0; i < depth; i++)
	{
		int index = hash->Str2Int(str, i, strlen((const char*)str)) % width;
		memoryAccess ++;
		if(sketch[i][index] < num)
		{
			sketch[i][index] = num;
		}
	}
}

uint CoveringSketch::Query(cuc *str)
{
	uint MIN = MAXV;
	for(int i = 0; i < depth; i++)
	{
		int index = hash->Str2Int(str, i, strlen((const char*)str)) % width;
		memoryAccess ++;
		if(sketch[i][index] < MIN)
		{
			MIN = sketch[i][index];
		}
	}
	return MIN;
}
