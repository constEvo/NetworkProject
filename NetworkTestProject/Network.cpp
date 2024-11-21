#include "Network.h"
#include <iostream>
#include <random>
#include "Node.h"
#include <algorithm>

//pseudo-random generator
using engine = std::mt19937;

void Network::attachNode(Node* node)
{
	nodes.push_back(node);
}

void Network::start(int depth)
{
	if (depth == 0) return;

	//generate random events for starting network
	for (int i = 0; i < depth; ++i)
	{
		Node* node = new Node(this);
		attachNode(node);
		node->generateEvent();
	}

	//made neighbours for start network
	for (int i = 0; i < nodes.size(); ++i)
	{
		int option = seedGenerator(0, nodes.size() - 1);
		addEdge(nodes[i], nodes[option]);
	}
}

void Network::simulate()
{
	char codeWord = ' ';

	std::vector<Node*> deleteNodes;

	int i = 0;

	while (nodes.size() < 2000) //codeWord != 'y')
	{
		std::cout << "ITERATION " << i << std::endl;

		notify(deleteNodes);

		//erase orphan nodes
		nodes.erase(
		std::remove_if(nodes.begin(), nodes.end(),
		[&deleteNodes](Node* node)
		{ return std::find(deleteNodes.begin(), deleteNodes.end(), node) != deleteNodes.end();})
		,nodes.end());

		//add new nodes to main nodes container
		nodes.insert(nodes.end(), newNodes.begin(), newNodes.end());

		newNodes.clear();
		deleteNodes.clear();

		nodesCount();

		//std::cout << "Do you wanna stop simulation y/n ? ";
		//std::cin >> codeWord;

	}	

	for (auto* node : nodes)
	{
		delete node;
	}
	nodes.clear();	
}

void Network::addEdge(Node* first_node, Node* second_node)
{
	if (first_node == second_node) return;
	
	if (second_node->nodeData.value)
	{
		first_node->nodeData.sumOfEvents = second_node->nodeData.value;
		first_node->nodeData.numOfEvents = 1;
	}
	second_node->followers.insert(std::make_pair(first_node, first_node->nodeData));
	first_node->connectionNotify(second_node, first_node);
}

void Network::detachNode(Node* node)
{
	auto iter = std::find(nodes.begin(), nodes.end(), node);
	if (iter != nodes.end())
	{
		nodes.erase(iter);
	}

}

void Network::notify(std::vector<Node*>& deleteNodes)
{
	for (auto node : nodes)
	{
		node->update();

		if (isOrphan(node))
		{
			deleteNodes.push_back(node);
		}
	}
}

void Network::nodesCount()
{
	std::cout << "There are " << nodes.size() << " nodes in the vector" << std::endl;
}

bool Network::isOrphan(Node* node)
{
	return node->followers.empty();
}

bool Network::isEmpty()
{
	return nodes.empty();
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

void Network::outputTypeEvent()
{
	std::cout << "Type event probability according order below: " << std::endl;
	std::cout << "1.Generate event" << std::endl;
	std::cout << "2.Follow node" << std::endl;
	std::cout << "3.Unfollow neighbour node" << std::endl;
	std::cout << "4.Add neighbour node" << std::endl;
	std::cout << "5.Do nothing" << std::endl;
}

void Network::inputProbability()
{
	float probability;
	//input probability for each event
	for (int i = 0; i < 5; ++i)
	{
		std::cout << i + 1 << ".";
		std::cin >> probability;
		std::cout << std::endl;
		Event event;
		event.eventId = i;
		event.eProbability = probability;
		eventProbability.push_back(event);
	}
}

std::vector<Event> Network::getEventProbability()
{
	return eventProbability;
}

void Network::attachNewNodes(Node* node)
{
	newNodes.push_back(node);
}




