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
		std::cout << "Sleeping ... " << i << "\n";
		sleep(1);
	}

	return nullptr;
}


void* wakingUp(void* arg)
{
	for (int i = 0; i < 10; ++i)
	{
		std::cout << "Waking up ... " << i << "\n";
		sleep(2);
	}

	return nullptr;	
}


void awake()
{
	for (int i = 0; i < 5; ++i)
	{
		std::cout << "I'm awake ... " << i << "\n";
		sleep(3);
	}
}


int main()
{
	std::signal(SIGINT, signalHandler);


	std::array<pthread_t, NUMBER_OF_THREADS> threads;
	std::cout << "Creating thread number: 2.\n";
	pthread_create(&threads.at(0), nullptr, wakingUp, nullptr );

	std::cout << "Creating thread number: 1.\n";
	pthread_create(&threads.at(1), nullptr, sleeping, nullptr );
	

	awake();

	// wait for everythread to be finished before end of program
	for (int i = 0; i < threads.size(); ++i)
	{
		pthread_join(threads.at(i), nullptr);
	}

	return 0;			
}
