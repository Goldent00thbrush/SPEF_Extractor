#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void readfromDEF()
{

	string word,numberofpins;
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
				cout << "the design name is:" << word << endl;
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
							cout << word<<endl;
							direction = false;
							point = false;
							do{
								
								DEF >> word;
								if (word == "DIRECTION")
								{
									DEF >> word;
									
									cout << word << endl;
									direction = true;
								}
								if (word == "FIXED" || word == "PLACED")
								{
									cout << word << endl;
									DEF>> word;
									DEF >> word;
									cout << "firstnumber" << word<<endl;
									DEF >> word;
									cout << "Secondnumber" << word << endl;
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
	system("PAUSE");
	return 0;
}