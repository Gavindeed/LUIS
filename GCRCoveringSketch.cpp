#include "GCRCoveringSketch.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

GCRCoveringSketch::GCRCoveringSketch(int d, int w, int b)
{
	depth = d;
	width = w;
	bits = b;
	hash = new HashFunction();
	sketch = new int*[d];
	for(int i = 0; i < d; i++)
	{
		sketch[i] = new int[w];
		memset(sketch[i], 0, sizeof(uint)*w);
	}
	MAXV = (1 << b) - 1;
	memoryAccess = 0;
}

bool GCRCoveringSketch::Insert(cuc *str, uint num)
{
	bool ok = false;
	for(int i = 0; i < depth; i++)
	{
		int index = hash->Str2Int(str, i, strlen((const char*)str)) % width;
		uint value = abs(sketch[i][index]);
		memoryAccess ++;
		if(value == 0)
		{
			sketch[i][index] = num;
			ok = true;
		}
		else if(value < num)
		{
			sketch[i][index] = num;
			sketch[i][index] = -sketch[i][index];
		}
		else
		{
			if(sketch[i][index] > 0)
			{
				sketch[i][index] = -sketch[i][index];
			}
		}
	}
	return ok;
}

/*
* Note: we use the sign bit of signed integer to serve as the flag bit
* Therefore, if the return value is larger than 0, it can be guaranteed to be correct
* Otherwise, it cannot be guaranteed
*/
uint GCRCoveringSketch::Query(cuc *str)
{
	uint MIN = MAXV+1;
	int res = 0;
	for(int i = 0; i < depth; i++)
	{
		int index = hash->Str2Int(str, i, strlen((const char*)str)) % width;
		uint value = abs(sketch[i][index]);
		memoryAccess ++;
		if(value < MIN)
		{
			MIN = value;
			res = sketch[i][index];
		}
		else if(sketch[i][index] > 0)
		{
			res = sketch[i][index];
			break;
		}
	}
	return res;
}
