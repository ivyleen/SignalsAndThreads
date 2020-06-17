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

	const unsigned int NUMBER_OF_ELEMENTS = 1000 * 1000 * 1000;
	const unsigned int STEP = NUMBER_OF_ELEMENTS / NUMBER_OF_THREADS;

	std::vector<uint64_t> sums (NUMBER_OF_THREADS);

	std::thread first  (sumBigNumber, std::ref ( sums[0] ), 0, STEP );
	std::thread second (sumBigNumber, std::ref ( sums[1] ), STEP, NUMBER_OF_ELEMENTS * STEP);

	first.join();
	second.join();

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
