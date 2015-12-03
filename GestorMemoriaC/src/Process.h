#ifndef PROCESS_H_
#define PROCESS_H_
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#define FIRSTPLACE 1
#define BESTPLACE 2
using namespace std;

class Process {
private:
	string name;
	int arrival, size, remaining, start;
public:
	static const int MAXMEMORIA = 2000;

	const string& getName() const { return name; }
	int getArrival() const { return arrival; }
	int getSize() const { return size; }
	int getRemaining() const { return remaining; }
	int getStart() const { return start; }

	void setArrival(int arrival) {
		if(arrival > 0) this->arrival = arrival;
	}

	void setName(const string& name) {
		if(name != "") this->name = name;
	}

	void setRemaining(int remaining) {
		if(remaining > 0) this->remaining = remaining;
	}

	void setSize(int size) {
		if(size > 0) this->size = size;
	}

	void setStart(int start) {
		if(start >= 0) this->start = start;
	}

	Process() {
		arrival = remaining = start = 0;
		size = MAXMEMORIA;
	}

	Process(string line) {
		string aname;
		int aarrival, asize, aremaining;
		istringstream iss(line);
		iss >> aname;
		iss >> aarrival;
		iss >> asize;
		iss >> aremaining;
		setName(aname);
		setArrival(aarrival);
		setSize(asize);
		setRemaining(aremaining);
		setStart(0);
	}

	Process(const Process& p) {
		name = p.name;
		arrival = p.arrival;
		size = p.size;
		remaining = p.remaining;
		start = 0;
	}

	string toString() {
		stringstream ss;
		string str;
		ss << "[" << start;
		str = ss.str();
		if(remaining > 0) ss << " " << name.c_str() << " ";
		else ss << " HUECO ";
		str = ss.str();
		ss << size << "]";
		str = ss.str();
		return ss.str();
	}

	void decrease() { if(remaining > 0) remaining--; }
};

#endif /* PROCESS_H_ */
