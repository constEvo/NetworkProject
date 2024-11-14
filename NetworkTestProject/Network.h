#pragma once
#include <unordered_set>
#include <unordered_map>




struct Node;


class Network
{
private:
	std::unordered_map<int, Node*> nodes;
	//count for id nodes
	int nextId = 1;
public:

	//functions to create network
	//form new network
	void generateNetwork(int startId, int depth);
	
	//add node
	Node* addNode(int id);

	//generate edge for network
	void addEdge(int id1, int id2);

	//func for network's nodes

	void followNode(Node* node);
	
	void collectSecondNeighbours(Node* node, std::unordered_set<Node*>& secondLevelNeighbours);

	//check if node has heighbours nodes
	bool isOrpahNode(std::unordered_map<int, Node*>::iterator& it);
	
	//deleting edge between neighbours nodes
	void deleteEdge(Node* node);
	
	//assign new event to node
	void addNewValue(Node* node);

	void connectionNotify(Node* sendingNode, Node* receivingNode);

	//seeding random generator values
	int seedGenerator(int start, int end);

	//add neighbour node for chosen node
	void addNeighbourNode(Node* node);
	
	void doNothing();
	
	void update();
	
};