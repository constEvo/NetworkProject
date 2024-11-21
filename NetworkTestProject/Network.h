#pragma once
#include <vector>

class Node;

struct Event
{
	int eventId;
	float eProbability;
};

class Network
{
public:

	Network() {}

	//node management func
	void attachNode(Node* node);
	void detachNode(Node* node);
	//add new node generated via event
	void attachNewNodes(Node* node);

	//probability input func
	void outputTypeEvent();
	void inputProbability();
	//seed random value within limit
	int seedGenerator(int start, int end);

	//simulation func
	void start(int depth);
	void addEdge(Node* first_node, Node* second_node);
	void simulate();
	bool isOrphan(Node* node);
	void notify(std::vector<Node*>& deleteNodes);


	void nodesCount();
	bool isEmpty();

	std::vector<Event> getEventProbability();
private:

	std::vector<Event> eventProbability;

	std::vector<Node*> newNodes;

	std::vector<Node*> nodes;
	
};

