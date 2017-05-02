#ifndef _SKETCH_H
#define _SKETCH_H

class Sketch
{
public:
	virtual void Insert(cuc *str, uint num) = 0;
	virtual uint Query(cuc *str) = 0;
	Sketch() {memoryAccess = 0;}
	uint GetMemory() {return memoryAccess;}
	
protected:
	uint memoryAccess;	
};

#endif
