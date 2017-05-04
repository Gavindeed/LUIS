# LUIS

## Introduction
Key-value (KV) store plays an important role in high-performance data-intensive applications, including high-performance databases, social networks, e-commerce platforms, *etc*. Many enterprises develop KV stores by using hash tables in main memory or flash memory, which achieves 100% correct rate within an average time complexity of O(1). In many applications, small and controllable error is allowed, while the optimization of memory usage, query and update speeds are highly desired. The ideal goal includes two parts: 1) the memory usage is small enough to fit into fast memory (SRAM); 2) the query and update speeds are always constant yet fast. Towards this goal, an effective solution is to introduce *approximate query* to KV stores, which we call *approximate KV stores* for convenience. In this paper, we propose a novel framework, namely **LUIS** (Lower-Upper Interval Squeeze), which is able to fit into the fast memory like SRAM, and achieves constant yet fast speed and high accuracy for approximate KV stores. *The key idea of LUIS is using two sketches to report a lower bound and an upper bound for the value. Using some optimization methods, the two bounds are equal with a high probability, and in this case, the reported value is guaranteed to be correct.* Our experimental results show that compared to Buffalo, our LUIS achieves up to 7391 times faster query speed and up to 4824 times higher correct rate.

## Building

	$ make

## Description of the codes:

* main.cpp: This file contains the main function.
	* It does the following works:
	* 1) read the parameters from the console
	* 2) select the data source based on the parameter
	* 3) select the schema based on the parameters
	* 4) select the metric to be measured
	* 5) read the data, let the schema work on it, record the real value for each key, and measure the metric
* XXXSketch.h and XXXSketch.cpp: The codes of each kind of sketches.
* scheme.h and scheme.cpp: The codes of LUIS<sub>B</sub>, LUIS<sub>G</sub>, LUIS<sub>C</sub>, and Buffalo, where LUIS<sub>G</sub> and LUIS<sub>C</sub> are able to share the same scheme (just by only focusing on the guaranteed correct cases, we can treat LUIS<sub>C</sub> as LUIS<sub>G</sub>).
* BloomFilter.h and BloomFilter.cpp: The codes of Bloom filters used by Buffalo.
* common.h and common.cpp: The codes of hash functions and other trivial things.