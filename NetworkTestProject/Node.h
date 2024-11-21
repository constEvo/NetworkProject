#pragma once
#include <unordered_map>
#include <vector>

class Network;



struct NodeData
{
	int id, value = 0, sumOfEvents = 0, numOfEvents = 0;
};

class Node
{
public:
	Network* network;

	Node(Network* newNetwork);


	//event functions 
	void generateEvent();
	void followNode();
	void collectSecondNeighbours(Node* node, std::vector<Node*>& secondLevelNeighbours);
	void unfollowNode();
	void generateNeighbourNode();
	void doNothing();
	void connectionNotify(Node* sendingNode, Node* receivingNode);
	void translateEventNotify(Node* sendingNode, Node* receivingNode, const NodeData& receivingData);
	void update();

	NodeData nodeData;
	std::unordered_map<Node*, NodeData> followers;
	std::unordered_map<Node*, std::pair<int, int> > receivedEvents;

private:

	static int static_id;

	

	
};

