#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct PINS
{
	string name;
	string direction;
	string pin_state;
	int firstcoordinate;
	int secondcoordinate;
};
PINS pins[14];
void readfromDEF()
{

	string word,numberofpins,design_name;
	ifstream DEF;
	bool designfound = false,pinsfound=false,done=false,direction,point;
	int count = 0;
	DEF.open("tmp.def");
	if (DEF.is_open())
	{
		while (!DEF.eof())
		{
			DEF >> word;
			if ((word == "DESIGN") && (designfound==false))
			{
				DEF >> word;
				design_name = word;
				designfound = true;
			}
			if ((word == "PINS") && (pinsfound == false))
			{
				DEF >> word; 
				numberofpins = word;
				cout << "number of pins: " << numberofpins << endl;
				while (done == false)
				{
					DEF >> word;
						if (word == "-")
						{
							DEF >> word;
							pins[count].name = word;
							direction = false;
							point = false;
							do{
								
								DEF >> word;
								if (word == "DIRECTION")
								{
									DEF >> word;
									pins[count].direction = word;
									direction = true;
								}
								if (word == "FIXED" || word == "PLACED")
								{
									pins[count].pin_state=word;
									DEF>> word;
									DEF >> word;
									pins[count].firstcoordinate = stoi(word);
									DEF >> word;
									pins[count].secondcoordinate = stoi(word);
									point = true;
								}
							}while ((direction == false) || (point==false));
							count++;
						}
						if (count == stoi(numberofpins))
						{
							done = true;
						}

				}
				pinsfound = true;
			}
		}
	}

	else
	{
		cout << "unable to open input file" << endl;
	}
	DEF.close();
}

void readfromLEF()
{

}

int main()
{

	readfromDEF();
	for (int i = 0; i < 14; i++)
	{
		cout << pins[i].name <<" ";
		cout << pins[i].direction<<" ";
		cout << pins[i].pin_state << " ";
		cout << pins[i].firstcoordinate <<" ";
		cout << pins[i].secondcoordinate<<" ";
		cout << endl;
	}
	system("PAUSE");
	return 0;
}