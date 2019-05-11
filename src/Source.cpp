#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

struct  PINS
{
	string name;
	string pin_state;
	int firstcoordinate;
	int secondcoordinate;
};
vector <PINS> pins;



struct Layer {
	string name; //Layer Name
	string resistance_unit; //Layer resistance unit (RPERSQ Mostly)
	string resistance_value; //Layer resistance value
	string capacitance_unit; //Layer Capacitance unit (CPERSQDIST Mostly)
	string capacitance_value; //Layer Capacitance value
	string edge_capacitance; //Layer edge capacitance 
	string width;
	string spacing;

};
vector < Layer > layers; //those are the layers of TYPE ROUTING only
vector < string > all_layers; //those are all the layers in a file

struct VIA {
	string name; //the VIA Name
	string resistance; //the VIA resistance
};
vector < VIA > via; //ALL the VIAS

struct coor
{
	string x, y;
};

struct conn {
	string layer;
	vector<coor> cord;

};
struct NETS
{
	string name;
	vector<conn> connection;
	vector <string> vias;

};
NETS nets[5000];

void readfromDEF()
{
	bool pinsfound = false, netsfound = false, finish2 = false;
	ifstream DEF;
	string word;
	DEF.open("cpu.def");

	if (DEF.is_open())
	{

		while (!DEF.eof())
		{
			DEF >> word;
			if (word == "PINS")
			{
				int index = 0;

				while (pinsfound == false)
				{
					DEF >> word;
					if (word == "-")
					{

						pins.push_back({ " "," ",0,0 });
						DEF >> word;
						pins[index].name = word;
					}
					if ((word == "PLACED") || (word == "FIXED"))
					{
						pins[index].pin_state = word;
						DEF >> word;
						DEF >> word;
						pins[index].firstcoordinate = stoi(word);

						DEF >> word;
						pins[index].secondcoordinate = stoi(word);
						index++;
					}

					if (word == "END")
						pinsfound = true;

				}
			}
			if (word == "NETS")
			{
				int index2 = 0;
				int metals_size = 0, firstc = 0;
				conn c;
				coor r;
				int j = 0, k = 0;
				DEF >> word;
				DEF >> word;
				while (word != "END")
				{
					DEF >> word;
					if (word == "-")
					{
						DEF >> word;
						nets[index2].name = word;
						j = 0;
					}
					if ((word == "ROUTED") || (word == "NEW"))
					{
						DEF >> word;
						c.layer = word;
						//nets[index2].connection[i].layer=word;
					}
					if (word == "(")
					{
						DEF >> word;
						r.x = word;
						// nets[index2].connection[i].cord[j].x=word;
						firstc++;
						DEF >> word;
						r.y = word;
						// nets[index2].connection[i].cord[j].y=word;
						j++;
						c.cord.push_back(r);

					}
					if (word == ")")
					{
						DEF >> word;
						if ((word != "NEW") && (word != "("))
						{

							nets[index2].vias.push_back(word);
							
							k++;
						}

					}
					if (word == ";")
					{
						
						index2++;
					}
				}

				nets[index2].connection.push_back(c);

			}

		}

	}
	else
	{
		cout << "cant open file" << endl;
	}


}

void readfromLEF() {
	string x, x1, x2, x3, x4, width_l, name,space;
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
						if (x == "WIDTH")
						{
							LEF >> width_l;

						}
						if (x == "SPACING")
						{
							LEF >> space;
						}
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
							layers.push_back({name,x1,x2,x3,x4,x,width_l,space});
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
float calculateWireCapacitance(float width, float spacing, float capacitance, float edge_capacitance) { //area*CPERSQDIST+length*EDGECAP from LEF
	return (width*spacing*capacitance) + (spacing*edge_capacitance); // (WIDTH*SPACING*CPERSQDIST)+(SPACING*EDGECAP)
}
float calculateSheetResistance(float sheet_resistance, float length, float width) { //per square //taken from LEF
	return sheet_resistance * (length / width); //parasitic_resistance
}
float calculateViaResistance(float via_resistance) {
	return via_resistance;
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
		
	cout << "*CAP" << endl;
	for (int i = 0; i < layers.size(); i++) {
		cout << i + 1 << "  " << layers[i].name << "  " << calculateWireCapacitance(stof(layers[i].width), stof(layers[i].spacing), stof(layers[i].capacitance_value), stof(layers[i].edge_capacitance)) << endl;
	}

	cout << "*RES" << endl;
	for (int i = 0; i < layers.size(); i++) {
		cout << i + 1 << "  " << layers[i].name << "  " << calculateSheetResistance(stof(layers[i].resistance_value), stof(layers[i].spacing), stof(layers[i].width)) << endl;
	}
	for (int i = 0; i < via.size(); i++) {
		cout << i + 1 << "  " << via[i].name << "  " << calculateViaResistance(stof(via[i].resistance)) << endl;
	}
	cout << "*END" << endl;
	system("PAUSE");
	return 0;
}
