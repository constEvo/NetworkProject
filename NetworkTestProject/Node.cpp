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
	nodeData.id = static_id;
}

void Node::generateEvent()
{
	this->nodeData.value = network->seedGenerator(1, 10);

	for (auto iter = followers.begin(); iter != followers.end(); ++iter)
	{
		(*iter).second.sumOfEvents += this->nodeData.value;
		(*iter).second.numOfEvents += 1;

		//print translated events
		translateEventNotify(this, (*iter).first, (*iter).second);
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
	this->nodeData.sumOfEvents = 0;
	this->nodeData.numOfEvents = 0;

	//update followers container
	chosenNode->followers.insert(std::make_pair(this, this->nodeData));
	connectionNotify(chosenNode, this);
}

void Node::collectSecondNeighbours(Node* node, std::vector<Node*>& secondLevelNeighbours)
{
	//create container to find unique second level neighbours
	std::vector<Node*> uniqueSecondLevelNeighbours;

	for (auto neighbour : node->followers)
	{
		for (auto secondNeighbour : neighbour.first->followers)
		{
			//find unique neighbours
			if (secondNeighbour.first != node
				&& std::find(uniqueSecondLevelNeighbours.begin(), uniqueSecondLevelNeighbours.end(), secondNeighbour.first) == uniqueSecondLevelNeighbours.end())
			{
				uniqueSecondLevelNeighbours.push_back(secondNeighbour.first);
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
	std::cout << "sending node is " << sendingNode->nodeData.id << " -> receiving node is "
		<< receivingNode->nodeData.id << " S = " << receivingNode->nodeData.sumOfEvents << std::endl;
	std::cout << "sending node is " << sendingNode->nodeData.id << " -> receiving node is "
		<< receivingNode->nodeData.id << " N = " << receivingNode->nodeData.numOfEvents << std::endl << std::endl;
}

void Node::translateEventNotify(Node* sendingNode, Node* receivingNode, const NodeData& receivingData)
{
	std::cout << "sending node is " << sendingNode->nodeData.id << " -> receiving node is "
		<< receivingNode->nodeData.id << " S = " << receivingData.sumOfEvents << std::endl;
	std::cout << "sending node is " << this->nodeData.id << " -> receiving node is "
		<< receivingNode->nodeData.id << " N = " << receivingData.numOfEvents << std::endl << std::endl;
}

void Node::generateNeighbourNode()
{
	Node* newNode = new Node(network);
	network->attachNewNodes(newNode);

	this->nodeData.sumOfEvents = 0;
	this->nodeData.numOfEvents = 0;

	newNode->followers.insert(std::make_pair(this, this->nodeData));
	connectionNotify(newNode, this);
}

void Node::doNothing()
{

}

void Node::update()
{
	float probability = network->seedGenerator(0.1, 100.0);

	//check probability for geretating value event
	if (probability < network->getEventProbability()[0].eProbability) 
	{
		std::cout << "generate event " << std::endl;
		generateEvent();
		
	}
	//check probability for follow node event
	else if (probability < network->getEventProbability()[0].eProbability + network->getEventProbability()[1].eProbability)
	{
		std::cout << "followNode" << std::endl;
		followNode();
		
	}
	//check probability for unfollow node event
	else if (probability < network->getEventProbability()[0].eProbability
		+ network->getEventProbability()[1].eProbability
		+ network->getEventProbability()[2].eProbability)
	{
		std::cout << "unfollowNode" << std::endl;
		unfollowNode();
	}
	//check probability for creating neighbour node event
	else if (probability < network->getEventProbability()[0].eProbability
		+ network->getEventProbability()[1].eProbability
		+ network->getEventProbability()[2].eProbability
		+ network->getEventProbability()[3].eProbability)
	{
		std::cout << "generateNeighbourNode" << std::endl;
		generateNeighbourNode();
	}
	else
	{
		std::cout << "doNothing" << std::endl;
		doNothing();
		
	}	
}









