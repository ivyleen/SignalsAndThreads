#include <iostream>
#include <csignal>
#include <unistd.h>
#include <pthread.h>
#include <array>

const int NUMBER_OF_THREADS = 2;

void signalHandler (int sigNum)
{
	std::cout << "Signal (" << sigNum << ") for program interrupted on process ID:" << getpid() << "\n";
	//exit(sigNum);
	write(STDOUT_FILENO, "I won't die\n", 13);
}


void* sleeping( void* arg)
{
	int* iptr = new int(0);
	while ( *iptr != 10)
	{
		sleep(1);
		std::cout << "Thread №2: Sleeping ... " << *iptr << "\n";
		(*iptr)++;
	}

	return iptr;
}


void* wakingUp(void* arg)
{
	int* iptr = static_cast<int*>(arg);

	while (*iptr != 10)
	{
		sleep(2);
		std::cout << "Thread №1: Waking up ... " << *iptr << "\n";
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
		std::cout << "Main function call: I'm awake ... " << i << "\n";
	}
}


int main()
{
	std::signal(SIGINT, signalHandler);
	std::signal(SIGTERM, signalHandler);
	std::signal(SIGKILL, signalHandler);

	void* i = 0;	

	// threads creation
	std::array<pthread_t, NUMBER_OF_THREADS> threads;
	std::cout << "Creating thread number: 1.\n";
	pthread_create(&threads.at(0), nullptr, wakingUp, &i );

	std::cout << "Creating thread number: 2.\n";
	pthread_create(&threads.at(1), nullptr, sleeping, nullptr );
	
	// normal call of a function
	awake();

	// wait for everythread to be finished before end of program
	for (int j = 0; j < threads.size(); ++j)
	{
		pthread_join(threads.at(j), &i);
	}

	std::cout << "i should be 10 now: " << *(static_cast<int*>(i)) << "\n";
	// terminate the program
	return 0;			
}
