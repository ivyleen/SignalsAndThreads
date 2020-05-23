#include <iostream>
#include <csignal>
#include <unistd.h>

void signalHandler (int sigNum)
{
	std::cout << "Signal (" << sigNum << ") - program interrupted.\n";
	exit(sigNum);
}


int main()
{
	std::signal(SIGINT, signalHandler);

	while (1)
	{
		std::cout << "Sleeping ...\n";
		sleep(1);
	}

	return 0;			
}
