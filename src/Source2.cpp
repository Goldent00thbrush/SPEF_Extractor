#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
using namespace std;
int nets_size = 0;
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
    int nets_size = 0;
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
                coor r;
                conn c;
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
                        nets_size++;
                    }
                    if ((word == "ROUTED") || (word == "NEW"))
                    {
                        DEF >> word;
                        c.layer = word;
                        //nets[index2].connection[i].layer=word;
                    }
                    if (word == "(")
                    {
bracket:                        DEF >> word;
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

                        if (word=="(") goto bracket;

                        if ((word != "NEW") && (word != "(") && (word != ";") && (word != "+"))
                        {

                            nets[index2].vias.push_back(word);
                        nets[index2].connection.push_back(c);
                        conn p;
                        c=p;
                            k++;

                        }
                      else  if ((word == "NEW") || (word == ";"))
                       {  nets[index2].connection.push_back(c);

                            conn p;
                            c=p;
                }

                    }
                    if (word == ";")
                    {

                        index2++;
                    }
                   // cout<<nets[index2].name<<" "<<nets[index2].connection.size()<<endl;
                }
              //  nets[index2].connection.push_back(c);



            if(word=="END")break;

            }

        }

    }
    else
    {
        cout << "cant open file" << endl;
    }


}

void readfromLEF() {
    string x, x1, x2, x3, x4, width_l, name, space;
    bool done = false;
    int count = 0;
    ifstream LEF;
    LEF.open("D://College//Semester 10-- Spring 2019//CSCE3304 - Digital Design II//Assignments//pro2//untitled//tmp.lef");
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
                            layers.push_back({ name,x1,x2,x3,x4,x,width_l,space });
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
                        via.push_back({ name2,res });
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
float calculateSegmentLength(int first_coordinate_x, int second_coordinate_x, int first_coordinate_y, int second_coordinate_y) {
   cout<<"x1: "<<first_coordinate_x<<"x2: "<<second_coordinate_x<<"y "<<sqrt(pow(first_coordinate_x + second_coordinate_x, 2))<<endl;
    return	sqrt(pow(first_coordinate_x + second_coordinate_x, 2) + pow(first_coordinate_y + second_coordinate_y, 2));
}
float calculateSegmentCapacitance(float width, float spacing, float capacitance, float edge_capacitance) { //area*CPERSQDIST+length*EDGECAP from LEF
    return (width*spacing*capacitance) + (spacing*edge_capacitance); // (WIDTH*SPACING*CPERSQDIST)+(SPACING*EDGECAP)
}
float calculateSheetResistance(float sheet_resistance, float length, float width) { //per square //taken from LEF
    return sheet_resistance * (length / width); //parasitic_resistance
}
float calculateViaResistance(float via_resistance) {
    return via_resistance;
}
string find_layer(string l)
{
    for (int i=0;i<layers.size();i++)
    {
        if (l==layers[i].name)
            return layers[i].width;
    }
}
int main() {

    readfromDEF();
    readfromLEF();

   // cout << calculateSheetResistance(stof(find_layer(nets[0].connection[0].layer)),
          //  cout<<calculateSegmentLength(
    nets_size=5000;
   /* for (int j = 0; j < nets_size; j++) {
        cout<<nets[j].name<<endl;
    }
  /*  for (int i=0;i<pins.size();i++)
    {
        cout<<pins[i].name<<"\t"<<pins[i].pin_state<<"\t"<<pins[i].firstcoordinate<<"\t"<<pins[i].secondcoordinate<<endl;
    }*/
    for (int j = 0; j < 10; j++) {
        cout<<"Net: "<<endl;
        cout<<nets[j].name<<endl;
        cout<<"conn: "<<endl;
        int r= nets[j].connection.size();
        cout<<r<<endl;
        for (int i=0;i<r;i++)
        {
            cout<< nets[j].connection[i].layer<<endl;
            cout<<"cord: "<<endl;
            int p= nets[j].connection[i].cord.size();
            for (int k=0;k<p;k++)
            {
                cout <<nets[j].connection[i].cord[k].x<<
                      nets[j].connection[i].cord[k].y<< endl;
            }
        }
    }
    /*
  //  string x=nets[1].connection[0].cord[0].x;
              //    cout<< x   <<endl;
                          //, stoi(nets[0].connection[0].cord[1].x), stoi(nets[0].connection[0].cord[0].y), stoi(nets[0].connection[0].cord[1].y))<<endl;
            //, stoi(find_layer(nets[0].connection[0].layer)) << endl;


    cout << "*CAP" << endl;
        for (int j = 0; j < nets_size; j++) {
            for (int k = 0; k < nets[j].connection.size()-1; k++) {// per segment

               // cout << i + 1 <<" "<< calculateSegmentCapacitance(stof(find_layer(nets[j].connection[k].layer), calculateSegmentLength(stoi(nets[j].connection[k].cord[j].x), stoi(nets[i].connection[j].cord[j+1].x), stoi(nets[i].connection[j].cord[j].y), stoi(nets[i ].connection[j].cord[j+1].y)), stof(layers[i].capacitance_value), stof(layers[i].edge_capacitance)) << endl;
            }
        }


    cout << "*RES" << endl;
    for (int i = 0; i < layers.size(); i++) { //per layers
        for (int j = 0; j < nets_size; j++) {//per net
            for (int k = 0; k < nets[j].connection.size()-1; k++) {// per segment
                cout << i + 1 << "  " << layers[i].name << "  " << calculateSheetResistance(stof(layers[i].resistance_value), calculateSegmentLength(stoi(nets[i].connection[j].cord[j].x), stoi(nets[i+1].connection[j].cord[j].x), stoi(nets[i].connection[j].cord[j].y), stoi(nets[i + 1].connection[j].cord[j].y)), stoi(layers[i].width)) << endl;
            }
        }
    }

    for (int i = 0; i < via.size(); i++) {
        cout << i + 1 << "  " << via[i].name << "  " << calculateViaResistance(stof(via[i].resistance)) << endl;
    }
    */
    return 0;
}
