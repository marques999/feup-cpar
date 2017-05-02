#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string.h>

#include "papi.h"

int eventMask = PAPI_NULL;

void PAPI_Error(int retval)
{
	printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
	exit(1);
}

int PAPI_Initialize()
{
	int rv = PAPI_library_init(PAPI_VER_CURRENT);

	if (rv != PAPI_VER_CURRENT)
	{
		std::cout << "PAPI library version mismatch!" << std::endl;
	}

	std::cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(rv)
		      << " MINOR: " << PAPI_VERSION_MINOR(rv) << " REVISION: "
		      << PAPI_VERSION_REVISION(rv) << "\n";

	rv = PAPI_create_eventset(&eventMask);

	if (rv != PAPI_OK)
	{
		std::cout << "ERRO: create eventset" << std::endl;
		PAPI_Error(rv);
	}

	rv = PAPI_add_event(eventMask, PAPI_L1_DCM);

	if (rv != PAPI_OK)
	{
		std::cout << "ERRO: PAPI_L1_DCM" << std::endl;
		PAPI_Error(rv);
	}

	rv = PAPI_add_event(eventMask, PAPI_L2_DCM);

	if (rv != PAPI_OK)
	{
		std::cout << "ERRO: PAPI_L2_DCM" << std::endl;
		PAPI_Error(rv);
	}

	rv = PAPI_add_event(eventMask, PAPI_TOT_INS);

	if (rv != PAPI_OK)
	{
		std::cout << "ERRO: PAPI_TOT_INS" << std::endl;
		PAPI_Error(rv);
	}

	return rv;
}

int PAPI_Destroy()
{
	int rv = PAPI_remove_event(eventMask, PAPI_L1_DCM);

	if (rv != PAPI_OK)
	{
		std::cout << "FAIL remove event" << std::endl;
		PAPI_Error(rv);
	}

	rv = PAPI_remove_event(eventMask, PAPI_L2_DCM);

	if (rv != PAPI_OK)
	{
		std::cout << "FAIL remove event" << std::endl;
		PAPI_Error(rv);
	}

	PAPI_destroy_eventset(&eventMask);

	return rv;
}

int PAPI_Reset(long long array[3])
{
	int rv = PAPI_stop(eventMask, array);

	if (rv != PAPI_OK)
	{
		std::cout << "ERRO: Stop PAPI" << std::endl;
		PAPI_Error(rv);
	}

	std::cout << "L1 DCM: " << array[0] << std::endl;
	std::cout << "L2 DCM: " << array[1] << std::endl;
	std::cout << "TOTAL: " << array[2] << std::endl;
	rv = PAPI_reset(eventMask);

	if (rv != PAPI_OK)
	{
		std::cout << "FAIL reset" << std::endl;
		PAPI_Error(rv);
	}

	std::cout << std::endl;

	return rv;
}

int PAPI_Begin()
{
	int rv = PAPI_start(eventMask);

	if (rv != PAPI_OK)
	{
		std::cout << "ERRO: Start PAPI" << std::endl;
		PAPI_Error(rv);
	}

	return rv;
}
