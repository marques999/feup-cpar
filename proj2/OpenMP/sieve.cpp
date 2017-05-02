#include <iomanip>
#include <iostream>
#include <omp.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "common.h"
#include "papi.h"
#include "parallel.h"
#include "sequential.h"

const char* modeLabels[] =
{
	"Sequential", "OpenMP"
};

const char* algorithmSequential[] =
{
	"Bitwise", "BoolArray", "ExcludeOdd", "FastMarking"
};

const char* algorithmOMP[] =
{
	"Static", "Dynamic", "FastMarking"
};

const char** algorithmLabels[] =
{
	algorithmSequential, algorithmOMP
};

const int algorithmLabelsLength[] =
{
	NUMBER_ALGORITHMS, NUMBER_ALGORITHMS_OMP
};

int main(int argc, const char* argv[])
{
	uint32_t modeIndex = 0;
	uint64_t maximumValue = 0;
	uint32_t algorithmIndex = 0;
	uint32_t maximumThreads = omp_get_max_threads();
	uint32_t numberThreads = maximumThreads;

	static struct termios oldt;
	static struct termios newt;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~ICANON;
	newt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	while (true)
	{
		printf("\033c");
		printf("+-----------------+----------------+\n");
		std::cout << "| [1] Mode        | " << std::left << std::setw(14) << modeLabels[modeIndex] << " |" << std::endl;
		std::cout << "| [2] Improvement | " << std::setw(14) << algorithmLabels[modeIndex][algorithmIndex] << " |" << std::endl;
		std::cout << "| [3] Limit       | " << std::setw(14) << maximumValue << " |" << std::endl;

		if (modeIndex == 1)
		{
			std::cout << "| [4] Threads     | " << std::setw(14) << numberThreads << " |" << std::endl;
		}

		printf("+-----------------+----------------+\n\n");
		printf("Press <ENTER> to validate your options...\n");

		char userInput;

		do
		{
			userInput = getchar();
		} while (userInput != '\n' && (userInput < '1' || userInput > '4'));

		switch (userInput)
		{
		case '1':

			algorithmIndex = 0;
			modeIndex = (modeIndex + 1) % 2;

			break;

		case '2':

			algorithmIndex = (algorithmIndex + 1) % algorithmLabelsLength[modeIndex];

			break;

		case '3':

			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

			do
			{
				printf("Please enter a valid integer: ");

				if (std::cin.fail())
				{
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
				}
			} while (!(std::cin >> maximumValue));

			tcsetattr(STDIN_FILENO, TCSANOW, &newt);

			break;

		case '4':

			if (modeIndex == 1 && ++numberThreads > maximumThreads)
			{
				numberThreads = 1;
			}

			break;

		case '\n':

			if (maximumValue <= 2)
			{
				break;
			}

			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

			if (PAPI_Initialize() != PAPI_OK)
			{
				return 1;
			}

			if (modeIndex == 0)
			{
				RunSequential(algorithmIndex, maximumValue);
			}
			else if (modeIndex == 1)
			{
				RunParallel(algorithmIndex, maximumValue, numberThreads);
			}

			if (PAPI_Destroy() != PAPI_OK)
			{
				return 1;
			}

			return 0;
		}
	}
}
