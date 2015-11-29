#ifndef MEMORY_H_
#define MEMORY_H_
#include "Process.h"
using namespace std;

class Process;
class Memory {
private:
	vector<Process> processes;

	void addProcess(Process& p, int pos) {
		p.setStart(processes[pos].getStart());
		processes[pos].setStart(processes[pos].getStart() + p.getSize());
		processes[pos].setSize(processes[pos].getSize() - p.getSize());
		if(processes[pos].getSize() == 0) processes.erase(processes.begin() + pos);
		processes.insert(processes.begin() + pos, p);
	}

public:
	Memory() {
		processes.push_back(Process());
	}

	bool empty() const { return processes.size() == 1; }

	string toString() {
		stringstream ss;
		for(unsigned int i = 0; i < processes.size(); i++) ss << processes[i].toString() << " ";
		return ss.str();
	}

	void decrease() {
		for(unsigned int i = 0; i < processes.size(); i++) processes[i].decrease();
		for(unsigned int i = 0; i < processes.size(); i++)
			if(processes[i].getRemaining() == 0)
				while(i+1 < processes.size() && processes[i+1].getRemaining() == 0) {
					processes[i].setSize(processes[i].getSize() + processes[i+1].getSize());
					processes.erase(processes.begin()+ i+1);
				}
	}

	bool firstPlace(Process& p) {
		unsigned int i;
		bool found = false;
		for(i = 0; i < processes.size() && !found;)
			if(processes[i].getRemaining() == 0 && p.getSize() <= processes[i].getSize())
				found = true;
			else i++;

		if(found) addProcess(p, i);
		return found;
	}

	bool bestPlace(Process& p) {
			int best = -1;
			for(unsigned int i = 0; i < processes.size(); i++)
				if(processes[i].getRemaining() == 0 && p.getSize() <= processes[i].getSize()) {
					if(best == -1) best = i;
					else if(processes[i].getSize() - p.getSize() < processes[best].getSize() - p.getSize()) best = i;
				}

			if(best != -1) addProcess(p, best);
			return best != -1 ? true : false;
		}
};

#endif /* MEMORY_H_ */
