/*
 * common.h
 *
 *  Created on: 16/03/2017
 *      Author: marques999
 */

#ifndef COMMON_H_
#define COMMON_H_

void printPrimes(const std::vector<bool> &v, const uint64_t offset);
void printPrimes(const bool* v, const uint64_t offset, const uint64_t sz);
void printPrimes(const uint8_t* v, const uint64_t offset, const uint64_t sz);

#endif /* COMMON_H_ */