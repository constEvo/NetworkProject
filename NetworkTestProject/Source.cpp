#include "Network.h"
#include <iostream>
#include <random>
#include <utility>

//pseudo-random generator
using engine = std::mt19937;

class Node
{
public:

	Node(Network* network) : network_(network)
	{
		network_->attachNode(this);
		std::cout << "Node " << ++static_id << " attached" << std::endl;
		id = static_id;
		value = 0;
		sumOfEvents = 0;
	}

	void generateEvent()
	{
		value = network_->seedGenerator(1, 10);
	}

	void followNode()
	{
		//add all second level neighbours excluding current node
		std::unordered_set<Node*> secondLevelNeighbours;

		collectSecondNeighbours(this, secondLevelNeighbours);

		if (secondLevelNeighbours.empty()) return;

		//choose one neighbour node within available second level neighbours nodes
		int index = network_->seedGenerator(0, secondLevelNeighbours.size() - 1);
		auto it = secondLevelNeighbours.begin();
		std::advance(it, index);
		Node* chosenNode = *it;

		//follow chosen node
		//add folowed node and it's events
		receivedEvents.insert(std::make_pair(chosenNode, chosenNode->sumOfEvents));
		//add followed nove own value event
		auto iter = receivedEvents.find(chosenNode);
		iter->second += chosenNode->value;
		connectionNotify(chosenNode);
	}

	void collectSecondNeighbours(Node* node, std::unordered_set<Node*>& secondLevelNeighbours)
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

	void unfollowNode()
	{
		if (neighbours.empty()) return;

		//chose neighbour node to delete
		int index = network_->seedGenerator(0, neighbours.size() - 1);
		auto it = neighbours.begin();
		std::advance(it, index);
		Node* chosenNode = *it;

		//delete node
		//neighbours.erase(chosenNode);
		receivedEvents.erase(chosenNode);
		//sumOfEvents -= chosenNode->sumOfEvents;
		//numberOfEvents -= chosenNode->numberOfEvents;
	}

	void connectionNotify(Node* sendingNode)
	{
		auto iter = this->receivedEvents.find(sendingNode);
		std::cout << "sending node is " << sendingNode->id << " -> receiving node is "
			<< this->id << " S = " << iter->second << std::endl;
		std::cout << "sending node is " << sendingNode->id << " -> receiving node is "
			<< this->id << " N = " << sendingNode->receivedEvents.size() + 1 << std::endl;
	}

	void generateNeighbourNode()
	{
		int newId = ++nextId;
		Node* newNode = addNode(newId);
		node->neighbours.insert(newNode);
		connectionNotify(newNode, node);
		node->sumOfEvents += newNode->sumOfEvents;
		node->numberOfEvents += newNode->numberOfEvents;
	}

	void doNothing()
	{

	}

	void update()
	{

	}

private:
	int id, value, sumOfEvents;
	Network* network_;
	static int static_id;
	std::unordered_set<Node*> neighbours; //neighbour node, sumOfEvents and numbOfEvents from the neighbour node
	std::unordered_map<Node*, int> receivedEvents;
};

int Node::static_id = 0;

class Network
{
public:
	void attachNode(Node* node)
	{
		nodes.push_back(node);
	}

	void detachNode(Node* node)
	{
		auto detachNode = find(nodes.begin(), nodes.end(), node);
		if (detachNode != nodes.end())
		{
			nodes.erase(detachNode);
		}
	}

	void notify()
	{
		std::vector<Node*>::iterator iter = nodes.begin();

		while (iter != nodes.end())
		{
			(*iter)->update();
			++iter;
		}
	}

	void nodesCount()
	{
		std::cout << "There are " << nodes.size() << " nodes in the vector" << std::endl;
	}

	bool isEmpty()
	{
		return nodes.empty();
	}

	int seedGenerator(int start, int end)
	{
		int randomValue;

		std::random_device os_seed;
		int seed = os_seed();
		engine generator(seed);
		std::uniform_int_distribution<int> distribute(start, end);
		randomValue = distribute(generator);
		return randomValue;
	}

private:
	std::vector<Node*> nodes;
};



/*
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
		case 0: followNode(it->second);
			std::cout << "followNode" << std::endl; break;
		case 1: deleteEdge(it->second);
			std::cout << "deleteEdge" << std::endl; break;
		case 2: addNeighbourNode(it->second);
			std::cout << "addNeighbour" << std::endl; break;
		case 3: doNothing();
			std::cout << "do nothing" << std::endl; break;
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

bool Network::isEmpty()
{
	if (nodes.empty())
	{
		return true;
	}
	return false;
}

*/


int main()
{
	int sizeOfNetwork;

	std::cout << "Type size of network : " << std::endl;
	std::cin >> sizeOfNetwork;

	/*
	Network* network = new Network();

	network->generateNetwork(1, sizeOfNetwork);

	network->update();
	*/



	return 0;
}


