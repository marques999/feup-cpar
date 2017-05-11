/*
 * common.h
 *
 *  Created on: 16/03/2017
 *      Author: marques999
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <math.h>
#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <string.h>

#define NUMBER_ALGORITHMS		4
#define NUMBER_ALGORITHMS_OMP	2
#define MAXIMUM_PRIMES			16

#define MIN(a,b) 				(((a)<(b))?(a):(b))
#define IS_PRIME(i) 			v[i >> 5] & (1 << (i & 31))

#define BLOCK_LOW(id, p, n) ((id)*(n)/(p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id, p, n) (BLOCK_HIGH(id,p,n)-BLOCK_LOW(id,p,n)+1)

typedef struct timespec nanoTime;
typedef double(*SequentialFunction)(const uint64_t maximumValue);
typedef double(*ParallelFunction)(const uint64_t maximumValue, int maximumThreads);

void printEven(const bool* v, const uint64_t sz);
void printPrimes(const bool* v, const uint64_t sz);
void printBitwise(const uint32_t* v, const uint64_t sz);

double getElapsed(nanoTime start, nanoTime finish);

#endif /* COMMON_H_ */
