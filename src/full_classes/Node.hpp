#ifndef NODE_HPP
#define NODE_HPP

#include "Environment.hpp"
#include "GateNode.hpp"
#include "LinkedStack.hpp"
#include "ScheduledGate.hpp"
#include <set>
#include <cassert>
#include <iostream>
class Queue;
using namespace std;

//Warning: you may need to run make clean after changing MAX_QUBITS
const int MAX_QUBITS = 20;
//extern int GLOBALCOUNTER;

class Node {
  public:
	Node * parent;//node from which this one expanded
	Environment * env;//object with functions/data shared by all nodes
    int cycle;//current cycle
	int cost;//the node's cost (in cycles)
	int cost2 = 0;//used as tiebreaker in some places
	
	int numUnscheduledGates;//the number of gates from the original circuit that are not yet part of this node's schedule
	bool expanded = false;//whether or not this node has been popped from the queue
	bool dead = false;//where or not this node has been marked as 'dead' by a filter
	
	//int debugID = GLOBALCOUNTER++;
	
	char qal[MAX_QUBITS];//qubit mapping
	char laq[MAX_QUBITS];//qubit mapping (inverted)
	
	ScheduledGate * lastNonSwapGate[MAX_QUBITS];//last scheduled non-swap gate per LOGICAL qubit
	ScheduledGate * lastGate[MAX_QUBITS];//last scheduled gate per PHYSICAL qubit
	
	//the number of cycles until the specified physical qubit is available
	inline int busyCycles(int physicalQubit) {
		ScheduledGate * sg = this->lastGate[physicalQubit];
		if(!sg) return 0;
		int cycles = sg->cycle + sg->latency - this->cycle;
		if(cycles < 0) return 0;
		return cycles;
	}
	
	std::set<GateNode*> readyGates;//set of gates in DAG whose parents have already been scheduled
	
	LinkedStack<ScheduledGate*> * scheduled;//list of scheduled gates. Warning: this linked list's data overlaps with the same list in parent node
	
	Node();
	
	~Node();
	
	//swap two physical qubits in qubit map, without scheduling a gate
	inline bool swapQubits(int physicalControl, int physicalTarget) {
		if(qal[physicalControl] < 0 && qal[physicalTarget] < 0) {
			return false;
		} else if(qal[physicalTarget] < 0) {
			laq[(int)qal[physicalControl]] = physicalTarget;
		} else if(qal[physicalControl] < 0) {
			laq[(int)qal[physicalTarget]] = physicalControl;
		} else {
			std::swap(laq[(int)qal[physicalControl]], laq[(int)qal[physicalTarget]]);
		}
		std::swap(qal[physicalControl], qal[physicalTarget]);
		return true;
	}
	
	//schedule a gate, or return false if it conflicts with an active gate
	//the gate parameter uses logical qubits (except in swaps); this function determines physical locations based on prior swaps
	//the timeOffset can be used if we want to schedule a gate to start X cycles in the future
	//this function adjusts qubit map when scheduling a swap
	bool scheduleGate(GateNode * gate, unsigned int timeOffset = 0);
	
	//prepares a new child node (without scheduling any more gates)
	Node * prepChild();
};

#endif
