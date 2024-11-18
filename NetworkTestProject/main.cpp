#include "Network.h"
#include <iostream>

int main()
{
	int sizeOfNetwork;

	Network* network = new Network();

	//input start probability parameters
	network->outputTypeEvent();
	network->inputProbability();
	
	//input start size of network
	int depth;
	std::cout << "input network's depth: ";
	std::cin >> depth;
	network->start(depth);

	//simulate network
	network->simulate();

	return 0;
}