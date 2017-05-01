# LUIS

Here are the source codes of LUIS and other related data structures

### Description of the codes:

* main.cpp: The main function
* XXXSketch.h and XXXSketch.cpp: The codes of each kind of sketches
* scheme.cpp and scheme.h: The codes of LUIS<sub>B</sub>, LUIS<sub>G</sub>, LUIS<sub>C</sub>, and Buffalo, where LUIS<sub>G</sub> and LUIS<sub>C</sub> are able to share the same scheme (just by only focusing on the guaranteed correct cases, we can treat LUIS<sub>C</sub> as LUIS<sub>G</sub>).
* BloomFilter.cpp and BloomFilter.h: The codes of Bloom filters used by Buffalo
* common.h: The codes of hash functions and other trivial things