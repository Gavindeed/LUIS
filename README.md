# LUIS

## Introduction
Key-value (KV) store plays an important role in high-performance data-intensive applications, including high-performance databases, social networks, e-commerce platforms, *etc*. Many enterprises develop KV stores by using hash tables in main memory or flash memory, which achieves 100% correct rate within an average time complexity of O(1). In many applications, small and controllable error is allowed, while the optimization of memory usage, query and update speeds are highly desired. The ideal goal includes two parts: 1) the memory usage is small enough to fit into fast memory (SRAM); 2) the query and update speeds are always constant yet fast. Towards this goal, an effective solution is to introduce *approximate query* to KV stores, which we call *approximate KV stores* for convenience. In this paper, we propose a novel framework, namely **LUIS** (Lower-Upper Interval Squeeze), which is able to fit into the fast memory like SRAM, and achieves constant yet fast speed and high accuracy for approximate KV stores. *The key idea of LUIS is using two sketches to report a lower bound and an upper bound for the value. Using some optimization methods, the two bounds are equal with a high probability, and in this case, the reported value is guaranteed to be correct.* Our experimental results show that compared to Buffalo, our LUIS achieves up to 7391 times faster query speed and up to 4824 times higher correct rate.

## Building

	$ make

## Description of the codes:

* main.cpp: This file contains the main function. It does the following works:
	* read the parameters from the console
	* select the data source based on the parameter
	* select the schema based on the parameters
	* select the metric to be measured
	* read the data, let the schema work on it, record the real value for each key, and measure the metric
* XXXSketch.h and XXXSketch.cpp: The codes of each kind of sketches.
* schema.h and schema.cpp: The codes of LUIS<sub>B</sub>, LUIS<sub>G</sub>, LUIS<sub>C</sub>, and Buffalo, where LUIS<sub>G</sub> and LUIS<sub>C</sub> are able to share the same scheme (just by only focusing on the guaranteed correct cases, we can treat LUIS<sub>C</sub> as LUIS<sub>G</sub>).
* BloomFilter.h and BloomFilter.cpp: The codes of Bloom filters used by Buffalo.
* common.h and common.cpp: The codes of hash functions and other trivial things.

## Usage

	$ ./LUIS [numType] [BPI] [fingerlength] [sketchType] [mode] [depth] [metric] [dataId]

There are 8 parameters needed.
* numType: the number of possible values, which means *M* in the paper
* BPI: the number of bits used per KV pair on average
* fingerlength: the length of the fingerprint
* sketchType: choose the type of sketches, 0 for CM sketches, 1 for CU sketches, 2 for Covering sketches
* mode: choose the schema to be tested
	* mode 0: test LUIS_B with BPI, numType, and sketchType, and depth is fixed at 2
	* mode 1: test LUIS_C with BPI, numType, and fingerlength, and depth is fixed at 2
	* mode 2: test LUIS_G with BPI and numType, and depth is fixed at 2
	* mode 3: test Buffalo with BPI and numType, and the number of hash functions is fixed at 4
	* mode 4: test LUIS_B with different depth
	* mode 5: test LUIS_C with different depth
	* mode 6: test LUIS_G with different depth
	* mode 7: test LUIS_B with different **RM**
	* mode 8: test LUIS_C with different **RM**
	* mode 9: test LUIS_G with different **RM**
	* mode 10: test LUIS_B with different numType, while BPI increases linearly as numType increases
	* mode 11: test LUIS_C with different numType, while BPI increases linearly as numType increases
	* mode 12: test LUIS_G with different numType, while BPI increases linearly as numType increases
* depth: choose the depth for sketches
* metric: choose the metric to be measured
	* metric 0: guaranteed correct rate
	* metric 1: half correct rate
	* metric 2: correct rate
	* metric 4: memory accesses per insertion
	* metric 5: queries per second
	* metric 6: insertions per second
	* metric 7: memory accesses per query
* dataId: choose the data source
	* dataId 0: Dataset-I, real medical devices
	* dataId 1: Dataset-II, real transaction records
	* dataId 2: Dataset-III, real images
	* dataId 3: Dataset-IV, synthetic dataset

The output will be a comma "," combined with the result of the metric.

## Data Sources

* Dataset-I: real medical devices. This dataset contains information of medical devices from the U.S. National Library of Medicine. The key is the device ID, and the value is its company.
* Dataset-II: real transaction records. This dataset contains the transaction records from a bank, and it is provided by PKDD'99 Discovery Challenge. We use the transaction ID as the key and use the account ID as the value.
* Dataset-III: real images. This dataset is from NUS-WIDE containing a set of images and their tags which are used for machine learning. We use these small images as the keys and use their tags as the values.
* Dataset-IV: synthetic dataset. This dataset is generated by our C++ program and it satisfies the uniform distribution. Each key is 13 bytes long and its value is generated in the function **GetItem3** in main.cpp.