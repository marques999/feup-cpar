#ifndef __PAPI_H_
#define __PAPI_H_

#include <papi.h>

int PAPI_Begin();
int PAPI_Destroy();
int PAPI_Initialize();
int PAPI_Reset(long long array[3]);

#endif /* __PAPI_H_ */
