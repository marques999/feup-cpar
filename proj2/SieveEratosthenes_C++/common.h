/*
 * common.h
 *
 *  Created on: 16/03/2017
 *      Author: marques999
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <iostream>
#include <vector>

#define NUMBER_ALGORITHMS	4
#define MAXIMUM_PRIMES		16
#define MIN(a,b) (((a)<(b))?(a):(b))

typedef struct timespec nanoTime;
typedef double(*SequentialFunction)(const uint64_t maximumValue);
typedef double(*ParallelFunction)(const uint64_t maximumValue, int maximumThreads);

void printPrimes(const std::vector<bool>& v);
void printPrimes(const bool* v, const uint64_t sz);
void printPrimes(const uint8_t* v, const uint64_t sz);

double getElapsed(nanoTime start, nanoTime finish);

#endif /* COMMON_H_ */