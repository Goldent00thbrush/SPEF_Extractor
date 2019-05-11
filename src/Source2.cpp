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
struct coor
{
    string x,y;
};

struct conn{
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
vector <string> number_c;
void readfromDEF()
{
    bool pinsfound = false,netsfound=false,finish2=false;
    ifstream DEF;
    string word;
    DEF.open("D://College//Semester 10-- Spring 2019//CSCE3304 - Digital Design II//Assignments//pro2//untitled//cpu.def");

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
                while (netsfound == false)
                {
                    DEF >> word;
                    if (word == "-")
                    {
                        DEF >> word;
                        nets[index2].name = word;

                    }
                    if ((word == "ROUTED") || (word=="NEW") )
                    {
                        int i=0;
                        DEF >> word;
                        c.layer=word;
                        //nets[index2].connection[i].layer=word;

                        finish2 = false;
                        int j=0,k=0;
                        coor r;
                        do
                        {
                            DEF >> word;
                            if (word == "(")
                            {
                                DEF >> word;
                                r.x=word;
                               // nets[index2].connection[i].cord[j].x=word;
                                firstc++;
                                DEF >> word;
                                r.y=word;
                               // nets[index2].connection[i].cord[j].y=word;
                                j++;
                                c.cord.push_back(r);

                            }
                            if (word == ")")
                            {
                                DEF >> word;
                                if ((word != "NEW") && (word !="(") )
                                {

                                    nets[index2].vias.push_back(word);
                                if (index2==1)   cout << nets[index2].vias.back() << endl;
                                    k++;
                                }


                            if (word == "NEW")
                                finish2 = true;
}
                            if (word == "NEW")
                                finish2 = true;
                        } while (finish2 == false);
                        nets[index2].connection.push_back(c);

                        i++;
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
    return 0;
}
