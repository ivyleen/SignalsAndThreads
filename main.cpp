#include <iostream>
#include <csignal>
#include <unistd.h>
#include <numeric>
#include <thread>
#include <vector>

const unsigned int NUMBER_OF_THREADS = 2;


void signalHandler (int sigNum)
{
	std::cout << "Signal (" << sigNum << ") for program interrupted on process ID:" << getpid() << "\n";
	//exit(sigNum);
	write(STDOUT_FILENO, "I won't die\n", 13);
}

void sumBigNumber (uint64_t &sum, uint64_t begin, uint64_t end)
{
	sum = 0;
	for (uint64_t i = begin; i < end; ++i)
	{
		sum += i;
	}
}

int main()
{
	std::signal(SIGINT, signalHandler);
	std::signal(SIGTERM, signalHandler);
	// it will not work because the KILL signal is a command
	//std::signal(SIGKILL, signalHandler);

	uint64_t NUMBER_OF_ELEMENTS = 1000 * 1000 * 1000;
	uint64_t STEP = NUMBER_OF_ELEMENTS / NUMBER_OF_THREADS;

	std::vector<uint64_t> sums (NUMBER_OF_THREADS);
	std::vector<std::thread> threads;

	for (uint64_t i = 0; i < NUMBER_OF_THREADS; i++)
	{
		threads.push_back (std::thread (sumBigNumber, 
					        std::ref ( sums[i] ), 
						i * STEP, 
						(i + 1) * STEP ) );
	}

	for ( std::thread &t : threads)
	{
		if ( t.joinable())
		{
			t.join();
		}	
	}

	uint64_t sum = std::accumulate ( sums.begin(), sums.end(), uint64_t(0));
	std::cout << "Partial sums: ";
	for (auto n : sums)
	{
		std::cout << n << ", ";
	}
	std::cout << "\n";

	std::cout << "Total sum: " << sum << std::endl;


	// terminate the program
	return 0;			
}
