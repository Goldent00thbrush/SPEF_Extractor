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
struct VIA {
    string name; //the VIA Name
    string resistance; //the VIA resistance
};
vector < VIA > via; //ALL the VIAS


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
    vector <string> p_type;
    vector <string> p_name;
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
                        while (word != "+")
						{
							DEF >> word;
							if (word == "(")
							{
								DEF >> word;
								nets[index2].p_type.push_back({ word });
								DEF >> word;
								nets[index2].p_name.push_back({ word });
							}

						}
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
        for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < nets[i].p_name.size(); j++)
			{
				cout << nets[i].p_type[j];
			}
			cout << endl;
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
float calculateSegmentResistance(float sheet_resistance, float length, float width) { //per square //taken from LEF
    return sheet_resistance * (length / width); //parasitic_resistance
}
float calculateViaResistance(float via_resistance) {
    return via_resistance;
}
string find_layerW(string l)
{
    for (int i=0;i<layers.size();i++)
    {
        if (l==layers[i].name)
            return layers[i].width;
    }
}
string find_layerR(string l)
{
    for (int i=0;i<layers.size();i++)
    {
        if (l==layers[i].name)
            return layers[i].resistance_value;
    }
}
string find_layerC(string l)
{
    for (int i=0;i<layers.size();i++)
    {
        if (l==layers[i].name)
            return layers[i].capacitance_value;
    }
}
string find_layerE(string l)
{
    for (int i=0;i<layers.size();i++)
    {
        if (l==layers[i].name)
            return layers[i].edge_capacitance;
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
   /* for (int j = 0; j < 10; j++) {
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
    }*/
/*
    int x=stoi(nets[0].connection[0].cord[10].x);
    int y=stoi(nets[0].connection[0].cord[10].y);

    int x1=stoi(nets[0].connection[0].cord[11].x);
    int y1=y;
    float z=calculateSegmentLength(x,x1,y,y1);
    cout<<z<<endl;
    cout<<z<<"  "<<stof(layers[0].width)<<" "<<z/stof(layers[0].width)<<endl;
    cout<<calculateSegmentResistance(stof(layers[0].resistance_value),z,stof(layers[0].width))<<endl;
    cout<<calculateSegmentCapacitance(stof(layers[0].width),stof(layers[0].spacing),)
   // int y1=stoi(nets[0].connection[0].cord[11].y);

                  cout<< x <<"  "<<y  <<endl;
                  cout<< x1 <<"  "<<nets[0].connection[0].cord[11].y  <<endl;
*/
                          //, stoi(nets[0].connection[0].cord[1].x), stoi(nets[0].connection[0].cord[0].y), stoi(nets[0].connection[0].cord[1].y))<<endl;
            //, stoi(find_layer(nets[0].connection[0].layer)) << endl;


    cout << "*CAP" << endl;
        for (int j = 0; j < nets_size; j++) {
            for (int k = 0; k < nets[j].connection.size()-1; k++) {// per segment
                string w= find_layerW(nets[j].connection[k].layer);
                string c=find_layerC(nets[j].connection[k].layer);
                string e =find_layerE(nets[j].connection[k].layer);
                for (int i=0;i<nets[j].connection[k].cord.size();i++){
                cout << i + 1 <<" "<< calculateSegmentCapacitance(stof(w), calculateSegmentLength(stoi(nets[j].connection[k].cord[i].x), stoi(nets[j].connection[k].cord[i+1].x), stoi(nets[j].connection[k].cord[i].y), stoi(nets[j].connection[k].cord[i+1].y)),  stof(c), stof(e)) << endl;
            }
        }
}

    cout << "*RES" << endl;
        for (int j = 0; j < nets_size; j++) {//per net
            for (int k = 0; k < nets[j].connection.size()-1; k++) {// per segment
               string w= find_layerW(nets[j].connection[k].layer);
               string r=find_layerR(nets[j].connection[k].layer);
               for (int i=0;i<nets[j].connection[k].cord.size();i++){
                cout <<nets[j].name << "  " << calculateSegmentResistance(stof(r), calculateSegmentLength(stoi(nets[j].connection[k].cord[i].x), stoi(nets[j].connection[k].cord[i+1].x), stoi(nets[j].connection[k].cord[i].y), stoi(nets[j].connection[k].cord[i+1].y)), stof(w)) << endl;
            }}
        }

//vias
        for(int j=0;j<via.size();j++){
        cout << calculateViaResistance(stof(via[j].resistance)) << endl;
    }

    return 0;
}
