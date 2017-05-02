CC = g++

LUIS : main.o scheme.o BloomFilter.o CMSketch.o CUSketch.o CoveringSketch.o GCRCoveringSketch.o CRCoveringSketch.o common.o
	$(CC) -o LUIS main.o scheme.o BloomFilter.o CMSketch.o CUSketch.o CoveringSketch.o GCRCoveringSketch.o CRCoveringSketch.o common.o

main.o : main.cpp
	$(CC) -c main.cpp

scheme.o : scheme.cpp scheme.h
	$(CC) -c scheme.cpp

BloomFilter.o : BloomFilter.cpp BloomFilter.h
	$(CC) -c BloomFilter.cpp

CMSketch.o : CMSketch.cpp CMSketch.h
	$(CC) -c CMSketch.cpp

CUSketch.o : CUSketch.cpp CUSketch.h
	$(CC) -c CUSketch.cpp

CoveringSketch.o : CoveringSketch.cpp CoveringSketch.h
	$(CC) -c CoveringSketch.cpp

GCRCoveringSketch.o : GCRCoveringSketch.cpp GCRCoveringSketch.h
	$(CC) -c GCRCoveringSketch.cpp

CRCoveringSketch.o : CRCoveringSketch.cpp CRCoveringSketch.h
	$(CC) -c CRCoveringSketch.cpp

common.o : common.cpp common.h
	$(CC) -c common.cpp

clean : 
	rm *.o LUIS