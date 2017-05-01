#include "CoveringSketch.h"

#include "CUSketch.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

CoveringSketch::CoveringSketch(int d, int w, int b, int f)
{
	depth = d;
	width = w;
	bits = b;
	finger = f;
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

bool CoveringSketch::Insert(cuc *str, uint num)
{
	bool ok = false;
	for(int i = 0; i < depth; i++)
	{
		int index = hash->Str2Int(str, i, strlen((const char*)str)) % width;
		uint value = (abs(sketch[i][index]) >> finger);
		memoryAccess ++;
		if(value == 0)
		{
			//memoryAccess ++;
			sketch[i][index] = (num << finger);
			uint print = hash->Str2Int(str, 10, strlen((const char*)str)) % (1 << finger);
			sketch[i][index] |= print;
			ok = true;
		}
		else if(value < num)
		{
			//memoryAccess ++;
			sketch[i][index] = (num << finger);
			uint print = hash->Str2Int(str, 10, strlen((const char*)str)) % (1 << finger);
			sketch[i][index] |= print;
			sketch[i][index] = -sketch[i][index];
			//ok = true;
		}
		else
		{
			if(sketch[i][index] > 0)
			{
				sketch[i][index] = -sketch[i][index];
				//memoryAccess ++;
			}
			
		}
	}
	return ok;
}

uint CoveringSketch::Query(cuc *str)
{
	uint MIN = MAXV+1;
	int res = 0;
	for(int i = 0; i < depth; i++)
	{
		int index = hash->Str2Int(str, i, strlen((const char*)str)) % width;
		uint value = (abs(sketch[i][index]) >> finger);
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

uint CoveringSketch::QueryO(cuc *str)
{
	uint MIN = MAXV+1;
	int res = 0;
	bool sure = false;
	for(int i = 0; i < depth; i++)
	{
		int index = hash->Str2Int(str, i, strlen((const char*)str)) % width;
		uint value = (abs(sketch[i][index]) >> finger);
		memoryAccess ++;
		if(value < MIN)
		{
			MIN = value;
			if(sure)
			{
				return 0;
			}
			res = sketch[i][index];
		}
		else if(sketch[i][index] > 0)
		{
			res = sketch[i][index];
			if(value > MIN || !sure)
			{
				return 0;
			}
			sure = true;
		}
	}
	return res;
}

void CoveringSketch::Remove(cuc *str, uint num)
{
	
}
