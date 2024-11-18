#include "Node.h"
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include "Network.h"



int Node::static_id = 0;

Node::Node(Network* newNetwork) : network(newNetwork)
{
	++static_id;
	nodeData = new NodeData();
	nodeData->id = static_id;
}

void Node::generateEvent()
{
	this->nodeData->value = network->seedGenerator(1, 10);

	for (auto iter = followers.begin(); iter != followers.end(); ++iter)
	{
		(*iter).second->sumOfEvents += this->nodeData->value;
		(*iter).second->numOfEvents += 1;
		connectionNotify(this, (*iter).first);
	}
}

void Node::followNode()
{
	//add all second level neighbours excluding current node
	std::vector<Node*> secondLevelNeighbours;

	collectSecondNeighbours(this, secondLevelNeighbours);

	if (secondLevelNeighbours.empty()) return;

	//choose one neighbour node within available second level neighbours nodes
	int index = network->seedGenerator(0, secondLevelNeighbours.size() - 1);
	auto it = secondLevelNeighbours.begin();
	std::advance(it, index);
	Node* chosenNode = *it;

	//recieve events from chosen node and increment number of received events
	this->nodeData->sumOfEvents = chosenNode->nodeData->value;
	if (chosenNode->nodeData->value == 0)
	{
		this->nodeData->numOfEvents = 1;
	}
	else
	{
		this->nodeData->numOfEvents = 1;
	}
	//update followers container
	chosenNode->followers.insert(std::make_pair(this, this->nodeData));
	connectionNotify(chosenNode, this);
}

void Node::collectSecondNeighbours(Node* node, std::vector<Node*>& secondLevelNeighbours)
{
	for (auto neighbour : node->followers)
	{
		for (auto secondNeighbour : neighbour.first->followers)
		{
			if (secondNeighbour.first != node)
			{
				secondLevelNeighbours.push_back(secondNeighbour.first);
			}
		}
	}
}

void Node::unfollowNode()
{
	if (followers.empty()) return;

	//chose neighbour node to delete
	int index = network->seedGenerator(0, followers.size() - 1);
	auto it = followers.begin();
	std::advance(it, index);
	Node* chosenNode = it->first;

	//unfollow neighbour node
	followers.erase(chosenNode);
}

void Node::connectionNotify(Node* sendingNode, Node* receivingNode)
{
	std::cout << "sending node is " << sendingNode->nodeData->id << " -> receiving node is "
		<< receivingNode->nodeData->id << " S = " << receivingNode->nodeData->sumOfEvents << std::endl;
	std::cout << "sending node is " << sendingNode->nodeData->id << " -> receiving node is "
		<< receivingNode->nodeData->id << " N = " << receivingNode->nodeData->numOfEvents << std::endl << std::endl;
}

void Node::generateNeighbourNode()
{
	Node* newNode = new Node(network);
	network->attachNewNodes(newNode);

	this->nodeData->sumOfEvents = newNode->nodeData->value;
	this->nodeData->numOfEvents = 0;

	newNode->followers.insert(std::make_pair(this, this->nodeData));
	connectionNotify(newNode, this);
}

void Node::doNothing()
{

}

void Node::update()
{
	
	float probability = network->seedGenerator(0, 100.0);

	for (auto i = 0; i < 5; ++i)
	{
		if (probability < network->getEventProbability()[i].eProbability)
		{
			triggerEvent(network->getEventProbability()[i].eventId);
			return;
		}
	}
	
}

void Node::triggerEvent(const int& option)
{
	switch (option)
	{
	case 0: generateEvent(); break;
	case 1: followNode(); break;
	case 2: unfollowNode(); break;
	case 3: generateNeighbourNode(); break;
	case 4: doNothing();  break;
	default: break;
	}
}








