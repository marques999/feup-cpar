#ifndef __PAPI_H_
#define __PAPI_H_

int PAPI_Begin();
int PAPI_Destroy();
int PAPI_Initialize();
int PAPI_Reset(long long rv[2]);

#endif /* __PAPI_H_ */