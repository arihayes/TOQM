#include "Latency.hpp"

//Latency example: 6 cycles per SWP; 2 cycles per 2-qubit gate; 1 cycle otherwise
class Latency_1_2_6 : public Latency {
  public:
	int getLatency(string gateName, int numQubits, int target, int control) {
		if(!gateName.compare("swp") || !gateName.compare("SWP")) {
			return 6;
		} else if(numQubits > 1) {
			return 2;
		} else {
			return 1;
		}
	}
};
