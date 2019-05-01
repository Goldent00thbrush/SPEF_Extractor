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
struct NETS
{
	string name;
	string connection[7][2];
};
NETS nets[51];

void readfromDEF()
{

	string word,numberofpins,numberofnets,design_name;
	ifstream DEF;
	bool designfound = false,pinsfound=false,done=false, done2= false,direction,point,netsfound=false, connectionsdone=false;
	int count = 0,count2=0,count3=0;
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
			if ((word == "NETS") && (netsfound == false))
			{
				DEF >> word;
				numberofnets = word;
				
				while (done2 == false)
				{
					DEF >> word;
					int i = 0;
							if (word == "-")
							{
								DEF >> word;
								nets[count2].name = word;
								
								
								do {
									DEF >> word;
									
									if (word == "(")
									{

										DEF >> word;
										nets[count2].connection[i][0] = word;
										DEF >> word;
										nets[count2].connection[i][1] = word;
									}
									if (word == ")")
									{
										i++;
									}


								
								} while (word!=";");
									count2++;

							}
							if (count2 == stoi(numberofnets))
							{
								done2 = true;
							}
							
							

				}
				netsfound = true;
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