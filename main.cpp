/*
* This file contains the main function.
* It does the following works:
* 1) read the parameters from the console
* 2) select the data source based on the parameter
* 3) select the schema based on the parameter
* 4) select the metric to be measured
* 5) read the data, let the schema work on it, record the real value for each key, and measure the metric
*/

#include "schema.h"
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <cmath>
#include <iostream>
#include <time.h>

using namespace std;

map<string, uint> dict;

long long sumBits;
int flows; 
int numType;
int output;
int dataId;
double keyLength;
FILE *file;

/* There are 4 data sources, and each data source has different number of KV pairs */
const char *dataname[4] = {"./data/gudid.dat", "./data/trans.dat", "./data/images.dat", "./data/d1"};
int items[4] = {1349853, 1056320, 269613, 1078658};

/* Select the data source */
void OpenData(const char *filename)
{
	if(dataId == 0)
	{
		file = fopen(filename, "r");
	}
	else if(dataId == 1)
	{
		file = fopen(filename, "r");
	}
	else if(dataId == 2)
	{
		file = fopen(filename, "r");
	}
	else if(dataId == 3)
	{
		file = fopen(filename, "rb");
	}
}

/* Functions GetItem0 ~ GetItem3 are used to read data, each call of these functions can read one single KV pair */
bool GetItem0(char *buf, int *type)
{
	if(fscanf(file, "%s", buf) > 0)
	{
		fscanf(file, "%d", type);
		return true;
	}
	else
	{
		return false;
	}	
} 

bool GetItem1(char *buf, int *type)
{
	if(fscanf(file, "%s", buf) > 0)
	{
		fscanf(file, "%d", type);
		return true;
	}
	else
	{
		return false;
	}	
}

bool GetItem2(char *buf, int *type)
{
	if(fscanf(file, "%s", buf) > 0)
	{
		fscanf(file, "%d", type);
		return true;
	}
	else
	{
		return false;
	}
} 

/* Note: this function also serves as generating the uniform-distributed dataset */
bool GetItem3(char *buf, int *type)
{
	static HashFunction *hash = new HashFunction();
	if(fread(buf, 1, 13, file) > 0)
	{
		buf[13] = '\0';
		*type = hash->Str2Int((cuc*)buf, 10, 13) % numType;
		return true;
	}
	else
	{
		return false;
	}
}

bool GetItem(char *buf, int *type)
{
	if(dataId == 0)
	{
		return GetItem0(buf, type);
	}
	else if(dataId == 1)
	{
		return GetItem1(buf, type);
	}
	else if(dataId == 2)
	{
		return GetItem2(buf, type);
	}
	else if(dataId == 3)
	{
		return GetItem3(buf, type);
	}
	else
	{
		return false;
	}
}

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

/* Test the Buffalo */
void BuffaloTest(int numType)
{
	int k = sumBits / (double)flows * log(2) + 0.5;
	BuffaloSchema *Schema = new BuffaloSchema(numType, sumBits/numType, 4);
	char buf[1024];
	dict.clear();
	uint sum = 0;
	int type;
	time_t start1 = clock(); 
	while(GetItem(buf, &type))
	{
		string t = buf;
		dict.insert(make_pair(t, type));
		sum ++;
		Schema->Insert((cuc*)buf, type);
	}
	time_t end1 = clock();
	uint mem1 = Schema->GetMemory();
	map<string, uint>::iterator it;
	uint error = 0;
	time_t start2 = clock();
	for(it = dict.begin(); it != dict.end(); it ++)
	{
		uint esti = Schema->Query((cuc*)(it->first).c_str());
		uint real = it->second;
		if(real != esti) 
		{
			error ++;
		}
	}
	time_t end2 = clock();
	uint mem2 = Schema->GetMemory();
	if(output == 2)
		printf(",%lf", 1 - error / (double)sum);
	else if(output == 4)
		printf(",%lf", log(mem1/(double)items[dataId])/log(2));
	else if(output == 5)
		printf(",%lf", items[dataId] * (double)CLOCKS_PER_SEC / (end2 - start2));
	else if(output == 6)
		printf(",%lf", items[dataId] * (double)CLOCKS_PER_SEC / (end1 - start1));
	else if(output == 7)
		printf(",%lf", log((mem2-mem1)/(double)items[dataId])/log(2));
}

