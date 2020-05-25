#include <iostream>
#include <csignal>
#include <unistd.h>
#include <array>
#include <thread>


const int NUMBER_OF_THREADS = 2;
static bool s_Sleeping = true;


void signalHandler (int sigNum)
{
	std::cout << "Signal (" << sigNum << ") for program interrupted on process ID:" << getpid() << "\n";
	//exit(sigNum);
	write(STDOUT_FILENO, "I won't die\n", 13);
}


void sleeping()
{
	using namespace std::literals::chrono_literals;

	while ( s_Sleeping )
	{
		std::cout << "Sleeping ... on thread: " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(1s);
	}

}


void wakingUp()
{
	using namespace std::literals::chrono_literals;

	while ( s_Sleeping )
	{
		std::cout << "Sleeping ... on thread: " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(2s);
	}
		
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
	// it will not work because the KILL signal is a command
	//std::signal(SIGKILL, signalHandler);

	std::thread sleepingThread( sleeping );
//	std::thread wakingUpThread( wakingUp );	

	// normal call of a function
	awake();

	//after main thread is over printing I'm awake stop the sleeping thread
	s_Sleeping = false;
	sleepingThread.join();
	
	// terminate the program
	return 0;			
}
