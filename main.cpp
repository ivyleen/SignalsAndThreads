#include <iostream>
#include <csignal>
#include <unistd.h>
#include <pthread.h>


void signalHandler (int sigNum)
{
	std::cout << "Signal (" << sigNum << ") - program interrupted.\n";
	exit(sigNum);
}


void* sleeping( void* arg)
{
	while (1)
	{
		std::cout << "Sleeping ...\n";
		sleep(1);
	}

	return NULL;
}


void awake()
{
	while (1)
	{
		std::cout << "I'm awake ...\n";
		sleep(2);
	}
}


int main()
{
	std::signal(SIGINT, signalHandler);


	pthread_t newthread;
	pthread_create(&newthread, NULL, sleeping, NULL);

	awake();

	return 0;			
}
