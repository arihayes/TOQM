#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "Node.hpp"
#include "Environment.hpp"
#include "Filter.hpp"
#include <iostream>

class Queue {
  private:
	///Push a node into the priority queue
	///Return false iff this fails for any reason
	///Pre-condition: our filters have already said this node is good
	///Pre-condition: newNode->cost has already been set
	virtual bool pushNode(Node * newNode) = 0;
	
  protected:
	Node * bestFinalNode = 0;
	int numPushed=0,numFiltered=0,numPopped=0;
	
  public:
	virtual ~Queue() {};
	
	virtual int setArgs(char** argv) {
		//This is used to set the queue's parameters via command-line
		//return number of args consumed
		
		return 0;
	}
	
	virtual int setArgs() {
		//This is used to set the queue's parameters via std::cin
		//return number of args consumed
		
		return 0;
	}
	
	///Pop a node and return it
	virtual Node* pop() = 0;
	
	///Return number of elements in queue
	virtual int size() = 0;
	
	///Push a node into the priority queue
	///Return false iff this fails for any reason
	///Pre-condition: newNode->cost has already been set
	bool push(Node * newNode) {
		numPushed++;
		if(!newNode->env->filter(newNode)) {
			bool success = this->pushNode(newNode);
			if(success) {
				return true;
			} else {
				std::cerr << "WARNING: pushNode(Node*) failed somehow.\n";
				return false;
			}
		}
		numFiltered++;
		return false;
	}
	
	inline Node * getBestFinalNode() {
		return bestFinalNode;
	}
};

#endif
