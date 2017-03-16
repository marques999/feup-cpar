#include <iostream>
#include <time.h>
#include <papi.h>
#include <iomanip>
#include <termios.h>
#include <unistd.h>

#include "sequential.h"
#include "parallel.h"

const char* modeLabels[] =
{ "Sequential", "OpenMP" };

const char* improvementlabels[] =
{ "BoolVector", "UnsignedChar", "NoSmallest", "BoolArray" };

int main(int argc, const char* argv[])
{
	unsigned modeIndex = 0;
	unsigned improvementIndex = 0;
	unsigned limitValue = 0;

	static struct termios oldt, newt;

	/*tcgetattr gets the parameters of the current terminal
	 STDIN_FILENO will tell tcgetattr that it should write the settings
	 of stdin to oldt*/
	tcgetattr(STDIN_FILENO, &oldt);
	/*now the settings will be copied*/
	newt = oldt;
	newt.c_lflag &= ~ICANON;
	newt.c_lflag &= ~ECHO;
	/*ICANON normally takes care that one line at a time will be processed
	 that means it will return if it sees a "\n" or an EOF or an EOL*/

	 /*Those new settings will be set to STDIN
	  TCSANOW tells tcsetattr to change attributes immediately. */
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	while (true)
	{
		printf("\033c");
		printf("+-----------------+----------------+\n");
		std::cout << "| [1] Mode        | " << std::left << std::setw(14)
			<< modeLabels[modeIndex] << " |" << std::endl;
		std::cout << "| [2] Improvement | " << std::setw(14)
			<< improvementlabels[improvementIndex] << " |" << std::endl;
		std::cout << "| [3] Limit       | " << std::setw(14) << limitValue
			<< " |" << std::endl;
		printf("+-----------------+----------------+\n\n");
		printf("Press <ENTER> to validate your options...\n");

		char userInput;

		do
		{
			userInput = getchar();
		} while (userInput != '\n' && (userInput < '1' || userInput > '3'));

		switch (userInput)
		{
		case '1':

			modeIndex = (modeIndex + 1) % 2;

			break;

		case '2':

			improvementIndex = (improvementIndex + 1) % 4;

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
			} while (!(std::cin >> limitValue));

			tcsetattr(STDIN_FILENO, TCSANOW, &newt);

			break;

		case '\n':

			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

			if (modeIndex == 0)
			{
				RunSequential(improvementIndex, limitValue);
			}

			return 0;
		}
	}

	return 0;
}