#include <iostream>
#include <random>
#include "Network.h"

//pseudo-random generator
using engine = std::mt19937;

struct Node
{
	int id, value, sumOfEvents, numberOfEvents;
	std::unordered_set<Node*> neighbours;

	Node (int id) : id(id) 
	{
		value = 0;
		sumOfEvents = 0;
		numberOfEvents = 1;
	}	
};


void Network::generateNetwork(int startId, int depth)
{
	if (depth == 0) return;
	addEdge(startId, startId + 1);
	generateNetwork(startId + 1, depth - 1);
}

Node* Network::addNode(int id)
{
	if (nodes.find(id) == nodes.end())
	{
		nodes[id] = new Node(id);
		addNewValue(nodes[id]);
		++nextId;
	}
	return nodes[id];
}

void Network::addEdge(int id1, int id2)
{
	Node* node1 = addNode(id1);
	Node* node2 = addNode(id2);
	node1->neighbours.insert(node2);
	connectionNotify(node2, node1);
	node1->sumOfEvents += node2->sumOfEvents;
	node1->numberOfEvents += node2->numberOfEvents;
}

void Network::followNode(Node* node)
{
	//add all second level neighbours excluding current node
	std::unordered_set<Node*> secondLevelNeighbours;

	collectSecondNeighbours(node, secondLevelNeighbours);


	if (secondLevelNeighbours.empty()) return;
	
	//choose one neighbour node within available second level neighbours nodes
	int index = seedGenerator(0, secondLevelNeighbours.size() - 1);
	auto it = secondLevelNeighbours.begin();
	std::advance(it, index);
	Node* chosenNode = *it;

	//follow chosen node
	node->neighbours.insert(chosenNode);
	connectionNotify(chosenNode, node);
	node->sumOfEvents += chosenNode->sumOfEvents;
	node->numberOfEvents += chosenNode->numberOfEvents;
}

void Network::collectSecondNeighbours(Node* node, std::unordered_set<Node*>& secondLevelNeighbours)
{
	for (auto neighbour : node->neighbours)
	{
		for (auto secondNeighbour : neighbour->neighbours)
		{
			if (secondNeighbour != node && node->neighbours.find(secondNeighbour) == node->neighbours.end())
			{
				secondLevelNeighbours.insert(secondNeighbour);
			}
		}
	}
}

bool Network::isOrpahNode(std::unordered_map<int, Node*>::iterator& it)
{
	if (it->second->neighbours.empty())
	{
		return true;
	}
	return false;
}

void Network::deleteEdge(Node* node)
{
	if (node->neighbours.empty()) return;

	//chose neighbour node to delete
	int index = seedGenerator(0, node->neighbours.size() - 1);
	auto it = node->neighbours.begin();
	std::advance(it, index);
	Node* chosenNode = *it;

	//delete node
	node->neighbours.erase(chosenNode);
	node->sumOfEvents -= chosenNode->sumOfEvents;
	node->numberOfEvents -= chosenNode->numberOfEvents;
}

void Network::addNewValue(Node* node)
{
	node->value = seedGenerator(1, 10);
	node->sumOfEvents += node->value;
}

void Network::connectionNotify(Node* sendingNode, Node* receivingNode)
{
	std::cout << "sending node is " << sendingNode->id << " -> receiving node is "
		<< receivingNode->id << " S = " << sendingNode->sumOfEvents << std::endl;
	std::cout << "sending node is " << sendingNode->id << " -> receiving node is "
		<< receivingNode->id << " N = " << sendingNode->numberOfEvents << std::endl;
}

int Network::seedGenerator(int start, int end)
{
	int randomValue;

	std::random_device os_seed;
	int seed = os_seed();
	engine generator(seed);
	std::uniform_int_distribution<int> distribute(start, end);
	randomValue = distribute(generator);
	return randomValue;
}

void Network::addNeighbourNode(Node* node)
{
	int newId = ++nextId;
	Node* newNode = addNode(newId);
	node->neighbours.insert(newNode);
	connectionNotify(newNode, node);
	node->sumOfEvents += newNode->sumOfEvents;
	node->numberOfEvents += newNode->numberOfEvents;
}

void Network::doNothing()
{
	//std::cout<<  "Didn't do a thing" << std::endl;
}

void Network::update()
{
	auto it = nodes.begin();

	while (it != nodes.end())
	{
		int option = seedGenerator(0, 3);

		switch (option)
		{
		case 0: followNode(it->second); break;
		case 1: deleteEdge(it->second); break;
		case 2: addNeighbourNode(it->second); break;
		case 3: doNothing(); break;
		default: break;
		}
		if (isOrpahNode(it))
		{
			delete it->second;
			it = nodes.erase(it);
			continue;
		}
		++it;
	}
}



int main()
{
	int sizeOfNetwork;
	
	std::cout << "Type size of network : " << std::endl;
	std::cin >> sizeOfNetwork;

	Network* network = new Network();

	network->generateNetwork(1, sizeOfNetwork);

	network->update();


	return 0;
}


