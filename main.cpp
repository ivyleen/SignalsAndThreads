#include <iostream>
#include <csignal>
#include <unistd.h>
#include <pthread.h>
#include <array>

const int NUMBER_OF_THREADS = 2;

void signalHandler (int sigNum)
{
	std::cout << "Signal (" << sigNum << ") - program interrupted.\n";
	exit(sigNum);
}


void* sleeping( void* arg)
{
	for (int i = 0; i < 10; ++i)
	{
		sleep(1);
		std::cout << "Sleeping ... " << i << "\n";
	}

	return nullptr;
}


void* wakingUp(void* arg)
{
	int* iptr = static_cast<int*>(arg);

	while (*iptr != 10)
	{
		sleep(2);
		std::cout << "Waking up ... " << *iptr << "\n";
		(*iptr)++;
	}

	*iptr = 0;
	return nullptr;	
}


void awake()
{
	for (int i = 0; i < 5; ++i)
	{
		sleep(3);
		std::cout << "I'm awake ... " << i << "\n";
	}
}


int main()
{
	std::signal(SIGINT, signalHandler);

	int* i = 0;

	// thread creating
	std::array<pthread_t, NUMBER_OF_THREADS> threads;
	std::cout << "Creating thread number: 1.\n";
	pthread_create(&threads.at(0), nullptr, wakingUp, &i );
	std::cout << "i is still: " << i << "\n";

	std::cout << "Creating thread number: 2.\n";
	//pthread_create(&threads.at(1), nullptr, sleeping, nullptr );
	
	// normal call of a function
	awake();

	// wait for everythread to be finished before end of program
	for (int j = 0; j < threads.size(); ++j)
	{
		pthread_join(threads.at(j), nullptr);
	}


	// terminate the program
	return 0;			
}
