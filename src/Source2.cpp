#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct  PINS
{
	string name;
	string pin_state;
	int firstcoordinate;
	int secondcoordinate;
};
vector <PINS> pins;
struct NETS
{
	string name;
	vector <string> metals;
	vector <string> c1;
	vector <string> c2;
	vector <string> vias;
};
NETS nets[5000];
vector <string> number_c;
void readfromDEF()
{
	bool pinsfound = false,netsfound=false,finish2=false;
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
				while (netsfound == false)
				{
					DEF >> word;
					if (word == "-")
					{
						DEF >> word;
						nets[index2].name = word;
						
					}
					if ((word == "ROUTED") || (word=="NEW"))
					{
						
						DEF >> word;
						nets[index2].metals.push_back(word);
						
						finish2 = false;
						do
						{
							DEF >> word;
							if (word == "(")
							{
								DEF >> word;
								nets[index2].c1.push_back(word);
								firstc++;
								DEF >> word;
								nets[index2].c2.push_back(word);
								
							}
							if (word == ")")
							{
								DEF >> word;
								if ((word != "NEW") && (word !="("))
								{
									
									nets[index2].vias.push_back(word);
									cout << nets[index2].vias[1] << endl;
									
								}
								
							}
							if (word == "NEW")
								finish2 = true;
							

						} while (finish2 == false);

					}
					if (word == ";")
						index2++;
					if (word == "END")
						netsfound = true;
					
				}
			}
				
			}
			
		}
		else
		{
			cout << "cant open file" << endl;
		}
	

}
int main()
{
	readfromDEF();
	system("PAUSE");
	return 0;
}