/* Test LUIS_B */
void BasicTest(int numType, int sketchType, int d)
{
	int bits = 0;
	bits = FindMS(numType) + 1;
	BasicSchema *Schema = new BasicSchema(numType, sketchType, d, sumBits/2/d/bits+3, bits);
	char buf[1024];
	dict.clear();
	uint sum = 0;
	int type;
	time_t start1 = clock();
	while(GetItem(buf, &type))
	{
		string t = buf;
		dict.insert(make_pair(t, type+1));
		sum ++;
		Schema->Insert((cuc*)buf, type+1);
	}
	time_t end1 = clock();
	uint mem1 = Schema->GetMemory();
	map<string, uint>::iterator it;
	uint error = 0;
	uint full = 0;
	uint half = 0;
	time_t start2 = clock();
	for(it = dict.begin(); it != dict.end(); it ++)
	{
		Bound esti = Schema->Query((cuc*)(it->first).c_str());
		uint real = it->second;
		if(real > esti.up || real < esti.down) 
		{
			error ++;
		}
		if(real == esti.up && real == esti.down)
		{
			full ++;
		}
		else if(real == esti.up || real == esti.down)
		{
			half ++;
		}
	}
	time_t end2 = clock();
	uint mem2 = Schema->GetMemory();
	if(output == 0)
		printf(",%lf", full / (double)sum);
	else if(output == 1)
		printf(",%lf", half / (double)sum);
	else if(output == 2)
		printf(",%lf", (full + half / 2) / (double)sum);
	else if(output == 4)
		printf(",%lf", log(mem1/(double)items[dataId])/log(2));
	else if(output == 5)
		printf(",%lf", items[dataId] * (double)CLOCKS_PER_SEC / (end2 - start2));
	else if(output == 6)
		printf(",%lf", items[dataId] * (double)CLOCKS_PER_SEC / (end1 - start1));
	else if(output == 7)
		printf(",%lf", log((mem2-mem1)/(double)items[dataId])/log(2));
}

/* Test LUIS_G */
void GCRTest(int numType, int d)
{
	int bits = FindMS(numType) + 1;
	GCROptimizedSchema *Schema = new GCROptimizedSchema(numType, d, sumBits/d/2/(bits+1)+1, bits);
	char buf[1024];
	dict.clear();
	uint sum = 0;
	int type;
	time_t start1 = clock();
	while(GetItem(buf, &type))
	{
		string t = buf;
		dict.insert(make_pair(t, type+1));
		sum ++;
		Schema->Insert((cuc*)buf, type+1);
	}
	time_t end1 = clock();
	uint mem1 = Schema->GetMemory();
	map<string, uint>::iterator it;
	uint error = 0;
	uint nonsense = 0;
	uint full = 0;
	uint half = 0;
	time_t start2 = clock();
	for(it = dict.begin(); it != dict.end(); it ++)
	{
		int estiV = Schema->Query((cuc*)(it->first).c_str());
		uint real = it->second; 
		uint esti = abs(estiV);
		if(estiV > 0)
		{
			full ++;
		}
		else if(esti != numType)
		{
			half ++;
		}
		if(esti != real)
		{
			error ++;
		}
		if(esti == numType)
		{
			nonsense ++;
		}
	}
	time_t end2 = clock();
	uint mem2 = Schema->GetMemory();
	if(output == 0)
		printf(",%lf", full / (double)sum);
	else if(output == 1)
		printf(",%lf", half / (double)sum);
	else if(output == 2)
		printf(",%lf", (full + half / 2) / (double)sum);
	else if(output == 4)
		printf(",%lf", log(mem1/(double)items[dataId])/log(2));
	else if(output == 5)
		printf(",%lf", items[dataId] * (double)CLOCKS_PER_SEC / (end2 - start2));
	else if(output == 6)
		printf(",%lf", items[dataId] * (double)CLOCKS_PER_SEC / (end1 - start1));
	else if(output == 7)
		printf(",%lf", log((mem2-mem1)/(double)items[dataId])/log(2));
}

