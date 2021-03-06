#ifndef COSTFUNC_HPP
#define COSTFUNC_HPP

#include "Node.hpp"
#include "NodeMod.hpp"

class CostFunc {
  public:
	virtual ~CostFunc() {};
	virtual int _getCost(Node * node) = 0;
	
	///Returns the cost of the node
	///This may invoke node modifiers prior to calculating the cost.
	int getCost(Node * node) {
		Environment * env = node->env;
		env->runNodeModifiers(node, MOD_TYPE_BEFORECOST);
		return _getCost(node);
	}
	
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
};

#endif
