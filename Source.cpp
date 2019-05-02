#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

struct PINS {
	string name; //pin name
	string direction; //direction of the pin I/O
	string pin_state; //Pin state ( PLACED or FIXED)
	int firstcoordinate; // the first coordinate of a pin ex: pin cout has a first coordinate 2197 
	int secondcoordinate; //the second coordinate of a pin ex: pin cout has a second coordinate 30
};
vector < PINS > pins; //ALL the pins

struct NETS {
	string name; //NET NAME
	string connection[8][2]; //CONNECTIONS OF THE NET (type[0] name[1]) ex: PIN A or AND2X1 Y
};
NETS nets[50]; //ALL the nets

struct Layer {
	string name; //Layer Name
	string resistance_unit; //Layer resistance unit (RPERSQ Mostly)
	string resistance_value; //Layer resistance value
	string capacitance_unit; //Layer Capacitance unit (CPERSQDIST Mostly)
	string capacitance_value; //Layer Capacitance value
	string edgecapacitance; //Layer edge capacitance 

};
vector < Layer > layers; //those are the layers of TYPE ROUTING only
vector < string > all_layers; //those are all the layers in a file

struct VIA {
	string name; //the VIA Name
	string resistance; //the VIA resistance
};
vector < VIA > via; //ALL the VIAS

void readfromDEF() {

	string word, numberofpins, numberofnets, design_name;
	ifstream DEF;
	bool designfound = false, pinsfound = false, done = false, done2 = false, direction, point, netsfound = false, connectionsdone = false;
	int count = 0, count2 = 0, count3 = 0;
	DEF.open("tmp.def");
	if (DEF.is_open()) {
		while (!DEF.eof()) {
			DEF >> word;
			if ((word == "DESIGN") && (designfound == false)) {
				DEF >> word;
				design_name = word;
				designfound = true;
			}
			if ((word == "PINS") && (pinsfound == false)) {
				DEF >> word;
				numberofpins = word;
				string n, dir, first, second, state;

				while (done == false) {
					DEF >> word;
					if (word == "-") {
						DEF >> n; //name
						direction = false;
						point = false;
						do {
							DEF >> word;

							if (word == "DIRECTION") {
								DEF >> dir;
								direction = true;
							}
							if (word == "FIXED" || word == "PLACED") {
								state = word;
								DEF >> word;
								DEF >> first;
								DEF >> second;
								point = true;
								pins.push_back({n,dir,state,stoi(first),stoi(second)});
							}
						} while ((direction == false) || (point == false));
						count++;
					}
					if (count == stoi(numberofpins)) {
						done = true;
					}
				}
				pinsfound = true;
			}

			if ((word == "NETS") && (netsfound == false)) {
				DEF >> word;
				numberofnets = word;

				while (done2 == false) {
					DEF >> word;
					int i = 0;
					if (word == "-") {
						DEF >> word;
						nets[count2].name = word;

						do {
							DEF >> word;

							if (word == "(") {

								DEF >> word;
								nets[count2].connection[i][0] = word;
								DEF >> word;
								nets[count2].connection[i][1] = word;
							}
							if (word == ")") {
								i++;
							}

						} while (word != ";");
						count2++;

					}
					if (count2 == stoi(numberofnets)) {
						done2 = true;
					}

				}
				netsfound = true;
			}
		}
	}
	else {
		cout << "unable to open DEF file" << endl;
	}
	DEF.close();
}

void readfromLEF() {
	string x, x1, x2, x3, x4, width, name;
	bool done = false;
	int count = 0;
	ifstream LEF;
	LEF.open("tmp.lef");
	if (LEF.is_open()) {
		while (!LEF.eof()) {
			LEF >> x;
			if (x == "LAYER") {

				LEF >> name;
				all_layers.push_back(name);
				sort(all_layers.begin(), all_layers.end());
				all_layers.erase(unique(all_layers.begin(), all_layers.end()), all_layers.end());
				LEF >> x;
				LEF >> x;

				if (x == "ROUTING") {
					done = false;
					do {
						LEF >> x;
						if (x == "RESISTANCE") {
							LEF >> x1; //units	
							LEF >> x2; //value		
						}
						if (x == "CAPACITANCE") {
							LEF >> x3;
							LEF >> x4;
							count++;
						}
						if (x == "EDGECAPACITANCE") {
							LEF >> x;
							layers.push_back({name,x1,x2,x3,x4,x});
							done = true;
						}
					} while (done == false);
				}
			}
			if (x == "VIA") {

				string res, name2;
				bool flag = false;
				LEF >> name2;
				do {
					LEF >> x;
					if (x == "RESISTANCE") {
						LEF >> res;
						via.push_back({name2,res});
						flag = true;
					}
				} while (flag == false);
			}
		}
	}
	else {
		cout << "unable to open LEF file" << endl;
	}
	LEF.close();
}

int main() {
	int choice;
	do {
		cout << "Which file would you like to enter? 1:DEF 2:LEF" << endl;
		cin >> choice;
	} while (choice < 1 || choice > 2);
	if (choice == 1)
		readfromDEF();
	else if (choice == 2)
		readfromLEF();

	system("PAUSE");
	return 0;
}