/* Test LUIS_C */
void CRTest(int numType, int f, int d)
{
	int bits = FindMS(numType) + 1;
	CROptimizedSchema *Schema = new CROptimizedSchema(numType, d, sumBits/d/2/(bits+f+1)+1, bits, f);
	char buf[1024];
	dict.clear();
	uint sum = 0;
	int type;
	time_t start1 = clock();
	while(GetItem(buf, &type))
	{
		string t = buf;
		dict.insert(make_pair(t, type+1));
		sum ++;
		Schema->Insert((cuc*)buf, type+1);
	}
	time_t end1 = clock();
	uint mem1 = Schema->GetMemory();
	map<string, uint>::iterator it;
	uint error = 0;
	uint nonsense = 0;
	uint full = 0;
	uint half = 0;
	time_t start2 = clock();
	for(it = dict.begin(); it != dict.end(); it ++)
	{
		int estiV = Schema->Query((cuc*)(it->first).c_str());
		uint real = it->second; 
		uint esti = abs(estiV);
		if(estiV > 0)
		{
			full ++;
		}
		else if(esti != numType)
		{
			half ++;
		}
		if(esti != real)
		{
			error ++;
		}
		if(esti == numType)
		{
			nonsense ++;
		}
	}
	time_t end2 = clock();
	uint mem2 = Schema->GetMemory();
	if(output == 0)
		printf(",%lf", full / (double)sum);
	else if(output == 1)
		printf(",%lf", half / (double)sum);
	else if(output == 2)
		printf(",%lf", 1 - error / (double)sum);
	else if(output == 4)
		printf(",%lf", log(mem1/(double)items[dataId])/log(2));
	else if(output == 5)
		printf(",%lf", items[dataId] * (double)CLOCKS_PER_SEC / (end2 - start2));
	else if(output == 6)
		printf(",%lf", items[dataId] * (double)CLOCKS_PER_SEC / (end1 - start1));
	else if(output == 7)
		printf(",%lf", log((mem2-mem1)/(double)items[dataId])/log(2));
}

int main(int argc, char **argv)
{
	numType = atoi(argv[1]);
	numType = (1 << numType) - 1;
	int basic = atoi(argv[2]);
	int fingerLength = atoi(argv[3]);
	int sketchType = atoi(argv[4]);
	int mode = atoi(argv[5]);
	output = atoi(argv[7]); 
	dataId = atoi(argv[8]);

	OpenData(dataname[dataId]);
	flows = items[dataId];
	keyLength = 32 * 8;	
	sumBits = flows * basic * 8;
	int d = 2;
	switch(mode)
	{
	case 0:
		BasicTest(numType, sketchType, d);
		break;
	case 1:
		CRTest(numType, fingerLength, d);
		break;
	case 2:
		GCRTest(numType, d);
		break;
	case 3:
		BuffaloTest(numType);
		break;
	case 4:
		d = atoi(argv[6]);
		BasicTest(numType, sketchType, d);
		break;
	case 5:
		d = atoi(argv[6]);
		CRTest(numType, fingerLength, d);
		break;
	case 6:
		d = atoi(argv[6]);
		GCRTest(numType, d);
		break;
	case 7:
		sumBits = flows / 100.0 * (keyLength + FindMS(numType) + 1) * basic;
		BasicTest(numType, sketchType, d);
		break;
	case 8:
		sumBits = flows / 100.0 * (keyLength + FindMS(numType) + 1) * basic;
		d = atoi(argv[6]);
		CRTest(numType, fingerLength, d);
		break;
	case 9:
		sumBits = flows / 100.0 * (keyLength + FindMS(numType) + 1) * basic;
		GCRTest(numType, d);
		break;
	case 10:
		sumBits = flows * atoi(argv[1]) * 4;
		BasicTest(numType, sketchType, d);
		break;
	case 11:
		sumBits = flows * atoi(argv[1]) * 4;
		CRTest(numType, fingerLength, d);
		break;
	case 12:
		sumBits = flows * atoi(argv[1]) * 4;
		GCRTest(numType, d);
		break;
	default:
		printf("mode error!\n");
	}
	return 0;
}
