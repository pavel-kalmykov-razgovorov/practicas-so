#include "Memory.h"
using namespace std;

void readFile(vector<Process>& fp, string fileName) {
	string line;
	ifstream in(fileName.c_str());
	if(in.is_open())
		while(getline(in,line))
			if(line != "") fp.push_back(Process(line));
	in.close();
}

void exportResult(vector<Process>& fp, Memory& mem, string fileName, int option) {
	int moment = 1;
	bool inserted = false;
	ofstream out(fileName.c_str());

	if(out.is_open()) {
		while(!fp.empty() || !mem.empty()) {
			for(unsigned int i = 0; i < fp.size(); i++)
				if(fp[i].getArrival() <= moment) {
					if(option == FIRSTPLACE) inserted = mem.firstPlace(fp[i]);
					else if(option == BESTPLACE) inserted = mem.bestPlace(fp[i]);
					if(inserted) fp.erase(fp.begin() + i--);
					else cerr << fp[i].getName() << " no pudo ser cargado en el instante " << moment << endl;
				}
			cout << moment << ": " << mem.toString() << endl;
			out << moment << ": " << mem.toString() << endl;
			moment++;
			mem.decrease();
		}
	}
	out.close();
}

int main(int argc, char const* argv[]) {
	vector<Process> fp;
	Memory memory;
	char option;
	string inputFileName, outputFileName;
	do {
		cout << "1.- PRIMER HUECO\n"
			 << "2.- MEJOR HUECO\n"
			 << "Opción: ";
		cin  >> option;
	} while(option != '1' && option != '2');
	cout << "Entrada: ";
	cin  >> inputFileName;
	cout << "Salida: ";
	cin  >> outputFileName;
	readFile(fp, inputFileName);
	exportResult(fp, memory, outputFileName, (int)option - '0');
}
